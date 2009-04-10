#include "gstvideoiface.h"

namespace Gst {

GstVideoIface::GstVideoIface(Core::VideoRendererIface *renderer)
: d(new Data) {
	d->renderer = renderer;
	d->sink = 0;
}

GstVideoIface::~GstVideoIface() {
	setSink(0);
	delete d;
}

void GstVideoIface::setSink(GstElement *sink) {
	if (d->sink == sink)
		return;
	if (d->sink)
		gst_object_unref(GST_OBJECT(d->sink));
	if ((d->sink = sink)) {
		gst_object_ref(GST_OBJECT(d->sink));
		gst_object_sink(GST_OBJECT(d->sink));
		g_object_set(G_OBJECT(d->sink), "sync", 1, NULL);
	}
}

}
