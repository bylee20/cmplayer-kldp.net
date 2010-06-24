#ifndef OSDRENDERER_HPP
#define OSDRENDERER_HPP

#include <QtCore/QObject>

class QSettings;		class RichString;
class ImageOverlayFilter;	class OsdStyle;
class QRect;			class QPoint;
class QPainter;			class QSize;

class OsdRenderer : public QObject {
	Q_OBJECT
public:
	OsdRenderer();
	~OsdRenderer();
	void setArea(const QRect &rect, double dis_x, double dis_y);
	QRect area() const;
	QPoint pos() const;
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const;
public slots:
	void update();
	virtual void clear() = 0;
signals:
	void areaChanged(const QRect &area);
	void styleChanged(const OsdStyle &style);
	void needToRerender();
protected:
	virtual void render(QPainter *painter) = 0;
	virtual QPoint posHint() const = 0;
	virtual QSize sizeHint() const = 0;
private:
	void customEvent(QEvent *event);
	void rerender();
	friend class NativeVideoRenderer;
	void setImageOverlay(ImageOverlayFilter *overlay);
	struct Data;
	Data *d;
};

#endif // OSDRENDERER_HPP
