// from avKode

#include <core/newframeevent.h>
#include "videorenderer.h"
#include "canvas.h"
#include <core/painterosdrenderer.h>
#include <QDebug>
#include <QPainter>
#include <QX11Info>
#include <QSize>
#include <QResizeEvent>
#include <cmath>
#include <QApplication>
#include <QMutex>
#include "image.h"

extern "C" {
#include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
}

#include <iostream>

namespace XVideo {



/*static int toYuv(int r, int g, int b) {
	#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	#define Yuv(R,G,B) ((((((66*R+129*G+25*B+128)>>8)+16)<<8)\
		|(((112*R-94*G-18*B+128)>>8)+128))<<8|(((-38*R-74*G+112*B+128)>>8)+128))
	#else
	#define Yuv(R,G,B) (((((((-38*R-74*G+112*B+128)>>8)+128)<<8)\
		|(((112*R-94*G-18*B+128)>>8)+128))<<8|(((66*R+129*G+25*B+128)>>8)+16))<<8)
	#endif
		return Yuv(r, g, b);
	#undef Yuv
}*/



// class VideoRenderer::Osd : public PainterOsdRenderer {
// public:
// 	Osd(VideoRenderer *video) {this->video = video; cleared = false;}
// 	void render() {cleared = false; video->update();}
// 	void clear() {if (!cleared) {cleared = true; video->update();}}
// 	void renderContents(QPainter *painter, const QSizeF &visual, const QSizeF &widget) {
// 		if (cleared)
// 			return;
// 		if (!text().isEmpty()) {
// 			if (prevVisual != visual.toSize()) {
// 				prevVisual = visual.toSize();
// 				updateFontSize(visual.width(), visual.height());
// 			}
// 			const QSizeF size = textSize(widget);
// 			if (size.height() < 1.0 || size.width() < 1.0)
// 				return;
// 			drawText(painter, QRectF(getPos(size, widget), size));
// 		}
// 		if (timeLineRate() >= 0.0) {
// 			const QSizeF size = timeLineSize(visual);
// 			drawTimeLine(painter, QRectF(getPos(size, widget), size));
// 		}
// 	}
// private:
// 	VideoRenderer *video;
// 	QSize prevVisual;
// 	bool cleared;
// };

struct VideoRenderer::Data {
	Canvas *canvas;
 // 	QList<Osd*> osds;
};

VideoRenderer::VideoRenderer(QWidget *parent)
: QWidget(parent), SoftwareRendererIface(this), d(new Data) {
	setMouseTracking(true);
	setWidgetSize(size());
	d->canvas = new Canvas(this);
	d->canvas->move(0, 0);
	d->canvas->open();
}

VideoRenderer::~VideoRenderer() {
	delete d;
}

// AbstractOsdRenderer *VideoRenderer::createOsd() {
// 	Osd *osd = new Osd(this);
// 	d->osds.push_back(osd);
// 	return osd;
// }

// void VideoRenderer::renderOsds(QPainter *painter, const QSizeF &visual, const QSizeF &widget) {
// 	for (int i=0; i<d->osds.size(); ++i)
// 		d->osds[i]->renderContents(painter, visual, widget);
// }

void VideoRenderer::rerender() {
	calculate();
	update();
}

void VideoRenderer::resizeEvent(QResizeEvent */*event*/) {
	d->canvas->resize(size());
	setWidgetSize(size());
	calculate();
}

void VideoRenderer::setFrame(const Core::VideoFrame &frame) {
	QApplication::postEvent(d->canvas, new Core::NewFrameEvent(frame));
}

void VideoRenderer::setColorProperty(const Core::ColorProperty &/*prop*/) {
}

}

