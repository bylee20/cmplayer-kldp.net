#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>
#include <QGraphicsView>

class OsdRenderer;	class VideoFormat;
class VideoUtil;	class ColorProperty;

class VideoScene : public QGraphicsScene {
	Q_OBJECT
public:
	enum Effect {
		NoEffect		= 0,
		FlipVertically		= 1 << 0,
		FlipHorizontally	= 1 << 1,
		Grayscale		= 1 << 2,
		InvertColor		= 1 << 3,
		Blur			= 1 << 4,
		Sharpen			= 1 << 5,
		RemapLuma		= 1 << 6,
		AutoContrast		= 1 << 7,
		IgnoreEffect		= 1 << 8
	};
	Q_DECLARE_FLAGS(Effects, Effect)
private:
	static const int FilterEffects = InvertColor | RemapLuma | AutoContrast;
	static const int KernelEffects = Blur | Sharpen;
public:
	QGraphicsView *view();
	QGLWidget *gl();
	~VideoScene();
	// takes ownership
	void addOsd(OsdRenderer *osd);
	QSizeF skinSizeHint() const;
	QSizeF sizeHint(double times) const;
	bool hasFrame() const;
	QImage frameImage() const;
	const VideoFormat &format() const;
	double aspectRatio() const;
	double cropRatio() const;
	double targetAspectRatio() const;
	double targetCropRatio(double fallback) const;
	double targetCropRatio() const {return targetCropRatio(targetAspectRatio());}
	void setLogoMode(bool on);
	void setColorProperty(const ColorProperty &prop);
	const ColorProperty &colorProperty() const;
	void clearEffects();
	void setEffect(Effect effect, bool on);
	void setEffects(Effects effect);
	double outputFrameRate(bool reset = true) const;
	static void init();
	static void fin();
	static VideoScene &get() {Q_ASSERT(obj != 0); return *obj;}
public slots:
	void setSkinVisible(bool visible);
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
	void setOverlayType(int type);
signals:
	void formatChanged(const VideoFormat &format);
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
private slots:
	void updateVertices();
private:
	friend class VideoView;
	bool needToPropagate(const QPointF &mouse);

	static VideoScene *obj;
	VideoScene();
	double widgetRatio() const {return (double)width()/(double)height();}
	static bool isSameRatio(double r1, double r2) {return (r1<0. && r2<0.) || qFuzzyCompare(r1, r2);}
	static int translateButton(Qt::MouseButton qbutton);
	void setUtil(VideoUtil *util);
	void updateSceneRect();
	void *lock(void ***planes);
	void unlock(void *id, void *const *plane);
	void display(void *id);
	void process(void **planes);
	void render(void **planes);
	void prepare(const VideoFormat *format);
	friend class LibVLC;

//	void updateSize();
	QRectF renderableArea() const;
	static QGLFormat makeFormat();
//	void paintEvent(QPaintEvent *event);
	void drawBackground(QPainter *painter, const QRectF &rect);
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

Q_DECLARE_OPERATORS_FOR_FLAGS(VideoScene::Effects)

#endif
