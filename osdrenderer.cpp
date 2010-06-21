#include "osdrenderer.hpp"
#include "imageoverlayfilter.hpp"
#include <QtCore/QSettings>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>

void OsdStyle::save(QSettings *set, const QString &group) const {
	set->beginGroup(group);
	set->setValue("font", font);
	set->setValue("bgColor", bgColor);
	set->setValue("fgColor", fgColor);
	set->setValue("borderWidth", borderWidth);
	set->setValue("textSize", textSize);
	set->setValue("alignment", int(alignment));
	if (scale == FitToWidth)
		set->setValue("scale", "FitToWidth");
	else if (scale == FitToHeight)
		set->setValue("scale", "FitToHeight");
	else
		set->setValue("scale", "FitToDiagonal");
	set->endGroup();
}

void OsdStyle::load(QSettings *set, const QString &group) {
	set->beginGroup(group);
	font = set->value("font", font).value<QFont>();
	bgColor = set->value("bgColor", bgColor).value<QColor>();
	fgColor = set->value("fgColor", fgColor).value<QColor>();
	borderWidth = set->value("borderWidth", borderWidth).toDouble();
	textSize = set->value("textSize", textSize).toDouble();
	alignment = Qt::Alignment(set->value("alignment", int(alignment)).toInt());
	const QString scale = set->value("scale", "FitToDiagonal").toString();
	if (scale == "FitToWidth")
		this->scale = FitToWidth;
	else if (scale == "FitToHeight")
		this->scale = FitToHeight;
	else
		this->scale = FitToDiagonal;
	set->endGroup();
}

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

	connect(this, SIGNAL(needToRerender()), this, SLOT(rerender()), Qt::QueuedConnection);
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

void OsdRenderer::rerender() {
	if (!d->overlay)
		return;
	QSize size = sizeHint();
	if (!size.isEmpty()) {
		size.rwidth() /= d->dis_x;
		size.rheight() /= d->dis_y;
		if (size.width()%2)
			size.rwidth() += 1;
		if (size.height()%2)
			size.rheight() += 1;
		QImage image(size, QImage::Format_ARGB32);
		image.fill(0);
		QPainter painter(&image);
		painter.scale(1.0/d->dis_x, 1.0/d->dis_y);
		render(&painter);
		d->overlay->setOverlay(d->id, image, pos());
	} else
		d->overlay->setOverlay(d->id, QImage(), pos());
}

void OsdRenderer::setImageOverlay(ImageOverlayFilter *overlay) {
	if ((d->overlay = overlay)) {
		d->id = d->overlay->newOverlay();
		rerender();
	}
}
