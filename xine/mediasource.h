#ifndef MPLAYERMPMEDIASOURCE_H
#define MPLAYERMPMEDIASOURCE_H

#include <QSharedDataPointer>
#include <QSize>
#include <QString>
#include "mplayer.h"

class QUrl;

namespace MPlayer {

class MediaInfo;

class MediaSource {
public:
	enum Type {Invalid = 0, LocalFile = 1, Url = 2, Disc = 3};
	MediaSource();
	explicit MediaSource(DiscType dt);
	explicit MediaSource(const QString &filePath);
	explicit MediaSource(const QUrl &url);
	MediaSource(const MediaSource &other);
	~MediaSource();
	MediaSource &operator = (const MediaSource &rhs);
	bool operator != (const MediaSource &rhs) const;
	bool operator == (const MediaSource &rhs) const;
	bool operator < (const MediaSource &rhs) const;
	QString filePath() const;
	Type type () const;
	bool isValid() const;
	bool isLocalFile() const;
	bool isDisc() const;
	QUrl url () const;
	QString displayName() const;
	const MediaInfo &info() const;
	DiscType discType() const;
	int titleNumber() const;
	MediaInfo &info();
	bool getInfo();
	bool gotInfo() const;
private:
	struct Data;
	friend class SourceInfo;
	QSharedDataPointer<Data> d;
};

}

#endif
