#ifndef BACKEND_XVIDEORENDERER_H
#define BACKEND_XVIDEORENDERER_H

#if 0
//from avKode

#include "directrendereriface.h"
#include <QtGui/QWidget>

namespace Core {

class XVideoRenderer : public QWidget, public DirectRendererIface {
public:
	enum XvId {XvRGB32 = 0x3, XvYUY2 = 0x32595559, XvI420 = 0x30323449, XvYV12 = 0x32315659};
	XVideoRenderer(QWidget *parent = 0);
	~XVideoRenderer();
	bool open();
	void close();
	bool checkXv();
	void rerender();
private:
	class Image;
	void recalculate();
	void changeEvent(QEvent *event);
	void customEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	struct Data;
	Data *d;
};

}

#endif

#endif
