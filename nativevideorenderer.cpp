#include "nativevideorenderer.hpp"
#include "gstvideoman.hpp"
#include "mrl.hpp"
#include <QtGui/QMouseEvent>
#include "playengine.hpp"
#include "osdrenderer.hpp"
#include <QtGui/QApplication>
#include <QtCore/QtEndian>
#include <QtGui/QPainter>
#include <QtGui/QTextDocument>
#include <gst/interfaces/xoverlay.h>
#include "gstvideoinfo.hpp"
#include <gst/interfaces/propertyprobe.h>
#include <QtCore/QDebug>
#include <gst/video/gstvideosink.h>
#include <gst/interfaces/navigation.h>
#include "gstvideobox2.hpp"

class NativeVideoRenderer::XOverlay : public QWidget {
public:
	XOverlay(PlayEngine *engine, NativeVideoRenderer *renderer, QWidget *parent)
	: QWidget(parent), m_engine(engine), m_renderer(renderer), m_nav(0)
	, m_vm(0), m_hm(0) {}
	void paintEvent(QPaintEvent */*event*/) {
		if (m_engine->state() != StoppedState)
			m_renderer->windowExposed();
		else {
			QPainter painter(this);
			painter.fillRect(rect(), Qt::black);
		}
	}
	void moveToCenter() {
		QWidget *w = parentWidget();
		const int x = (w->width() - width())*0.5 + 0.5;
		const int y = (w->height() - height())*0.5 + 0.5;
		move(x, y);
	}
	void mouseMoveEvent(QMouseEvent *event) {
		sendNavEvent(event, "mouse-move");
		QWidget::mouseMoveEvent(event);
	}
	void mousePressEvent(QMouseEvent *event) {
		sendNavEvent(event, "mouse-button-press");
		QWidget::mousePressEvent(event);
	}
	void mouseReleaseEvent(QMouseEvent *event) {
		sendNavEvent(event, "mouse-button-release");
		QWidget::mouseReleaseEvent(event);
	}
	void setNavigation(GstNavigation *nav) {
		m_nav = nav;
	}
	void setMargin(int vm, int hm) {
		m_vm = vm;
		m_hm = hm;
	}
private:
	void sendNavEvent(QMouseEvent *event, const char *name) {
		if (!m_nav || !m_engine->mrl().toString().startsWith("dvd:"))
			return;
		const int button = buttonNumber(event->button());
		const QPoint pos = navMap(event->pos());
		gst_navigation_send_mouse_event(m_nav, name, button, pos.x(), pos.y());
	}

	QPoint navMap(const QPoint &pos) {
		QPoint p = pos;
		if (x() < 0)
			p.rx() += x();
		if (y() < 0)
			p.ry() += y();
		return p;
	}

	static int buttonNumber(Qt::MouseButton button) {
		int b = 0;
		switch (button) {
		case Qt::RightButton:	++b;
		case Qt::MidButton:	++b;
		case Qt::LeftButton:	++b;
			break;
		default:		b = 0;
		}
		return b;
	}
	PlayEngine *m_engine;
	NativeVideoRenderer *m_renderer;
	GstNavigation *m_nav;
	int m_vm, m_hm;
};

class NativeVideoRenderer::CropBox : public QWidget {
public:
	CropBox(PlayEngine *engine, NativeVideoRenderer *parent)
	: QWidget(parent), m_xo(new XOverlay(engine, parent, this)) {

	}
	bool crop(double ratio) {
		if (ratio < 0) {
			resize(parentWidget()->size());
		} else {
			QSizeF size(ratio, 1.0);
			size.scale(parentWidget()->size(), Qt::KeepAspectRatio);
			resize(size.toSize());
		}
		moveToCenter();
		return ratio > 0;
	}
	XOverlay *xOverlay() const {return m_xo;}
private:
	void moveToCenter() {
		QWidget *w = parentWidget();
		const int x = (w->width() - width())*0.5 + 0.5;
		const int y = (w->height() - height())*0.5 + 0.5;
		move(x, y);
	}
	XOverlay *m_xo;
};

struct NativeVideoRenderer::Data {
	GstElement *bin, *sink, *info;
	GstVideoMan *man;
	PlayEngine *engine;
	XOverlay *xo;
	QSize frameSize;
	QSize sceneSize;
	QSize expandedSize;
	double frameRate;
	QList<OsdRenderer*> osd;
	double aspectRatio, cropRatio;
	CropBox *crop;
	GstNavigation *nav;
};

//	GstElement *vrate = gst_element_factory_make("videorate", 0);

NativeVideoRenderer::NativeVideoRenderer(PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->expandedSize = d->frameSize = QSize(400, 300);
	d->engine = engine;
	d->crop = new CropBox(d->engine, this);
	d->xo = d->crop->xOverlay();
	d->nav = 0;
	d->aspectRatio = -1.0;
	d->cropRatio = -1;

	d->bin = gst_bin_new("native-video-renderer");
	gst_object_ref(GST_OBJECT(d->bin));
	gst_object_sink(GST_OBJECT(d->bin));

	d->sink = gst_element_factory_make("xvimagesink", 0);
	d->info = GST_ELEMENT(g_object_new(GstVideoInfo::gtype(), 0));
	reinterpret_cast<GstVideoInfo*>(d->info)->d->renderer = this;
//	d->overlay = GST_ELEMENT(g_object_new(GstImageOverlay::gtype(), 0));
//	reinterpret_cast<GstImageOverlay*>(d->overlay)->d->engine = engine;

	GstElement *queue = gst_element_factory_make("queue", 0);
	d->man = GST_VIDEO_MAN(g_object_new(gst_video_man_get_type(), 0));

	gst_bin_add_many(GST_BIN(d->bin), GST_ELEMENT(d->man), d->info, /*d->overlay, */queue, d->sink, NULL);
	gst_element_link_many(queue, d->info, d->man, /*d->overlay, */d->sink, NULL);

	GstPad *pad = gst_element_get_pad(queue, "sink");
	gst_element_add_pad(d->bin, gst_ghost_pad_new("sink", pad));
	gst_object_unref(GST_OBJECT(pad));

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(mrlChanged()));

	g_object_set(G_OBJECT(d->sink), "handle-events", 0, NULL);

	if (GST_IS_NAVIGATION(d->sink))
		d->nav = GST_NAVIGATION(d->sink);

	setMouseTracking(true);
	d->xo->setMouseTracking(true);
	d->crop->setMouseTracking(true);

	d->xo->setNavigation(d->nav);

//	int vmargin = -1;
//	int hmargin = 0;
//	g_object_set(G_OBJECT(d->box), "top", 0, NULL);
//	g_object_set(G_OBJECT(d->box), "bottom", 15, NULL);
//	g_object_set(G_OBJECT(d->box), "left", -hmargin, NULL);
//	g_object_set(G_OBJECT(d->box), "right", -hmargin, NULL);
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

static double ratio(const QSize &size) {
	return (double)size.width()/size.height();
}

void NativeVideoRenderer::updateXOverlayGeometry() {
	const QSizeF widget(size());
	QSizeF video = d->aspectRatio > 0 ? QSizeF(d->aspectRatio, 1.0) : d->frameSize;
	video.scale(widget, Qt::KeepAspectRatio);
	QSizeF xo(desktopRatio(), 1);
	if (d->crop->crop(d->cropRatio)) {
		xo.scale(d->crop->size(), Qt::KeepAspectRatioByExpanding);
	} else
		xo.scale(video, Qt::KeepAspectRatioByExpanding);

	d->xo->resize(xo.toSize());
	d->xo->moveToCenter();

	QRect area(QPoint(0, 0), d->xo->size());
	area &= QRect(-d->xo->pos(), d->crop->size());
	QSizeF box(desktopRatio(), 1.0);
	box.scale(d->frameSize, Qt::KeepAspectRatioByExpanding);
	const double r = (double)d->expandedSize.width()/d->xo->width();
	area.setSize(area.size()*r);
	area.moveTo(area.topLeft()*r);
	for (int i=0; i<d->osd.size(); ++i)
		d->osd[i]->setArea(area
			, box.width() / d->expandedSize.width()
			, box.height() / d->expandedSize.height());
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
	d->frameSize = size;
	updateBoxSize();
	updateXOverlayGeometry();
}

void NativeVideoRenderer::updateBoxSize() {

	const double aspectRatio = (d->aspectRatio > 0 ? d->aspectRatio : ratio(d->frameSize));
	const double desktopRatio = ::desktopRatio();
	int hmargin = 0;
	int vmargin = 0;

	if (aspectRatio >= desktopRatio)
		vmargin = d->frameSize.height()*(aspectRatio/desktopRatio - 1.0)*0.5;
	else
		hmargin = d->frameSize.width()*(desktopRatio/aspectRatio - 1.0)*0.5;

	d->man->setBorder(hmargin, vmargin);
	d->expandedSize = d->frameSize + 2*QSize(hmargin, vmargin);

	d->xo->setMargin(vmargin, hmargin);
}

QSize NativeVideoRenderer::sizeHint() const {
	if (d->cropRatio < 0) {
		if (d->aspectRatio < 0)
			return d->frameSize;
		QSizeF size(d->aspectRatio, 1.0);
		size.scale(d->frameSize, Qt::KeepAspectRatioByExpanding);
		return size.toSize();
	} else {
		QSizeF size(d->cropRatio, 1.0);
		size.scale(d->frameSize, Qt::KeepAspectRatio);
		return size.toSize();
	}
}

void NativeVideoRenderer::addOsdRenderer(OsdRenderer *osd) {
	d->osd.push_back(osd);
	osd->setVideoMan(d->man);
}

void NativeVideoRenderer::setFrameRate(double frameRate) {
	if (d->frameRate != frameRate)
		emit frameRateChanged(d->frameRate = frameRate);
}

double NativeVideoRenderer::frameRate() const {
	return d->frameRate;
}

double NativeVideoRenderer::aspectRatio() const {
	return d->aspectRatio;
}

double NativeVideoRenderer::cropRatio() const {
	return d->cropRatio;
}

void NativeVideoRenderer::setAspectRatio(double ratio) {
	if (isSameRatio(ratio, d->aspectRatio))
		return;
	d->aspectRatio = ratio;
	if (qFuzzyCompare(d->aspectRatio, ::ratio(d->frameSize)))
		return;
	updateBoxSize();
	updateXOverlayGeometry();
}

void NativeVideoRenderer::setCropRatio(double ratio) {
	if (isSameRatio(ratio, d->cropRatio))
		return;
	d->cropRatio = ratio;
	updateXOverlayGeometry();
}

GstNavigation *NativeVideoRenderer::nav() const {
	return d->nav;
}
