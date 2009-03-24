#include "nativerenderer.h"
#include "xinepost.h"
#include "xineosd.h"
#include <core/newframeevent.h>
#include <core/videoframe.h>
#include <core/utility.h>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

namespace Xine {

struct NativeRenderer::Data {
	Core::PlayEngine *engine;
	QList<XineOsd*> osds;
	double destAspect;
};

NativeRenderer::NativeRenderer(Core::PlayEngine *engine, XineStream *stream)
: Core::NativeRenderer(engine), XineVideoIface(stream), d(new Data) {
	d->destAspect = 1.;
	d->engine = engine;
	screen()->installEventFilter(this);
	connect(this, SIGNAL(osdRectChanged(const QRect&)), this, SLOT(updateOsdRect(const QRect&)));
}

NativeRenderer::~NativeRenderer() {
	for (int i=0; i<d->osds.size(); ++i)
		delete d->osds[i];
	delete d;
}

void NativeRenderer::cbDestSize(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_width, int *dest_height, double *dest_pixel_aspect) {
	NativeRenderer *self = reinterpret_cast<NativeRenderer*>(user_data);
	QWidget *screen = self->screen();
	*dest_width = screen->width();
	*dest_height = screen->height();
	*dest_pixel_aspect = self->d->destAspect;
}

void NativeRenderer::cbFrameOutput(void *user_data, int video_width, int video_height,
		double video_pixel_aspect, int *dest_x, int *dest_y, int *dest_width, int *dest_height,
		double *dest_pixel_aspect, int *win_x, int *win_y) {
	NativeRenderer *self = reinterpret_cast<NativeRenderer*>(user_data);
	QWidget *screen = self->screen();
	*dest_x = 0;
	*dest_y = 0;
	*win_x = screen->x();
	*win_y = screen->y();
	*dest_width = screen->width();
	*dest_height = screen->height();
	*dest_pixel_aspect = self->d->destAspect;
	Core::VideoFrame frame;
	Core::VideoFrame::Info info;
	info.size.rwidth() = video_width;
	info.size.rheight() = video_height;
	info.pixelAspectRatio = video_pixel_aspect;
	frame.setInfo(info);
	QApplication::postEvent(self, new Core::NewFrameEvent(frame));
}

bool NativeRenderer::eventFilter(QObject *obj, QEvent *event) {
	if (obj == screen()) {
		QWidget *screen = this->screen();
		switch(event->type()) {
		case QEvent::Paint:
			if (!d->engine->isStopped()) {
				const QRect rect = screen->rect();
				xcb_expose_event_t xcb_event;
				memset(&xcb_event, 0, sizeof(xcb_event));
				xcb_event.window = screen->winId();
				xcb_event.x = rect.x();
				xcb_event.y = rect.y();
				xcb_event.width = rect.width();
				xcb_event.height = rect.height();
				xcb_event.count = 0;
				xine_port_send_gui_data(stream()->videoPort
						, XINE_GUI_SEND_EXPOSE_EVENT, &xcb_event);
				return true;
			}
			break;
		case QEvent::MouseMove: {
			QMouseEvent *me = static_cast<QMouseEvent*>(event);
			mouseMoved(me->pos());
			break;
		} case QEvent::MouseButtonPress: {
			QMouseEvent *me = static_cast<QMouseEvent*>(event);
			mouseClicked(me->pos(), me->button());
			break;
		} default:
			break;
		}
	}
	return Core::NativeRenderer::eventFilter(obj, event);
}

void NativeRenderer::customEvent(QEvent *event) {
	Core::BaseEvent *be = static_cast<Core::BaseEvent*>(event);
	if (be->type() != Core::BaseEvent::NewFrame)
		return;
	const Core::VideoFrame::Info info = ((Core::NewFrameEvent*)be)->frame().info();
	if (frameInfo() != info)
		updateFrameInfo(info);
}

Core::AbstractOsdRenderer *NativeRenderer::createOsd() {
	XineOsd *osd = new XineOsd(stream());
	d->osds.push_back(osd);
	return osd;
}

void NativeRenderer::updateOsdRect(const QRect &rect) {
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->setRect(rect);
}

void NativeRenderer::setVideoSize(const QSize &size) {
	if (videoSize() != size) {
		updateVideoSize(size);
		rerender();
	}
}

void NativeRenderer::rerender() {
	d->destAspect = videoRatio() / aspectRatioF();
	Core::NativeRenderer::rerender();
}

const Core::PlayEngine *NativeRenderer::engine() const {
	return d->engine;
}

}
