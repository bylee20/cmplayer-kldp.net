//#include "playengine.h"
//#include "mediasource.h"
//#include "videooutput.h"
//#include "audiooutput.h"
//#include "subtitleoutput.h"
//#include "informations.h"
//#include "abrepeater.h"
//#include "mediainfo.h"
//#include "mplayerprocess.h"
//#include "dvdinfo.h"
//#include <QUrl>
//#include <QFile>
//#include <QSize>
//#include <QDebug>
//
//namespace MPlayer {
//
//struct PlayEngine::Data {
//	Data(PlayEngine *parent);
//	void init();
//	void updateInfo();
//public:
//	PlayEngine *p;
//	MPlayerProcess *proc;
//	VideoOutput *video;
//	AudioOutput *audio;
//	SubtitleOutput *subout;
//	ABRepeater *repeater;
//	State state;
//	qint64 curTime;
//	double speed;
//	bool justFinished, noVideo, playStopped;
//	const QString dontmessup;
//	QStringList options;
//	MediaSource source;
//	int osdLevel;
//};
//
//PlayEngine::Data::Data(PlayEngine *p)
//: p(p), proc(new MPlayerProcess(p)), video(0), audio(0), subout(0)
//, repeater(new ABRepeater(p)), state(StoppedState), curTime(0)
//, speed(1.0), justFinished(false), noVideo(true), playStopped(false)
//, dontmessup(Informations::get()->privatePath() +"/input.conf")
//, source(), osdLevel(0) {}
//
//void PlayEngine::Data::init() {
//	connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), p, SLOT(slotProcFinished()));
//	connect(p, SIGNAL(stateChanged(MPlayer::State, MPlayer::State)),
//			p, SLOT(slotStateChanged(MPlayer::State, MPlayer::State)));
//	connect(p, SIGNAL(started()), p, SLOT(update()));
//}
//
//void PlayEngine::setState(State state) {
//	if(d->state != state) {
//		State old = d->state;
//		emit stateChanged(d->state = state, old);
//	}
//}
//
//void PlayEngine::Data::updateInfo() {
//	const MediaInfo &info = source.info();
//	if (info.isValid()) {
//		emit p->totalTimeChanged(info.length());
//		if (info.hasVideo() && video)
//			video->setVideoSize(info.videoSize());
//	}
//}
//
//PlayEngine::PlayEngine(QObject *parent)
//: Controller(parent), d(new Data(this)) {
//	d->init();
//	QFile file(d->dontmessup);
//	if (!file.exists() && file.open(QFile::WriteOnly)) {
//		file.write(
//			"## prevent mplayer from messing up our shortcuts\n\n"
//			"RIGHT gui_about\nLEFT gui_about\nDOWN gui_about\n"
//			"UP gui_about\nPGUP gui_about\nPGDWN gui_about\n"
//			"- gui_about\n+ gui_about\nESC gui_about\nENTER gui_about\n"
//			"SPACE pausing_keep invalid_command\nHOME gui_about\n"
//			"END gui_about\n> gui_about\n< gui_about\nINS gui_about\n"
//			"DEL gui_about\n[ gui_about\n] gui_about\n{ gui_about\n"
//			"} gui_about\nBS gui_about\nTAB gui_about\n. gui_about\n"
//			"# gui_about\n@ gui_about\n! gui_about\n9 gui_about\n"
//			"/ gui_about\n0 gui_about\n* gui_about\n1 gui_about\n"
//			"2 gui_about\n3 gui_about\n4 gui_about\n5 gui_about\n"
//			"6 gui_about\n7 gui_about\n8 gui_about\na gui_about\n"
//			"b gui_about\nc gui_about\nd gui_about\ne gui_about\n"
//			"F invalid_command\nf invalid_command\ng gui_about\n"
//			"h gui_about\ni gui_about\nj gui_about\nk gui_about\n"
//			"l gui_about\nm gui_about\nn gui_about\no gui_about\n"
//			"p gui_about\nq gui_about\nr gui_about\ns gui_about\n"
//			"t gui_about\nT gui_about\nu gui_about\nv gui_about\n"
//			"w gui_about\nx gui_about\ny gui_about\nz gui_about\n"
//			"S gui_about\n"
//		);
//	}
//}
//
//PlayEngine::~PlayEngine() {
//	delete d;
//}
//
//void PlayEngine::slotStateChanged(MPlayer::State /*newState*/, MPlayer::State /*oldState*/) {
//	emit seekableChanged(!isStopped());
//}
//
//void PlayEngine::slotProcFinished() {
//	setState(StoppedState);
//	if (d->justFinished) {
//		emit finished();
//		d->justFinished = false;
//	}
//}
//
//void PlayEngine::stop() {
//	qint64 time = d->curTime;
//	if (d->state == StoppedState || !tellmp("quit"))
//		return;
//	emit stopped(time);
//	if (!d->proc->waitForFinished(5000))
//		d->proc->kill();
//}
//
//void PlayEngine::seek(qint64 time, bool relative) {
//	tellmp("seek " + QString::number(static_cast<double>(time)/1000) + (relative ? " 0" : " 2"));
//}
//
//void PlayEngine::replay() {
//	qint64 time = d->curTime;
//	stop();
//	start(time);
//}
//
//bool PlayEngine::start(qint64 time) {
//	if (isRunning())
//		stop();
//	if (!d->source.isValid())
//		return false;
//	if ((d->source.isLocalFile() || d->source.isDisc()) && !d->source.gotInfo() && d->source.getInfo())
//		d->updateInfo();
//	QStringList args;
//	args << "-slave" << "-noquiet" << "-nofs" << "-input" << "conf=\"" + d->dontmessup + '"'
//		<< "-fontconfig" << "-zoom" << "-nokeepaspect"// << "-noautosub"
//		<< "-osdlevel" << QString::number(d->osdLevel);
//	if (!d->source.gotInfo())
//		args << "-identify";
//	if (d->video) {
//		args << "-wid" << QString::number(d->video->videoWID());
//		if (d->video->isSoftwareEqualizerEnabled())
//			args << "-vf-add" << "eq2";
//		if (!d->video->driver().isEmpty())
//			args << "-vo" << d->video->driver();
//		if (d->source.gotInfo() && d->video->monitorRatio() < d->source.info().aspectRatio()) {
//			args << "-vf-add" << "expand=:::::" + QString::number(d->video->monitorRatio());
//			d->video->setExpanded(true);
//		} else
//			d->video->setExpanded(false);
//	} else
//		args << "-vo" << "null";
//	if (d->audio) {
//		if (!d->audio->driver().isEmpty())
//			args << "-ao" << d->audio->driver();
//		if (d->audio->isEnabledSoftwareVolume())
//			args << "-softvol" << "-softvol-max" << QString::number(d->audio->volumeAmplification());
//	} else
//		args << "-ao" << "null";
//	if (d->subout) {
//		SubtitleOutput::Font font = d->subout->font();
//		if (!font.family.isEmpty())
//			args << "-font" << font.family;
//		if (!d->subout->encoding().isEmpty())
//			args << "-subcp" << d->subout->encoding();
//		args << "-subfont-autoscale" << QString::number(d->subout->autoScale());
//	}
//	//if (general.autoPitch)
//	//	args << "-af" << "scaletempo";
//	if (time > 1000)
//		args << "-ss" << QString::number(static_cast<double>(time)/1000.0);
//
//	args += d->options;
//
//	args << d->source.url().toString();
//
//	qDebug("%s %s", qPrintable(Informations::get()->mplayerPath()), qPrintable(args.join(" ")));
//	d->proc->start(Informations::get()->mplayerPath(), args);
//	if (!d->proc->waitForStarted())
//		return false;
//	return true;
//}
//
//void PlayEngine::setCurrentSource(const MediaSource &source) {
//	if (!isStopped())
//		stop();
//	if (d->source != source) {
//		emit currentSourceChanged(d->source = source);
//		if (d->source.gotInfo())
//			d->updateInfo();
//		else
//			emit totalTimeChanged(0);
//	}
//}
//
//bool PlayEngine::tellmp(const QString &command) {
//	if (isRunning()) {
//		d->proc->write(command.toLocal8Bit() + "\n");
//		qDebug("told: %s", qPrintable(command));
//		return true;
//	} else
//		qDebug("couldn't tell: %s", qPrintable(command));
//	return false;
//}
//
//void PlayEngine::play() {
//	if (isStopped()) {
//		start();
//	} else if (isPaused())
//		tellmp("pause");
//}
//
//bool PlayEngine::isRunning() {
//	return d->proc->state() != QProcess::NotRunning;
//}
//
//void PlayEngine::setSpeed(double speed) {
//	if (d->speed != speed) {
//		emit speedChanged(d->speed = qBound(0.1, speed, 100.0));
//		tellmp("speed_set " + QString::number(d->speed));
//	}
//}
//
//void PlayEngine::link(Controller *controller) {
//	VideoOutput *video = qobject_cast<VideoOutput*>(controller);
//	if (video) {
//		if (d->video)
//			unlink(d->video);
//		d->video = video;
//		return;
//	}
//	AudioOutput *audio = qobject_cast<AudioOutput*>(controller);
//	if (audio) {
//		if (d->audio)
//			unlink(d->audio);
//		d->audio = audio;
//		return;
//	}
//	SubtitleOutput *subout = qobject_cast<SubtitleOutput*>(controller);
//	if (subout) {
//		if (d->subout)
//			unlink(d->subout);
//		d->subout = subout;
//		return;
//	}
//}
//
//void PlayEngine::unlink(Controller *controller) {
//	if (d->video && d->video == controller) {
//		d->video = 0;
//	} else if (d->audio && d->audio == controller) {
//		d->audio = 0;
//	} else if (d->subout && d->subout == controller)
//		d->subout = 0;
//}
//
//double PlayEngine::speed() const {
//	return d->speed;
//}
//
//const MediaSource &PlayEngine::currentSource() const {
//	return d->source;
//}
//
//qint64 PlayEngine::currentTime() const {
//	return d->curTime;
//}
//
//bool PlayEngine::hasVideo() const {
//	return !d->noVideo;
//}
//
//bool PlayEngine::isSeekable() const {
//	return d->state != StoppedState;
//}
//
//qint64 PlayEngine::remainingTime() const {
//	return d->source.info().length() - d->curTime;
//}
//
//State PlayEngine::state() const {
//	return d->state;
//}
//
//qint32 PlayEngine::tickInterval() const {
//	return 100;
//}
//
//qint64 PlayEngine::totalTime() const {
//	return d->source.info().length();
//}
//
//VideoOutput *PlayEngine::videoOutput() const {
//	return d->video;
//}
//
//SubtitleOutput *PlayEngine::subtitleOutput() const {
//	return d->subout;
//}
//
//AudioOutput *PlayEngine::audioOutput() const {
//	return d->audio;
//}
//
//void PlayEngine::setOptions(const QStringList &options) {
//	d->options = options;
//}
//
//const QStringList &PlayEngine::options() const {
//	return d->options;
//}
//
//ABRepeater *PlayEngine::repeater() const {
//	return d->repeater;
//}
//
//void PlayEngine::update() {
//	if (isRunning())
//		tellmp("speed_set " + QString::number(d->speed));
//}
//
//void PlayEngine::setOSDLevel(int level) {
//	int temp = qBound(0, level, 3);
//	if (temp != d->osdLevel) {
//		d->osdLevel = level;
//		tellmp("osd " + QString::number(d->osdLevel));
//	}
//}
//
//int PlayEngine::osdLevel() const {
//	return d->osdLevel;
//}
//
//void PlayEngine::setCurrentTime(qint64 time) {
//	emit tick(d->curTime = time);
//}
//
//void PlayEngine::exiting() {
//	emit aboutToFinished();
//	d->justFinished = true;
//}
//
//void PlayEngine::seekChapter(int n) {
//	if (d->source.isDisc() && d->source.gotInfo()) {
//		int title = d->source.titleNumber();
//		if (title > 0 && title <= d->source.info().dvd().titles.size() && n > 0
//				&& n <= d->source.info().dvd().titles[title-1].chapters.size())
//			seek(d->source.info().dvd().titles[title-1].chapters[n-1], false);
//	}
//}
//
//void PlayEngine::playTitle(int number) {
//	if (d->source.isDisc() && d->source.gotInfo()
//			&& number > 0 && number <= d->source.info().dvd().titles.size()) {
//		stop();
//		setCurrentSource(MediaSource(QUrl("dvd://" + QString::number(number))));
//		play();
//	}
//}
//
//}
