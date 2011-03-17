#ifndef TEXTOSDRENDERER_HPP
#define TEXTOSDRENDERER_HPP

#include "osdrenderer.hpp"

class TextOsdRenderer : public OsdRenderer {
	Q_OBJECT
public:
	TextOsdRenderer(Qt::Alignment align = Qt::AlignTop | Qt::AlignHCenter);
	~TextOsdRenderer();
	RichString text() const;
	void showText(const RichString &text, int last = -1);
	void setMargin(double top, double bottom, double right, double left);
	void render(QPainter *painter, const QPointF &pos);
	QPointF posHint() const;
	QSizeF size() const;
	void setBackgroundSize(const QSize &size);
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const;
	void renderDirectly(QPainter *painter, const QPointF &pos);
public slots:
	void clear();
private:
	QPointF getOrigin() const;
	class SineCosine;
	void updateSize();
	QString bgHtml() const;
	QString fgHtml() const;
	void cache();
	void updateFont();
	struct Data;
	Data *d;
};

#endif // TEXTOSDRENDERER_HPP
