#ifndef GSTVIDEOINFO_HPP
#define GSTVIDEOINFO_HPP

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/video/video.h>
#include <QtCore/QSize>

class NativeVideoRenderer;

class GstVideoInfo {
	GstBaseTransform parent;
public:
	static GType gtype();
//private:
	static GstFlowReturn transform_ip(GstBaseTransform *base, GstBuffer *outbuf);
	bool setCaps(GstCaps *in, GstCaps *out);
	struct Data {
		Data(): renderer(0) {}
		NativeVideoRenderer *renderer;
	};
	Data *d;
};

struct GstVideoInfoClass {
	GstBaseTransformClass parent;
private:
};

#define GST_TYPE_VIDEO_INFO (gst_video_info_get_type())
#define GST_VIDEO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_INFO,GstVideoInfo))
#define GST_VIDEO_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_INFO,GstVideoInfoClass))
#define GST_IS_VIDEO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_INFO))
#define GST_IS_VIDEO_INFO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_INFO))

#endif // GSTVIDEOINFO_HPP
