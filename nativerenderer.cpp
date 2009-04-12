#include "nativerenderer.h"
#include "playengine.h"
#include <QtGui/QApplication>
#include <QtCore/QDebug>
// #include <X11/Xlib.h>
#include <gst/interfaces/xoverlay.h>
#include <gst/interfaces/propertyprobe.h>

namespace Gst {

NativeRenderer::NativeRenderer(PlayEngine *engine, XType type)
: Core::NativeRenderer(engine), GstVideoIface(this), d(new Data) {
	d->type = type;
	GstElement *sink = makeSink(sinkName(d->type));
	qDebug() << sink;
	if (d->type == Xv && sink) {
		if (gst_element_set_state(sink, GST_STATE_READY) != GST_STATE_CHANGE_SUCCESS) {
			gst_object_unref(GST_OBJECT(sink));
			sink = 0;
		} else {
			g_object_set(G_OBJECT(sink), "brightness", 0, NULL);
			g_object_set(G_OBJECT(sink), "contrast", 0, NULL);
			g_object_set(G_OBJECT(sink), "hue", 0, NULL);
			g_object_set(G_OBJECT(sink), "saturation", 0, NULL);
		}
	}
	if (sink)
		setSink(sink);
	installEventFilter(screen());
	setOverlay();
}

NativeRenderer::~NativeRenderer() {
	delete d;
}

void NativeRenderer::setOverlay() {
	if (sink() && GST_IS_X_OVERLAY(sink())) {
		WId windowId = screenWinId();
		QApplication::syncX();
		gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink()) ,  windowId);
	}
	windowExposed();
}

void NativeRenderer::windowExposed() {
	QApplication::syncX();
	if (sink() && GST_IS_X_OVERLAY(sink()))
		gst_x_overlay_expose(GST_X_OVERLAY(sink()));
}

bool NativeRenderer::eventFilter(QObject *obj, QEvent *event) {
	if (obj == screen() && event->type() == QEvent::Paint) {
		windowExposed();
		return true;
	}
	return Core::NativeRenderer::eventFilter(obj, event);
}

QString NativeRenderer::sinkName(XType type) {
	if (type == Xv)
		return QString("xvimagesink");
	else if (type == X11)
		return QString("ximagesink");
	return QString();
}

bool NativeRenderer::isAvaiable(XType type) {
	GstElementFactory *f = gst_element_factory_find(sinkName(type).toLocal8Bit());
	if (!f)
		return false;
	gst_object_unref(GST_OBJECT(f));
	return true;
}

GstElement *NativeRenderer::makeSink(const QString &name) {
	return gst_element_factory_make(name.toLocal8Bit(), 0);
}

} // namespace Gst
