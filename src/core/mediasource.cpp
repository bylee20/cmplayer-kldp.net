#include "mediasource.h"
#include <QFileInfo>
#include <QUrl>
#include <QString>

namespace Core {

MediaSource::MediaSource()
: d(new Data) {}

MediaSource::MediaSource(const QUrl &url)
: d(new Data) {
	d->url = url;
	if (!url.isEmpty()) {
		const QString scheme = url.scheme();
		if (scheme == "file")
			d->type = LocalFile;
		else if (scheme == "dvd") {
			d->type = Dvd;
		} else
			d->type = Url;
	}
}

MediaSource::MediaSource(const MediaSource &other)
: d(other.d) {}

QString MediaSource::displayName() const {
	if (d->type == LocalFile)
		return QFileInfo(filePath()).fileName();
	else if (isDisc())
		return QObject::tr("DVD Title");
	else
		return d->url.toString();
}

}
