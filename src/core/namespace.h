#ifndef CORE_NAMESPACE_H
#define CORE_NAMESPACE_H

namespace Core {

enum State {Playing, Stopped, Paused};

enum VideoType {AutoVideo = 0, OpenGL, Native};

enum VideoProperty {Brightness = 0, Saturation, Contrast, Hue};

enum MediaType {Unknown = 0, LocalFile, Url, Dvd};

}

#endif