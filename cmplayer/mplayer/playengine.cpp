#include "playengine.h"
#include "videooutput.h"
#include "audiooutput.h"
#include "subtitleoutput.h"
#include "info.h"
#include <backend/utility.h>
#include "mediainfo.h"
#include "config.h"
#include "mplayerprocess.h"
#include <QFile>
#include <QSize>
#include <QDebug>

namespace Backend {

namespace MPlayer {

struct PlayEngine::Data {
	Data(PlayEngine *parent);
	void init();
	void updateInfo();
public:
	PlayEngine *p;
	MediaInfo info;
	MPlayerProcess *proc;
	VideoOutput *video;
	AudioOutput *audio;
	SubtitleOutput *subout;
	bool justFinished, gotInfo;
	const QString dontmessup;
	int osdLevel;
	Config config;
};

PlayEngine::Data::Data(PlayEngine *p)
: p(p), proc(new MPlayerProcess(p)), video(new VideoOutput(p))
, audio(new AudioOutput(p)), subout(new SubtitleOutput(p))
, justFinished(false), gotInfo(false)
, dontmessup(Info::privatePath() +"/input.conf"), osdLevel(0) {}

void PlayEngine::Data::init() {
	connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), p, SLOT(slotProcFinished()));
	connect(p, SIGNAL(stateChanged(Backend::State, Backend::State)),
			p, SLOT(slotStateChanged(Backend::State, Backend::State)));
	connect(p, SIGNAL(started()), p, SLOT(update()));
}

void PlayEngine::Data::updateInfo() {
	if (info.isValid()) {
		emit p->totalTimeChanged(info.length());
		if (info.hasVideo() && video)
			video->updateVideoSize(info.videoSize());
	}
}

PlayEngine::PlayEngine(const Backend::FactoryIface *factory, QObject *parent)
: Backend::PlayEngine(factory, parent), d(new Data(this)) {
	d->init();
	setAudio(d->audio);
	setVideo(d->video);
	setSubtitle(d->subout);
	QFile file(d->dontmessup);
	if (!file.exists() && file.open(QFile::WriteOnly)) {
		file.write(
			"## prevent mplayer from messing up our shortcuts\n\n"
			"RIGHT gui_about\nLEFT gui_about\nDOWN gui_about\n"
			"UP gui_about\nPGUP gui_about\nPGDWN gui_about\n"
			"- gui_about\n+ gui_about\nESC gui_about\nENTER gui_about\n"
			"SPACE pausing_keep invalid_command\nHOME gui_about\n"
			"END gui_about\n> gui_about\n< gui_about\nINS gui_about\n"
			"DEL gui_about\n[ gui_about\n] gui_about\n{ gui_about\n"
			"} gui_about\nBS gui_about\nTAB gui_about\n. gui_about\n"
			"# gui_about\n@ gui_about\n! gui_about\n9 gui_about\n"
			"/ gui_about\n0 gui_about\n* gui_about\n1 gui_about\n"
			"2 gui_about\n3 gui_about\n4 gui_about\n5 gui_about\n"
			"6 gui_about\n7 gui_about\n8 gui_about\na gui_about\n"
			"b gui_about\nc gui_about\nd gui_about\ne gui_about\n"
			"F invalid_command\nf invalid_command\ng gui_about\n"
			"h gui_about\ni gui_about\nj gui_about\nk gui_about\n"
			"l gui_about\nm gui_about\nn gui_about\no gui_about\n"
			"p gui_about\nq gui_about\nr gui_about\ns gui_about\n"
			"t gui_about\nT gui_about\nu gui_about\nv gui_about\n"
			"w gui_about\nx gui_about\ny gui_about\nz gui_about\n"
			"S gui_about\n"
		);
	}
}

PlayEngine::~PlayEngine() {
	delete d;
}

void PlayEngine::slotStateChanged(Backend::State /*state*/, Backend::State /*old*/){
	setSeekable(!isStopped());
}

void PlayEngine::slotProcFinished() {
	setState(StoppedState);
	if (d->justFinished) {
		emit finished(currentSource());
		d->justFinished = false;
	}
}

void PlayEngine::stop() {
	int time = currentTime();
	if (state() == StoppedState || !tellmp("quit"))
		return;
	emit stopped(currentSource(), time);
	if (!d->proc->waitForFinished(5000))
		d->proc->kill();
}

void PlayEngine::seek(int time, bool relative, bool /*showTime*/) {
	tellmp("seek " + QString::number(static_cast<double>(time)/1000) + (relative ? " 0" : " 2"));
}

void PlayEngine::replay() {
	int time = currentTime();
	stop();
	start(time);
}

bool PlayEngine::start(int time) {
	if (isRunning())
		stop();
	MediaSource source = currentSource();
	if (!source.isValid())
		return false;
	if ((source.isLocalFile() || source.isDisc()) && !d->gotInfo && (d->gotInfo = d->info.get(source)))
		d->updateInfo();
	QStringList args;
	args << "-slave" << "-noquiet" << "-nofs" << "-input"
			<< "conf=\"" + d->dontmessup + '"'
		<< "-fontconfig" << "-zoom" << "-nokeepaspect"// << "-noautosub"
		<< "-osdlevel" << QString::number(d->osdLevel);
	if (!d->gotInfo)
		args << "-identify";
	if (d->video) {
		args << "-wid" << QString::number(d->video->internalWidgetId());
		if (d->config.videoDriver() != "auto")
			args << "-vo" << d->config.videoDriver();
		if (d->gotInfo && Utility::desktopRatio() < d->info.aspectRatio()) {
			args << "-vf-add" << "expand=:::::" + QString::number(Utility::desktopRatio());
			d->video->expand(true);
		} else
			d->video->setExpanded(false);
	} else
		args << "-vo" << "null";
	if (d->audio) {
		if (d->config.audioDriver() != "auto")
			args << "-ao" << d->config.audioDriver();
		args << "-softvol" << "-softvol-max"
				<< QString::number(d->config.volumeAmplifiaction()*100);
	} else
		args << "-ao" << "null";
	if (d->subout) {
		QString family = d->subout->style().font.family();
		if (!family.isEmpty())
			args << "-font" << family;
		if (!d->subout->encoding().isEmpty())
			args << "-subcp" << d->subout->encoding();
		int mode = 1;
		if (d->subout->style().scale == OsdStyle::FitToDiagonal)
			mode = 3;
		else if (d->subout->style().scale == OsdStyle::FitToWidth)
			mode = 2;
		args << "-subfont-autoscale" << QString::number(mode);
	}
	//if (general.autoPitch)
	//	args << "-af" << "scaletempo";
	if (time > 1000)
		args << "-ss" << QString::number(static_cast<double>(time)/1000.0);

	if (!d->config.options().isEmpty())
		args += d->config.options();

	args << source.url().toString();
	static Info info;
	qDebug("%s %s", qPrintable(d->config.mplayer()), qPrintable(args.join(" ")));
	d->proc->start(d->config.mplayer(), args);
	if (!d->proc->waitForStarted())
		return false;
	return true;
}

void PlayEngine::updateCurrentSource(const MediaSource &source) {
	if ((d->gotInfo = d->info.get(source)))
		d->updateInfo();
	else
		emit totalTimeChanged(0);
}

bool PlayEngine::tellmp(const QString &command) {
	if (isRunning()) {
		d->proc->write(command.toLocal8Bit() + "\n");
		qDebug("told: %s", qPrintable(command));
		return true;
	} else
		qDebug("couldn't tell: %s", qPrintable(command));
	return false;
}

void PlayEngine::play() {
	if (isStopped()) {
		start();
	} else if (isPaused())
		tellmp("pause");
}

bool PlayEngine::isRunning() {
	return d->proc->state() != QProcess::NotRunning;
}

void PlayEngine::updateSpeed(double speed) {
	tellmp("speed_set " + QString::number(speed));
}

VideoOutput *PlayEngine::videoOutput() const {
	return d->video;
}

SubtitleOutput *PlayEngine::subtitleOutput() const {
	return d->subout;
}

AudioOutput *PlayEngine::audioOutput() const {
	return d->audio;
}

void PlayEngine::update() {
	if (isRunning())
		tellmp("speed_set " + QString::number(speed()));
}

void PlayEngine::setOSDLevel(int level) {
	int temp = qBound(0, level, 3);
	if (temp != d->osdLevel) {
		d->osdLevel = level;
		tellmp("osd " + QString::number(d->osdLevel));
	}
}

int PlayEngine::osdLevel() const {
	return d->osdLevel;
}

void PlayEngine::exiting() {
	d->justFinished = true;
}

void PlayEngine::seekChapter(int n) {
// 	if (d->source.isDisc() && d->source.gotInfo()) {
// 		int title = d->source.titleNumber();
// 		if (title > 0 && title <= d->source.info().dvd().titles.size() && n > 0
// 				&& n <= d->source.info().dvd().titles[title-1].chapters.size())
// 			seek(d->source.info().dvd().titles[title-1].chapters[n-1], false);
// 	}
}

void PlayEngine::playTitle(int number) {
// 	if (d->source.isDisc() && d->source.gotInfo()
// 			&& number > 0 && number <= d->source.info().dvd().titles.size()) {
// 		stop();
// 		setCurrentSource(MediaSource(QUrl("dvd://" + QString::number(number))));
// 		play();
// 	}
}

}

}
