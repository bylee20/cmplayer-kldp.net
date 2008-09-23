// #include "info.h"
#include "mediasource.h"
#include <QtCore/QFileInfo>

namespace Backend {

MediaSource::MediaSource()
: d(new Data) {}

MediaSource::MediaSource(Type type, const QString &info)
: d(new Data) {
	d->type = type;
	switch(d->type) {
	case LocalFile:
		d->url = QUrl::fromLocalFile(info);
		break;
	case Disc:
		d->url.setUrl("dvd:" + info);
		break;
	case Url:
		d->url.setUrl(info);
		break;
	default:
		break;
	}
}

MediaSource::MediaSource(const QString &filePath)
: d(new Data) {
	d->url = QUrl::fromLocalFile(filePath);
	if (!d->url.isEmpty()) {
		d->type = LocalFile;
	}
}

MediaSource::MediaSource(const QUrl &url)
: d(new Data) {
	d->url = url;
	if (!url.isEmpty()) {
		const QString scheme = url.scheme();
		if (scheme == "file")
			d->type = LocalFile;
		else if (scheme == "dvd") {
			d->type = Disc;
		} else
			d->type = Url;
	}
}

QString MediaSource::displayName() const {
	if (d->type == LocalFile)
		return QFileInfo(filePath()).fileName();
	else if (isDisc())
		return QObject::trUtf8("DVD 타이틀");
	else
		return d->url.toString();
}


QString MediaSource::toMrl() const {
	return d->url.toString();
}

// bool MediaSource::getInfo() const {
// 	return Info::get(const_cast<MediaSource*>(this));
// }

}
