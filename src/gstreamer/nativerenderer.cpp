//#include "nativerenderer.h"
//
//namespace Gst {
//
//NativeRenderer::NativeRenderer()
//: d(new Data) {
//	d->sink = gst_element_factory_make ("xvimagesink", 0);
//	if (d->sink) {
//		if (gst_element_set_state(d->sink, GST_STATE_READY) != GST_STATE_CHANGE_SUCCESS) {
//			gst_object_unref(GST_OBJECT(d->sink));
//			d->sink = 0;
//		} else {
//			g_object_set(G_OBJECT(d->sink), "brightness", 0, NULL);
//			g_object_set(G_OBJECT(d->sink), "contrast", 0, NULL);
//			g_object_set(G_OBJECT(d->sink), "hue", 0, NULL);
//			g_object_set(G_OBJECT(d->sink), "saturation", 0, NULL);
//		}
//	}
//
//	if (!d->sink)
//		d->sink = gst_element_factory_make ("ximagesink", NULL);
//	gst_object_ref(GST_OBJECT(d->sink));
//	gst_object_sink(GST_OBJECT(d->sink));
//	setOverlay();
//}
//
//NativeRenderer::~NativeRenderer() {
//	gst_object_unref(GST_OBJECT(d->sink));
//	delete d;
//}
//
//} // namespace Gst
