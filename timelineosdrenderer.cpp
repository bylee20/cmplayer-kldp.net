#include "timelineosdrenderer.hpp"
#include <QtCore/QTimer>
#include <QtCore/QPoint>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

struct TimeLineOsdRenderer::Data {
	int time, duration;
	QTimer clearer;
	QPoint pos;
};

TimeLineOsdRenderer::TimeLineOsdRenderer(): d(new Data) {
	d->time = d->duration = -1;
	d->clearer.setSingleShot(true);
	connect(&d->clearer, SIGNAL(timeout()), this, SLOT(clear()));
	OsdStyle style = this->style();
	style.fgColor.setAlphaF(0.5);
	style.bgColor.setAlphaF(0.5);
	setStyle(style);

	connect(this, SIGNAL(areaChanged(QRect)), this, SLOT(slotAreaChanged(QRect)));
	connect(this, SIGNAL(styleChanged(OsdStyle)), this, SLOT(slotStyleChanged(OsdStyle)));
}

TimeLineOsdRenderer::~TimeLineOsdRenderer() {
	delete d;
}

void TimeLineOsdRenderer::show(int time, int duration, int last) {
	if (time < 0 || duration <= 0)
		return;
	d->clearer.stop();
	d->time = time;
	d->duration = duration;
	emit needToRerender();
//	invokeRerender();
	d->clearer.start(last);
}

void TimeLineOsdRenderer::clear() {
	d->time = d->duration = -1;
	emit needToRerender();
//	invokeRerender();
}

void TimeLineOsdRenderer::render(QPainter *painter) {
	if (d->time < 0 || d->duration <= 0)
		return;

	const OsdStyle &style = this->style();
	const QSizeF size = sizeHint();
	const int b = style.borderWidth * size.height() + 0.5;

	const double rate = (double)d->time/d->duration;
	const double inner_w = (size.width()-2*b)*rate;
	const QRectF left(0, b, b, size.height()-2*b-1);
	const QRectF inner(left.topRight(), left.bottomRight() + QPointF(inner_w, 0));
	const QRectF right(inner.topRight(), QSize(size.width() - inner.right(), inner.height()));
	const QRectF top(0, 0, size.width(), b);
	const QRectF bottom(0, size.height()-b-1, size.width(), b);

	painter->save();
	painter->fillRect(top, style.bgColor);
	painter->fillRect(bottom, style.bgColor);
	painter->fillRect(left, style.bgColor);
	painter->fillRect(right, style.bgColor);
	painter->fillRect(inner, style.fgColor);
	painter->restore();
}

QPoint TimeLineOsdRenderer::posHint() const {
	return d->pos;
}

QSize TimeLineOsdRenderer::sizeHint() const {
	const int height = area().height()*0.05 + 0.5;
	const int width = area().width()*0.8 + 0.5;
	return QSize(width, height);
}

void TimeLineOsdRenderer::slotAreaChanged(const QRect &area) {
	const QSize size = sizeHint();
	d->pos = area.topLeft();
	d->pos.rx() += (area.width() - size.width())*0.5 + 0.5;
	d->pos.ry() += (area.height() - size.height())*0.5 + 0.5;
	emit needToRerender();
//	invokeRerender();
}

void TimeLineOsdRenderer::slotStyleChanged(const OsdStyle &/*style*/) {
	emit needToRerender();
//	invokeRerender();
}
