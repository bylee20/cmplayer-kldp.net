#include "audiocontroller.hpp"
#include "videorenderer.hpp"
#include "playengine.hpp"
#include "libvlc.hpp"
#include <QtCore/QDebug>

struct LibVlc::Data {
	PlayEngine *engine;
	AudioController *audio;
	VideoRenderer *video;
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	EventHandler eventHandler;
};

LibVlc *LibVlc::s = 0;

void LibVlc::init() {
	s = new LibVlc;
	Data *d = s->d;
	d->engine = new PlayEngine;
	d->audio = new AudioController;
	d->video = new VideoRenderer;
	d->eventHandler.self = 0;
	d->video->setEventHandler(&d->eventHandler);
}

void LibVlc::release() {
	Data *d = s->d;
	delete d->engine;
	delete d->audio;
	delete s;
}

void LibVlc::cbAudioPrepare(void *data, int channels) {
	AudioController *audio = reinterpret_cast<LibVlc*>(data)->d->audio;
	if (audio)
		audio->prepare(channels);
}

void LibVlc::cbAudioDoWork(void *data, int samples, float *buffer) {
	AudioController *audio = reinterpret_cast<LibVlc*>(data)->d->audio;
	if (audio)
		audio->apply(samples, buffer);
}

void *LibVlc::cbVideoLock(void *data, void *planes, int dataLength) {
	VideoRenderer *video = reinterpret_cast<LibVlc*>(data)->d->video;
	if (video)
		return video->lock(reinterpret_cast<VideoFrame::Plane*>(planes), dataLength);
	return 0;
}

void LibVlc::cbVideoUnlock(void *data, void *id, void *const *plane) {
	VideoRenderer *video = reinterpret_cast<LibVlc*>(data)->d->video;
	if (video)
		video->unlock(id, plane);
}

void LibVlc::cbVideoDisplay(void *data, void *id) {
	VideoRenderer *video = reinterpret_cast<LibVlc*>(data)->d->video;
	if (video)
		video->display(id);
}

void LibVlc::cbVideoPrepare(void *data, quint32 fourcc, int width, int height, double sar, double fps) {
	VideoRenderer *video = reinterpret_cast<LibVlc*>(data)->d->video;
	if (video)
		video->prepare(fourcc, width, height, sar, fps);
}

void LibVlc::cbManageEvent(const libvlc_event_t *event, void *data) {
	LibVlc *self = reinterpret_cast<LibVlc*>(data);
	Data *d = self->d;
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

LibVlc::LibVlc(): d(new Data) {
	d->audio = 0;
	d->video = 0;
	d->engine = 0;

	char pvLock[125], pvUnlock[125], pvDisplay[125], pvPrepare[125];
	char paDoWork[125], paPrepare[125];
	char pData[125], pvEventHandler[125];
	sprintf(pvLock, "%lld", (long long int)(intptr_t)(void*)cbVideoLock);
	sprintf(pvUnlock, "%lld", (long long int)(intptr_t)(void*)cbVideoUnlock);
	sprintf(pvDisplay, "%lld", (long long int)(intptr_t)(void*)cbVideoDisplay);
	sprintf(pvPrepare, "%lld", (long long int)(intptr_t)(void*)cbVideoPrepare);
	sprintf(paDoWork, "%lld", (long long int)(intptr_t)(void*)cbAudioDoWork);
	sprintf(paPrepare, "%lld", (long long int)(intptr_t)(void*)cbAudioPrepare);
	sprintf(pData, "%lld", (long long int)(intptr_t)(void*)this);
	sprintf(pvEventHandler, "%lld", (long long int)(intptr_t)(void*)&d->eventHandler);
	const char *const args[] = {
		"-I", "dummy",
		"--ignore-config",
		"--extraintf=logger",
//		"--verbose=2",
		"--quiet",
		"--no-xlib",
		"--reset-plugins-cache",
		"--no-media-library",
		"--no-osd",
		"--no-sub-autodetect-file",
		"--no-stats",
		"--no-video-title-show",
		"--album-art=0",
		"--cmplayer-vout-chroma", "YV12",
		"--cmplayer-vout-cb-lock", pvLock,
		"--cmplayer-vout-cb-unlock", pvUnlock,
		"--cmplayer-vout-cb-display", pvDisplay,
		"--cmplayer-vout-cb-prepare", pvPrepare,
		"--cmplayer-vout-data", pData,
		"--cmplayer-vout-event-handler", pvEventHandler,
		"--vout", "cmplayer-vout",
		"--cmplayer-afilter-cb-prepare", paPrepare,
		"--cmplayer-afilter-cb-do-work", paDoWork,
		"--cmplayer-afilter-data", pData,
		"--audio-filter", "cmplayer-afilter",
	};
	d->inst = libvlc_new(sizeof(args)/sizeof(*args), args);
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
		libvlc_event_attach(man, events[i], cbManageEvent, this);
}

LibVlc::~LibVlc() {
	libvlc_media_player_stop(d->mp);
	libvlc_media_player_release(d->mp);
	libvlc_release(d->inst);
	delete d;
}

PlayEngine *LibVlc::engine() {
	return get().d->engine;
}

AudioController *LibVlc::audio() {
	return get().d->audio;
}

VideoRenderer *LibVlc::video() {
	return get().d->video;
}

libvlc_instance_t *LibVlc::inst() {
	return get().d->inst;
}

libvlc_media_player_t *LibVlc::mp() {
	return get().d->mp;
}

libvlc_media_t *LibVlc::newMedia(const Mrl &mrl) {
	return libvlc_media_new_location(get().inst(), mrl.toString().toLocal8Bit());
}

void LibVlc::outputError() {
	qDebug() << "libvlc error:" << libvlc_errmsg();
}
