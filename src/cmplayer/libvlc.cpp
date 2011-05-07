#include "audiocontroller.hpp"
#include "info.hpp"
#include "videorenderer.hpp"
#include "videoframe.hpp"
#include "playengine.hpp"
#include "libvlc.hpp"
#include <QtCore/QDebug>

struct LibVLC::Data {
	PlayEngine *engine;
	AudioController *audio;
	VideoScene *video;
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	VideoUtil vUtil;
	AudioUtil aUtil;
};

LibVLC::Data *LibVLC::d = 0;

void LibVLC::init() {
	Q_ASSERT(d == 0);
	d = new Data;

	d->vUtil.vd = 0;
	d->aUtil.af = 0;
	d->aUtil.scaletempo_enabled = 0;

#define PTR_TO_ARG(ptr) (QByteArray().setNum((qint64)(qptrdiff)(void*)(ptr)))
	QList<QByteArray> args;
	args << "-I" << "dummy" << "--ignore-config" << "--extraintf=logger"
		<< "--quiet"
//		<< "--verbose=2"
#ifndef Q_WS_X11
		<< "--no-xlib"
#endif
		<< "--reset-plugins-cache" << "--plugin-path" << Info::pluginPath()
		<< "--no-media-library" << "--no-osd" << "--no-sub-autodetect-file"
		<< "--no-stats" << "--no-video-title-show" << "--album-art=0"
		<< "--cmplayer-vout-chroma" << "AUTO"
		<< "--cmplayer-vout-cb-lock" << PTR_TO_ARG(cbVideoLock)
		<< "--cmplayer-vout-cb-unlock" << PTR_TO_ARG(cbVideoUnlock)
		<< "--cmplayer-vout-cb-display" << PTR_TO_ARG(cbVideoDisplay)
		<< "--cmplayer-vout-cb-render" << PTR_TO_ARG(cbVideoRender)
		<< "--cmplayer-vout-cb-prepare" << PTR_TO_ARG(cbVideoPrepare)
		<< "--cmplayer-vout-data" << PTR_TO_ARG(d)
		<< "--cmplayer-vout-util" << PTR_TO_ARG(&d->vUtil)
		<< "--vout" << "cmplayer-vout"
		<< "--cmplayer-vfilter-data" << PTR_TO_ARG(d)
		<< "--cmplayer-vfilter-cb-process" << PTR_TO_ARG(cbVideoProcess)
		<< "--video-filter" << "cmplayer-vfilter"
		<< "--cmplayer-afilter-cb-prepare" << PTR_TO_ARG(cbAudioPrepare)
		<< "--cmplayer-afilter-cb-process" << PTR_TO_ARG(cbAudioProcess)
		<< "--cmplayer-afilter-data" << PTR_TO_ARG(d)
		<< "--cmplayer-afilter-util" << PTR_TO_ARG(&d->aUtil)
		<< "--audio-filter"<< "cmplayer-afilter";
	const QByteArray add = qgetenv("CMPLAYER_VLC_OPTION");
	if (!add.isEmpty())
		args.append(add.split(','));
	qDebug() << "Iniitalize vlc with:";
	qDebug() << args;

	QVector<const char*> argv(args.size());
	for (int i=0; i<argv.size(); ++i)
		argv[i] = args[i].constData();
	d->inst = libvlc_new(argv.count(), argv.constData());
	d->mp = libvlc_media_player_new(d->inst);
	libvlc_video_set_mouse_input(d->mp, 0);
	libvlc_event_manager_t *man = libvlc_media_player_event_manager(d->mp);
	libvlc_event_type_t events[] = {
		libvlc_MediaPlayerTimeChanged,
		libvlc_MediaPlayerTitleChanged,
		libvlc_MediaPlayerSeekableChanged,
		libvlc_MediaPlayerLengthChanged,
		libvlc_MediaPlayerPlaying,
		libvlc_MediaPlayerPaused,
		libvlc_MediaPlayerEndReached,
		libvlc_MediaPlayerOpening,
		libvlc_MediaPlayerBuffering,
		libvlc_MediaPlayerStopped,
		libvlc_MediaPlayerEncounteredError,
	};
	int evCount = sizeof(events) / sizeof(*events);
	for (int i = 0; i < evCount; i++)
		libvlc_event_attach(man, events[i], cbManageEvent, d);

	d->engine = &PlayEngine::get();
	d->audio = &AudioController::get();
	d->video = &VideoScene::get();
	d->engine->setMediaPlayer(d->mp);
	d->video->setUtil(&d->vUtil);
	d->audio->setUtil(&d->aUtil);
}

void LibVLC::fin() {
	if (!d)
		return;
	libvlc_media_player_stop(d->mp);
	libvlc_media_player_release(d->mp);
	libvlc_release(d->inst);
	d->mp = 0;
	d->inst = 0;
	d->audio = 0;
	d->video = 0;
	d->engine = 0;
	delete d;
	d = 0;
}

void LibVLC::cbAudioPrepare(void *data, const AudioFormat *format) {
	Q_ASSERT(d != 0 && data == d);
	Q_ASSERT(d->audio != 0);
	d->audio->prepare(format);
}

AudioBuffer *LibVLC::cbAudioProcess(void *data, AudioBuffer *in) {
	Q_ASSERT(d != 0 && data == d);
	if (d->audio)
		return d->audio->process(in);
	return 0;
}

void LibVLC::cbVideoProcess(void *data, void **planes) {
	Q_ASSERT(d != 0 && data == d);
	if (d->video)
		d->video->process(planes);
}

void LibVLC::cbVideoRender(void *data, void **planes) {
	Q_ASSERT(d != 0 && data == d);
	if (d->video)
		d->video->render(planes);
}

void *LibVLC::cbVideoLock(void *data, void ***planes) {
	Q_ASSERT(d != 0 && data == d);
	if (d->video)
		return d->video->lock(planes);
	return 0;
}

void LibVLC::cbVideoUnlock(void *data, void *id, void *const *plane) {
	Q_ASSERT(d != 0 && data == d);
	if (d->video)
		d->video->unlock(id, plane);
}

void LibVLC::cbVideoDisplay(void *data, void *id) {
	Q_ASSERT(d != 0 && data == d);
	if (d->video)
		d->video->display(id);
}

void LibVLC::cbVideoPrepare(void *data, const VideoFormat *format) {
	Q_ASSERT(d != 0 && data == d);
	Q_ASSERT(d->video != 0);
	d->video->prepare(format);
}

void LibVLC::cbManageEvent(const libvlc_event_t *event, void *data) {
	Q_ASSERT(d != 0 && data == d);
	switch (event->type) {
	case libvlc_MediaPlayerSeekableChanged:
	case libvlc_MediaPlayerTimeChanged:
	case libvlc_MediaPlayerPlaying:
	case libvlc_MediaPlayerPaused:
	case libvlc_MediaPlayerEndReached:
	case libvlc_MediaPlayerOpening:
	case libvlc_MediaPlayerBuffering:
	case libvlc_MediaPlayerStopped:
	case libvlc_MediaPlayerEncounteredError:
	case libvlc_MediaPlayerLengthChanged:
	case libvlc_MediaPlayerTitleChanged:
		if (d->engine)
			d->engine->parseEvent(event);
		break;
	default:
		break;
	}
}

LibVLC::~LibVLC() {
	delete d;
}

PlayEngine *LibVLC::engine() {Q_ASSERT(d != 0 && d->engine != 0); return d->engine;}
AudioController *LibVLC::audio() {Q_ASSERT(d != 0 && d->audio != 0); return d->audio;}
VideoScene *LibVLC::video() {Q_ASSERT(d != 0 && d->video != 0); return d->video;}
libvlc_instance_t *LibVLC::inst() {Q_ASSERT(d != 0 && d->inst != 0); return d->inst;}
libvlc_media_player_t *LibVLC::mp() {Q_ASSERT(d != 0 && d->mp != 0); return d->mp;}
libvlc_media_t *LibVLC::newMedia(const Mrl &mrl) {
	return libvlc_media_new_location(inst(), mrl.toString().toLocal8Bit());
}

void LibVLC::outputError() {
	qDebug() << "libvlc error:" << libvlc_errmsg();
}
