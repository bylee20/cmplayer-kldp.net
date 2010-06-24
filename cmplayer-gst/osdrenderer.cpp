#include "osdrenderer.hpp"
#include "imageoverlayfilter.hpp"
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QEvent>
#include "osdstyle.hpp"

static const int RerenderEvent = QEvent::User + 100;

struct OsdRenderer::Data {
	OsdStyle style;
	QRect area;
	ImageOverlayFilter *overlay;
	int id;
	double dis_x, dis_y;
};

OsdRenderer::OsdRenderer()
: d(new Data) {
	d->area = QRect(0, 0, 400, 300);
	d->overlay = 0;
	d->id = -1;
	d->dis_x = d->dis_y = 1.0;
}

OsdRenderer::~OsdRenderer() {
	delete d;
}

void OsdRenderer::setStyle(const OsdStyle &style) {
	d->style = style;
	styleChanged(d->style);
}

const OsdStyle &OsdRenderer::style() const {
	return d->style;
}

void OsdRenderer::setArea(const QRect &rect, double dis_x, double dis_y) {
	if (d->area != rect || !qFuzzyCompare(d->dis_x, dis_x) || !qFuzzyCompare(d->dis_y, dis_y)) {
		d->area = rect;
		d->dis_x = dis_x;
		d->dis_y = dis_y;
		areaChanged(rect);
	}
}

QPoint OsdRenderer::pos() const {
	QPoint p = posHint();
	p.rx() /= d->dis_x;
	p.ry() /= d->dis_y;
	if (p.x()%2)
		++p.rx();
	if (p.y()%2)
		++p.ry();
	return p;
}

QRect OsdRenderer::area() const {
	return d->area;
}

void OsdRenderer::update() {
	emit needToRerender();
	QEvent *event = new QEvent(static_cast<QEvent::Type>(RerenderEvent));
	QCoreApplication::postEvent(this, event);
}

void OsdRenderer::rerender() {
	if (!d->overlay)
		return;
	QSize size = sizeHint();
	QPoint pos = this->pos();
	int x = 0, y = 0;
	if (!size.isEmpty()) {
		size.rwidth() /= d->dis_x;
		size.rheight() /= d->dis_y;
		if (size.width()%2)
			size.rwidth() += 1;
		if (size.height()%2)
			size.rheight() += 1;

		if (pos.x() < 0) {
			size.rwidth() += pos.x();
			x = pos.x();
			pos.rx() = 0;
		}
		if (pos.y() < 0) {
			size.rheight() += pos.y();
			y = pos.y();
			pos.ry() = 0;
		}
	}
	if (size.isEmpty())
		d->overlay->setOverlay(d->id, 0, QSize(0, 0), QPoint(0, 0));
	else {
		uchar *data = new uchar[size.width()*size.height()<<2];
		QImage image(data, size.width(), size.height(), QImage::Format_ARGB32);
		image.fill(0);
		QPainter painter(&image);
		painter.translate(x, y);
		painter.scale(1.0/d->dis_x, 1.0/d->dis_y);
		render(&painter);
		d->overlay->setOverlay(d->id, data, size, pos);
	}
}

void OsdRenderer::setImageOverlay(ImageOverlayFilter *overlay) {
	if (d->overlay != overlay) {
		if (d->overlay)
			d->overlay->removeOverlay(d->id);
		d->overlay = overlay;
		if (d->overlay) {
			d->id = d->overlay->newOverlay();
			update();
		}
	}
}

void OsdRenderer::customEvent(QEvent *event) {
	if (event->type() == RerenderEvent) {
		rerender();
		event->accept();
	}
}
