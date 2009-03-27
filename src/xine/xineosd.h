#ifndef XINE_XINEOSD_H
#define XINE_XINEOSD_H

#include <core/painterosdrenderer.h>
#include <xine.h>

namespace Xine {

class XineStream;

class XineOsd : public Core::PainterOsdRenderer {
	Q_OBJECT
public:
	XineOsd(XineStream *stream);
	~XineOsd();
	virtual void clear();
	virtual void drawTimeLine(QPainter *painter, const QRectF &rect);
	void setRect(const QRect &rect);
private slots:
	void alloc();
	void free();
private:
	static const int TimeLineBoxCount = 35;
	void updateStyle(const Core::OsdStyle &style);
	int border(double len) const {return qMax(qRound(style().borderWidth * len), 1);}
	virtual void render();
	class Clut;
	void drawPixmap(const QPixmap &pixmap);
	void drawImage(const QImage &image);
	void drawImage(const QImage &image, const QRect &rect);
	struct Data;
	Data *d;
};

}

#endif
