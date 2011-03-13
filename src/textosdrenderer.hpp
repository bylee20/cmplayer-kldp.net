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
	double scaler() const;
public slots:
	void clear();
private slots:
	void updateStyle(const OsdStyle &style);
private:
	void updateFontSize();
	struct Data;
	Data *d;
};

#endif // TEXTOSDRENDERER_HPP
