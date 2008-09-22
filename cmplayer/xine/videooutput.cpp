#include <backend/utility.h>
#include "xinepost.h"
#include "videooutput.h"
#include "xinestream.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>
#include <QPainter>
#include "playengine.h"
#include <QApplication>
#include <QDebug>

namespace Backend {

namespace Xine {

VideoOutput::VideoOutput(PlayEngine *engine, XineStream *stream)
: Backend::VideoOutput(engine) {
	m_engine = engine;
	m_stream = stream;
	m_port = 0;
	
	int screen_nbr = 0;
	if (!(m_video.connection = xcb_connect(0, &screen_nbr)))
		return;
	m_video.visual.connection = m_video.connection;
	xcb_screen_iterator_t screenIt = xcb_setup_roots_iterator(xcb_get_setup(m_video.connection));
	while ((screenIt.rem > 1) && (screen_nbr > 0)) {
		xcb_screen_next(&screenIt);
		--screen_nbr;
	}
	m_video.visual.screen = screenIt.data;
	m_video.visual.window = internalWidget()->winId();
	m_video.visual.user_data = this;
	m_video.visual.dest_size_cb = cbDestSize;
	m_video.visual.frame_output_cb = cbFrameOutput;
	QApplication::syncX();
	internalWidget()->installEventFilter(this);
}

VideoOutput::~VideoOutput() {
	if (m_video.connection)
		xcb_disconnect(m_video.connection);
}

void VideoOutput::cbDestSize(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_width, int *dest_height, double *dest_pixel_aspect) {
	VideoOutput *v = reinterpret_cast<VideoOutput*>(user_data);
	*dest_width = v->internalWidget()->width();
	*dest_height = v->internalWidget()->height();
	*dest_pixel_aspect = v->videoRatio()/v->aspectRatioF();
}

void VideoOutput::cbFrameOutput(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_x, int *dest_y, int *dest_width, int *dest_height,
		double *dest_pixel_aspect, int *win_x, int *win_y) {
	VideoOutput *v = reinterpret_cast<VideoOutput*>(user_data);
	QWidget *w = v->internalWidget();
	*dest_x = 0;
	*dest_y = 0;
	*win_x = w->x();
	*win_y = w->y();
	*dest_width = w->width();
	*dest_height = w->height();
	*dest_pixel_aspect = v->videoRatio()/v->aspectRatioF();
}

QRectF VideoOutput::videoRect() const {
	if (!isExpanded())
		return QRect(QPoint(0, 0), videoSize());
	QWidget *w = internalWidget();
	QRectF rect(QPointF(0.0, 0.0), w->size());
	if (videoRatio() > Utility::desktopRatio())
		rect.setHeight(rect.height()/videoOverDesktop());
	else
		rect.setWidth(rect.width()*videoOverDesktop());
	rect.moveTopLeft(QPoint((w->width()-rect.width())/2, (w->height()-rect.height())/2));
	return rect;
}

bool VideoOutput::eventFilter(QObject *obj, QEvent *event) {
	if (obj != internalWidget() || !m_port)
		return QObject::eventFilter(obj, event);
	QWidget *w = static_cast<QWidget*>(obj);
	switch (event->type()) {
	case QEvent::User+100:
		w->repaint();
	case QEvent::Paint:
		if (!m_engine->isStopped()) {
			const QRect &rect = static_cast<QPaintEvent*>(event)->rect();
			xcb_expose_event_t xcb_event;
			memset(&xcb_event, 0, sizeof(xcb_event));
			xcb_event.window = w->winId();
			xcb_event.x = rect.x();
			xcb_event.y = rect.y();
			xcb_event.width = rect.width();
			xcb_event.height = rect.height();
			xcb_event.count = 0;
			xine_port_send_gui_data(m_port, XINE_GUI_SEND_EXPOSE_EVENT, &xcb_event);
		} else {
			QPainter painter(w);
			painter.drawImage(w->rect(), m_image, m_image.rect());
		}
		return true;
	case QEvent::MouseButtonPress:
		if (m_engine->currentSource().isDisc()) {
			QMouseEvent *me = static_cast<QMouseEvent*>(event);
			uint8_t button = 0;
			switch (me->button()) {
				case Qt::RightButton:
					++button;
				case Qt::MidButton:
					++button;
				case Qt::LeftButton:
					++button;
					break;
				default:
					button = 0;
			}
			if (button) {
				QPoint pos = videoRect().topLeft().toPoint();
				x11_rectangle_t rect;
				rect.x = me->x() - pos.x();
				rect.y = me->y() - pos.y();
				rect.w = 0;
				rect.h = 0;
				xine_port_send_gui_data(m_port, XINE_GUI_SEND_TRANSLATE_GUI_TO_VIDEO, &rect);
				if (m_stream->isOpen()) {
					xine_input_data_t input;
					input.button = button;
					input.x = rect.x;
					input.y = rect.y;
					xine_event_t event;
					event.type = XINE_EVENT_INPUT_MOUSE_BUTTON;
					event.data = &input;
					event.data_length = sizeof(input);
					xine_event_send(m_stream->stream(), &event);
				}
			}
		}
		return false;
	case QEvent::MouseMove:
		if (m_engine->currentSource().isDisc()) {
			QMouseEvent *me = static_cast<QMouseEvent*>(event);
			QPoint pos = videoRect().topLeft().toPoint();
			x11_rectangle_t rect;
			rect.x = me->x() - pos.x();
			rect.y = me->y() - pos.y();
			rect.w = 0;
			rect.h = 0;
			xine_port_send_gui_data(m_port, XINE_GUI_SEND_TRANSLATE_GUI_TO_VIDEO, &rect);
			if (m_stream->isOpen()) {
				xine_input_data_t input;
				input.button = 0;
				input.x = rect.x;
				input.y = rect.y;
				xine_event_t event;
				event.type = XINE_EVENT_INPUT_MOUSE_MOVE;
				event.data = &input;
				event.data_length = sizeof(input);
				xine_event_send(m_stream->stream(), &event);
			}
		}
		return false;
	default:
		return false;
	}
	return false;
}

void VideoOutput::updateBrightness(double value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_BRIGHTNESS, eq(value));
}

void VideoOutput::updateContrast(double value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_CONTRAST, eq(value));
}

void VideoOutput::updateHue(double value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_HUE, eq(value));
}

void VideoOutput::updateSaturation(double value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_SATURATION, eq(value));
}

void *VideoOutput::visual() {
	return &m_video.visual;
}


QSize VideoOutput::visualSize(bool scaled) const {
	if (scaled) {
		if (cropRatio() > 0.0) {
			return widgetSizeHint();
		} else {
			QSize size = visualWidget()->size();
			size.scale(videoSize(), Qt::KeepAspectRatio);
			return size;
		}
	} else {
		if (cropRatio() > 0.0) {
			QSize size = widgetSizeHint();
			size.scale(internalWidget()->size(), Qt::KeepAspectRatio);
			return size;
		} else {
			return visualWidget()->size();
		}
	}
}

QRect VideoOutput::osdRect(bool forScaled) const {
	if (forScaled && !isExpanded())
		return QRect(QPoint(0, 0), widgetSizeHint());
	QPoint pos = internalWidget()->mapTo(visualWidget(), QPoint(0, 0));
	QSize size(qMin(visualWidget()->width(), internalWidget()->width()), qMin(visualWidget()->height(), internalWidget()->height()));
	if (pos.y() < 0)
		pos.setY(-pos.y());
	if (pos.x() < 0)
		pos.setX(-pos.x());
	if (forScaled) {
		const double scale = videoRect().width()/widgetSizeHint().width();
		pos /= scale;
		size /= scale;
	}
	return QRect(pos, size);
}

void VideoOutput::updateSizeInfo() {
	if (!m_engine->hasVideo())
		return;
	QSize size;
	size.setWidth(xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_VIDEO_WIDTH));
	size.setHeight(xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_VIDEO_HEIGHT));
	updateVideoSize(size);
}

bool VideoOutput::updateExpand(bool expanded) {
	if (expanded) {
		XinePost *post = m_stream->addVideoPost("expand");
		if (!post)
			return false;
		XinePost::DoubleParam *param
				= static_cast<XinePost::DoubleParam*>(post->parameters()->value("aspect"));
		if (!param)
			return false;
		param->setValue(Utility::desktopRatio());
	} else
		m_stream->removeVideoPost("expand");
	return true;
}

}

}
