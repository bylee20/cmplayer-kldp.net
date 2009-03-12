#ifndef OPENGL_VIDEORENDERER_H
#define OPENGL_VIDEORENDERER_H

#include <QtOpenGL/QGLWidget>
#include <core/openglvideorendereriface.h>

namespace OpenGL {

class VideoRenderer : public QGLWidget, public Core::OpenGLVideoRendererIface {
	Q_OBJECT
public:
	VideoRenderer(QWidget *parent = 0);
	~VideoRenderer();
	void setVideoProperties(double b, double c, double s, double h);
	virtual Core::VideoType type() const {return Core::OpenGL;}
	virtual QSize sizeHint() const {return widgetSizeHint();}
	virtual void rerender();
	virtual Core::AbstractOsdRenderer *createOsd();
	virtual void setFrame(const Core::VideoFrame &frame);
protected:
	void customEvent(QEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
private:
	static const char *yuy2ToRgb, *yv12ToRgb, *rgbToRgb;
	static QGLFormat makeFormat();
	void calculate();
	typedef void (*_glProgramStringARB) (GLenum, GLenum, GLsizei, const GLvoid *);
	typedef void (*_glBindProgramARB) (GLenum, GLuint);
	typedef void (*_glDeleteProgramsARB) (GLsizei, const GLuint *);
	typedef void (*_glGenProgramsARB) (GLsizei, GLuint *);
	typedef void (*_glProgramLocalParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
	typedef void (*_glActiveTexture) (GLenum);
	_glProgramStringARB glProgramStringARB;
	_glBindProgramARB glBindProgramARB;
	_glDeleteProgramsARB glDeleteProgramsARB;
	_glGenProgramsARB glGenProgramsARB;
	_glActiveTexture glActiveTexture;
	_glProgramLocalParameter4fARB glProgramLocalParameter4fARB;
	struct Data;
	Data *d;
};

}

#endif
