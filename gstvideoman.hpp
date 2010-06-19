#ifndef GSTVIDEOMAN_HPP
#define GSTVIDEOMAN_HPP

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <QtCore/QMutex>
//#include "ffmpegcolorspace/avcodec.h"
#ifndef UINT64_C
#define UINT64_C Q_UINT64_C
#endif
extern "C" {
#include <libavcodec/avcodec.h>
}
#undef UINT64_C
#include <gst/video/video.h>
#include <QtCore/QMultiMap>
#include <QtGui/QImage>

#define GST_TYPE_VIDEO_MAN 	      (gst_video_man_get_type())
#define GST_VIDEO_MAN(obj) 	      (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_MAN,GstVideoMan))
#define GST_VIDEO_MAN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_MAN,GstVideoManClass))
#define GST_IS_VIDEO_MAN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_MAN))
#define GST_IS_VIDEO_MAN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_MAN))

GType gst_video_man_get_type (void);

class ImageOverlay;
typedef QMultiMap<double, ImageOverlay*> OverlayMap;

struct GstVideoMan {
	GstBaseTransform parent;
	ImageOverlay *getOverlay(int id);
public:
	void ctor();
	void dtor();
	struct Data {
		QMutex mutex;
		int in_w, in_h, out_w, out_h;
//		gboolean interlaced;
		float fps;
		PixelFormat from_pixfmt, to_pixfmt;
//		int deltaHeight, deltaWidth;
		int border_h, border_v;
		int tempBufferSize;
		GstVideoFormat out_format;
		uchar *tempBuffer;
		int crop_v, crop_h;
		OverlayMap overlay;
	};
	Data *d;
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

#endif // GSTVIDEOMAN_HPP
