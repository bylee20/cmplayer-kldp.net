#include "playengine.h"
#include "info.h"
#include "config.h"
#include "mediainfo.h"
#include "mplayerprocess.h"
#include <core/subtitle.h>
#include <core/utility.h>
#include <core/osdstyle.h>
#include <core/nativerenderer.h>
#include <core/baseevent.h>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QTemporaryFile>
#include <QtCore/QThread>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtCore/QVariant>

namespace MPlayer {

class SnapshotEvent : public Core::BaseEvent {
public:
	static const int Type = BaseEvent::UserType + 1;
	SnapshotEvent(): Core::BaseEvent(Type) {}
	QImage snapshot;
};
	
class PlayEngine::Thread : public QThread {
public:
	Thread() {
		quit = false;
	}
	~Thread() {
		QFile::remove(this->file);
	}
	PlayEngine *engine;
	QString dir;
	bool quit;
	void stop() {
		if (isRunning()) {
			quit = true;
			if (!wait(30000))
				terminate();
		}
	}
	void getSnapshot(const QString &file) {
		stop();
		quit = false;
		if (this->file != file) {
			QFile::remove(this->file);
			this->file = file;
		}
		start();
	}
private:
	void run() {
		SnapshotEvent *event = new SnapshotEvent;
		for (int i = 0; i<1000 && !quit && event->snapshot.isNull(); ++i) {
			msleep(60);
			event->snapshot = QImage(file);
		}
		if (!quit)
			QApplication::postEvent(engine, event);
	}
	QString file;
};

struct PlayEngine::Data {
	Data(): tempsub(getTempSub()) {}
	static QString getTempSub() {
		return Info::privatePath() + "/cmplayer-mplayer-temp.smi";
	}
	Info info;
	MediaInfo mediaInfo;
	MPlayerProcess *proc;
	bool justFinished, gotInfo, seeking, needToUpdateMuted, needToUpdateSub;
	const QString tempsub;
	int osdLevel;
	Core::NativeRenderer *renderer;
	QString vo, ao, snapshot;
	QMap<QString, int> tracks;
	Thread thread;
	QMap<QString, QString> cmd;
};

PlayEngine::PlayEngine(QObject *parent)
: Core::PlayEngine(parent), d(new Data) {
	d->proc = new MPlayerProcess(this);
	d->proc->setWorkingDirectory(Info::privatePath());
	d->seeking = d->justFinished = d->gotInfo = false;
	d->osdLevel = m_time = 0;
	d->renderer = new Core::NativeRenderer(this);
	d->thread.engine = this;
	d->needToUpdateSub = d->needToUpdateMuted = false;
	setVideoRenderer(d->renderer);

	connect(d->proc, SIGNAL(finished(int, QProcess::ExitStatus))
			, this, SLOT(slotProcFinished()));
	connect(d->proc, SIGNAL(gotSnapshot(const QString&))
			, this, SLOT(slotGotSnapshot(const QString&)));
	connect(this, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(d->renderer, SIGNAL(osdRectChanged(const QRect&))
			, this, SLOT(slotOsdRectChanged()));
}

PlayEngine::~PlayEngine() {
	stop();
	d->thread.stop();
	d->renderer->hide();
	setVideoRenderer(0);
	delete d->renderer;
	delete d->proc;
	delete d;
}

const QStringList &PlayEngine::getDefaultArgs() {
	static const QStringList args = QStringList()
			<< "-slave" << "-noquiet" << "-nofs" << "-nomouseinput"
			<< "-input" << ("conf=\"" + getDontMessUp() + '"')
			<< "-fontconfig" << "-zoom" << "-nokeepaspect"
			<< "-noautosub" << "-osdlevel" << QString::number(0)
			<< "-utf8" << "-subcp" << "UFT-8" << "-softvol"
			<< "-softvol-max" << QString::number(1000.0)
			<< "-vf-add" << "screenshot";
	
	return args;
}

const QString &PlayEngine::getDontMessUp() {
	static QString fileName;
	if (fileName.isEmpty()) {
		fileName = Info::privatePath() + "/input2.conf";
		QFile file(fileName);
		if (!file.exists() && file.open(QFile::WriteOnly)) {
			file.write(// from smplayer
				"## prevent mplayer from messing up our shortcuts\n\n"
				"RIGHT invalid_command\nLEFT invalid_command\n"
				"DOWN invalid_command\nUP invalid_command\n"
				"PGUP invalid_command\nPGDWN invalid_command\n"
				"- invalid_command\n+ invalid_command\n"
				"ESC invalid_command\nENTER invalid_command\n"
				"SPACE pausing_keep invalid_command\nHOME invalid_command\n"
				"END invalid_command\n> invalid_command\n< invalid_command\n"
				"INS invalid_command\nDEL invalid_command\n[ invalid_command\n"
				"] invalid_command\n{ invalid_command\n} invalid_command\n"
				"BS invalid_command\nTAB invalid_command\n. invalid_command\n"
				"# invalid_command\n@ invalid_command\n! invalid_command\n"
				"9 invalid_command\n/ invalid_command\n0 invalid_command\n"
				"* invalid_command\n1 invalid_command\n2 invalid_command\n"
				"3 invalid_command\n4 invalid_command\n5 invalid_command\n"
				"6 invalid_command\n7 invalid_command\n8 invalid_command\n"
				"a invalid_command\nb invalid_command\nc invalid_command\n"
				"d invalid_command\ne invalid_command\nF invalid_command\n"
				"f invalid_command\ng invalid_command\nh invalid_command\n"
				"i invalid_command\nj invalid_command\nk invalid_command\n"
				"l invalid_command\nm invalid_command\nn invalid_command\n"
				"o invalid_command\np invalid_command\nq invalid_command\n"
				"r invalid_command\ns invalid_command\nt invalid_command\n"
				"T invalid_command\nu invalid_command\nv invalid_command\n"
				"w invalid_command\nx invalid_command\ny invalid_command\n"
				"z invalid_command\nS invalid_command\n"
			);
		}
	}
	return fileName;
}

void PlayEngine::updateInfo() {
	if (d->mediaInfo.isValid()) {
		const double oldFrameRate = d->mediaInfo.frameRate();
		setFrameRate(d->mediaInfo.frameRate());
		if (qAbs(oldFrameRate - d->mediaInfo.frameRate()) < 1.0e-5)
			updateSubtitle(subtitle());
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

void PlayEngine::slotStateChanged(Core::State state, Core::State /*old*/){
	setSeekable(!isStopped());
	if (state == Core::Playing)
		doCommands();
}

void PlayEngine::slotProcFinished() {
	if (d->justFinished) {
		emit finished(currentSource());
		setState(Core::Finished);
		d->justFinished = false;
	} else
		setState(Core::Stopped);
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
	if ((source.isLocalFile() || source.isDisc())
			&& !d->gotInfo && (d->gotInfo = d->mediaInfo.get(source)))
		updateInfo();
	int mode = 1;
	if (subtitleStyle().scale == Core::OsdStyle::FitToDiagonal)
		mode = 3;
	else if (subtitleStyle().scale == Core::OsdStyle::FitToWidth)
		mode = 2;
	QStringList args = getDefaultArgs();
	args << "-font" << subtitleStyle().font.family()
			<< "-subfont-autoscale" << QString::number(mode)
			<< "-subfont-osd-scale" << QString::number(messageStyle().textSize*100.0)
			<< "-subfont-text-scale" << QString::number(subtitleStyle().textSize*100.0)
			<< "-subdelay" << QString::number(-syncDelay()*0.001)
			<< "-wid" << QString::number(d->renderer->screenWinId())
			<< "-speed" << QString::number(speed());
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
	if (useSoftwareEqualizer() && d->info.videoFilters().contains("eq2"))
		args << "-vf-add" << "eq2";
	if (time > 1000)
		args << "-ss" << QString::number(double(time)/1000.);
// 	if (!d->config.options().isEmpty())
// 		args += d->config.options();
	args << (source.isDisc() ? "dvd://" : source.url().toString());
	qDebug("%s %s", "mplayer", qPrintable(args.join(" ")));
	d->proc->start("mplayer", args);
	if (d->proc->waitForStarted()) {
		updateSubtitle(subtitle());
		updateVolume();
		updateCurrentTrack(currentTrack());
		updateColorProperty();
		updateSubtitleVisiblity(isSubtitleVisible());
		updateSubtitlePos(subtitlePos());
		return true;
	} else
		return false;
}

bool PlayEngine::enqueueCommand(const QString &cmd, const QString &full) {
	return isPlaying() ? false : (d->cmd[cmd] = full, true);
}

void PlayEngine::doCommands() {
	QMap<QString, QString>::const_iterator it = d->cmd.begin();
	for (; it != d->cmd.end(); ++it)
		tellmp(it.value());
	d->cmd.clear();
	if (d->needToUpdateMuted)
		updateMuted();
	if (d->needToUpdateSub)
		applySubtitle(subtitle());
}

bool PlayEngine::tellmp(const QString &cmd) {
	if (d->proc->isRunning()) {
		d->proc->write(cmd.toLocal8Bit() + "\n");
		qDebug("told: %s", qPrintable(cmd));
		return true;
	} else
		qDebug("couldn't tell: %s", qPrintable(cmd));
	return false;
}

bool PlayEngine::tellmp1(const QString &cmd, const QVariant &value, bool enqueue) {
	QString command = cmd;
	command += " ";
	command += value.toString();
	return (enqueue && enqueueCommand(cmd, command)) ? true : tellmp(command);
}

bool PlayEngine::tellmp2(const QString &cmd, const QVariant &value
		, const QVariant &option, bool enqueue) {
	QString command = cmd;
	command += " ";
	command += value.toString();
	command += " ";
	command += option.toString();
	return (enqueue && enqueueCommand(cmd, command)) ? true : tellmp(command);
}

void PlayEngine::slotOsdRectChanged() {
	updateSubtitlePos(subtitlePos());
}

void PlayEngine::update() {

}

void PlayEngine::setOsdLevel(int level) {
	tellmp1("osd", qBound(0, level, 3));
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

void PlayEngine::seek(int time, bool relative, bool showTimeLine, int /*timeout*/) {
	if (showTimeLine)
		setOsdLevel(1);
	if (!d->seeking) {
		d->seeking = true;
		tellmp2("seek", time*0.001, relative ? 0 : 2);
		d->seeking = false;
	}
	if (showTimeLine)
		setOsdLevel(0);
}

void PlayEngine::seek(int time) {
	if (!d->seeking) {
		d->seeking = true;
		tellmp2("seek", time*0.001, 2);
		d->seeking = false;
	}
}

void PlayEngine::showMessage(const QString &message, int duration) {
	if (isPlaying())
		tellmp2("osd_show_text", "\"" + message + "\"", QString::number(duration));
}

void PlayEngine::showTimeLine(int time, int duration, int timeout) {
	QString text = Core::Utility::msecsToString(time);
	text += "/";
	text += Core::Utility::msecsToString(duration);
	showMessage(text, timeout);
}

void PlayEngine::updateMuted() {
	d->needToUpdateMuted = !tellmp1("mute",  isMuted() ? 1 : 0);
}

void PlayEngine::updateVolume() {
	tellmp2("volume", realVolume()*10.0, 1, true);
	if (isPlaying())
		updateMuted();
	else
		d->needToUpdateMuted = true;
}

void PlayEngine::updateSpeed(double speed) {;
	tellmp1("speed_set", speed, true);
}

void PlayEngine::updateColorProperty(Core::ColorProperty::Value prop, double value) {
	const int temp = qBound(-100, qRound(value*100.), 100);
	switch(prop) {
	case Core::ColorProperty::Brightness:
		tellmp2("brightness", temp, 1, true);
		break;
	case Core::ColorProperty::Contrast:
		tellmp2("contrast", temp, 1, true);
		break;
	case Core::ColorProperty::Saturation:
		tellmp2("saturation", temp, 1, true);
		break;
	case Core::ColorProperty::Hue:
		tellmp2("hue", temp, 1, true);
		break;
	default:
		break;
	}
}

void PlayEngine::updateColorProperty() {
	const Core::ColorProperty &prop = colorProperty();
	tellmp2("brightness", qBound(-100, qRound(prop.brightness()*100.), 100), 1, true);
	tellmp2("contrast", qBound(-100, qRound(prop.contrast()*100.), 100), 1, true);
	tellmp2("saturation", qBound(-100, qRound(prop.saturation()*100.), 100), 1, true);
	tellmp2("hue", qBound(-100, qRound(prop.hue()*100.), 100), 1, true);
}

void PlayEngine::updateSubtitle(const Core::Subtitle &sub) {
	if (isPlaying())
		applySubtitle(sub);
	else
		d->needToUpdateSub = true;
}

void PlayEngine::applySubtitle(const Core::Subtitle &sub) {
	d->needToUpdateSub = !(tellmp("sub_select -1") && tellmp("sub_remove")
			&& sub.save(d->tempsub, "UTF-8", frameRate())
			&& tellmp("sub_load \"" + d->tempsub + '\"')
			&& tellmp("sub_select 0"));
}

void PlayEngine::updateSubtitleStyle(const Core::OsdStyle &/*style*/) {
// 	Core::PlayEngine::updateSubtitleStyle(style);
// 	const double size = qBound(0.0, style.size*100.0, 100.0);
// 	d->engine->tellmp("sub_scale " + QString::number(size) + " 1");
}

void PlayEngine::updateSubtitleVisiblity(bool visible) {
	tellmp(visible ? " 1" : "sub_visibility 0");
}

int PlayEngine::toRealSubPos(double pos) const {
	const QRect r = d->renderer->osdRect();
	const double h = (double)r.bottom() + (double)r.top();
	return qBound(0, qRound(pos*((double)r.height() + h)/(2.*h)*100.), 100);
}

void PlayEngine::updateSubtitlePos(double pos) {
	tellmp2("sub_pos", toRealSubPos(pos), 1, true);
}

void PlayEngine::updateSyncDelay(int delay) {
	tellmp2("sub_delay", -double(delay)*0.001, 1, true);
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
	return d->tracks.contains(track) && tellmp1("switch_audio", d->tracks[track]);
}

void PlayEngine::triggerSnapshot() {
	tellmp1("screenshot", 0);
}

void PlayEngine::slotGotSnapshot(const QString &file) {
	d->thread.getSnapshot(d->proc->workingDirectory() + '/' + file);
}

void PlayEngine::customEvent(QEvent *event) {
	if (static_cast<Core::BaseEvent*>(event)->type() == SnapshotEvent::Type)
		emit snapshotTaken(static_cast<SnapshotEvent*>(event)->snapshot);
	else
		Core::PlayEngine::customEvent(event);
}

// void PlayEngine::updateVideo() {}
// void PlayEngine::updateAudio() {}

}

