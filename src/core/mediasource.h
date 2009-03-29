#ifndef BACKEND_MEDIASOURCE_H
#define BACKEND_MEDIASOURCE_H

#include "namespace.h"
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QSharedData>

namespace Core {

class MediaInfo;

class MediaSource {
public:
	explicit MediaSource(const QUrl &url);
	MediaSource();
	MediaSource(const MediaSource &other);
	bool operator != (const MediaSource &rhs) const;
	bool operator == (const MediaSource &rhs) const;
	bool operator < (const MediaSource &rhs) const;
	QString filePath() const;
	MediaType type () const;
	bool isValid() const;
	bool isLocalFile() const;
	bool isDisc() const;
	QUrl url () const;
	QString displayName() const;
	QString toMrl() const;
private:
	struct Data : public QSharedData {
		Data(): type(Unknown) {}
		Data(const Data &rhs)
		: QSharedData(rhs), type(rhs.type), url(rhs.url) {}
		MediaType type;
		QUrl url;
	};
	QSharedDataPointer<Data> d;
};

inline bool MediaSource::operator != (const MediaSource &rhs) const {
	return d->type != rhs.d->type || d->url != rhs.d->url;
}

inline bool MediaSource::operator == (const MediaSource &rhs) const {
	return !operator!=(rhs);
}

inline bool MediaSource::operator < (const MediaSource &rhs) const {
	return (isLocalFile() ? filePath() : d->url.toString())
			< (rhs.isLocalFile() ? rhs.filePath() : rhs.d->url.toString());
}

inline QString MediaSource::filePath() const {
	return d->url.toLocalFile();
}

inline MediaType MediaSource::type () const {
	return d->type;
}

inline bool MediaSource::isValid() const {
	return d->type != Unknown;
}

inline bool MediaSource::isLocalFile() const {
	return d->type == LocalFile;
}

inline QUrl MediaSource::url () const {
	return isValid() ? d->url : QUrl();
}

inline bool MediaSource::isDisc() const {
	return d->type == Dvd;
}

inline QString MediaSource::toMrl() const {
	return d->url.toString();
}

}

#endif
