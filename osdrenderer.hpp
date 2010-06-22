#ifndef OSDRENDERER_HPP
#define OSDRENDERER_HPP

#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QColor>

class QSettings;		class RichString;
class ImageOverlayFilter;

class OsdStyle {
public:
	enum Scale {FitToWidth, FitToHeight, FitToDiagonal};
	OsdStyle(): bgColor(Qt::black), fgColor(Qt::white) {
		alignment = Qt::AlignCenter;
		borderWidth = 0.05;
		textSize = 0.03;
		scale = FitToDiagonal;
	}
	void save(QSettings *set, const QString &group) const;
	void load(QSettings *set, const QString &group);
	QFont font;
	QColor bgColor, fgColor;
	double borderWidth, textSize;
	Qt::Alignment alignment;
	Scale scale;
};


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
//	void needToRerender();
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
