#include "playengine.h"
#include "info.h"
#include "config.h"
#include "mediainfo.h"
#include "mplayerprocess.h"
#include <core/subtitle.h>
#include <core/utility.h>
#include <core/osdstyle.h>
#include <core/nativerenderer.h>
#include <QtCore/QTemporaryFile>

namespace MPlayer {

struct PlayEngine::Data {
	Data(): tempsub(getTempSub()) {}
	static QString getTempSub() {
		QTemporaryFile file(Info::privatePath() + "/cmplayer-mplayer-XXXXXX.smi");
		return file.open() ? file.fileName() : Info::privatePath() + "/temp.smi";
	}
	Info info;
	MediaInfo mediaInfo;
	MPlayerProcess *proc;
	bool justFinished, gotInfo, seeking;
	const QString tempsub;
	int osdLevel;
	Core::NativeRenderer *renderer;
	QString vo, ao;
	QMap<QString, int> tracks;
};

PlayEngine::PlayEngine(QObject *parent)
: Core::PlayEngine(parent), d(new Data) {
	d->proc = new MPlayerProcess(this);
	d->seeking = d->justFinished = d->gotInfo = false;
	d->osdLevel = 0;
	m_time = 0;
	d->renderer = new Core::NativeRenderer(this);
	setVideoRenderer(d->renderer);

	connect(d->proc, SIGNAL(finished(int, QProcess::ExitStatus))
		, this, SLOT(slotProcFinished()));
	connect(this, SIGNAL(stateChanged(Core::State, Core::State))
		, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(d->renderer, SIGNAL(osdRectChanged(const QRect&))
		, this, SLOT(slotOsdRectChanged()));
}

PlayEngine::~PlayEngine() {
	stop();
	d->renderer->hide();
	setVideoRenderer(0);
	delete d->renderer;
	delete d->proc;
	QFile::remove(d->tempsub);
	delete d;
}

const QStringList &PlayEngine::getDefaultArgs() {
	static const QStringList args = QStringList() << "-slave" << "-noquiet" << "-nofs"
		<< "-input" << ("conf=\"" + getDontMessUp() + '"') << "-fontconfig" << "-zoom"
		<< "-nokeepaspect" << "-noautosub" << "-osdlevel" << QString::number(0)
		<< "-utf8" << "-subcp" << "UFT-8" << "-softvol" << "-softvol-max" << QString::number(1000.0);
	return args;
}

const QString &PlayEngine::getDontMessUp() {
	static QString fileName;
	if (fileName.isEmpty()) {
		fileName = Info::privatePath() + "/input.conf";
		QFile file(fileName);
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
	return fileName;
}

void PlayEngine::updateInfo() {
	if (d->mediaInfo.isValid()) {
		setDuration(d->mediaInfo.length());
		setHasVideo(d->mediaInfo.hasVideo());
		if (d->mediaInfo.hasVideo()) {
			d->renderer->setVideoSize(d->mediaInfo.videoSize());
			d->renderer->setFrameSize(d->mediaInfo.videoSize());
		}
		d->tracks.clear();
		QMap<int, QString>::const_iterator it = d->mediaInfo.tracks().begin();
		const QString track = "Track %1 %2";
		for (int i=0; it != d->mediaInfo.tracks().end(); ++it, ++i) {
			d->tracks[track.arg(i).arg(it.value())] = it.key();
		}
		setTracks(d->tracks.keys());
	}
}


void PlayEngine::slotStateChanged(Core::State /*state*/, Core::State /*old*/){
	setSeekable(!isStopped());
}

void PlayEngine::slotProcFinished() {
	setState(Core::Stopped);
	if (d->justFinished) {
		emit finished(currentSource());
		d->justFinished = false;
	}
}

void PlayEngine::replay() {
	int time = currentTime();
	stop();
	start(time);
}

bool PlayEngine::start(int time) {
	if (d->proc->isRunning())
		stop();
	Core::MediaSource source = currentSource();
	if (!source.isValid())
		return false;
	if ((source.isLocalFile() || source.isDisc()) && !d->gotInfo && (d->gotInfo = d->mediaInfo.get(source)))
		updateInfo();
	int mode = 1;
	if (subtitleStyle().scale == Core::OsdStyle::FitToDiagonal)
		mode = 3;
	else if (subtitleStyle().scale == Core::OsdStyle::FitToWidth)
		mode = 2;
	QStringList args = getDefaultArgs();
	args << "-font" << subtitleStyle().font.family()
			<< "-subfont-autoscale" << QString::number(mode)
			<< "-subdelay" << QString::number(syncDelay()*0.001)
			<< "-wid" << QString::number(d->renderer->screenWinId());
	if (!d->gotInfo)
		args << "-identify";
	if (!d->vo.isEmpty())
		args << "-vo" << d->vo;
	if (!d->ao.isEmpty())
		args << "-ao" << d->ao;
	if (d->gotInfo && Core::Utility::desktopRatio() < d->mediaInfo.aspectRatio()) {
		static const QString rate = QString::number(Core::Utility::desktopRatio());
		args << "-vf-add" << "expand=:::::" + rate;
		QSize size = d->mediaInfo.videoSize();
		size.rheight() *= d->mediaInfo.aspectRatio()/Core::Utility::desktopRatio();
		d->renderer->setFrameSize(size);
	} else
		d->renderer->setFrameSize(d->renderer->videoSize());
	if (d->info.audioFilter().contains("scaletempo"))
		args << "-af-add" << "scaletempo";
	if (isVolumeNormalized() && d->info.audioFilter().contains("volnorm"))
		args << "-af-add" << "volnorm";
	if (time > 1000)
		args << "-ss" << QString::number(double(time)/1000.);
// 	if (!d->config.options().isEmpty())
// 		args += d->config.options();
	args << (source.isDisc() ? "dvd://" : source.url().toString());
	qDebug("%s %s", "mplayer", qPrintable(args.join(" ")));
	d->proc->start("mplayer", args);
	if (!d->proc->waitForStarted())
		return false;
	updateVolume();
	updateSubtitle(subtitle());
	updateSubtitlePos(subtitlePos());
	return true;
}

bool PlayEngine::tellmp(const QString &command) {
	if (d->proc->isRunning()) {
		d->proc->write(command.toLocal8Bit() + "\n");
		qDebug("told: %s", qPrintable(command));
		return true;
	} else
		qDebug("couldn't tell: %s", qPrintable(command));
	return false;
}

bool PlayEngine::tellmp(const QString &command, const QString &value, const QString &option) {
	QString cmd = command;
	cmd += " ";
	cmd += value;
	if (!option.isEmpty()) {
		cmd += " ";
		cmd += option;
	}
	return tellmp(cmd);
}

void PlayEngine::slotOsdRectChanged() {
	updateSubtitlePos(subtitlePos());
}

void PlayEngine::update() {

}

void PlayEngine::setOsdLevel(int level) {
	tellmp("osd", qBound(0, level, 3));
}

void PlayEngine::exiting() {
	d->justFinished = true;
}

// void PlayEngine::seekChapter(int n) {
// 	if (d->source.isDisc() && d->source.gotInfo()) {
// 		int title = d->source.titleNumber();
// 		if (title > 0 && title <= d->source.info().dvd().titles.size() && n > 0
// 				&& n <= d->source.info().dvd().titles[title-1].chapters.size())
// 			seek(d->source.info().dvd().titles[title-1].chapters[n-1], false);
// 	}
// }

// void PlayEngine::playTitle(int number) {
// 	if (d->source.isDisc() && d->source.gotInfo()
// 			&& number > 0 && number <= d->source.info().dvd().titles.size()) {
// 		stop();
// 		setCurrentSource(MediaSource(QUrl("dvd://" + QString::number(number))));
// 		play();
// 	}
// }

const Core::Info &PlayEngine::info() const {
	return d->info;
}

void PlayEngine::play(int time) {
	start(time);
}

void PlayEngine::play() {
	if (isStopped()) {
		start();
	} else if (isPaused())
		tellmp("pause");
}

void PlayEngine::stop() {
	if (!isStopped()) {
		const int time = m_time;
		if (!tellmp("quit"))
			return;
		emit stopped(currentSource(), time);
		if (!d->proc->waitForFinished(5000))
			d->proc->kill();
	}
}

void PlayEngine::pause() {
	if (isPlaying())
		tellmp("pause");
}

void PlayEngine::seek(int time, bool relative, bool showTimeLine, int /*duration*/) {
	if (showTimeLine)
		setOsdLevel(1);
	if (!d->seeking) {
		d->seeking = true;
		tellmp("seek", double(time)/1000.0, relative ? 0 : 2);
		d->seeking = false;
	}
	if (showTimeLine)
		setOsdLevel(0);
}

void PlayEngine::seek(int time) {
	if (!d->seeking) {
		d->seeking = true;
		tellmp("seek", double(time)/1000.0, 2);
		d->seeking = false;
	}
}

void PlayEngine::showMessage(const QString &message, int duration) {
	if (!tellmp("osd_show_text", "\"" + message + "\"", QString::number(duration)))
		Core::PlayEngine::showMessage(message, duration);
}

void PlayEngine::showTimeLine(int time, int duration) {
	QString text = Core::Utility::msecsToString(time);
	text += "/";
	text += Core::Utility::msecsToString(this->duration());
	showMessage(text, duration);
}

void PlayEngine::updateVolume() {
	tellmp("volume", realVolume()*10.0, 1);
	tellmp("mute",  isMuted() ? 1 : 0);
}

void PlayEngine::updateSpeed(double speed) {;
	tellmp("speed_set", speed);
}

void PlayEngine::updateVideoProperty(Core::VideoProperty prop, double value) {
	const int temp = qBound(-100, qRound(value*100.), 100);
	switch(prop) {
	case Core::Brightness:
		tellmp("brightness", temp, 1);
		break;
	case Core::Contrast:
		tellmp("contrast", temp, 1);
		break;
	case Core::Saturation:
		tellmp("saturation", temp, 1);
		break;
	case Core::Hue:
		tellmp("hue", temp, 1);
		break;
	default:
		break;
	}
}

void PlayEngine::updateVideoProperties(double b, double c, double s, double h) {
	tellmp("brightness", qBound(-100, qRound(b*100.), 100), 1);
	tellmp("contrast", qBound(-100, qRound(c*100.), 100), 1);
	tellmp("saturation", qBound(-100, qRound(s*100.), 100), 1);
	tellmp("hue", qBound(-100, qRound(h*100.), 100), 1);
}

void PlayEngine::updateSubtitle(const Core::Subtitle &subtitle) {
	tellmp("sub_select -1") && tellmp("sub_remove");
	Core::Subtitle::save(d->tempsub, subtitle, "UTF-8") && !subtitle.isEmpty()
		&& tellmp("sub_load \"" + d->tempsub + '\"') && tellmp("sub_select 0");
}

void PlayEngine::updateSubtitleStyle(const Core::OsdStyle &/*style*/) {
// 	Core::PlayEngine::updateSubtitleStyle(style);
// 	const double size = qBound(0.0, style.size*100.0, 100.0);
// 	d->engine->tellmp("sub_scale " + QString::number(size) + " 1");
}

void PlayEngine::updateSubtitleVisiblity(bool visible) {
	tellmp(visible ? "sub_visibility 1" : "sub_visibility 0");
}

int PlayEngine::toRealSubPos(double pos) const {
	const QRect r = d->renderer->osdRect();
	const double h = (double)r.bottom() + (double)r.top();
	return qBound(0, qRound(pos*((double)r.height() + h)/(2.*h)*100.), 100);
}

void PlayEngine::updateSubtitlePos(double pos) {
	tellmp("sub_pos", toRealSubPos(pos), 1);
}

void PlayEngine::updateSyncDelay(int delay) {
	tellmp("sub_delay", double(delay)/1000.0, 1);
}

void PlayEngine::updateCurrentSource(const Core::MediaSource &source) {
	if ((d->gotInfo = d->mediaInfo.get(source)))
		updateInfo();
	else
		setDuration(0);
}

bool PlayEngine::updateVideoRenderer(const QString &name) {
	const int idx = d->info.videoRenderer().indexOf(name);
	if (idx == -1)
		return false;
	d->vo = idx == 0 ? "" : name;
	if (isPlaying() || isPaused())
		replay();
	return true;
}

bool PlayEngine::updateAudioRenderer(const QString &name) {
	const int idx = d->info.audioRenderer().indexOf(name);
	if (idx == -1)
		return false;
	d->ao = idx == 0 ? "" : name;
	if (isPlaying() || isPaused())
		replay();
	return true;
}

bool PlayEngine::updateCurrentTrack(const QString &track) {
	return d->tracks.contains(track) && tellmp("switch_audio", d->tracks[track]);
}

// void PlayEngine::updateVideo() {}
// void PlayEngine::updateAudio() {}

}

