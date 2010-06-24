#ifndef NATIVEVIDEORENDERER_HPP
#define NATIVEVIDEORENDERER_HPP

#include <QtGui/QWidget>
#include <gst/gst.h>
#include <gst/interfaces/navigation.h>

class PlayEngine;	class OsdRenderer;
class VideoInfo;	class ColorProperty;

class NativeVideoRenderer : public QWidget {
	Q_OBJECT
public:
	NativeVideoRenderer(PlayEngine *engine, QWidget *parent = 0);
	~NativeVideoRenderer();
	GstElement *bin() const;
	QSize sizeHint() const;
	void addOsdRenderer(OsdRenderer *osd);
	double frameRate() const;
	double aspectRatio() const;
	double cropRatio() const;
	GstNavigation *nav() const;
	void setColorProperty(const ColorProperty &prop);
	void setSoftwareEqualizerEnabled(bool enabeld);
	const ColorProperty &colorProperty() const;
	bool isSoftwareEqualizerEnabled() const;
public slots:
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
signals:
	void frameRateChanged(double frameRate);
private slots:
	void mrlChanged();
	void setXOverlay();
	void setInfo(const VideoInfo &info);
protected:
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	QPoint mapToNav(const QPoint &from) const;
	void sendNavEvent(QMouseEvent *event, const char *name);
	friend class VideoManipulator;
	void showFrame(GstBuffer *buffer);
	GstBuffer *allocBuffer(int size, GstCaps *caps);
	static bool isSameRatio(double r1, double r2) {
		return (r1 < 0) ? r2 < 0 : qFuzzyCompare(r1, r2);
	}
	void updateBoxSize();
	void updateXOverlayGeometry();
	void windowExposed();
	class XOverlay;
	struct Data;
	Data *d;
};

#endif // NATIVEVIDEORENDERER_HPP
