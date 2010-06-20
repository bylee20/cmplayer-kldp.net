#ifndef GSTVIDEOMAN_HPP
#define GSTVIDEOMAN_HPP

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/base/gstbasetransform.h>
#include <QtCore/QMutex>
#include <QtCore/QMultiMap>
#include <QtGui/QImage>
#ifndef UINT64_C
#define UINT64_C Q_UINT64_C
#endif
extern "C" {
#include <libavcodec/avcodec.h>
}
#undef UINT64_C

class ImageOverlay;		class NativeVideoRenderer;

typedef QMultiMap<double, ImageOverlay*> OverlayMap;

struct VideoInfo {
	int width, height;
	double fps;
};

struct GstVideoMan {
	GstBaseTransform parent;
	ImageOverlay *getOverlay(int id);
public:
	void ctor();
	void dtor();
	struct Data {
		QMutex mutex;
		int in_width, in_height, out_width, out_height;
		GstVideoFormat in_format, out_format;
		PixelFormat from_pixfmt, to_pixfmt;
		double in_fps, out_fps;
		double in_par, out_par; // pixel-aspect-ratio
		int border_h, border_v;
		int tempBufferSize;
		uchar *tempBuffer;
		int crop_v, crop_h;
		OverlayMap overlay;
		NativeVideoRenderer *renderer;
	};
	Data *d;

	QRect videoRect() const {
		const int x = (d->border_h+d->crop_h)>>1;
		const int y = (d->border_v+d->crop_v)>>1;
		const int width = d->out_width-(x<<1);
		const int height = d->out_height-(y<<1);
		return QRect(x, y, width, height);
	}
	void updateTempBuffer();
	void setBorder(int h, int v);
	void crop(int h, int v);
	bool setCaps(GstCaps *in, GstCaps *out);
	GstCaps *transformCaps(GstPadDirection dir, GstCaps *caps);
	GstFlowReturn transform(GstBuffer *in, GstBuffer *out);
	bool getUnitSize(GstCaps *caps, uint *size);
	void fixateCaps(GstPadDirection dir, GstCaps *caps, GstCaps *opp);
	int newOverlay();
	void setOverlay(int id, const QImage &image, const QPoint &pos);
	void setZIndex(int id, double zIndex);
};

struct GstVideoManClass {
	GstBaseTransformClass parent;
};

GType gst_video_man_get_type (void);

#define GST_TYPE_VIDEO_MAN 	      (gst_video_man_get_type())
#define GST_VIDEO_MAN(obj) 	      (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_MAN,GstVideoMan))
#define GST_VIDEO_MAN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_MAN,GstVideoManClass))
#define GST_IS_VIDEO_MAN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_MAN))
#define GST_IS_VIDEO_MAN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_MAN))

#endif // GSTVIDEOMAN_HPP
