#include "softwarerenderer.h"

#if HAS_RAW_OUTPUT

#include "xinestream.h"
#include <QtCore/QDebug>
#include <QtGui/QWidget>
#include <core/newframeevent.h>
#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>
#include "events.h"
#include <QtGui/QPainter>

namespace Xine {

class SoftwareRenderer::Overlay {
public:
	Overlay(const raw_overlay_t &ovl)
		: m_data((const char*)ovl.ovl_rgba, ovl.ovl_w * ovl.ovl_h * 4)
		, m_image((const uchar*)(m_data.constData()), ovl.ovl_w
		, ovl.ovl_h, QImage::Format_ARGB32)
		, m_pos(ovl.ovl_x, ovl.ovl_y) {
			m_image = m_image.rgbSwapped();
		}
	const QByteArray &data() const {return m_data;}
	const QImage &image() const {return m_image;}
	const QPoint &pos() const {return m_pos;}
private:
	QByteArray m_data;
	QImage m_image;
	QPoint m_pos;
};

class NewOverlayEvent : public Core::BaseEvent {
public:
	NewOverlayEvent(const QList<SoftwareRenderer::Overlay> overlays)
	: BaseEvent(NewOverlay), overlays(overlays) {}
	const QList<SoftwareRenderer::Overlay> overlays;
};

struct SoftwareRenderer::Data {
	Core::SoftwareRendererIface *renderer;
	QList<Overlay> overlays;
	raw_visual_t visual;
	Core::SoftwareRendererType softwareType;
};

SoftwareRenderer::SoftwareRenderer(Core::SoftwareRendererType type, XineStream *stream)
: XineVideoIface(stream), d(new Data) {
	d->renderer = Core::SoftwareRendererIface::create(type);
	if (d->renderer) {
		d->renderer->setObject(this);
		d->renderer->target()->setMouseTracking(true);
		d->softwareType = type;
	} else
		d->softwareType = Core::UnknownRenderer;
	d->visual.user_data = this;
	d->visual.supported_formats = XINE_VORAW_YV12 | XINE_VORAW_YUY2;
	d->visual.raw_output_cb = cbRawOutput;
	d->visual.raw_overlay_cb = cbRawOverlay;

}

SoftwareRenderer::~SoftwareRenderer() {
	delete d->renderer;
	delete d;
}

Core::SoftwareRendererIface *SoftwareRenderer::renderer() {
	return d->renderer;
}

void *SoftwareRenderer::visual() {return &d->visual;}

void SoftwareRenderer::cbRawOutput(void *user_data, int frame_format, int frame_width, int frame_height,
		 double frame_aspect, void *data0, void *data1, void *data2) {
	Core::VideoFrame *frame = new Core::VideoFrame();
	Core::VideoFrame::Info info;
	info.setSize(frame_width, frame_height);
	info.pixelAspectRatio = frame_aspect / double(frame_width) * double(frame_height);
	if (frame_format == XINE_VORAW_YV12) {
		info.type = Core::VideoFrame::YV12;
		const int count1 = frame_width*frame_height;
		const int count2 = (frame_width/2) * (frame_height/2);
		frame->reserve(count1 + count2*2 + 1);
		frame->append(reinterpret_cast<const char*>(data0), count1);
		frame->append(reinterpret_cast<const char*>(data2), count2);
		frame->append(reinterpret_cast<const char*>(data1), count2);
	} else if (frame_format == XINE_VORAW_YUY2) {
		info.type = Core::VideoFrame::YUY2;
		frame->setData(data0, frame_width*frame_height*2);
	} else {
		info.type = Core::VideoFrame::RGB;
		frame->setData(data0, frame_width*frame_height*3);
	}
	frame->setInfo(info);
	Core::SoftwareRendererIface *r = reinterpret_cast<SoftwareRenderer*>(user_data)->d->renderer;
	if (r)
		r->setFrame(*frame);
	delete frame;
}

void SoftwareRenderer::cbRawOverlay(void *user_data, int num_ovl, raw_overlay_t *overlays_array) {
	QList<Overlay> *overlays = new QList<Overlay>();
	for (int i=0; i<num_ovl; ++i)
		overlays->append(overlays_array[i]);
	NewOverlayEvent *event = new NewOverlayEvent(*overlays);
	delete overlays;
	QApplication::postEvent(reinterpret_cast<SoftwareRenderer*>(user_data), event);
}

void SoftwareRenderer::customEvent(QEvent *event) {
	Core::BaseEvent *be = static_cast<Core::BaseEvent*>(event);
	if (be->type() == NewOverlay) {
		d->overlays = static_cast<NewOverlayEvent*>(be)->overlays;
		if (d->renderer)
			d->renderer->rerender();
	}
}

void SoftwareRenderer::overdraw(QPainter *painter) {
	if (!d->overlays.isEmpty()) {
		const QList<Overlay> *ovls = &d->overlays;
		for (int i=0; i<ovls->size(); ++i) {
			const Overlay &ovl = (*ovls)[i];
			const QRectF rect(d->renderer->mapScreenToWidget(ovl.pos())
					, d->renderer->scaleScreenToWidget(ovl.image().size()));
			painter->drawImage(rect, ovl.image(), ovl.image().rect());
		}
	}
}

void SoftwareRenderer::mouseMoveEvent(const QPoint &pos) {
	if (isMouseTrackingActivated())
		mouseMoved(d->renderer->mapWidgetToScreen(pos).toPoint());
}

void SoftwareRenderer::mousePressEvent(const QPoint &pos, Qt::MouseButton button) {
	if (isMouseTrackingActivated())
		mouseClicked(d->renderer->mapWidgetToScreen(pos).toPoint(), button);
}


} // namespace Xine

#endif

