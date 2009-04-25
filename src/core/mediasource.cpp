#include "mediasource.h"

namespace Core {

QString MediaSource::displayName() const {
	if (d->type == LocalFile)
		return d->mrl.fileName();
	else if (isDisc())
		return QObject::tr("DVD Title");
	else
		return d->mrl.toString();
}

void MediaSource::init(const Mrl &mrl) {
	d->mrl = mrl;
	if (!mrl.isEmpty()) {
		const QString scheme = mrl.scheme();
		if (scheme == "file")
			d->type = LocalFile;
		else if (scheme == "dvd") {
			d->type = Dvd;
		} else
			d->type = Url;
	}
}

}
