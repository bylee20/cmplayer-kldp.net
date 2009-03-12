#ifndef BACKEND_ABSTRACTOSDRENDERER_H
#define BACKEND_ABSTRACTOSDRENDERER_H

#include <QtCore/QString>
#include "osdstyle.h"

class QSizeF;				class QPainter;
class QPointF;				class QRectF;

namespace Core {

class OsdStyle;

class AbstractOsdRenderer : public QObject {
	Q_OBJECT
public:
	AbstractOsdRenderer();
	virtual ~AbstractOsdRenderer();
	void renderText(const QString &text);
	void renderTimeLine(double percent);
	void renderText(const QString &text, int duration);
	void renderTimeLine(double percent, int duration);
	void setText(const QString &text) {d->text = text;}
	const QString &text() const {return d->text;}
	double timeLineRate() const {return d->timeRate;}
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const {return d->style;}
	void setBottomMargin(double margin) {setMargin(d->bottom, margin);}
	void setTopMargin(double margin) {setMargin(d->top, margin);}
	void setLeftMargin(double margin) {setMargin(d->left, margin);}
	void setRightMargin(double margin) {setMargin(d->right, margin);}
	double bottomMargin() const {return d->bottom;}
	double topMargin() const {return d->top;}
	double leftMargin() const {return d->left;}
	double rightMargin() const {return d->right;}
	virtual bool grab(QImage &/*image*/) const {return false;}
	virtual void setRect(const QRect &/*rect*/) {}
public slots:
	virtual void render(int duration);
	virtual void render() = 0;
	virtual void clear() = 0;
protected:
	virtual void updateStyle(const OsdStyle &/*style*/) {}
private:
	void setMargin(double &margin, double value);
	struct Data {
		QString text;
		double timeRate, top, bottom, left, right;
		OsdStyle style;
		QTimer *hider;
	};
	Data *d;
};

}

#endif
