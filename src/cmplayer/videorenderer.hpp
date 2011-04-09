#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>

class OsdRenderer;	class VideoFormat;
class VideoUtil;	class ColorProperty;

class VideoRenderer : public QGLWidget {
	Q_OBJECT
public:
	enum Effect {
		NoEffect		= 0,
		FlipVertically		= 1 << 0,
		FlipHorizontally	= 1 << 1,
		Grayscale		= 1 << 2,
		InvertColor		= 1 << 3,
		Blur			= 1 << 4,
		Sharpen			= 1 << 5
	};
	Q_DECLARE_FLAGS(Effects, Effect)
	VideoRenderer(QWidget *parent = 0);
	~VideoRenderer();
	// takes ownership
	void addOsd(OsdRenderer *osd);
	QSize sizeHint() const;
	bool hasFrame() const;
	QImage frameImage() const;
	double frameRate() const;
	double aspectRatio() const;
	double cropRatio() const;
	double targetAspectRatio() const;
	double targetCropRatio(double fallback) const;
	double targetCropRatio() const {return targetCropRatio(targetAspectRatio());}
	void setLogoMode(bool on);
	void setColorProperty(const ColorProperty &prop);
	const ColorProperty &colorProperty() const;
	void setFixedRenderSize(const QSize &size);
	void clearEffects();
	void setEffect(Effect effect, bool on);
	void setEffects(Effects effect);
public slots:
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
signals:
	void frameRateChanged(double frameRate);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	double widgetRatio() const {return (double)width()/(double)height();}
	static int translateButton(Qt::MouseButton qbutton);
	void setUtil(VideoUtil *util);
	void *lock(void ***planes);
	void unlock(void *id, void *const *plane);
	void display(void *id);
	void prepare(const VideoFormat *format);
	friend class LibVLC;

	void updateSize();
	QSize renderableSize() const;
	static QGLFormat makeFormat();
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool event(QEvent *event);
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

Q_DECLARE_OPERATORS_FOR_FLAGS(VideoRenderer::Effects)

#endif
