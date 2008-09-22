#ifndef BACKEND_MEDIASOURCE_H
#define BACKEND_MEDIASOURCE_H

#include <QSharedDataPointer>
#include <QString>
#include <QUrl>
#include <QSize>

namespace Backend {

class MediaSource {
public:
	enum Type {Invalid = 0, LocalFile = 1, Url = 2, Disc = 3};
	MediaSource();
	MediaSource(Type, const QString &info);
	MediaSource(const QString &filePath);
	MediaSource(const QUrl &url);
	bool operator != (const MediaSource &rhs) const {
		return d->type != rhs.d->type || d->url != rhs.d->url;
	}
	bool operator == (const MediaSource &rhs) const {
		return d->type == rhs.d->type && d->url == rhs.d->url;
	}
	bool operator < (const MediaSource &rhs) const {
		if (isLocalFile())
			return filePath() < rhs.filePath();
		return d->url.toString() < rhs.d->url.toString();
	}
	QString filePath() const {return d->url.toLocalFile();}
	Type type () const {return d->type;}
	bool isValid() const {return d->type != Invalid;}
	bool isLocalFile() const {return d->type == LocalFile;}
	bool isDisc() const {return d->type == Disc;}
	QUrl url () const {return isValid() ? d->url : QUrl();}
	QString displayName() const;
	QString toMrl() const;
// 	bool getInfo() const;
// 	bool gotInfo() const {return d->gotInfo;}
// 	bool hasVideo() const {return d->hasVideo;}
// 	const QSize &videoSize() const {return d->videoSize;}
// 	int duration() const {return d->duration;}
private:
	friend class Info;
	struct Data : public QSharedData {
		Data(): type(Invalid), url()/*, duration(0), hasVideo(false)
		, gotInfo(false), videoSize()*/ {}
		Data(const Data &other): QSharedData(other), type(other.type)
		, url(other.url)/*, duration(other.duration), hasVideo(other.hasVideo)
		, gotInfo(other.gotInfo), videoSize(other.videoSize)*/ {}
		~Data() {}
		Type type;
		QUrl url;
// 		int duration;
// 		bool hasVideo, gotInfo;
// 		QSize videoSize;
	};
	QSharedDataPointer<Data> d;
};

}

#endif
