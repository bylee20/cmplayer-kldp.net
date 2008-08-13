#include "videowidget.h"
#include "videooutput.h"
#include "xinestream.h"
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

#if (USE_XCB)
#include <QPaintEvent>
#include <QPainter>
#else
#include <X11/Xlib.h>
#endif

namespace Xine {

VideoWidget::VideoWidget(VideoOutput *video, QWidget *parent)
: QWidget(parent), m_video(video) {
#if (USE_XCB)
	int screen_nbr = 0;
	if (!(m_connection = xcb_connect(NULL, &screen_nbr)))
		return;
	m_visual.connection = m_connection;
	xcb_screen_iterator_t screenIt = xcb_setup_roots_iterator(xcb_get_setup(m_connection));
	while ((screenIt.rem > 1) && (screen_nbr > 0)) {
		xcb_screen_next(&screenIt);
		--screen_nbr;
	}
	m_visual.screen = screenIt.data;
	m_visual.window = winId();
#else
	XInitThreads();
	m_display = XOpenDisplay(NULL);
	m_visual.display = m_display;
	m_visual.screen = DefaultScreen(m_display);
	m_visual.d = winId();
#endif
	m_visual.user_data = this;
	m_visual.dest_size_cb = cbDestSize;
	m_visual.frame_output_cb = cbFrameOutput;
	QApplication::syncX();
}

VideoWidget::~VideoWidget() {
	if (m_connection)
#if USE_XCB
		xcb_disconnect(m_connection);
#else
		XCloseDisplay(m_display);
#endif
}

void VideoWidget::cbDestSize(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_width, int *dest_height, double *dest_pixel_aspect) {
	VideoWidget *w = reinterpret_cast<VideoWidget*>(user_data);
	*dest_width = w->width();
	*dest_height = w->height();
	*dest_pixel_aspect = 1;
}

void VideoWidget::cbFrameOutput(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_x, int *dest_y, int *dest_width, int *dest_height,
		double *dest_pixel_aspect, int *win_x, int *win_y) {
	VideoWidget *w = reinterpret_cast<VideoWidget*>(user_data);
	*dest_x = 0;
	*dest_y = 0;
	*win_x = w->x();
	*win_y = w->y();
	*dest_width = w->width();
	*dest_height = w->height();
	*dest_pixel_aspect = 1;
}

void VideoWidget::mouseMoveEvent(QMouseEvent *event) {
	if (m_video->port()) {
		x11_rectangle_t rect;
		rect.x = event->x();
		rect.y = event->y();
		rect.w = 0;
		rect.h = 0;
		xine_port_send_gui_data(m_video->port(), XINE_GUI_SEND_TRANSLATE_GUI_TO_VIDEO, &rect);
		if (m_video->stream()->isOpen()) {
			xine_input_data_t input;
			input.button = 0;
			input.x = rect.x;
			input.y = rect.y;
			xine_event_t event;
			event.type = XINE_EVENT_INPUT_MOUSE_MOVE;
			event.data = &input;
			event.data_length = sizeof(input);
			xine_event_send(m_video->stream()->stream(), &event);
		}
	}
    QWidget::mouseMoveEvent(event);
}

void VideoWidget::mousePressEvent(QMouseEvent *event) {
	uint8_t button = 0;
	switch (event->button()) {
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
	if (button && m_video->port()) {
		x11_rectangle_t rect;
		rect.x = event->x();
		rect.y = event->y();
		rect.w = 0;
		rect.h = 0;
		xine_port_send_gui_data(m_video->port(), XINE_GUI_SEND_TRANSLATE_GUI_TO_VIDEO, &rect);
		if (m_video->stream()->isOpen()) {
			xine_input_data_t input;
			input.button = button;
			input.x = rect.x;
			input.y = rect.y;
			xine_event_t event;
			event.type = XINE_EVENT_INPUT_MOUSE_BUTTON;
			event.data = &input;
			event.data_length = sizeof(input);
			xine_event_send(m_video->stream()->stream(), &event);
		}
	}
	QWidget::mousePressEvent(event);
}

#if (USE_XCB)
void VideoWidget::paintEvent(QPaintEvent *event) {
	if (!m_video->stream()->isStopped()) {
		const QRect &rect = event->rect();
		xcb_expose_event_t xcb_event;
		memset(&xcb_event, 0, sizeof(xcb_event));
		xcb_event.window = winId();
		xcb_event.x = rect.x();
		xcb_event.y = rect.y();
		xcb_event.width = rect.width();
		xcb_event.height = rect.height();
		xcb_event.count = 0;
		xine_port_send_gui_data(m_video->port(), XINE_GUI_SEND_EXPOSE_EVENT, &xcb_event);
	}
	QPainter p(this);
	p.fillRect(rect(), Qt::black);
	QWidget::paintEvent(event);
}
#else
bool VideoWidget::x11Event(XEvent *event) {
	if (m_video->port()) {
		if (event->type == Expose && event->xexpose.count == 0)
			xine_port_send_gui_data(m_video->port(), XINE_GUI_SEND_EXPOSE_EVENT, event);
	}
	return false;
}
#endif

QSize VideoWidget::sizeHint() const {
	return m_video->widgetSizeHint();
}

}
