#ifndef GSTVIDEOMAN_P_HPP
#define GSTVIDEOMAN_P_HPP

#include <QtCore/QtGlobal>
#include <gst/gst.h>
#include <QtCore/QMutex>
#include <QtGui/QImage>
#ifndef UINT64_C
#define UINT64_C Q_UINT64_C
#endif
extern "C" {
#include <libavcodec/avcodec.h>
}
#undef UINT64_C
#include <gst/video/video.h>

struct ImageOverlay {
	mutable QMutex mutex;
	ImageOverlay(int id = 0, double zIndex = 0.0)
	: zIndex(zIndex), id(id) {}
	double zIndex; int id;
	QImage image;
	QPoint pos;
};

struct YUV420Frame {
	void init(uchar *data, GstVideoFormat format, int width, int height);
	void init(uchar *data, int width, int height);
	void fill();
	void lay(const YUV420Frame &image, int crop_h, int crop_v);
	void blend(const ImageOverlay &overlay);
	bool fromPicture(const AVPicture &pic, PixelFormat format, int width, int height);
	int size_y() const {return width_y*height_y;}
	int size_uv() const {return width_uv*height_uv;}
	uchar *y, *u, *v;
	int width_y, width_uv;
	int height_y, height_uv;
};

GstCaps *makeVideoCaps(const char *mimetype, const char *fieldname, ...);
GstCaps *capsFromPixFmt(PixelFormat pix_fmt);
void capsWithCodec(const GstCaps *caps, AVCodecContext *context);

//void fillFrame(YUV420Frame *frame, uchar *data, GstVideoFormat format, int width, int height);

//void manFrame(YUV420Frame *frame, const YUV420Frame &image);
//	       , const AVPicture &src, int src_w, int src_h/*, int crop_w, int crop_h*/);

//bool convertImageFormat(AVPicture *dest, PixelFormat dest_fmt
//		, const AVPicture &src, PixelFormat src_fmt, int width, int height);

#endif // GSTVIDEOMAN_P_HPP
