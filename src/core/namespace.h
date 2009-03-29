#ifndef CORE_NAMESPACE_H
#define CORE_NAMESPACE_H

#include <QtCore/QString>

namespace Core {

enum State {Playing, Stopped, Paused, Finished};

enum VideoType {AutoVideo = 0, OpenGL, Native};

enum MediaType {Unknown = 0, LocalFile, Url, Dvd};

class VideoStream {
public:
	int bitRate, width, height;
	double frameRate;
};

class AudioStream {
public:
	int bitRate, samplingRate, channels;
	QString language;
};

class SubtitleStream {
public:
	QString format, language;
};

}

#endif
