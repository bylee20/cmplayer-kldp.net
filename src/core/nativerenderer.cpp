#include "nativerenderer.h"
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include "playengine.h"
#include <QtCore/QDebug>

namespace Core {

class NativeRenderer::Screen : public QWidget {
public:
	Screen(NativeRenderer *renderer, QWidget *parent)
	: QWidget(parent), renderer(renderer) {}
	bool originalX11Event(XEvent *event) {
		return QWidget::x11Event(event);
	}
private:
	bool x11Event(XEvent *event) {
		return renderer->screenX11Event(event);
	}
	NativeRenderer *renderer;
};
	
NativeRenderer::NativeRenderer(PlayEngine *engine)
: QWidget(), VideoRendererIface(this), d(new Data) {
	d->engine = engine;
	d->video = new QWidget(this);
	d->screen = new Screen(this, d->video);
	d->screen->show();
	d->screen->move(0, 0);
	d->screen->setAutoFillBackground(true);
	d->screen->setMouseTracking(true);
	d->screen->installEventFilter(this);
	d->video->setMouseTracking(true);
	d->video->setAutoFillBackground(true);
	d->video->installEventFilter(this);
	setMouseTracking(true);
	setAutoFillBackground(true);
	d->screen->setAttribute(Qt::WA_PaintOnScreen);
	d->screen->setAttribute(Qt::WA_OpaquePaintEvent);
	installEventFilter(this);
}

NativeRenderer::~NativeRenderer() {
	delete d;
}

bool NativeRenderer::eventFilter(QObject *obj, QEvent *event) {
	if (event->type() == QEvent::Paint && (obj == this || obj == d->video
			|| (obj == d->screen && d->engine->isStopped()))) {
		QWidget *widget = static_cast<QWidget*>(obj);
		QPainter painter(widget);
		painter.fillRect(widget->rect(), Qt::black);
		return true;
	} else
		return QWidget::eventFilter(obj, event);
}

void NativeRenderer::resizeEvent(QResizeEvent */*event*/) {
	rerender();
}

void NativeRenderer::setVideoSize(const QSize &size) {
	if (videoSize() != size) {
		updateVideoSize(size);
		rerender();
	}
}

void NativeRenderer::setFrameSize(const QSize &size) {
	VideoFrame::Info info = frameInfo();
	info.size = size;
	setFrameInfo(info);
}

void NativeRenderer::setFrameInfo(const VideoFrame::Info &frame) {
	if (frame != frameInfo()) {
		updateFrameInfo(frame);
		rerender();
	}
}

int NativeRenderer::screenWinId() const {
	return (int)d->screen->winId();
}

void NativeRenderer::rerender() {
// 	d->destAspect = videoRatio() / aspectRatioF();
	QSizeF widget = size();
	const double aspect = aspectRatioF();
	QSizeF video(aspect, 1.0), screen(frameInfo().size);
	const bool expanded = frameInfo().size != videoSize();
	if (expanded) {
		video.scale(widget, Qt::KeepAspectRatio);
		screen.rwidth() *= aspect / videoRatio();
		screen.scale(video, Qt::KeepAspectRatioByExpanding);
	}
	if (cropRatio() > .0) {
		video = QSizeF(cropRatio(), 1.);
		video.scale(widget, Qt::KeepAspectRatio);
		screen.scale(video, Qt::KeepAspectRatioByExpanding);
		if (cropRatio() < aspect)
			screen *= aspect / cropRatio();
	} else {
		if (expanded) {
			video.setHeight(qMin(widget.height(), screen.height()));
			video.setWidth(qMin(widget.width(), screen.width()));
		} else {
			video.scale(widget, Qt::KeepAspectRatio);
			screen = video;
		}
	}
	QRectF osd;
	const double rate = (screen.width() / frameInfo().size.width());
	osd.setSize(video/rate);
	osd.moveLeft((frameInfo().size.width() - osd.width())/2. + .5);
	osd.moveTop((frameInfo().size.height() - osd.height())/2. +.5);
	if (osd.toRect() != d->osdRect)
		emit osdRectChanged(d->osdRect = osd.toRect());
	QPoint pos;
	d->video->resize(video.toSize());
	d->screen->resize(screen.toSize());
	pos.setX((widget.width() - video.width())/2.);
	pos.setY((widget.height() - video.height())/2.);
	d->video->move(pos);
	pos.setX((video.width() - screen.width())/2);
	pos.setY((video.height() - screen.height())/2);
	d->screen->move(pos);
}

void NativeRenderer::calculate() {
	rerender();
}

bool NativeRenderer::screenX11Event(XEvent *event) {
	return static_cast<Screen*>(d->screen)->originalX11Event(event);
}

}
