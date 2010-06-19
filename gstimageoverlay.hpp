#ifndef GSTIMAGEOVERLAY_HPP
#define GSTIMAGEOVERLAY_HPP

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <QtGui/QImage>
#include <QtCore/QMutex>
#include <gst/video/video.h>
#include <QtCore/QMultiMap>

class OsdRenderer;		class PlayEngine;

struct GstImageOverlayClass {
	GstBaseTransformClass parent;
};

class GstImageOverlay {
	GstBaseTransform parent;
	struct Image {
		mutable QMutex *mutex;
		Image(int id = 0, double zIndex = 0.0)
		: mutex(0), zIndex(zIndex), id(id) {}
		double zIndex; int id;
		QImage image;
		QPoint pos;
	};
	Image *getImage(int id);
	void blend(GstBuffer *buffer, const Image &image);
public:
	int register_() const;
	void setImage(int id, const QImage &image, const QPoint &pos);
	void setZIndex(int id, double zIndex);
	struct Data {
		Data(): frameFormat(GST_VIDEO_FORMAT_UNKNOWN), engine(0) {}
//		QMutex mutex;
		QSize frameSize;
		GstVideoFormat frameFormat;
		QMultiMap<double, Image> image;
		PlayEngine *engine;
	};
	static GstCaps *makeDefualtCaps();
	static GType gtype();
	GstFlowReturn transform_ip(GstBuffer *outbuf);
	bool set_caps(GstCaps *in, GstCaps *out);
	Data *d;
};

#define GST_TYPE_IMAGE_OVERLAY (gst_image_overlay_get_type())
#define GST_IMAGE_OVERLAY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_IMAGE_OVERLAY,GstImageOverlay))
#define GST_IMAGE_OVERLAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_IMAGE_OVERLAY,GstImageOverlayClass))
#define GST_IS_IMAGE_OVERLAY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_IMAGE_OVERLAY))
#define GST_IS_IMAGE_OVERLAY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_IMAGE_OVERLAY))


#endif // GSTIMAGEOVERLAY_HPP
