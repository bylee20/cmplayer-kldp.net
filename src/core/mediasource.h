#ifndef CORE_MEDIASOURCE_H
#define CORE_MEDIASOURCE_H

#include "namespace.h"
#include "mrl.h"
#include <QtCore/QSharedData>

namespace Core {

class MediaInfo;

class MediaSource {
public:
	MediaSource(const Mrl &mrl): d(new Data) {init(mrl);}
	MediaSource(const QUrl &url): d(new Data) {init(url);}
	MediaSource(): d(new Data) {}
	MediaSource(const MediaSource &other): d(other.d) {}
	bool operator != (const MediaSource &rhs) const {
		return d->type != rhs.d->type || d->mrl != rhs.d->mrl;
	}
	bool operator == (const MediaSource &rhs) const {
		return !operator!=(rhs);
	}
	bool operator < (const MediaSource &rhs) const {
		return d->mrl.location() < rhs.d->mrl.location();
	}
	QString fileName() const {return d->mrl.toLocalFile();}
	MediaType type () const {return d->type;}
	bool isValid() const {return d->type != Unknown;}
	bool isLocalFile() const {return d->type == LocalFile;}
	Mrl mrl() const {return isValid() ? d->mrl : Mrl();}
	bool isDisc() const {return d->type == Dvd;}
	QString displayName() const;
private:
	void init(const Mrl &mrl);
	struct Data : public QSharedData {
		Data(): type(Unknown) {}
		Data(const Data &rhs)
		: QSharedData(rhs), type(rhs.type), mrl(rhs.mrl) {}
		MediaType type;
		Mrl mrl;
	};
	QSharedDataPointer<Data> d;
};

}

#endif
