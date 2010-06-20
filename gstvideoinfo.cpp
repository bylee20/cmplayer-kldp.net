//#include "gstvideoinfo.hpp"
//#include "nativevideorenderer.hpp"
//#include <gst/controller/gstcontroller.h>
//#include <gst/video/video.h>
//#include <QtCore/QDebug>
//#include <QtGui/QPainter>
//
//static GType gst_video_info_get_type();
//
//GST_BOILERPLATE(GstVideoInfo, gst_video_info, GstBaseTransform, GST_TYPE_BASE_TRANSFORM);
//
//GType GstVideoInfo::gtype() {return gst_video_info_get_type();}
//
//static void gst_video_info_finalize(GObject *obj) {
//	delete GST_VIDEO_INFO(obj)->d;
//	G_OBJECT_CLASS(parent_class)->finalize(obj);
//}
//
//void gst_video_info_base_init(gpointer klass) {
//	GstElementClass *elem = GST_ELEMENT_CLASS(klass);
//	static GstStaticPadTemplate sinkTemplate =
//		GST_STATIC_PAD_TEMPLATE (
//			"sink",
//			GST_PAD_SINK,
//			GST_PAD_ALWAYS,
//			GST_STATIC_CAPS ("ANY")
//		);
//	static GstStaticPadTemplate srcTemplate =
//		GST_STATIC_PAD_TEMPLATE (
//			"src",
//			GST_PAD_SRC,
//			GST_PAD_ALWAYS,
//			GST_STATIC_CAPS ("ANY")
//		);
//	gst_element_class_add_pad_template(elem, gst_static_pad_template_get(&srcTemplate));
//	gst_element_class_add_pad_template(elem, gst_static_pad_template_get(&sinkTemplate));
//}
//
//static GstFlowReturn gst_video_info_transform_ip(GstBaseTransform */*base*/, GstBuffer */*outbuf*/) {
//	return GST_FLOW_OK;
//}
//
//static gboolean gst_video_info_set_caps(GstBaseTransform *trans, GstCaps *in, GstCaps *out) {
//	return GST_VIDEO_INFO(trans)->setCaps(in, out);
//}
//
//static void gst_video_info_class_init(GstVideoInfoClass *klass) {
//	GObjectClass *gobj = G_OBJECT_CLASS(klass);
//	gobj->finalize = gst_video_info_finalize;
//
//	GstBaseTransformClass *trans = GST_BASE_TRANSFORM_CLASS(klass);
//	trans->transform_ip = gst_video_info_transform_ip;
//	trans->set_caps = gst_video_info_set_caps;
//}
//
//static void gst_video_info_init(GstVideoInfo *self, GstVideoInfoClass *klass) {
//	Q_UNUSED(klass);
//	self->d = new GstVideoInfo::Data;
//}
//
//bool GstVideoInfo::setCaps(GstCaps */*in*/, GstCaps *out) {
//	QSize s;	GstVideoFormat format;
//	if (!gst_video_format_parse_caps(out, &format, &s.rwidth(), &s.rheight()))
//		return false;
//	int fr_d, fr_n;
//	if (!gst_video_parse_caps_framerate(out, &fr_n, &fr_d))
//		return false;
//	int pa_n, pa_d;
//	gst_video_parse_caps_pixel_aspect_ratio(out, &pa_n, &pa_d);
////	qDebug() << pa_n << pa_d;
//	d->renderer->setFrameSize(s);
//	d->renderer->setFrameRate((double)fr_n/fr_d);
//	return true;
//}
//
