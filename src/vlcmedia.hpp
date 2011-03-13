#ifndef VLCMEDIA_H
#define VLCMEDIA_H

#include "mrl.hpp"
#include <vlc/vlc.h>

class VlcMedia {
public:
	VlcMedia(const Mrl &mrl);
	~VlcMedia();
	libvlc_media_t *media();
	Mrl mrl() const;
private:
	static void cbEventManage(const libvlc_event_t *event, void *data);
	void parseEvent(const libvlc_event_t *event);
	struct Data;
	Data *d;
};

#endif // VLCMEDIA_H
