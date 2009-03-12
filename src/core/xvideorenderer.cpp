#if 0

// from avKode

#include "newframeevent.h"
#include "xvideorenderer.h"
#include "painterosdrenderer.h"
#include <QDebug>
#include <QPainter>
#include <QX11Info>
#include <QSize>
#include <QResizeEvent>
#include <cmath>
#include <QApplication>

#include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xatom.h>

#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>

#include <assert.h>
#include <iostream>

namespace Core {

class XVideoRenderer::Image {
public:
	Image(): m_dis(0), m_port(-1), m_img(0) {}
	~Image() {free();}
	void setPort(int port) {m_port = port;}
	void setDisplay(Display *display) {m_dis = display;}
// 	void setImage(const QImage &image) {
// 		if (m_port < 0 || !m_dis)
// 			return;
// 		if (!m_img || image.size() != m_size) {
// 			if (m_img)
// 				free();
// 			m_size = image.size();
// 			m_img = XvCreateImage(m_dis, m_port, XvRGB32, m_data.data(), m_size.width(), m_size.height());
// 		}
// 		m_data = QByteArray(image.
// 		m_image = image;
// // 		m_data = data;
// 	}
	void setData(const QByteArray &data, const QSize &size) {
		if (m_port < 0 || !m_dis)
			return;
// 		if (!m_img || size != m_size) {
			if (m_img)
				free();
		m_size = size;
		m_data = data;
		m_img = XvCreateImage(m_dis, m_port, XvRGB32, m_data.data(), m_size.width(), m_size.height());
// 		}

	}
	void setVideoFrame(const VideoFrame &frame) {
// 		free();
		if (m_port < 0 || !m_dis)
			return;
		if (!m_img || m_size != frame.size()) {
			m_size = frame.size();
			create();
		}
		if (!m_img) {
			std::cout << "XVideoRenderer: CreateImage failed" << std::endl;
			return;
		}
		m_frame = frame;
	}
	int width() const {return m_size.width();}
	int height() const {return m_size.height();}
	const QSize &size() const {return m_size;}
	XvImage *xv() {
// 		if (m_img)
// 			m_img->data = m_data.data();
// 			m_img->data = m_frame.data();
// 			m_img->data = (char*)m_image.bits();
		return m_img;
	}
	void free() {if (m_img) {XFree(m_img); m_img = 0;}}
	bool isNull() const {return !m_img || m_size.isEmpty();}
private:
	void create() {
		if (m_img) free();
		m_img = XvCreateImage(m_dis, m_port, XvRGB32, m_frame.data(), m_size.width(), m_size.height());
		qDebug() << "data_size" << m_img->data_size;
	}
	Display *m_dis;
	int m_port;
	XvImage *m_img;
	QSize m_size;
// 	char buffer[1000*1000];
	VideoFrame m_frame;
	QByteArray m_data;
	QImage m_image;
};

struct XVideoRenderer::Data {
	Data(): display(0), window(0), port(-1) {}
	Display *display;
	Window window;
	GC gc;
	int port;
	Image image;
// 	QList<Osd*> osds;
	QImage img;
// 	QPixmap pix;
	QByteArray data;
// 	QSize pixmapSize;
// 	QSize visualSize;
// 	QRect imageRect;
// 	int vMargin, hMargin;
	XvImage *xvImage;
	QByteArray xvData;
// 	XvImage *xvImage;
// 	QByteArray
};

// class XVideoRenderer::Osd : public PainterOsdRenderer {
// public:
// 	Osd(XVideoRenderer *video) {this->video = video; cleared = false;}
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
// 	XVideoRenderer *video;
// 	QSize prevVisual;
// 	bool cleared;
// };

XVideoRenderer::XVideoRenderer(QWidget *parent)
: QWidget(parent), DirectRendererIface(this) {
	d = new Data;
	d->xvImage = 0;
	setAutoFillBackground(true);
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	open();

}

XVideoRenderer::~XVideoRenderer() {
	if (d->xvImage) {
		XFree(d->xvImage);
		d->xvImage = 0;
	}
	if (d->port != -1) {
		close();
	}
	delete d;
}

bool XVideoRenderer::open() {
	d->display = x11Info().display();
	d->window = winId();
	if (checkXv()) {
		d->gc = XCreateGC(d->display,d->window, 0, NULL);
		d->image.setDisplay(d->display);
		d->image.setPort(d->port);
		return true;
	}
	std::cout << "Xv failed\n";
	return false;
}

bool XVideoRenderer::checkXv() {
	std::cout << "XVideoRenderer::checkXv()" << std::endl;
	unsigned int p_version = 0, p_release = 0, p_request_base = 0;
	unsigned int p_event_base = 0, p_error_base = 0;
	if (XvQueryExtension(d->display
			, &p_version, &p_release, &p_request_base
			, &p_event_base, &p_error_base) != Success) {
		std::cerr << "XVideoRenderer: QueryExtension failed" << std::endl;
		return false;
	}
	unsigned int p_num_adaptors = 0;
	XvAdaptorInfo *ai;
	if (XvQueryAdaptors(d->display, d->window, &p_num_adaptors, &ai) != Success) {
		std::cerr << "XVideoRenderer: QueryAdaptors failed" << std::endl;
		return false;
	}
	if (p_num_adaptors == 0)
		return false;
	int overlay = 0;
	int blitter = 0;
	int other = 0;
	// Search for a suitable adaptor
	for (unsigned int i = 0; i < p_num_adaptors && d->port == -1; i++) {
		if ((ai[i].type & XvInputMask) && (ai[i].type & XvImageMask)) {
			std::cout << "XVideoRenderer: Checking base-port " << ai[i].base_id << std::endl;
			QString name = QString::fromAscii(ai[i].name);
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
		std::cout << "XVideoRenderer: Found blitter adaptor" << std::endl;
		adaptor = blitter;
	} else if (overlay != 0) {
		std::cout << "XVideoRenderer: Only found overlay adaptor" << std::endl;
		adaptor = overlay;
	} else if (other != 0) {
		std::cout << "XVideoRenderer: Found unrecognized adaptor" << std::endl;
		adaptor = other;
	}
	if (adaptor > 0) {
// 		d->color_formats = 0;
		int nr_formats;
		XvImageFormatValues *ifvs = XvListImageFormats(d->display
				, ai[adaptor].base_id, &nr_formats);
		for(int j=0; j<nr_formats; j++) {
			switch(ifvs[j].id) {
			case XvRGB32:
				qDebug() << "XV_RGB32";
		//                 d->color_formats |= RGB32;
				break;
			case XvI420:
				qDebug() << "XV_I420";
		//                 d->color_formats |= YV12;
				break;
			case XvYUY2:
				qDebug() << "XV_YUY2";
		//                 d->color_formats |= YUY2;
				break;
			case XvYV12:
				qDebug() << "XV_YV12";
	//                 d->color_formats |= YUY2;
				break;
			default:
				break;
			}
		}
		XFree(ifvs);
		XvPortID xv_p = ai[adaptor].base_id;
		for (; xv_p < ai[adaptor].base_id + ai[adaptor].num_ports; ++xv_p) {
			d->port = xv_p;
			break;
		}
	}
	if (p_num_adaptors > 0)
		XvFreeAdaptorInfo(ai);
	if (d->port == -1) {
		std::cerr << "XVideoRenderer: No Xv port found" << std::endl;
		return false;
	} else
		std::cout << "XVideoRenderer: Using Xv port: " << d->port << std::endl;
	return true;
}

void XVideoRenderer::close() {
	d->image.free();
	XFreeGC(d->display, d->gc);
	d->window  = 0;
	d->port = -1;
}

void XVideoRenderer::changeEvent(QEvent *event) {
	QWidget::changeEvent(event);
	if (event->type() == QEvent::ParentChange) {
		close();
		open();
	}
}

void XVideoRenderer::customEvent(QEvent *event) {
	if (static_cast<BaseEvent*>(event)->type() != BaseEvent::NewFrame)
		return;
	const VideoFrame frame = static_cast<NewFrameEvent*>(event)->frame();
	d->data = QByteArray(frame.constData(), frame.count());
	d->img = QImage((const uchar *)d->data.data(), frame.width(), frame.height(),QImage::Format_RGB32);
	updateVideoSize(d->img.size());
	update();
}

// AbstractOsdRenderer *XVideoRenderer::createOsd() {
// 	Osd *osd = new Osd(this);
// 	d->osds.push_back(osd);
// 	return osd;
// }

// void XVideoRenderer::renderOsds(QPainter *painter, const QSizeF &visual, const QSizeF &widget) {
// 	for (int i=0; i<d->osds.size(); ++i)
// 		d->osds[i]->renderContents(painter, visual, widget);
// }

void XVideoRenderer::rerender() {
	recalculate();
	update();
}

void XVideoRenderer::recalculate() {
	QSizeF pixmapF = videoSize();
	double aspect = ((aspectRatio() > 0.0) ? aspectRatio() : -1.0);
	if (aspect > 0.0) {
		const double height = std::sqrt(pixmapF.width() * pixmapF.height() / aspect);
		pixmapF.setWidth(aspect*height);
		pixmapF.setHeight(height);
	} else
		aspect = pixmapF.width() / pixmapF.height();
	const double widgetRatio = double(width())/double(height());
	if (aspect > widgetRatio)
		pixmapF.rheight() *= aspect/widgetRatio;
	else
		pixmapF.rwidth() *= widgetRatio/aspect;
	setRenderingSize(pixmapF);
	if (d->xvImage)
		XFree(d->xvImage);
	d->xvImage = XvCreateImage(d->display, d->port, XvRGB32, d->xvData.data()
		, qRound(pixmapF.width()), qRound(pixmapF.height()));
}

void XVideoRenderer::resizeEvent(QResizeEvent *event) {
	recalculate();
// 	update();
}

void XVideoRenderer::paintEvent(QPaintEvent *event) {
	if (!d->xvImage || d->img.isNull() || renderingSize().isEmpty()) {
		QPainter painter(this);
		painter.fillRect(rect(), Qt::black);
		return;
	}
	QPixmap p(d->xvImage->width, d->xvImage->height);
	p.fill(Qt::black);
	QPainter painter(&p);
	painter.drawImage(imageRect(), d->img);
	const double vMargin = verticalMargin();
	const double hMargin = horizontalMargin();
	if (vMargin > 0.5) {
		QRectF rect(1.0, 1.0, p.width(), vMargin);
		painter.fillRect(rect, Qt::black);
		rect.moveTop(p.height() - vMargin);
		painter.fillRect(rect, Qt::black);
	}
	if (hMargin > 0.5) {
		QRectF rect(1.0, 1.0, hMargin, p.height());
		painter.fillRect(rect, Qt::black);
		rect.moveLeft(p.width() - hMargin);
		painter.fillRect(rect, Qt::black);
	}
	renderOsds(&painter, visualSize(), p.size());
	const QImage img = p.toImage();

	d->xvData = QByteArray((const char*)img.bits(), img.numBytes());
	d->xvImage->data = d->xvData.data();

	XvPutImage(d->display, d->port, d->window, d->gc, d->xvImage
		   , 0, 0, d->xvImage->width, d->xvImage->height
		   , 0, 0, width(), height());
	QApplication::syncX();
}

}

#endif
