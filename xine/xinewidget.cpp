#include <QApplication>
#include <QX11Info>
#include <QPaintEvent>
#include <QDir>
#include "xinewidget.h"
#include <QPainter>
#include "xineengine.h"

namespace Xine {

XineWidget::XineWidget(XineEngine *engine, const QString &videoDriver, QWidget *parent)
: QWidget(parent) {
	QPalette palette = this->palette();
	palette.setColor(QPalette::Background, QColor(Qt::black));
	setPalette(palette);
	setUpdatesEnabled(false);
	setMouseTracking(true);

	m_engine = engine;
	m_valid = false;
	m_videoPort = 0;
	m_connection = 0;
	if (!m_engine->xine())
		return;

	int screen_nbr = 0;
	m_connection = xcb_connect(NULL, &screen_nbr);
	if (m_connection) {
		m_visual.connection = m_connection;
		xcb_screen_iterator_t screenIt = xcb_setup_roots_iterator(xcb_get_setup(m_connection));
		while ((screenIt.rem > 1) && (screen_nbr > 0)) {
			xcb_screen_next(&screenIt);
			--screen_nbr;
		}
		m_visual.screen = screenIt.data;
		m_visual.window = winId();
		m_visual.user_data = this;
		m_visual.dest_size_cb = cbDestSize;
		m_visual.frame_output_cb = cbFrameOutput;
		// make sure all Qt<->X communication is done, else  will crash
		QApplication::syncX();
		m_videoPort = xine_open_video_driver(m_engine->xine(), videoDriver.toLatin1(),
			XINE_VISUAL_TYPE_XCB, static_cast<void *>(&m_visual));
    }
	m_valid = m_connection && m_videoPort;
}

XineWidget::~XineWidget() {
	if (m_videoPort)
		xine_close_video_driver(m_engine->xine(), m_videoPort);
	if (m_connection)
		xcb_disconnect(m_connection);
}

void XineWidget::cbDestSize(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_width, int *dest_height, double *dest_pixel_aspect) {
	XineWidget *w = reinterpret_cast<XineWidget*>(user_data);
	*dest_width = w->width();
	*dest_height = w->height();
	*dest_pixel_aspect = 1;
}

void XineWidget::cbFrameOutput(void *user_data, int /*video_width*/, int /*video_height*/,
		double /*video_pixel_aspect*/, int *dest_x, int *dest_y, int *dest_width, int *dest_height,
		double *dest_pixel_aspect, int *win_x, int *win_y) {
	XineWidget *w = reinterpret_cast<XineWidget*>(user_data);
	*dest_x = 0;
	*dest_y = 0;
	*win_x = w->x();
	*win_y = w->y();
	*dest_width = w->width();
	*dest_height = w->height();
	*dest_pixel_aspect = 1;
}


void XineWidget::paintEvent(QPaintEvent *event) {
	if (m_engine->isInitialized()) {
		const QRect &rect = event->rect();

		xcb_expose_event_t xcb_event;
		memset(&xcb_event, 0, sizeof(xcb_event));

		xcb_event.window = winId();
		xcb_event.x = rect.x();
		xcb_event.y = rect.y();
		xcb_event.width = rect.width();
		xcb_event.height = rect.height();
		xcb_event.count = 0;

		xine_port_send_gui_data(m_videoPort, XINE_GUI_SEND_EXPOSE_EVENT, &xcb_event);
	} else {
		QPainter p(this);
		p.fillRect(rect(), Qt::black);
    }
	QWidget::paintEvent(event);
}

}
