#ifndef OSDRENDERER_HPP
#define OSDRENDERER_HPP

#include <QtCore/QObject>
#include <QtCore/QRectF>

class RichString;	class OsdStyle;
class QPainter;

class OsdRenderer : public QObject {
	Q_OBJECT
public:
	OsdRenderer();
	~OsdRenderer();
	virtual void render(QPainter *painter, const QPointF &pos) = 0;
	virtual QPointF posHint() const = 0;
	virtual QSizeF size() const = 0;
	virtual void setBackgroundSize(const QSize &size) = 0;
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const;
signals:
	void sizeChanged(const QSizeF &size);
	void styleChanged(const OsdStyle &style);
	void needToRerender();
private:
	struct Data;
	Data *d;
};

#endif // OSDRENDERER_HPP
