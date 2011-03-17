#ifndef LIBVLC_H
#define LIBVLC_H

#include <vlc/vlc.h>
#include <QtCore/QString>
#include "mrl.hpp"

class AudioController;
class VideoRenderer;
class PlayEngine;

class LibVlc {
public:
	struct EventHandler {
		EventHandler *self;
		void (*mousePressed)(EventHandler *self, int button);
		void (*mouseReleased)(EventHandler *self, int button);
		void (*mouseMoved)(EventHandler *self, int x, int y);
		void *vd;
	};

	~LibVlc();
	static void init();
	static void release();

	static void outputError();

	static PlayEngine *engine();
	static AudioController *audio();
	static VideoRenderer *video();
//	static void setAudio(AudioController *audio);
//	static void setVideo(GLRenderer *video);
//	static void setEngine(PlayEngine *engine);
	static libvlc_instance_t *inst();
	static libvlc_media_player_t *mp();
	static libvlc_media_t *newMedia(const Mrl &mrl);
private:
	static void cbAudioPrepare(void *data, int channels);
	static void cbAudioDoWork(void *data, int samples, float *buffer);
	static void *cbVideoLock(void *data, void *planes, int dataLength);
	static void cbVideoUnlock(void *data, void *id, void *const *plane);
	static void cbVideoDisplay(void *data, void *id);
	static void cbVideoPrepare(void *data, quint32 fourcc, int width, int height, double sar, double fps);
	static void cbManageEvent(const libvlc_event_t *event, void *data);
	LibVlc();
	static LibVlc *s;
	static LibVlc &get() {return *s;}
	struct Data;
	Data *d;
};

#endif // LIBVLC_H
