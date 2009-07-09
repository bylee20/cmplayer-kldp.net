#include <QtGui/QX11Info>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <core/newframeevent.h>
#include "canvas.h"
#include "image.h"
#include "videorenderer.h"



extern "C" {
#include <X11/Xlib.h>
}
#include <iostream>

namespace XVideo {

struct Canvas::Data {
	Display *display;
	Window window;
	GC gc;
	int port;
	bool frameSet;
	Image *scene;
	VideoRenderer *renderer;
};

Canvas::Canvas(VideoRenderer *parent)
: QWidget(parent), m_d(new Data) {
	d().display = 0;
	d().window = 0;
	d().port = -1;
	d().frameSet = false;
	d().renderer = parent;
	d().scene = 0;
	setMouseTracking(true);
//	setAutoFillBackground(true);
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
//	setAttribute(Qt::WA_PaintUnclipped, true);
//	setAttribute(Qt::WA_StaticContents, true);
//	setAttribute(Qt::WA_NoSystemBackground, true);
}

Canvas::~Canvas() {
	if (d().port != -1)
		close();
	delete m_d;
}

bool Canvas::open() {
	d().display = x11Info().display();
	d().window = winId();
	if (checkXv()) {
		d().gc = XCreateGC(d().display,d().window, 0, 0);
		if (d().scene)
			delete d().scene;
		d().scene = new Image(d().display, d().port);
//		if (d().top)
//			delete d().top;
//		d().top = new Image(d().display, d().port);
//		Core::VideoFrame frame;
//		Core::VideoFrame::Info info;
//		info.type = Core::VideoFrame::RGB;
//		info.size = QSize(10, 10);
//		info.pixelAspectRatio = 1.0;
//		const QByteArray black(10*10*4, 0x00);
//		frame.setInfo(info);
//		frame.setData(black.data(), black.size());
//		d().top->setVideoFrame(frame);
		return true;
	}
	return false;
}

void Canvas::close() {
	delete d().scene;
//	delete d().top;
	d().scene = 0;
	XFreeGC(d().display, d().gc);
	d().window  = 0;
	d().port = -1;
}

bool Canvas::checkXv() {
	std::cout << "VideoRenderer::checkXv()" << std::endl;
	unsigned int p_version = 0, p_release = 0, p_request_base = 0;
	unsigned int p_event_base = 0, p_error_base = 0;
	if (XvQueryExtension(d().display
			, &p_version, &p_release, &p_request_base
			, &p_event_base, &p_error_base) != Success) {
		std::cerr << "VideoRenderer: QueryExtension failed" << std::endl;
		return false;
	}
	unsigned int p_num_adaptors = 0;
	XvAdaptorInfo *ai;
	if (XvQueryAdaptors(d().display, d().window, &p_num_adaptors, &ai) != Success) {
		std::cerr << "VideoRenderer: QueryAdaptors failed" << std::endl;
		return false;
	}
	if (p_num_adaptors == 0)
		return false;
	int overlay = 0, blitter = 0, other = 0;
	// Search for a suitable adaptor
	for (unsigned int i = 0; i < p_num_adaptors && d().port == -1; i++) {
		if ((ai[i].type & XvInputMask) && (ai[i].type & XvImageMask)) {
			std::cout << "VideoRenderer: Checking base-port " << ai[i].base_id << std::endl;
			const QString name = QString::fromAscii(ai[i].name);
			if (name.endsWith("Overlay"))
				overlay = i;
			else if (name.endsWith("Blitter"))
				blitter = i;
			else
				other = i;
		}
	}
	int adaptor = 0;
	if (blitter != 0) {
		std::cout << "VideoRenderer: Found blitter adaptor" << std::endl;
		adaptor = blitter;
	} else if (overlay != 0) {
		std::cout << "VideoRenderer: Only found overlay adaptor" << std::endl;
		adaptor = overlay;
	} else if (other != 0) {
		std::cout << "VideoRenderer: Found unrecognized adaptor" << std::endl;
		adaptor = other;
	}
	if (adaptor > 0) {
		Image::Formats formats;
		int nr_formats;
		XvImageFormatValues *ifvs = XvListImageFormats(d().display
				, ai[adaptor].base_id, &nr_formats);
		for(int j=0; j<nr_formats; j++) {
			switch(ifvs[j].id) {
#define PUSH_BACK_FORMAT(f) case f: {formats.push_back(f); break;}
				PUSH_BACK_FORMAT(Image::XvRGB32);
				PUSH_BACK_FORMAT(Image::XvI420);
				PUSH_BACK_FORMAT(Image::XvYUY2);
				PUSH_BACK_FORMAT(Image::XvYV12);
#undef PUSH_BACK_FORMAT
			default:
				break;
			}
		}
		Image::setAvailableFormats(formats);
		XFree(ifvs);
		XvPortID xv_p = ai[adaptor].base_id;
		qDebug() << xv_p << ( ai[adaptor].base_id + ai[adaptor].num_ports );
		for (; xv_p < ai[adaptor].base_id + ai[adaptor].num_ports; ++xv_p) {
			d().port = xv_p;
			break;
		}
	}
	if (p_num_adaptors > 0)
		XvFreeAdaptorInfo(ai);
	if (d().port == -1) {
		std::cerr << "VideoRenderer: No Xv port found" << std::endl;
		return false;
	} else
		std::cout << "VideoRenderer: Using Xv port: " << d().port << std::endl;
	return true;
}

void Canvas::customEvent(QEvent *event) {
	if (static_cast<Core::BaseEvent*>(event)->type() != Core::BaseEvent::NewFrame)
		return;
	const Core::VideoFrame &frame = ((const Core::NewFrameEvent*)event)->frame();
	d().frameSet = d().scene && !frame.isNull() && frame.info().type != Core::VideoFrame::NoFrame;
	d().renderer->updateFrameInfo(frame.info(), true);
	if (d().frameSet && (d().frameSet = d().scene->setVideoFrame(frame))) {
//		if (d().pixmap.size() != frame.info().size) {
//			d().pixmap = QPixmap(frame.info().size);
//			d().pixmap.fill(Qt::blue);
//			qDebug() << d().pixmap.depth();
//			d().pixmap.detach();
//			const Qt::HANDLE hd = d().pixmap.handle();
//			d().pixgc = XCreateGC(d().pixmap.x11Info().display(), hd, 0, 0);
//			d().scene->put(hd, d().gc, d().pixmap.rect());
//			XFreeGC(d().pixmap.x11Info().display(), d().pixgc);
//			d().pixmap.save("test.png");
//		}
	}
	update();
}

void Canvas::paintEvent(QPaintEvent */*event*/) {
	if (!d().scene || d().scene->isNull() || !d().frameSet) {
		QPainter painter(this);
		painter.fillRect(rect(), Qt::black);
		return;
	}
//		d().scene->put(d().window, d().gc, rect());
//		QPainter painter(this);
//		painter.setPen(Qt::white);
//		painter.drawText(QPoint(100, 100), "Video Title");
//		QApplication::syncX();
	d().scene->put(d().window, d().gc, d().renderer->imageRect().toRect());
	QPainter painter;
	painter.begin(this);
	const double vm = d().renderer->vmargin();
	if (vm > 0.5) {
		QRectF rect(0.0, 0.0, width(), vm);
		painter.fillRect(rect, Qt::black);
		rect.moveTop(height() - vm);
		painter.fillRect(rect, Qt::black);
	}
	const double hm = d().renderer->hmargin();
	if (hm > 0.5) {
		QRectF rect(0.0, 0.0, hm, height());
		painter.fillRect(rect, Qt::black);
		rect.moveLeft(width() - hm);
		painter.fillRect(rect, Qt::black);
	}
	QApplication::syncX();
	//	painter.fillRect(QRect(0, 0, 0, 0), Qt::white);
	//	for (int i=0; i<d().osds.size(); ++i)
	//		d().osds[i]->renderContents(&painter);
	//	if (object())
	//		object()->overdraw(&painter);
	//	painter.end();
}

}
