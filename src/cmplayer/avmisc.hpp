#ifndef AVMISC_HPP
#define AVMISC_HPP

#include <QtCore/QtGlobal>

#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
#define FOURCC(a, b, c, d) \
	(((quint32)d)|(((quint32)c)<<8)|(((quint32)b)<<16)|(((quint32)a)<<24))
#else
#define FOURCC(a, b, c, d) \
	(((quint32)a)|(((quint32)b)<<8)|(((quint32)c)<<16)|(((quint32)d)<<24))
#endif

struct AudioBuffer {
	void *block;
	void *data;
	int samples;
	int size;
};

struct AudioUtil {
	void *af;
	AudioBuffer *(*allocBuffer)(void *af, int size);
	void (*freeBuffer)(void *af, AudioBuffer *buffer);
	int scaletempoEnabled;
};

struct AudioFormat {
	int channels;
	int sampleRate;
};

#define VIDEO_FRAME_MAX_PLANE_COUNT (3)

struct VideoUtil {
	void *vd;
	void (*mousePresseEvent)(void *vd, int button);
	void (*mouseReleaseEvent)(void *vd, int button);
	void (*mouseMoveEvent)(void *vd, int x, int y);
};

struct FramePlane {
	int dataPitch, dataLines;
	int framePitch, frameLines;
};

struct VideoFormat {
	uint32_t fourcc;
	int width;
	int height;
	FramePlane planes[VIDEO_FRAME_MAX_PLANE_COUNT];
	int planeCount;
	double sar;
	double fps;
};

#endif // AVMISC_HPP
