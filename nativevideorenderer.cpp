#include "nativevideorenderer.hpp"
#include "playengine.hpp"
#include "osdrenderer.hpp"
#include <QtGui/QApplication>
#include <QtCore/QtEndian>
#include <QtGui/QPainter>
#include <QtGui/QTextDocument>
#include "gstimageoverlay.hpp"
#include <gst/interfaces/xoverlay.h>
#include "gstvideoinfo.hpp"
#include <gst/interfaces/propertyprobe.h>
#include <QtCore/QDebug>
#include <gst/video/gstvideosink.h>

//	g_object_set(G_OBJECT(cf1), "caps", gst_caps_new_simple(
//		"video/x-raw-yuv",
//		"width", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"height", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 2147483647, 1,
//		"format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('A', 'Y', 'U', 'V'), NULL), NULL);
//	g_object_set(G_OBJECT(cf1), "caps", gst_caps_new_simple(
//		"video/x-raw-rgb",
//		"width", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"height", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 2147483647, 1,
//		"bpp", G_TYPE_INT, 32,
//		"depth", G_TYPE_INT, 32,
//		"red_mask", G_TYPE_INT, int(0xff00),
//		"green_mask", G_TYPE_INT, int(0xff0000),
//		"blue_mask", G_TYPE_INT, int(0xff000000),
//		"alpha_mask", G_TYPE_INT, int(0xff),
//		"endianness", G_TYPE_INT, 4321, NULL), NULL);

struct CustomPad {
	GstPad pad;
};



class NativeVideoRenderer::XOverlay : public QWidget {
public:
	XOverlay(PlayEngine *engine, NativeVideoRenderer *renderer)
	: QWidget(renderer), m_engine(engine), m_renderer(renderer) {}
	void paintEvent(QPaintEvent */*event*/) {
		if (m_engine->state() != StoppedState)
			m_renderer->windowExposed();
		else {
			QPainter painter(this);
			painter.fillRect(rect(), Qt::black);
		}
	}
private:
	PlayEngine *m_engine;
	NativeVideoRenderer *m_renderer;
};

struct NativeVideoRenderer::Data {
	GstElement *bin, *sink, *box, *info, *overlay;
	PlayEngine *engine;
	XOverlay *xo;
	QSize frameSize;
	QSize sceneSize;
	double frameRate;
	QList<OsdRenderer*> osd;
};

//	GstElement *vrate = gst_element_factory_make("videorate", 0);

NativeVideoRenderer::NativeVideoRenderer(PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->frameSize = QSize(400, 300);
	d->engine = engine;
	d->xo = new XOverlay(d->engine, this);

	d->bin = gst_bin_new("native-video-renderer");
	gst_object_ref(GST_OBJECT(d->bin));
	gst_object_sink(GST_OBJECT(d->bin));

	d->sink = gst_element_factory_make("xvimagesink", 0);
	d->box = gst_element_factory_make("videobox", 0);
	d->info = GST_ELEMENT(g_object_new(GstVideoInfo::gtype(), 0));
	reinterpret_cast<GstVideoInfo*>(d->info)->d->renderer = this;
	d->overlay = GST_ELEMENT(g_object_new(GstImageOverlay::gtype(), 0));

	GstElement *queue = gst_element_factory_make("queue", 0);
	GstElement *conv1 = gst_element_factory_make("ffmpegcolorspace", 0);

	gst_bin_add_many(GST_BIN(d->bin), d->box, d->info, d->overlay, queue, conv1, d->sink, NULL);
	gst_element_link_many(queue, d->info, conv1, NULL);
	gst_element_link_filtered(conv1, d->box, GstImageOverlay::makeDefualtCaps());
	gst_element_link_many(d->box, d->overlay, d->sink, NULL);

	GstPad *pad = gst_element_get_pad(queue, "sink");
	gst_element_add_pad(d->bin, gst_ghost_pad_new("sink", pad));
	gst_object_unref(GST_OBJECT(pad));

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(mrlChanged()));
}

NativeVideoRenderer::~NativeVideoRenderer() {
	gst_object_unref(GST_OBJECT(d->bin));
	delete d;
}

void NativeVideoRenderer::mrlChanged() {
	setXOverlay();
	d->frameSize = QSize(400, 300);
}

GstElement *NativeVideoRenderer::bin() const {
	return d->bin;
}

void NativeVideoRenderer::windowExposed() {
	QApplication::syncX();
	if (d->sink && GST_IS_X_OVERLAY(d->sink))
		gst_x_overlay_expose(GST_X_OVERLAY(d->sink));
}

void NativeVideoRenderer::setXOverlay() {
	if (d->sink && GST_IS_X_OVERLAY(d->sink)) {
		WId wid = d->xo->winId();
		QApplication::syncX();
		gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(d->sink), wid);
	}
	windowExposed();
}

void NativeVideoRenderer::showEvent(QShowEvent *event) {
	d->xo->setAttribute(Qt::WA_NoSystemBackground, true);
	d->xo->setAttribute(Qt::WA_PaintOnScreen, true);
	setXOverlay();
	QWidget::showEvent(event);
}

QRect NativeVideoRenderer::xoverlayGeometry() const {
	QSize size = d->frameSize;
	size.scale(this->size(), Qt::KeepAspectRatio);
	const int x = (width() - size.width())*0.5;
	const int y = (height() - size.height())*0.5;
	return QRect(QPoint(x, y), size);
}

static double aspectRatio(const QSize &size) {
	return (double)size.width()/size.height();
}

//static double aspectRatio(int width, int height) {
//	return (double)width/height;
//}

void NativeVideoRenderer::updateXOverlayGeometry() {
	const double sceneRatio = aspectRatio(d->sceneSize);
	const double widgetRaito = aspectRatio(size());
	const double frameRatio = aspectRatio(d->frameSize);
//	qDebug() << sceneRatio << widgetRaito << frameRatio;
	QSize size = d->sceneSize;
//	QSize visual = d->sceneSize;
	if (sceneRatio <= frameRatio) {
		if (sceneRatio <= widgetRaito && widgetRaito <= frameRatio) {
			size.scale(this->size(), Qt::KeepAspectRatioByExpanding);
//			visual.setHeight(visual.width()/widgetRaito);
		} else if (widgetRaito < sceneRatio) {
			size.scale(this->size(), Qt::KeepAspectRatio);
//			visual.setHeight(visual.width()/widgetRaito);
		} else {
			size.scale(this->size(), Qt::KeepAspectRatioByExpanding);
			size *= frameRatio/widgetRaito;
		}
	} else {
		if (widgetRaito > sceneRatio) {
			size.scale(this->size(), Qt::KeepAspectRatio);
		} else if (sceneRatio >= widgetRaito && widgetRaito >= frameRatio) {
			size.scale(this->size(), Qt::KeepAspectRatioByExpanding);
		} else {
			size.scale(this->size(), Qt::KeepAspectRatioByExpanding);
			size *= widgetRaito/frameRatio;
		}
	}
	const int x = 0.5*(width() - size.width()) + 0.5;
	const int y = 0.5*(height() - size.height()) + 0.5;
//	QRect xoRect(x, y, size.width(), size.height());
	d->xo->setGeometry(x, y, size.width(), size.height());
	const QRect wRect(-x, -y, width(), height());
	const QRect xRect(0, 0, size.width(), size.height());
	const double r = (double)d->sceneSize.width() / size.width();
	QRect vRect = (wRect & xRect);
	vRect.setSize(vRect.size()*r);
	vRect.moveTo(vRect.topLeft()*r);

//	qDebug() << vRect;
//	qDebug() << r << d->sceneSize.width() << size.width();
//	qDebug() << this->size() << d->sceneSize << r;
//	const QSize visualSize = this->size()*r;
//	qDebug() << visualSize << d->sceneSize;
//	GstImageOverlay *o = reinterpret_cast<GstImageOverlay*>(d->overlay);
//	for (int i=0; i<o->d->osd.size(); ++i) {
//		o->d->osd[i]->setArea(vRect);
//	}
	for (int i=0; i<d->osd.size(); ++i)
		d->osd[i]->setArea(vRect);
}

void NativeVideoRenderer::resizeEvent(QResizeEvent *event) {
	updateXOverlayGeometry();
	windowExposed();
	QWidget::resizeEvent(event);
}

void NativeVideoRenderer::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);
}

void NativeVideoRenderer::setFrameSize(const QSize &size) {
	if (d->frameSize == size)
		return;
	d->sceneSize = d->frameSize = size;

	const double frameAspect = aspectRatio(d->frameSize);
	const double targetAspect = desktopRatio();
	int hmargin = 0;
	int vmargin = 0;
	if (frameAspect > targetAspect) {
		vmargin = 0.5*d->frameSize.width()*(1.0/targetAspect - 1.0/frameAspect) + 0.5;
		d->sceneSize.rheight() *= frameAspect/targetAspect;
	} else if (frameAspect < targetAspect) {
		hmargin = 0.5*d->frameSize.height()*(targetAspect - frameAspect) + 0.5;
		d->sceneSize.rwidth() *= targetAspect/frameAspect;
	}
	g_object_set(G_OBJECT(d->box), "top", -vmargin, NULL);
	g_object_set(G_OBJECT(d->box), "bottom", -vmargin, NULL);
	g_object_set(G_OBJECT(d->box), "left", -hmargin, NULL);
	g_object_set(G_OBJECT(d->box), "right", -hmargin, NULL);

	updateXOverlayGeometry();
}

QSize NativeVideoRenderer::sizeHint() const {
	return d->frameSize;
}

void NativeVideoRenderer::addOsdRenderer(OsdRenderer *osd) {
	d->osd.push_back(osd);
	osd->setImageOverlay(reinterpret_cast<GstImageOverlay*>(d->overlay));
}

void NativeVideoRenderer::setFrameRate(double frameRate) {
	if (d->frameRate != frameRate)
		emit frameRateChanged(d->frameRate = frameRate);
}

double NativeVideoRenderer::frameRate() const {
	return d->frameRate;
}
