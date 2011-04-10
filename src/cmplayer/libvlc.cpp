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
	VideoRenderer *video;
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	VideoUtil vUtil;
	AudioUtil aUtil;
};

LibVLC *LibVLC::s = 0;

void LibVLC::init() {
	s = new LibVLC;
	Data *d = s->d;
	d->engine = new PlayEngine;
	d->audio = new AudioController;
	d->video = new VideoRenderer;

	d->vUtil.vd = 0;
	d->aUtil.af = 0;
	d->aUtil.scaletempoEnabled = 0;

	d->video->setUtil(&d->vUtil);
	d->audio->setUtil(&d->aUtil);
}

void LibVLC::release() {
	Data *d = s->d;
	delete d->engine;
	delete d->audio;
	delete d->video;
	libvlc_media_player_stop(d->mp);
	libvlc_media_player_release(d->mp);
	libvlc_release(d->inst);
	delete s;
}

void LibVLC::cbAudioPrepare(void *data, const AudioFormat *format) {
	AudioController *audio = reinterpret_cast<LibVLC*>(data)->d->audio;
	if (audio)
		audio->prepare(format);
}

AudioBuffer *LibVLC::cbAudioProcess(void *data, AudioBuffer *in) {
	AudioController *audio = reinterpret_cast<LibVLC*>(data)->d->audio;
	if (audio)
		return audio->process(in);
	return 0;
}

void *LibVLC::cbVideoLock(void *data, void ***planes) {
	VideoRenderer *video = reinterpret_cast<LibVLC*>(data)->d->video;
	if (video)
		return video->lock(planes);
	return 0;
}

void LibVLC::cbVideoUnlock(void *data, void *id, void *const *plane) {
	VideoRenderer *video = reinterpret_cast<LibVLC*>(data)->d->video;
	if (video)
		video->unlock(id, plane);
}

void LibVLC::cbVideoDisplay(void *data, void *id) {
	VideoRenderer *video = reinterpret_cast<LibVLC*>(data)->d->video;
	if (video)
		video->display(id);
}

void LibVLC::cbVideoPrepare(void *data, const VideoFormat *format) {
	VideoRenderer *video = reinterpret_cast<LibVLC*>(data)->d->video;
	if (video)
		video->prepare(format);
}

void LibVLC::cbManageEvent(const libvlc_event_t *event, void *data) {
	LibVLC *self = reinterpret_cast<LibVLC*>(data);
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

LibVLC::LibVLC(): d(new Data) {
	d->audio = 0;
	d->video = 0;
	d->engine = 0;

	char pvLock[125], pvUnlock[125], pvDisplay[125], pvPrepare[125];
	char paProcess[125], paPrepare[125], paUtil[125];
	char pData[125], pvUtil[125], paScaletempo[125];
	sprintf(pvLock, "%lld", (long long int)(intptr_t)(void*)cbVideoLock);
	sprintf(pvUnlock, "%lld", (long long int)(intptr_t)(void*)cbVideoUnlock);
	sprintf(pvDisplay, "%lld", (long long int)(intptr_t)(void*)cbVideoDisplay);
	sprintf(pvPrepare, "%lld", (long long int)(intptr_t)(void*)cbVideoPrepare);
	sprintf(paProcess, "%lld", (long long int)(intptr_t)(void*)cbAudioProcess);
	sprintf(paPrepare, "%lld", (long long int)(intptr_t)(void*)cbAudioPrepare);
	sprintf(pData, "%lld", (long long int)(intptr_t)(void*)this);
	sprintf(pvUtil, "%lld", (long long int)(intptr_t)(void*)&d->vUtil);
	sprintf(paUtil, "%lld", (long long int)(intptr_t)(void*)&d->aUtil);
	sprintf(paScaletempo, "%lld", (long long int)(intptr_t)&d->aUtil.scaletempoEnabled);

	const char *const args[] = {
		"-I", "dummy",
		"--ignore-config",
		"--extraintf=logger",
		"--quiet",
//		"--verbose=2",
	#ifdef Q_WS_X11
		"--no-xlib",
	#endif
		"--reset-plugins-cache",
		"--plugin-path", Info::pluginPath(),
		"--no-media-library",
		"--no-osd",
		"--no-sub-autodetect-file",
		"--no-stats",
		"--no-video-title-show",
		"--album-art=0",
		"--cmplayer-vout-chroma", "AUTO",
		"--cmplayer-vout-cb-lock", pvLock,
		"--cmplayer-vout-cb-unlock", pvUnlock,
		"--cmplayer-vout-cb-display", pvDisplay,
		"--cmplayer-vout-cb-prepare", pvPrepare,
		"--cmplayer-vout-data", pData,
		"--cmplayer-vout-util", pvUtil,
		"--vout", "cmplayer-vout",
		"--cmplayer-afilter-cb-prepare", paPrepare,
		"--cmplayer-afilter-cb-process", paProcess,
		"--cmplayer-afilter-data", pData,
		"--cmplayer-afilter-util", paUtil,
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

LibVLC::~LibVLC() {
	delete d;
}

PlayEngine *LibVLC::engine() {
	return get().d->engine;
}

AudioController *LibVLC::audio() {
	return get().d->audio;
}

VideoRenderer *LibVLC::video() {
	return get().d->video;
}

libvlc_instance_t *LibVLC::inst() {
	return get().d->inst;
}

libvlc_media_player_t *LibVLC::mp() {
	return get().d->mp;
}

libvlc_media_t *LibVLC::newMedia(const Mrl &mrl) {
	return libvlc_media_new_location(get().inst(), mrl.toString().toLocal8Bit());
}

void LibVLC::outputError() {
	qDebug() << "libvlc error:" << libvlc_errmsg();
}
