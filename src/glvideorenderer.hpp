//#ifndef GLVIDEORENDERER_HPP
//#define GLVIDEORENDERER_HPP
//
//#include <QtOpenGL/QGLWidget>
//#include "videoframe.hpp"
//#include <gst/gst.h>
//
//class VideoFrame;		class ColorProperty;
//
//class GLVideoRenderer : public QGLWidget {
//	Q_OBJECT
//public:
////	class Object {
////	public:
////		virtual ~Object() {}
////		virtual SoftwareRendererIface *renderer() = 0;
////		virtual void overdraw(QPainter */*painter*/) {}
////		virtual void mouseMoveEvent(const QPoint &/*pos*/) {}
////		virtual void mousePresseEvent(const QPoint &/*pos*/, Qt::MouseButton /*btn*/) {}
////	};
//	GLVideoRenderer(QWidget *parent = 0);
//	~GLVideoRenderer();
//	GstElement *sink() const;
////	virtual QSize sizeHint() const {return widgetSizeHint();}
//	virtual void rerender();
////	virtual Core::AbstractOsdRenderer *createOsd();
//	void setFrame(const VideoFrame &frame);
//	void setColorProperty(const ColorProperty &prop);
////	const QRectF &imageRect() const {return m_rect;}
////	QPointF mapWidgetToScreen(const QPointF &pos) {
////		return QPointF((pos.x()-m_rect.x())/m_hscale, (pos.y()-m_rect.y())/m_vscale);
////	}
////	QPointF mapScreenToWidget(const QPointF &pos) {
////		return QPointF(pos.x()*m_hscale+m_rect.x(), pos.y()*m_vscale+m_rect.y());
////	}
////	QSizeF scaleWidgetToScreen(const QSizeF &size) {
////		return QSizeF(size.width() / m_hscale, size.height() / m_vscale);
////	}
////	QSizeF scaleScreenToWidget(const QSizeF &size) {
////		return QSizeF(size.width() * m_hscale, size.height() * m_vscale);
////	}
//protected:
//	void customEvent(QEvent *event);
////	void mouseMoveEvent(QMouseEvent *event);
////	void mousePressEvent(QMouseEvent *event);
//	void paintEvent(QPaintEvent *event);
////	void resizeEvent(QResizeEvent *event);
//private:
//	static const char *yuy2ToRgb, *yv12ToRgb;
//	static QGLFormat makeFormat();
////	void calculate();
//	typedef void (*_glProgramStringARB) (GLenum, GLenum, GLsizei, const GLvoid *);
//	typedef void (*_glBindProgramARB) (GLenum, GLuint);
//	typedef void (*_glDeleteProgramsARB) (GLsizei, const GLuint *);
//	typedef void (*_glGenProgramsARB) (GLsizei, GLuint *);
//	typedef void (*_glProgramLocalParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
//	typedef void (*_glActiveTexture) (GLenum);
//	_glProgramStringARB glProgramStringARB;
//	_glBindProgramARB glBindProgramARB;
//	_glDeleteProgramsARB glDeleteProgramsARB;
//	_glGenProgramsARB glGenProgramsARB;
//	_glActiveTexture glActiveTexture;
//	_glProgramLocalParameter4fARB glProgramLocalParameter4fARB;
////	void setObject(Object *obj) {m_obj = obj;}
////	virtual void setFrame(const VideoFrame &frame) = 0;
////	virtual void setColorProperty(const ColorProperty &prop) = 0;
//protected:
////	void setImageRect(const QRectF &rect) {m_rect = rect;}
////	void setImageRect(double x, double y, const QSizeF &size) {
////		m_rect.setX(x); m_rect.setY(y); m_rect.setSize(size);
////	}
////	double &vscale() {return m_vscale;}
////	double &hscale() {return m_hscale;}
////	const double &vscale() const {return m_vscale;}
////	const double &hscale() const {return m_hscale;}
////	double vmargin() const {return m_vmargin;}
////	double hmargin() const {return m_hmargin;}
////	void setWidgetSize(const QSize &size) {m_widget = size;}
////	const QSizeF &visualSize() const {return m_visual;}
////	const QSizeF &widgetSize() const {return m_widget;}
////	Object *object() {return m_obj;}
//private:
//	void calculate();
//	void updateFrameInfo(const VideoFrame::Info &info);
//	struct Data;
//	Data *d;
////	class Loader;
////	QRectF m_rect;
////	double m_vscale, m_hscale, m_vmargin, m_hmargin;
////	QSizeF m_visual, m_widget;
////	Object *m_obj;
//};
//
//#endif // GLVIDEORENDERER_HPP
