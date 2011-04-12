#ifndef LIBVLC_H
#define LIBVLC_H

#include <vlc/vlc.h>
#include <QtCore/QString>
#include "mrl.hpp"

class AudioController;		class AudioBuffer;
class VideoRenderer;		class PlayEngine;
class AudioFormat;		class VideoFormat;

class LibVLC {
public:
	~LibVLC();
	static void init();
	static void release();
	static void outputError();
	static PlayEngine *engine();
	static AudioController *audio();
	static VideoRenderer *video();
	static libvlc_instance_t *inst();
	static libvlc_media_player_t *mp();
	static libvlc_media_t *newMedia(const Mrl &mrl);
private:
	static void cbAudioPrepare(void *data, const AudioFormat *format);
	static AudioBuffer *cbAudioProcess(void *data, AudioBuffer *in);
	static void *cbVideoLock(void *data, void ***planes);
	static void cbVideoRender(void *data, void **planes);
	static void cbVideoProcess(void *data, void **planes);
	static void cbVideoUnlock(void *data, void *id, void *const *plane);
	static void cbVideoDisplay(void *data, void *id);
	static void cbVideoPrepare(void *data, const VideoFormat *format);
	static void cbManageEvent(const libvlc_event_t *event, void *data);
	LibVLC();
	static LibVLC *s;
	static LibVLC &get() {return *s;}
	struct Data;
	Data *d;
};

#endif // LIBVLC_H
