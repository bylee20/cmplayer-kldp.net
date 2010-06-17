#ifndef OSDRENDERER_HPP
#define OSDRENDERER_HPP

#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QColor>

class QSettings;		class RichString;
class GstImageOverlay;

class OsdStyle {
public:
	enum Scale {FitToWidth, FitToHeight, FitToDiagonal};
	OsdStyle(): bgColor(Qt::black), fgColor(Qt::white) {
		alignment = Qt::AlignCenter;
		borderWidth = 0.05;
		textSize = 0.05;
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
	void setArea(const QRect &rect);
	QRect area() const;
	QPoint pos() const;
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const;
public slots:
	void rerender();
	virtual void clear() = 0;
protected:
	virtual void render(QPainter *painter) = 0;
	virtual QPoint posHint() const = 0;
	virtual QSize sizeHint() const = 0;
	virtual void areaChanged(const QRect &area) = 0;
	virtual void styleChanged(const OsdStyle &style) = 0;
	void invokeRerender() {
		QMetaObject::invokeMethod(this, "rerender", Qt::QueuedConnection);
	}
private:
	friend class NativeVideoRenderer;
	void setImageOverlay(GstImageOverlay *overlay);
	struct Data;
	Data *d;
};

#endif // OSDRENDERER_HPP
