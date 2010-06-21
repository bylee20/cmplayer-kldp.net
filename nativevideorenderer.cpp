#include "nativevideorenderer.hpp"
#include "gstvideoman.hpp"
#include "playengine.hpp"
#include "osdrenderer.hpp"
#include <QtGui/QMouseEvent>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <gst/interfaces/xoverlay.h>
#include <gst/interfaces/navigation.h>
#include <gst/video/gstvideosink.h>
#include "videomanipulator.hpp"

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

struct NativeVideoRenderer::Data {
	GstElement *bin, *sink;
	VideoManipulator man;
	PlayEngine *engine;
	XOverlay *xo;
	QSize frameSize;
	QSize sceneSize;
	QSize expandedSize;
	double frameRate;
	QList<OsdRenderer*> osd;
	double aspectRatio, cropRatio;
	GstNavigation *nav;
};

NativeVideoRenderer::NativeVideoRenderer(PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->expandedSize = d->frameSize = QSize(400, 300);
	d->engine = engine;
	d->xo = new XOverlay(engine, this, this);
	d->nav = 0;
	d->aspectRatio = -1.0;
	d->cropRatio = -1;

	d->bin = gst_bin_new("native-video-renderer");
	gst_object_ref(GST_OBJECT(d->bin));
	gst_object_sink(GST_OBJECT(d->bin));

	GstElement *queue = gst_element_factory_make("queue", 0);
//	d->man = GST_VIDEO_MAN(g_object_new(GstVideoManClass::getType(), 0));
	d->sink = gst_element_factory_make("xvimagesink", 0);
	GstElement *conv = gst_element_factory_make("ffmpegcolorspace", 0);
	gst_bin_add_many(GST_BIN(d->bin), conv, d->man.element(), queue, d->sink, NULL);
	gst_element_link_many(queue, conv, d->man.element(), d->sink, NULL);

	GstPad *pad = gst_element_get_pad(queue, "sink");
	gst_element_add_pad(d->bin, gst_ghost_pad_new("sink", pad));
	gst_object_unref(GST_OBJECT(pad));

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(mrlChanged()));

	if (GST_IS_NAVIGATION(d->sink))
		d->nav = GST_NAVIGATION(d->sink);

	setMouseTracking(true);
	d->xo->setMouseTracking(true);
	d->xo->setNavigation(d->nav);

	updateXOverlayGeometry();
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
	QRectF video = d->man.videoRect();
	QSizeF scaled(desktopRatio(), 1.0);
	scaled.scale(d->expandedSize, Qt::KeepAspectRatio);
	const double scale_h = scaled.width()/d->expandedSize.width();
	const double scale_v = scaled.height()/d->expandedSize.height();
	video.setWidth(video.width()*scale_h);
	video.setHeight(video.height()*scale_v);
	video.moveTop(video.top()*scale_v);
	video.moveLeft(video.left()*scale_h);
	QSizeF box = video.size();
	box.scale(size(), Qt::KeepAspectRatio);
	const int w = scaled.width()*(box.width()/video.width()) + 0.5;
	const int h = scaled.height()*(box.height()/video.height()) + 0.5;
	d->xo->resize(w, h);
	d->xo->moveToCenter();

	QSizeF widget(size());
	widget.scale(video.size(), Qt::KeepAspectRatioByExpanding);
	QPoint pos = video.topLeft().toPoint();
	QSize size = video.size().toSize();
	if (widget.width() > scaled.width()) {
		pos.rx() = 0;
		size.rwidth() = scaled.width() + 0.5;
	} else if (widget.width() > size.width()) {
		pos.rx() -= qRound(0.5*(widget.width()-size.width()));
		size.rwidth() = widget.width() + 0.5;
	}
	if (widget.height() > scaled.height()) {
		pos.ry() = 0;
		size.rheight() = scaled.height() + 0.5;
	} else if (widget.height() > size.height()) {
		pos.ry() -= qRound(0.5*(widget.height()-size.height()));
		size.rheight() = widget.height();
	}
	const QRect area(pos, size);
	for (int i=0; i<d->osd.size(); ++i)
		d->osd[i]->setArea(area, scale_h, scale_v);
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

void NativeVideoRenderer::updateBoxSize() {
	const double aspectRatio = (d->aspectRatio > 0 ? d->aspectRatio : ratio(d->frameSize));
	const double desktopRatio = ::desktopRatio();
	int hmargin = 0, vmargin = 0;
	if (aspectRatio >= desktopRatio)
		vmargin = qRound(d->frameSize.height()*(aspectRatio/desktopRatio - 1.0));
	else
		hmargin = qRound(d->frameSize.width()*(desktopRatio/aspectRatio - 1.0));
	d->man.setBorder(hmargin, vmargin);
	d->expandedSize = d->frameSize + QSize(hmargin, vmargin);
	int crop_h = 0, crop_v = 0;
	if (d->cropRatio > 0) {
		QSizeF xo(desktopRatio, 1.0);
		xo.scale(d->expandedSize, Qt::KeepAspectRatio);
		QSizeF video(aspectRatio, 1.0);
		video.scale(xo, Qt::KeepAspectRatio);
		QSizeF crop(d->cropRatio, 1.0);
		crop.scale(video, Qt::KeepAspectRatio);
		const double ch = video.width() - crop.width();
		const double cv = video.height() - crop.height();
		crop_h = ch*d->expandedSize.width()/xo.width() + 0.5;
		crop_v = cv*d->expandedSize.height()/xo.height() + 0.5;
	}
	d->man.crop(crop_h, crop_v);
}

QSize NativeVideoRenderer::sizeHint() const {
	QSizeF size(d->aspectRatio, 1.0);
	if (d->aspectRatio > 0)
		size.scale(d->expandedSize, Qt::KeepAspectRatio);
	else
		size = d->frameSize;
	if (d->cropRatio > 0) {
		QSizeF crop(d->cropRatio, 1.0);
		crop.scale(size, Qt::KeepAspectRatio);
		size = crop;
	}
	return size.toSize();
}

void NativeVideoRenderer::addOsdRenderer(OsdRenderer *osd) {
	d->osd.push_back(osd);
//	osd->setVideoMan(d->man);
}

void NativeVideoRenderer::setInfo(const VideoInfo &info) {
	if (!qFuzzyCompare(d->frameRate, info.fps))
		emit frameRateChanged(d->frameRate = info.fps);
	d->frameSize.setWidth(info.width);
	d->frameSize.setHeight(info.height);
	updateBoxSize();
	updateXOverlayGeometry();
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
	if (!isSameRatio(ratio, d->aspectRatio)) {
		d->aspectRatio = ratio;
		updateBoxSize();
		updateXOverlayGeometry();
		if (d->engine->isPaused()) {
//			d->man->rerender();
			d->engine->flush();
		}
	}
}

void NativeVideoRenderer::setCropRatio(double ratio) {
	if (!isSameRatio(ratio, d->cropRatio)) {
		d->cropRatio = ratio;
		updateBoxSize();
		updateXOverlayGeometry();
	}
}

GstNavigation *NativeVideoRenderer::nav() const {
	return d->nav;
}

void NativeVideoRenderer::showFrame(GstBuffer *buffer) {
	GstBaseSink *sink = GST_BASE_SINK(d->sink);
	GstBaseSinkClass *klass = GST_BASE_SINK_GET_CLASS(sink);
	if (klass->render)
		klass->render(sink, buffer);
}

GstBuffer *NativeVideoRenderer::allocBuffer(int size, GstCaps *caps) {
	GstBaseSink *sink = GST_BASE_SINK(d->sink);
	GstBaseSinkClass *klass = GST_BASE_SINK_GET_CLASS(sink);
	GstBuffer *buffer;
	if (GST_FLOW_OK != klass->buffer_alloc(sink, 0, size, caps, &buffer))
		return 0;
	return buffer;
}
