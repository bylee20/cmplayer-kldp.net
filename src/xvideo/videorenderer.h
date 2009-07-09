#ifndef XVIDEO_XVIDEORENDERER_H
#define XVIDEO_XVIDEORENDERER_H

//from avKode

#include <core/softwarerendereriface.h>
#include <QtGui/QWidget>

namespace XVideo {

class VideoRenderer : public QWidget, public Core::SoftwareRendererIface {
public:
	VideoRenderer(QWidget *parent = 0);
	~VideoRenderer();
	void rerender();
	Core::VideoType type() const {return Core::SoftwareVideo;}
	QSize sizeHint() const {return widgetSizeHint();}
private:
//	bool open();
//	bool checkXv();
//	void close();
	friend class Canvas;
	virtual void setFrame(const Core::VideoFrame &frame);
	virtual void setColorProperty(const Core::ColorProperty &prop);
//	void calculate();
//	void changeEvent(QEvent *event);
//	void customEvent(QEvent *event);
//	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	struct Data;
	Data *d;
};

}

#endif
