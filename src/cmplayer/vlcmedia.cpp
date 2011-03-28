#include "vlcmedia.hpp"
#include "libvlc.hpp"
#include <QtCore/QStringList>

struct VLCMedia::Data {
	Mrl mrl;
	libvlc_media_t *m;
	libvlc_event_manager_t *evMan;
};

VLCMedia::VLCMedia(const Mrl &mrl): d(new Data) {
	d->evMan = 0;
	d->mrl = mrl;
	if (mrl.isLocalFile())
		d->m = libvlc_media_new_path(LibVLC::inst(), mrl.toLocalFile().toLocal8Bit());
	else
		d->m = libvlc_media_new_location(LibVLC::inst(), mrl.toString().toLocal8Bit());
	if (!d->m) {
		LibVLC::outputError();
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

VLCMedia::~VLCMedia() {
	if (d->m)
		libvlc_media_release(d->m);
	delete d;
}

Mrl VLCMedia::mrl() const {
	return d->mrl;
}

libvlc_media_t *VLCMedia::media() {
	return d->m;
}

void VLCMedia::cbEventManage(const libvlc_event_t *event, void *data) {
	reinterpret_cast<VLCMedia*>(data)->parseEvent(event);
}

void VLCMedia::parseEvent(const libvlc_event_t *event) {
	switch (event->type) {
	default:
		break;
	}
}

void VLCMedia::addOption(const QStringList &opt) {
	QByteArray buffer;
	for (int i=0; i<opt.size(); ++i) {
		buffer = opt[i].toLocal8Bit();
		libvlc_media_add_option(d->m, buffer.constData());
	}
}

void VLCMedia::addOption(const QString &opt) {
	Q_ASSERT(d->m != 0);
	const QByteArray buffer = opt.toLocal8Bit();
	libvlc_media_add_option(d->m, buffer.data());
}


