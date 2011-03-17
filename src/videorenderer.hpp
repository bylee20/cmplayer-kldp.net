#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>
#include "framebufferobjectoverlay.hpp"
#include "colorproperty.hpp"

class OsdRenderer;

class VideoRenderer : public QGLWidget {
	Q_OBJECT
public:
	VideoRenderer(QWidget *parent = 0);
	~VideoRenderer();
	// takes ownership
	void addOsd(OsdRenderer *osd);
	QSize sizeHint() const;
	double frameRate() const;
	double aspectRatio() const;
	double cropRatio() const;
	void setLogoMode(bool on);
	void setColorProperty(const ColorProperty &prop);
	const ColorProperty &colorProperty() const;
	void setFixedRenderSize(const QSize &size);
public slots:
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
signals:
	void frameRateChanged(double frameRate);
private:
	void *lock(void **plane);
	void unlock(void *id, void *const *plane);
	void display(void *id);
	void prepare(quint32 fourcc, int width, int height, double fps);
	friend class LibVlc;

	void updateSize();
	QSize renderableSize() const;
	static const char *yv12ToRgb;
	static QGLFormat makeFormat();
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void customEvent(QEvent *event);
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

#endif
