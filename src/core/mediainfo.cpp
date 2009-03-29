// #include "mediainfo.h"
// #include "mediasource.h"
// #include <QtCore/QMap>
// #include <QtCore/QSize>
// #define UNICODE
// #include <MediaInfo/MediaInfo.h>
// 
// namespace Core {
// 
// inline static QString get(MediaInfoLib::MediaInfo &info, MediaInfoLib::stream_t stream
// 		, size_t nb, const MediaInfoLib::String &param) {
// 	return QString::fromStdWString(info.Get(stream, nb, param)).trimmed();
// }
// inline static int getInt(MediaInfoLib::MediaInfo &info, MediaInfoLib::stream_t stream
// 		, size_t nb, const MediaInfoLib::String &param) {
// 	return get(info, stream, nb, param).toInt();
// }
// inline static double getDouble(MediaInfoLib::MediaInfo &info, MediaInfoLib::stream_t stream
// 		, size_t nb, const MediaInfoLib::String &param) {
// 	return get(info, stream, nb, param).toDouble();
// }
// 
// MediaInfo::MediaInfo(const MediaSource &source)
// : d(new Data) {
// 	d->source = new MediaSource(source);
// 	MediaInfoLib::MediaInfo info;
// 	info.Open(source.filePath().toStdWString());
// #define STREAM(str) MediaInfoLib::Stream_##str
// // 	qDebug() << QString::fromStdWString(info.Option(_T("Info_Parameters")));
// 	d->fileSize = getInt(info, STREAM(General), 0, _T("FileSize"));
// 	d->duration = getInt(info, STREAM(General), 0, _T("Duration"));
// 	d->bitRate = getInt(info, STREAM(General), 0, _T("OverallBitRate"));
// 	d->title = get(info, STREAM(General), 0, _T("Title"));
// 	d->artist = get(info, STREAM(General), 0, _T("Performer"));
// 	d->album = get(info, STREAM(General), 0, _T("Album"));
// 	d->genre = get(info, STREAM(General), 0, _T("Genre"));
// 	
// 	const int videoCount = info.Count_Get(STREAM(Video));
// 	for (int i=0; i<videoCount; ++i) {
// 		VideoStream stream;
// 		stream.bitRate = getInt(info, STREAM(Video), i, _T("BitRate"));
// 		stream.width = getInt(info, STREAM(Video), i, _T("Width"));
// 		stream.height = getInt(info, STREAM(Video), i, _T("Height"));
// 		stream.frameRate = getDouble(info, STREAM(Video), i, _T("FrameRate"));
// 		
// 	}
// 
// 	const int audioCount = info.Count_Get(STREAM(Audio));
// 	for (int i=0; i<audioCount; ++i) {
// 		AudioStream stream;
// 		stream.bitRate = getInt(info, STREAM(Audio), i, _T("BitRate"));
// 		stream.samplingRate = getInt(info, STREAM(Audio), i, _T("SamplingRate"));
// 		stream.channels = getInt(info, STREAM(Audio), i, _T("Channel(s)"));
// 		stream.language = get(info, STREAM(Audio), i, _T("Language/String"));
// 		d->audioStream.push_back(stream);
// 	}
// 	
// 	const int subtitleCount = info.Count_Get(STREAM(Text));
// 	for (int i=0; i<subtitleCount; ++i) {
// 		SubtitleStream stream;
// 		stream.format = get(info, STREAM(Text), i, _T("Format"));
// 		stream.language = get(info, STREAM(Text), i, _T("Language/String"));
// 		d->subtitleStream.push_back(stream);
// 	}
// #undef STREAM
// }
// 
// MediaInfo::MediaInfo(const MediaInfo &other)
// : d(new Data) {
// 	*d->source = *other.d->source;
// 	if ((d->valid = other.d->valid)) {
// 		d->duration = other.d->duration;
// 		d->fileSize = other.d->fileSize;
// 		d->frameRate = other.d->frameRate;
// 		d->bitRate = other.d->bitRate;
// 		d->audioStream = other.d->audioStream;
// 		d->videoStream = other.d->videoStream;
// 		d->subtitleStream = other.d->subtitleStream;
// 		d->title = other.d->title;
// 		d->artist = other.d->artist;
// 		d->album = other.d->album;
// 		d->genre = other.d->genre;
// 	}
// }
// 
// MediaInfo::~MediaInfo() {
// 	delete d->source;
// 	delete d;
// }
// 
// }
