//from KDE Phonon GStreamer backend

#include <QApplication>
#include <core/newframeevent.h>
#include <QtGui/QWidget>
#include "qwidgetvideosink.h"
#include <QtCore/QDebug>

namespace Gst {

static GType qwidget_video_sink_get_type();

GstVideoSinkClass *QWidgetVideoSink::parentClass = 0;

GType QWidgetVideoSink::gtype() {return qwidget_video_sink_get_type();}

GST_BOILERPLATE_FULL(QWidgetVideoSink, qwidget_video_sink, GstVideoSink
		, GST_TYPE_VIDEO_SINK, QWidgetVideoSink::additionalInitialize);

void QWidgetVideoSink::additionalInitialize(GType type) {
	static const GInterfaceInfo ifaceInfo = {(GInterfaceInitFunc)initializeIface, 0, 0};
	static const GInterfaceInfo navInfo = {(GInterfaceInitFunc)initializeNavigation, 0, 0};
	g_type_add_interface_static(type, GST_TYPE_IMPLEMENTS_INTERFACE, &ifaceInfo);
	g_type_add_interface_static(type, GST_TYPE_NAVIGATION, &navInfo);
}

void QWidgetVideoSink::initializeIface(GstImplementsInterfaceClass *klass) {
	klass->supported = isSupported;
}

gboolean QWidgetVideoSink::isSupported(GstImplementsInterface */*iface*/, GType type) {
	return type == GST_TYPE_NAVIGATION;
}

void QWidgetVideoSink::initializeNavigation(GstNavigationInterface *iface) {
	iface->send_event = sendEventToNavigation;
}

void QWidgetVideoSink::sendEventToNavigation(GstNavigation *navigation, GstStructure *structure) {
	QWidgetVideoSink *self = GST_QWIDGET_VIDEO_SINK(navigation);
	
	GstVideoRectangle src, dst, result;
	src.w = GST_VIDEO_SINK_WIDTH(self);
	src.h = GST_VIDEO_SINK_HEIGHT(self);
	dst.w = self->width;
	dst.h = self->height;
	gst_video_sink_center_rect(src, dst, &result, false);
	
	GstEvent *event = gst_event_new_navigation(structure);

	double x = 0.0, y = 0.0;
	gst_structure_get_double(structure, "pointer_x", &x);
	gst_structure_get_double(structure, "pointer_y", &y);
	GstPad *pad = gst_pad_get_peer(GST_VIDEO_SINK_PAD(self));
	if (GST_IS_PAD(pad) && GST_IS_EVENT(event)) {
		gst_pad_send_event(pad, event);
		gst_object_unref(pad);
	}
}

void QWidgetVideoSinkClass::initialize() {
	if (!m_init) {
		GstElementClass *klass = GST_ELEMENT_CLASS(this);
		static const QList<guint32> formats = QList<guint32>()
			<< GST_MAKE_FOURCC('I', '4', '2', '0')
			<< GST_MAKE_FOURCC('Y', 'V', '1', '2')
			<< GST_MAKE_FOURCC('Y', 'U', 'Y', '2');
		GstCaps *caps = gst_caps_new_empty();
		for (int i = 0; i < formats.size(); i++) {
			GstStructure *structure = gst_structure_new("video/x-raw-yuv"
				, "format", GST_TYPE_FOURCC, formats[i]
				, "framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 100, 1
				, "width", GST_TYPE_INT_RANGE, 1, G_MAXINT
				, "height", GST_TYPE_INT_RANGE, 1, G_MAXINT
				, "bpp", G_TYPE_INT, 32, NULL);
			gst_caps_append_structure(caps, structure);
		}
		GstPadTemplate *pad = gst_pad_template_new("sink", GST_PAD_SINK, GST_PAD_ALWAYS, caps);
		gst_element_class_add_pad_template (klass, pad);
		m_init = true;
	}
}

static void qwidget_video_sink_base_init(gpointer klass) {
	QWidgetVideoSinkClass *self = GST_QWIDGET_VIDEO_SINK_CLASS(klass);
	if (self)
		self->initialize();
}

void QWidgetVideoSink::initialize(QWidgetVideoSinkClass *klass) {
	if (!klass->m_klass) {
		GstBaseSinkClass *sink = GST_BASE_SINK_CLASS(klass);
		GstElementClass *element = GST_ELEMENT_CLASS(klass);
		
		parentClass = reinterpret_cast<GstVideoSinkClass*>(g_type_class_peek_parent(klass));
		
		sink->set_caps = setCaps;
		sink->preroll = render;
		sink->render = render;
		
		element->change_state = changeState;
		klass->m_klass = true;
	}
}

static void qwidget_video_sink_class_init(QWidgetVideoSinkClass *klass) {
	QWidgetVideoSink::initialize(klass);
}

static void qwidget_video_sink_init(QWidgetVideoSink *self, QWidgetVideoSinkClass */*klass*/) {
	self->sink = 0;
	self->width = 0;
	self->height = 0;
	self->bpp = 0;
	self->depth = 0;
}

gboolean QWidgetVideoSink::setCaps(GstBaseSink* sink, GstCaps* caps) {
	GstStructure* data = gst_caps_get_structure(caps, 0);
	QWidgetVideoSink *self = GST_QWIDGET_VIDEO_SINK(sink);
	gst_structure_get_fourcc(data, "format", &self->fourcc);
	gst_structure_get_int(data, "width", &self->width);
	gst_structure_get_int(data, "height", &self->height);
// 	gst_structure_get_int(data, "bpp", &self->bpp);
// 	gst_structure_get_int(data, "depth", &self->depth);
// 	const GValue *framerate = gst_structure_get_value(data, "framerate");
	const GValue *aspect = gst_structure_get_value(data, "pixel-aspect-ratio");
	if (aspect) {
		const gint n = gst_value_get_fraction_numerator(aspect);
		const gint d = gst_value_get_fraction_denominator(aspect);
		self->pixelAspect = double(n)/double(d);
	} else
		self->pixelAspect = 1.0;
	return true;
}

GstStateChangeReturn QWidgetVideoSink::changeState(GstElement* element, GstStateChange transition) {
	return GST_ELEMENT_CLASS(parentClass)->change_state(element, transition);
}

GstFlowReturn QWidgetVideoSink::render(GstBaseSink* sink, GstBuffer* buffer) {
	if (buffer != 0) {
		QWidgetVideoSink *self = GST_QWIDGET_VIDEO_SINK(sink);
		Core::NewFrameEvent *event = 0;
		Core::VideoFrame *frame = new Core::VideoFrame();
		Core::VideoFrame::Info info;
		info.setSize(self->width, self->height);
		switch (self->fourcc) {
		case GST_MAKE_FOURCC('I', '4', '2', '0'):
			info.type = Core::VideoFrame::I420;
			break;
		case GST_MAKE_FOURCC('Y', 'V', '1', '2'):
			info.type = Core::VideoFrame::YV12;
			break;
		case GST_MAKE_FOURCC('Y', 'U', 'Y', '2'):
			info.type = Core::VideoFrame::YUY2;
			break;
		default:
			info.type = Core::VideoFrame::NoFrame;
			break;
		}
		info.pixelAspectRatio = self->pixelAspect;
		frame->setInfo(info);
		frame->setData(buffer->data, buffer->size);
		event = new Core::NewFrameEvent(*frame);
		delete frame;
		QApplication::postEvent(self->sink, event);
		return GST_FLOW_OK;
	} else
		return GST_FLOW_ERROR;
}

}
