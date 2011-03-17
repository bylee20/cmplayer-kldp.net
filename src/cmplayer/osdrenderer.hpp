#ifndef OSDRENDERER_HPP
#define OSDRENDERER_HPP

#include <QtCore/QObject>
#include <QtCore/QRectF>

class RichString;	class OsdStyle;
class QPainter;

class OsdRenderer : public QObject {
	Q_OBJECT
public:
	static int cachedSize(double size);
	static int cachedSize(int size) {return cachedSize(double(size));}
	static QSize cachedSize(const QSize &size) {
		return QSize(cachedSize(size.width()), cachedSize(size.height()));
	}
	static QSize cachedSize(const QSizeF &size) {
		return QSize(cachedSize(size.width()), cachedSize(size.height()));
	}

	OsdRenderer() {}
	~OsdRenderer() {}
	virtual void render(QPainter *painter, const QPointF &pos) = 0;
	virtual QPointF posHint() const = 0;
	virtual QSizeF size() const = 0;
	virtual void setBackgroundSize(const QSize &size) = 0;
	virtual const OsdStyle &style() const = 0;
	double width() const {return size().width();}
	double height() const {return size().height();}
public slots:
	virtual void setStyle(const OsdStyle &style) = 0;
signals:
	void sizeChanged(const QSizeF &size);
	void styleChanged(const OsdStyle &style);
	void needToRerender();
};

#endif // OSDRENDERER_HPP
