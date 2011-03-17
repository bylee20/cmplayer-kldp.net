#include "vlcmedia.hpp"
#include "libvlc.hpp"

struct VlcMedia::Data {
	Mrl mrl;
	libvlc_media_t *m;
	libvlc_event_manager_t *evMan;
};

VlcMedia::VlcMedia(const Mrl &mrl): d(new Data) {
	d->evMan = 0;
	d->mrl = mrl;
	if (mrl.isLocalFile())
		d->m = libvlc_media_new_path(LibVlc::inst(), mrl.toLocalFile().toLocal8Bit());
	else
		d->m = libvlc_media_new_location(LibVlc::inst(), mrl.toString().toLocal8Bit());
	if (!d->m) {
		LibVlc::outputError();
		return;
	}
	d->evMan = libvlc_media_event_manager(d->m);
	libvlc_event_type_t events[] = {
		libvlc_MediaMetaChanged,
	};
	const int evCount = sizeof(events)/sizeof(*events);
	for (int i=0; i<evCount; ++i)
		libvlc_event_attach(d->evMan, events[i], cbEventManage, this);
}

VlcMedia::~VlcMedia() {
	if (d->m)
		libvlc_media_release(d->m);
	delete d;
}

Mrl VlcMedia::mrl() const {
	return d->mrl;
}

libvlc_media_t *VlcMedia::media() {
	return d->m;
}

void VlcMedia::cbEventManage(const libvlc_event_t *event, void *data) {
	reinterpret_cast<VlcMedia*>(data)->parseEvent(event);
}

void VlcMedia::parseEvent(const libvlc_event_t *event) {
	switch (event->type) {
	default:
		break;
	}
}
