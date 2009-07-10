#ifndef CORE_PAINTEROSDRENDERER_H
#define CORE_PAINTEROSDRENDERER_H

#include "abstractosdrenderer.h"

namespace Core {

class PainterOsdRenderer : public AbstractOsdRenderer {
public:
	PainterOsdRenderer();
	~PainterOsdRenderer();
protected:
	void drawText(QPainter *painter, const QSizeF &visual, const QSizeF &device);
	void drawTimeLine(QPainter *painter, const QSizeF &visual, const QSizeF &device);
	void drawTimeLine(QPainter *painter, const QRectF &rect);
	void drawText(QPainter *painter, const QRectF &rect);
	void updateFontSize(double w, double h);
	QPointF getPos(const QSizeF &area, const QSizeF &background);
	void applyAlignment();
	void applyFont();
	QSizeF textSize(const QSizeF &area);
	QSizeF timeLineSize(const QSizeF &area);
	void updateStyle(const OsdStyle &/*style*/) {applyAlignment(); applyFont();}
private:
	struct Data;
	Data *d;
};

}

#endif
