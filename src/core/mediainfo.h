// #ifndef CORE_MEDIAINFO_H
// #define CORE_MEDIAINFO_H
// 
// #include <QtCore/QString>
// #include <QtCore/QList>
// #include <QtCore/QSharedData>
// #include "namespace.h"
// 
// namespace Core {
// 
// class MediaSource;
// 
// class MediaInfo {
// public:
// 	MediaInfo(const MediaSource &source);
// 	MediaInfo(const MediaInfo &other);
// 	virtual ~MediaInfo();
// 	bool isValid() const {return d->valid;}
// 	int fileSize() const {return d->fileSize;}
// 	int duration() const {return d->duration;}
// 	double frameRate() const {return d->frameRate;}
// 	int bitRate() const {return d->bitRate;}
// 	bool hasVideo() const {return !d->videoStream.isEmpty();}
// 	const QList<AudioStream> &audioStream() const {return d->audioStream;}
// 	const QList<VideoStream> &videoStream() const {return d->videoStream;}
// 	const QList<SubtitleStream> &subtitleStream() const {return d->subtitleStream;}
// 	const QString &title() const {return d->title;}
// 	const QString &artist() const {return d->artist;}
// 	const QString &album() const {return d->album;}
// 	const QString &genre() const {return d->genre;}
// protected:
// 	struct Data {
// 		int duration, fileSize;
// 		bool valid;
// 		double frameRate;
// 		int bitRate;
// // 		QMap<int, QString> tracks;
// 		QList<AudioStream> audioStream;
// 		QList<VideoStream> videoStream;
// 		QList<SubtitleStream> subtitleStream;
// 		MediaSource *source;
// 		QString title, artist, album, genre;
// 	};
// 	Data *d;
// private:
// 	 MediaInfo &operator = (const MediaInfo &rhs);
// };
// 
// }
// 
// #endif
