#include "xcbnativerenderer.h"
#include <QtGui/QApplication>

namespace Xine {

XcbNativeRenderer::XcbNativeRenderer(Core::PlayEngine *engine, Xine::XineStream *stream)
: NativeRenderer(engine, stream) {
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
	m_visual.window = screenWinId();
	m_visual.user_data = this;
	m_visual.dest_size_cb = cbDestSize;
	m_visual.frame_output_cb = cbFrameOutput;
	QApplication::syncX();
}

XcbNativeRenderer::~XcbNativeRenderer() {
	if (m_connection)
		xcb_disconnect(m_connection);
}

int XcbNativeRenderer::xineType() const {
	return XINE_VISUAL_TYPE_XCB;
}

void *XcbNativeRenderer::visual() {
	return &m_visual;
}

bool XcbNativeRenderer::eventFilter(QObject *obj, QEvent *event) {
	if (obj == screen()) {
		QWidget *screen = this->screen();
		if (event->type() == QEvent::Paint) {
			if (!engine()->isStopped()) {
				const QRect rect = screen->rect();
				xcb_expose_event_t xcb_event;
				memset(&xcb_event, 0, sizeof(xcb_event));
				xcb_event.window = screen->winId();
				xcb_event.x = rect.x();
				xcb_event.y = rect.y();
				xcb_event.width = rect.width();
				xcb_event.height = rect.height();
				xcb_event.count = 0;
				xine_port_send_gui_data(stream()->videoPort
						, XINE_GUI_SEND_EXPOSE_EVENT, &xcb_event);
				return true;
			}
		}
	}
	return ::Xine::NativeRenderer::eventFilter(obj, event);
}

}
