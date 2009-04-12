#ifndef CORE_OPENGLVIDEORENDERERIFACE_H
#define CORE_OPENGLVIDEORENDERERIFACE_H

#include "videorendereriface.h"
#include <QtCore/QPointF>
#include <QtCore/QRectF>

class QPainter;

namespace Core {

class ColorProperty;

class OpenGLVideoRendererIface : public VideoRendererIface {
public:
	class Object {
	public:
		virtual ~Object() {}
		virtual void overdraw(QPainter */*painter*/) {}
		virtual void mouseMoveEvent(const QPoint &/*pos*/) {}
		virtual void mousePresseEvent(const QPoint &/*pos*/, Qt::MouseButton /*btn*/) {}
	};
	OpenGLVideoRendererIface(QWidget *widget);
	~OpenGLVideoRendererIface();
	const QRectF &imageRect() const {return m_rect;}
	QPointF mapWidgetToScreen(const QPointF &pos) {
		return QPointF((pos.x()-m_rect.x())/m_hscale, (pos.y()-m_rect.y())/m_vscale);
	}
	QPointF mapScreenToWidget(const QPointF &pos) {
		return QPointF(pos.x()*m_hscale+m_rect.x(), pos.y()*m_vscale+m_rect.y());
	}
	QSizeF scaleWidgetToScreen(const QSizeF &size) {
		return QSizeF(size.width() / m_hscale, size.height() / m_vscale);
	}
	QSizeF scaleScreenToWidget(const QSizeF &size) {
		return QSizeF(size.width() * m_hscale, size.height() * m_vscale);
	}
	void setObject(Object *obj) {m_obj = obj;}
	virtual void setFrame(const VideoFrame &frame) = 0;
	virtual void setColorProperty(const ColorProperty &prop) = 0;
protected:
	void setImageRect(const QRectF &rect) {m_rect = rect;}
	void setImageRect(double x, double y, const QSizeF &size) {
		m_rect.setX(x); m_rect.setY(y); m_rect.setSize(size);
	}
	double &vscale() {return m_vscale;}
	double &hscale() {return m_hscale;}
	const double &vscale() const {return m_vscale;}
	const double &hscale() const {return m_hscale;}
	Object *object() {return m_obj;}
private:
	QRectF m_rect;
	double m_vscale, m_hscale;
	Object *m_obj;
};

}

#endif
