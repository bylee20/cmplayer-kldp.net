#include "painterosdrenderer.h"
#include <QtCore/QRegExp>
#include <QtGui/QPainter>
#include <QtGui/QTextDocument>
#include <QtCore/QDebug>
#include <cmath>
#include "osdstyle.h"

namespace Core {

struct PainterOsdRenderer::Data {
	static const int Count = 12;
	static double Sines[Count];
	static double Cosines[Count];
	QTextDocument doc;
	QPointF points[Count];
	int px;
	double bw;
};

double PainterOsdRenderer::Data::Sines[PainterOsdRenderer::Data::Count];
double PainterOsdRenderer::Data::Cosines[PainterOsdRenderer::Data::Count];

PainterOsdRenderer::PainterOsdRenderer()
: d(new Data) {
	static bool init = false;
	if (!init) {
		const double pi = 3.14159265;
		const double factor = pi*2.0/double(Data::Count);
		for (int i=0; i<Data::Count; ++i) {
			const double theta = double(i)*factor;
			Data::Sines[i] = std::sin(theta);
			Data::Cosines[i] = std::cos(theta);
		}
		init = true;
	}
	d->bw = d->px = -1;
	applyFont();
	applyAlignment();
}

PainterOsdRenderer::~PainterOsdRenderer() {
	delete d;
}

QSizeF PainterOsdRenderer::textSize(const QSizeF &area) {
	if (d->px < 1 || text().isEmpty())
		return QSizeF();
	d->doc.setTextWidth(area.width() - 2.0*d->bw - leftMargin() - rightMargin());
	d->doc.setHtml(text());
	return d->doc.size();
	
}

void PainterOsdRenderer::drawText(QPainter *painter, const QSizeF &visual, const QSizeF &device) {
	if (!text().isEmpty()) {
		const QSizeF box = textSize(visual);
		drawText(painter, QRectF(getPos(box, device), box));
	}
}

void PainterOsdRenderer::drawText(QPainter *painter, const QRectF &rect) {
	if (text().isEmpty() || rect.width() < 0.5 || rect.height() < 0.5)
		return;
	painter->save();
	static QRegExp rxColor("\\s+[cC][oO][lL][oO][rR]\\s*=\\s*[^>\\s\\t]+");
	QString bgText = text();
	d->doc.setTextWidth(rect.width() - 2.0*d->bw - leftMargin() - rightMargin());
	d->doc.setHtml(QString("<font color='%1'>").arg(style().bgColor.name())
			+ bgText.remove(rxColor) + "</font>");

	painter->setOpacity(style().bgColor.alphaF());
	QPointF pos = rect.topLeft();
	for (int i=0; i<Data::Count; ++i) {
		painter->translate(d->points[i] + pos);
		d->doc.drawContents(painter);
		painter->resetTransform();
	}

	d->doc.setHtml(QString("<font color='%1'>").arg(style().fgColor.name())+text()+"</font>");
	
	painter->setOpacity(style().fgColor.alphaF());
	painter->translate(d->bw + pos.x(), d->bw + pos.y());
	d->doc.drawContents(painter);
	
	painter->restore();
}

void PainterOsdRenderer::drawTimeLine(QPainter *painter, const QSizeF &visual, const QSizeF &device) {
	if (timeLineRate() >= 0.0) {
		const QSizeF box = timeLineSize(visual);
		drawTimeLine(painter, QRectF(getPos(box, device), box));
	}
}

QSizeF PainterOsdRenderer::timeLineSize(const QSizeF &area) {
	return QSizeF(area.width()*0.8, area.height()*0.05);
}

void PainterOsdRenderer::drawTimeLine(QPainter *painter, const QRectF &rect) {
	const double border = style().borderWidth * rect.height();
	QRectF r = rect;
	painter->save();
	painter->setOpacity(style().bgColor.alphaF());
	painter->fillRect(r, style().bgColor);
	r.translate(border, border);
	r.setWidth((rect.width() - 2.0*border) * timeLineRate());
	r.setHeight(rect.height() - 2.0*border);
	painter->setOpacity(style().fgColor.alphaF());
	painter->fillRect(r, style().fgColor);
	painter->restore();
}

QPointF PainterOsdRenderer::getPos(const QSizeF &size, const QSizeF &widget) {
	double x = 0.0, y = 0.0;
	if (style().alignment & Qt::AlignHCenter)
		x = (widget.width() - size.width())/2.0;
	else if (style().alignment & Qt::AlignRight)
		x = widget.width() - size.width() - rightMargin() * widget.width();
	else
		x = leftMargin() * widget.width();
	if (style().alignment & Qt::AlignBottom)
		y = widget.height() - size.height() - bottomMargin() * widget.height();
	else if (style().alignment & Qt::AlignVCenter)
		y = (widget.height() - size.height())/2.0;
	else
		y = topMargin() * widget.height();
	return QPointF(x, y);
}

void PainterOsdRenderer::updateFontSize(double w, double h) {
	int px = 0;
	if (style().scale == Core::OsdStyle::FitToDiagonal)
		px = qRound(std::sqrt(h*h + w*w) * style().textSize);
	else if (style().scale == Core::OsdStyle::FitToWidth)
		px = qRound(w * style().textSize);
	else
		px = qRound(h * style().textSize);
	if (d->px != px) {
		d->px = px;
		d->bw = style().borderWidth*d->px;
		d->bw = qMax(d->bw, 1.0);
		for (int i=0; i<Data::Count; ++i) {
			d->points[i].setX(d->bw*(1+Data::Sines[i]));
			d->points[i].setY(d->bw*(1+Data::Cosines[i]));
		}
	}
	applyFont();
}

void PainterOsdRenderer::applyAlignment() {
	QTextOption option(style().alignment);
	option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	option.setUseDesignMetrics(true);
	d->doc.setDefaultTextOption(option);
}

void PainterOsdRenderer::applyFont() {
	QFont font = style().font;
	if (d->px > 0)
		font.setPixelSize(d->px);
	d->doc.setDefaultFont(font);
}

}
