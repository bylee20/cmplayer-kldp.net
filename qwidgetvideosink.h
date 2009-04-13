#ifndef GST_QWIDGETVIDEOSINK_H
#define GST_QWIDGETVIDEOSINK_H

//from KDE's Phonon GStreamer backend
#include <gst/video/gstvideosink.h>
#include <gst/interfaces/navigation.h>

namespace Gst {

struct QWidgetVideoSinkClass {
	GstVideoSinkClass parent;
	QWidgetVideoSinkClass(): m_init(false), m_klass(false) {}
	void initialize();
private:
	friend class QWidgetVideoSink;
	bool m_init, m_klass;
};

class QWidgetVideoSink {
public:
	GstVideoSink parent;
	static GType gtype();
	QWidgetVideoSink(): m_init(false) {}
	static void additionalInitialize(GType type);
	static gboolean isSupported(GstImplementsInterface *iface, GType type);
	static void initializeIface(GstImplementsInterfaceClass *klass);
	static void initialize(QWidgetVideoSinkClass *klass);
	static void initializeNavigation(GstNavigationInterface *iface);
	static void sendEventToNavigation(GstNavigation *navigation, GstStructure *structure);
	QWidget *sink;
	gint width;
	gint height;
	gint bpp;
	gint depth;
	gdouble pixelAspect;
	guint fourcc;
private:
	friend class QWidgetVideoSinkClass;
	static gboolean setCaps(GstBaseSink* self, GstCaps* caps);
	static GstStateChangeReturn changeState(GstElement* self, GstStateChange transition);
	static GstFlowReturn render(GstBaseSink* self, GstBuffer* buffer);
	bool m_initBase, m_init;
};

/* Standard macros for defining types for this element.  */
#define GST_TYPE_QWIDGET_VIDEO_SINK (qwidget_video_sink_get_type())
#define GST_QWIDGET_VIDEO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_QWIDGET_VIDEO_SINK, QWidgetVideoSink))
#define GST_QWIDGET_VIDEO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_QWIDGET_VIDEO_SINK, QWidgetVideoSinkClass))
#define GST_IS_QWIDGET_VIDEO_SINK_(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_QWIDGET_VIDEO_SINK))
#define GST_IS_QWIDGET_VIDEO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_QWIDGET_VIDEO_SINK))

}

#endif
