#ifndef NATIVEVIDEORENDERER_HPP
#define NATIVEVIDEORENDERER_HPP

#include <QtGui/QWidget>
#include <gst/gst.h>

class PlayEngine;	class OsdRenderer;

class NativeVideoRenderer : public QWidget {
	Q_OBJECT
public:
	NativeVideoRenderer(PlayEngine *engine, QWidget *parent = 0);
	~NativeVideoRenderer();
	GstElement *bin() const;
	QSize sizeHint() const;
	void addOsdRenderer(OsdRenderer *osd);
	double frameRate() const;
signals:
	void frameRateChanged(double frameRate);
private slots:
	void mrlChanged();
	void setXOverlay();
protected:
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
private:
	friend class GstVideoInfo;
	void setFrameSize(const QSize &size);
	void setFrameRate(double frameRate);
	QRect xoverlayGeometry() const;
	void updateXOverlayGeometry();
	void windowExposed();
	class XOverlay;
	struct Data;
	Data *d;
};

#endif // NATIVEVIDEORENDERER_HPP
