#include "glrenderer.h"
#include "playengine.h"
#include <QDebug>
#include <gst/interfaces/navigation.h>
#include <QtGui/QMouseEvent>

namespace Gst {

struct GLRenderer::Data {
	GstElement *bin;
	PlayEngine *engine;
};

GLRenderer::GLRenderer(PlayEngine *engine)
: d(new Data) {
	d->bin = 0;
	d->engine = engine;
	setMouseTracking(true);
}

GLRenderer::~GLRenderer() {
	delete d;
}

void GLRenderer::setVideoBin(GstElement *bin) {
	d->bin = bin;
}

void GLRenderer::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton)
		navEvent("mouse-button-release", 1, event->pos());
	Core::GLRenderer::mouseReleaseEvent(event);
}

void GLRenderer::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton)
		navEvent("mouse-button-press", 1, event->pos());
	Core::GLRenderer::mousePressEvent(event);
}

void GLRenderer::mouseMoveEvent(QMouseEvent *event) {
	navEvent("mouse-move", 0, event->pos());
	Core::GLRenderer::mouseMoveEvent(event);
}

void GLRenderer::navEvent(const char *name, const int button, const QPoint &pos) {
	if (!d->bin || !d->engine->currentSource().isDisc())
		return;
	GstElement *iface = gst_bin_get_by_interface(GST_BIN(d->bin), GST_TYPE_NAVIGATION);
	GstNavigation *nav = GST_NAVIGATION(iface);
	if (nav) {
		const QPointF p = mapWidgetToScreen(pos);
		gst_navigation_send_mouse_event(nav, name, button, p.x(), p.y());
	}
	if (iface)
		gst_object_unref(iface);
}

}
