#include "videoplayer.h"
#include "pref.h"
#include "dummyengine.h"
#include "recentinfo.h"
#include <core/info.h>
#include <core/backendiface.h>
#include <QtGui/QStackedWidget>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QPluginLoader>

#define ENGINE_SET(setter, value) {d->dummy->setter((value)); if (d->engine != d->dummy) d->engine->setter((value));}
#define TO_PERCENT(rate) (qRound((rate)*100.0))
#define TO_RATE(percent) (static_cast<double>((percent))*0.01)
#define IS_DIFF_CLIP(val1, val2, min, max) ((val1) != (val2 = qBound((min), (val2), (max))))
#define DEC_ENGINE_CALL0(func) void VideoPlayer::func() {d->engine->func();}
#define DEC_ENGINE_CALL1(func, type) void VideoPlayer::func(type value) {d->engine->func(value);}
#define DEC_ENGINE_CALL_RETURN(ret, func) ret VideoPlayer::func() const {return d->engine->func();}
#define DEC_ENGINE_GETTER(type, getter) type VideoPlayer::getter() const {return d->dummy->getter();}
#define DEC_ENGINE_GETTER_RATE(getter) int VideoPlayer::getter() const {return TO_PERCENT(d->dummy->getter());}
#define DEC_ENGINE_SETTER(type, setter) void VideoPlayer::setter(type value) {ENGINE_SET(setter, value);}
#define DEC_ENGINE_SETTER_CHECK(type, setter, getter) \
void VideoPlayer::setter(type value) {if (d->dummy->getter() != value) ENGINE_SET(setter, value);}
#define DEC_ENGINE_SETTER_CHECK_CLIP(type, setter, getter, min, max) \
void VideoPlayer::setter(type value) {\
	if (IS_DIFF_CLIP(d->dummy->getter(), value, min, max)) ENGINE_SET(setter, value);}
#define DEC_ENGINE_SETTER_CHECK_CLIP_RATE(setter, getter, min, max) \
void VideoPlayer::setter(int value) {\
	if (IS_DIFF_CLIP(TO_PERCENT(d->dummy->getter()), value, min, max)) ENGINE_SET(setter, TO_RATE(value));}
#define DEC_ENGINE_PROP(type, setter, getter)\
DEC_ENGINE_SETTER(type, setter) DEC_ENGINE_GETTER(type, getter)
#define DEC_ENGINE_PROP_CHECK(type, setter, getter)\
DEC_ENGINE_SETTER_CHECK(type, setter, getter) DEC_ENGINE_GETTER(type, getter)
#define DEC_ENGINE_PROP_CHECK_CLIP(type, setter, getter, min, max) \
DEC_ENGINE_SETTER_CHECK_CLIP(type, setter, getter, min, max) DEC_ENGINE_GETTER(type, getter)
#define DEC_ENGINE_PROP_CHECK_CLIP_RATE(setter, getter, min, max) \
DEC_ENGINE_SETTER_CHECK_CLIP_RATE(setter, getter, min, max) DEC_ENGINE_GETTER_RATE(getter)

struct VideoPlayer::Backend {
	~Backend() {
		for (BackendMap::iterator it = map.begin(); it != map.end(); ++it)
			delete it.value();
	}
	BackendMap map;
	QSet<QString> file;
};

struct VideoPlayer::Data {
	QWidget *main;
	QStackedWidget *stack;
	QSize minSize, maxSize;
	bool keepSize, changing;
	QMap<QString, Core::PlayEngine*> engines;
	DummyEngine *dummy;
	Core::PlayEngine *engine;
	Core::MediaSource *next;
	static Backend backend;
};

VideoPlayer::Backend VideoPlayer::Data::backend;

VideoPlayer::VideoPlayer(QWidget *main, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->engine = 0;
	
	d->stack = new QStackedWidget(this);
	d->stack->resize(size());
	d->stack->move(0, 0);
	d->stack->setMouseTracking(true);

	d->main = main;
	d->dummy = new DummyEngine;
	d->minSize = minimumSize();
	d->maxSize = maximumSize();
	d->changing = d->keepSize = false;
	d->next = 0;
	
	setMinimumSize(320, 240);
	setMouseTracking(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(d->dummy, SIGNAL(currentSourceChanged(const Core::MediaSource&))
		, this, SIGNAL(currentSourceChanged(const Core::MediaSource&)));
	connect(d->dummy, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
	connect(d->dummy, SIGNAL(mutedChanged(bool)), this, SIGNAL(mutedChanged(bool)));
	
	d->dummy->widget()->setContextMenuPolicy(Qt::CustomContextMenu);	
	connect(d->dummy->widget(), SIGNAL(customContextMenuRequested(const QPoint&))
			, this, SIGNAL(customContextMenuRequested(const QPoint&)));
// 	connect(engine, SIGNAL(tracksChanged(const QStringList&))
// 			, this, SIGNAL(tracksChanged(const QStringList&)));
// 	connect(engine, SIGNAL(currentTrackChanged(const QString&))
// 			, this, SIGNAL(currentTrackChanged(const QString&)));
// 	connect(engine, SIGNAL(spusChanged(const QStringList&))
// 			, this, SIGNAL(spusChanged(const QStringList&)));
// 	connect(engine, SIGNAL(currentSpuChanged(const QString&))
// 			, this, SIGNAL(currentSpuChanged(const QString&)));
	d->stack->addWidget(d->dummy->widget());
	d->stack->setCurrentWidget(d->dummy->widget());
	
	d->engine = d->dummy;
}

VideoPlayer::~VideoPlayer() {
	QMap<QString, Core::PlayEngine*>::iterator it = d->engines.begin();
	for (; it != d->engines.end(); ++it)
		delete it.value();
	delete d->next;
	delete d;
}

void VideoPlayer::keepSize(bool keep) {
	if ((d->keepSize = keep)) {
		setFixedSize(size());
	} else {
		setMaximumSize(d->maxSize);
		setMinimumSize(d->minSize);
		resize();
	}
}

QSize VideoPlayer::sizeHint() const {
	QSize hint = d->stack->currentWidget()->sizeHint();
	if (hint.width() < d->minSize.width())
		hint.setWidth(d->minSize.width());
	if (hint.height() < d->minSize.height())
		hint.setHeight(d->minSize.height());
	return hint;
}

void VideoPlayer::resize() {
	d->stack->resize(d->main->isFullScreen() ? d->main->size() : size());
}

void VideoPlayer::resizeEvent(QResizeEvent */*event*/) {
	if (!d->keepSize)
		resize();
}

void VideoPlayer::setBackend(const QString &name) {
	if (d->engine->info().name() == name)
		return;
	int time = -1;
	d->changing = true;
	if (!d->engine->isStopped()) {
		time = d->engine->currentTime();
		d->engine->stop();
	}
	
	QMap<QString, Core::PlayEngine*>::iterator it = d->engines.find(name);
	if (it == d->engines.end()) {
		Core::BackendIface *backend = Data::backend.map.value(name, 0);
		if (!backend)
			return;
		Core::PlayEngine *engine = backend->createPlayEngine();
		engine->widget()->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(engine->widget(), SIGNAL(customContextMenuRequested(const QPoint&))
				, this, SIGNAL(customContextMenuRequested(const QPoint&)));
		connect(engine, SIGNAL(stateChanged(Core::State, Core::State))
				, this, SLOT(slotStateChanged(Core::State, Core::State)));
		connect(engine, SIGNAL(finished(Core::MediaSource))
				, this, SLOT(slotFinished(Core::MediaSource)));
		connect(engine, SIGNAL(stopped(Core::MediaSource, int))
				, this, SLOT(slotStopped(Core::MediaSource, int)));
		connect(engine, SIGNAL(started()), this, SIGNAL(started()));
		connect(engine, SIGNAL(durationChanged(int))
				, this, SIGNAL(durationChanged(int)));
		connect(engine, SIGNAL(seekableChanged(bool))
				, this, SIGNAL(seekableChanged(bool)));
		connect(engine, SIGNAL(tick(int)), this, SIGNAL(tick(int)));
		connect(engine, SIGNAL(tracksChanged(const QStringList&))
				, this, SIGNAL(tracksChanged(const QStringList&)));
		connect(engine, SIGNAL(currentTrackChanged(const QString&))
				, this, SIGNAL(currentTrackChanged(const QString&)));
		connect(engine, SIGNAL(spusChanged(const QStringList&))
				, this, SIGNAL(spusChanged(const QStringList&)));
		connect(engine, SIGNAL(currentSpuChanged(const QString&))
				, this, SIGNAL(currentSpuChanged(const QString&)));
		connect(engine, SIGNAL(snapshotTaken(const QImage&))
				, this, SIGNAL(snapshotTaken(const QImage&)));
		d->stack->addWidget(engine->widget());
		it = d->engines.insert(name, engine);
	}
	d->engine = it.value();
	if (d->engine != d->dummy) {
		d->engine->setSpeed(d->dummy->speed());
		d->engine->setMuted(d->dummy->isMuted());
		d->engine->setVolume(d->dummy->volume());
		d->engine->setAmplifyingRate(d->dummy->amplifyingRate());
		d->engine->setAspectRatio(d->dummy->aspectRatio());
		d->engine->setCropRatio(d->dummy->cropRatio());
		d->engine->setSubtitlePos(d->dummy->subtitlePos());
		d->engine->setSyncDelay(d->dummy->syncDelay());
		d->engine->setSubtitle(d->dummy->subtitle());
		d->engine->setSubtitleVisible(d->dummy->isSubtitleVisible());
		d->engine->setSubtitleStyle(d->dummy->subtitleStyle());
		d->engine->setCurrentSource(d->dummy->currentSource());
		d->engine->setVolumeNormalized(d->dummy->isVolumeNormalized());
		d->engine->setUseSoftwareEqualizer(d->dummy->useSoftwareEqualizer());
		d->engine->setColorProperty(d->dummy->colorProperty());
		d->dummy->info().copy(d->engine->info());
	}
	emit backendChanged(name);
	if (time != -1)
		play(time);
}

const VideoPlayer::Map &VideoPlayer::engines() const {
	return d->engines;
}

const Core::PlayEngine *VideoPlayer::engine() const {
	return d->engine;
}

Core::MediaSource VideoPlayer::nextSource() const {
	return d->next ? *d->next : Core::MediaSource();
}

void VideoPlayer::setNextSource(const Core::MediaSource &source) {
	if (source.isValid()) {
		if (!d->next)
			d->next = new Core::MediaSource(source);
		else if (*d->next != source)
			*d->next = source;
	} else {
		delete d->next;
		d->next = 0;
	}
}

bool VideoPlayer::hasNextSource() const {
	return d->next != 0;
}

Core::PlayEngine *VideoPlayer::osdEngine() {
	if (d->engine->widget() == d->stack->currentWidget())
		return d->engine;
	return d->dummy;
}

void VideoPlayer::showMessage(const QString &message) {
	osdEngine()->showMessage(message);
}

void VideoPlayer::stop() {
	d->engine->stop();
}

void VideoPlayer::play(int time) {
	d->engine->play(time);
}

void VideoPlayer::play() {
	d->engine->play();
}

void VideoPlayer::playNext(int time) {
	if (d->next) {
		d->changing = true;
		setCurrentSource(*d->next);
		delete d->next;
		d->next = 0;
		play(time);
	}
}

bool VideoPlayer::changingSource() const {
	return d->changing;
}

void VideoPlayer::slotStateChanged(Core::State state, Core::State old) {
	bool doEmit = true;
	if (state == Core::Playing) {
		d->changing = false;
		if (d->engine->hasVideo())
			d->stack->setCurrentWidget(d->engine->widget());
		else
			d->stack->setCurrentWidget(d->dummy->widget());
	} else if (state == Core::Finished && d->next) {
		playNext(RecentInfo::get()->stoppedTime(*d->next));
		doEmit = false;
	} else if (state == Core::Stopped) {
		if (d->changing) {
			d->changing = false;
			doEmit = false;
		} else
			d->stack->setCurrentWidget(d->dummy->widget());
	}
	if (doEmit)
		emit stateChanged(state, old);
}

void VideoPlayer::slotFinished(Core::MediaSource source) {
	RecentInfo::get()->setFinished(source);
	emit finished(source);
}

void VideoPlayer::slotStopped(Core::MediaSource source, int time) {
	RecentInfo::get()->setStopped(source, time);
	emit stopped(source, time);
}

void VideoPlayer::setCurrentSource(Core::MediaSource source) {
	const QString mrl = source.toMrl();
	if (d->dummy->currentSource() != source) {
		d->dummy->setCurrentSource(source);
		setBackend(Pref::get()->backendName(source.type()));
		d->engine->setCurrentSource(source);
	}
}

void VideoPlayer::seek(int time, bool relative, bool show) {
	if (show) {
		Core::PlayEngine *osd = osdEngine();
		if (osd != d->engine) {
			d->engine->seek(time, relative, false);
			osd->showTimeLine(d->engine->currentTime(), d->engine->duration(), 2000);
			return;
		}
	}
	d->engine->seek(time, relative, show);
}

void VideoPlayer::setColorProperty(Core::ColorProperty::Value prop, int value) {
	if (IS_DIFF_CLIP(TO_PERCENT(d->dummy->colorProperty()[prop]), value, -100, 100)) {
		d->dummy->setColorProperty(prop, TO_RATE(value));
		d->engine->setColorProperty(prop, TO_RATE(value));
	}
}

int VideoPlayer::colorProperty(Core::ColorProperty::Value prop) const {
	return TO_PERCENT(d->dummy->colorProperty()[prop]);
}

const QString &VideoPlayer::videoRenderer() const {
	return d->engine->videoRenderer();
}

const QString &VideoPlayer::audioRenderer() const {
	return d->engine->audioRenderer();
}

bool VideoPlayer::setVideoRenderer(const QString &renderer) {
	if (d->engine == d->dummy)
		return false;
	d->changing = true;
	return d->engine->setVideoRenderer(renderer);
}

bool VideoPlayer::setAudioRenderer(const QString &renderer) {
	if (d->engine == d->dummy)
		return false;
	d->changing = true;
	return d->engine->setAudioRenderer(renderer);
}

DEC_ENGINE_PROP_CHECK(int, setSyncDelay, syncDelay)
DEC_ENGINE_PROP_CHECK(double, setAspectRatio, aspectRatio)
DEC_ENGINE_PROP_CHECK(double, setCropRatio, cropRatio)
DEC_ENGINE_PROP_CHECK(bool, setMuted, isMuted)
DEC_ENGINE_PROP_CHECK_CLIP(int, setVolume, volume, 0, 100)
DEC_ENGINE_PROP_CHECK_CLIP_RATE(setSpeed, speed, 10, 1000)
DEC_ENGINE_PROP_CHECK_CLIP_RATE(setSubtitlePos, subtitlePos, 0, 100)
DEC_ENGINE_PROP_CHECK_CLIP_RATE(setAmplifyingRate, amplifyingRate, 10, 1000)

DEC_ENGINE_GETTER(Core::MediaSource, currentSource)
DEC_ENGINE_SETTER(const Core::OsdStyle &, setSubtitleStyle)
DEC_ENGINE_SETTER(const Core::Subtitle &, setSubtitle)
DEC_ENGINE_SETTER(const QString &, setCurrentTrack)
DEC_ENGINE_SETTER(const QString &, setCurrentSpu)
DEC_ENGINE_SETTER_CHECK(bool, setVolumeNormalized, isVolumeNormalized)
DEC_ENGINE_SETTER_CHECK(bool, setSubtitleVisible, isSubtitleVisible)
DEC_ENGINE_SETTER_CHECK(bool, setUseSoftwareEqualizer, useSoftwareEqualizer);

DEC_ENGINE_CALL0(triggerSnapshot)
DEC_ENGINE_CALL0(pause)
DEC_ENGINE_CALL0(toggleDvdMenu)

DEC_ENGINE_CALL_RETURN(bool, isPlaying)
DEC_ENGINE_CALL_RETURN(bool, isPaused)
DEC_ENGINE_CALL_RETURN(bool, isStopped)
DEC_ENGINE_CALL_RETURN(bool, isSeekable)
DEC_ENGINE_CALL_RETURN(int, duration)
DEC_ENGINE_CALL_RETURN(int, currentTime)
DEC_ENGINE_CALL_RETURN(bool, hasVideo);
DEC_ENGINE_CALL_RETURN(Core::State, state)
DEC_ENGINE_CALL_RETURN(Core::ABRepeater*, repeater)
DEC_ENGINE_CALL_RETURN(double, frameRate);

#undef ENGINE_SET
#undef TO_PERCENT
#undef TO_RATE
#undef IS_DIFF_CLIP
#undef DEC_ENGINE_CALL0
#undef DEC_ENGINE_CALL1
#undef DEC_ENGINE_CALL_RETURN
#undef DEC_ENGINE_GETTER
#undef DEC_ENGINE_GETTER_RATE
#undef DEC_ENGINE_SETTER
#undef DEC_ENGINE_SETTER_CHECK
#undef DEC_ENGINE_SETTER_CHECK_CLIP
#undef DEC_ENGINE_SETTER_CHECK_CLIP_RATE
#undef DEC_ENGINE_PROP
#undef DEC_ENGINE_PROP_CHECK
#undef DEC_ENGINE_PROP_CHECK_CLIP
#undef DEC_ENGINE_PROP_CHECK_CLIP_RATE

const BackendMap &VideoPlayer::load(const QString &path) {
	QDir dir(path.isEmpty() ? Core::Info::pluginPath() : path);
	if (dir.exists()) {
		static QRegExp rxFilter("^libcmplayer_engine_(.+).so$");
		const QFileInfoList file = dir.entryInfoList(QDir::Files);
		for (int i=0; i<file.size(); ++i) {
			if (rxFilter.indexIn(file[i].fileName()) == -1)
				continue;
			QPluginLoader loader(file[i].absoluteFilePath());
			if (!loader.load()) {
				qFatal("%s", qPrintable(loader.errorString()));
				continue;
			}
			Core::BackendIface *backend = 0;
			backend = qobject_cast<Core::BackendIface*>(loader.instance());
			if (!backend)
				continue;
			const QString name = backend->info()->name();
			if (!Data::backend.map.contains(name))
				Data::backend.map[name] = backend;
		}
	}
	return Data::backend.map;
}

const BackendMap &VideoPlayer::backend() {
	return Data::backend.map;
}
