#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>
#include <QGraphicsView>

class OsdRenderer;	class VideoFormat;
class VideoUtil;	class ColorProperty;

namespace Skin {
class Helper;
}

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
	enum SkinMode {AlwaysSkin, /*AutoSkin, */NeverSkin};
	QGraphicsView *view();
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
	QRectF renderableArea() const;
	void setSkinMode(SkinMode mode);
	void setSkin(Skin::Helper *skin);
	SkinMode skinMode() const;
	bool inScreen(const QPointF &pos) const;
public slots:
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
	void drawBackground(QPainter *painter, const QRectF &rect);
	bool event(QEvent *event);
private slots:
	void updateVertices();
private:
	void updateSkinVisible(const QPointF &pos = QCursor::pos());
	VideoScene(VideoUtil *util);
	bool needToPropagate(const QPointF &mouse);
	void updateSceneRect();
	void *lock(void ***planes);
	void unlock(void *id, void *const *plane);
	void display(void *id);
	void process(void **planes);
	void render(void **planes);
	void prepare(const VideoFormat *format);

	class VideoView;
	class FrameRateMeasure;
	friend class VideoView;
	friend class LibVLC;

	typedef void (*_glActiveTexture) (GLenum);
	_glActiveTexture glActiveTexture;
	struct Data;	Data *d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VideoScene::Effects)

#endif
