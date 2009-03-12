#include "xinevideoiface.h"
#include "xinestream.h"
#include <QtCore/QPoint>

namespace Xine {

void XineVideoIface::mouseMoved(const QPoint &pos) {
	if (!m_tracking || !m_stream->videoPort)
		return;
	x11_rectangle_t rect;
	rect.x = pos.x();
	rect.y = pos.y();
	rect.w = 0;
	rect.h = 0;
	xine_port_send_gui_data(m_stream->videoPort, XINE_GUI_SEND_TRANSLATE_GUI_TO_VIDEO, &rect);
	if (m_stream->stream) {
		xine_input_data_t input;
		input.button = 0;
		input.x = rect.x;
		input.y = rect.y;
		xine_event_t event;
		event.type = XINE_EVENT_INPUT_MOUSE_MOVE;
		event.data = &input;
		event.data_length = sizeof(input);
		xine_event_send(m_stream->stream, &event);
	}
}

void XineVideoIface::mouseClicked(const QPoint &pos, int button) {
	uint8_t b = 0;
	switch (button) {
	case Qt::RightButton:
		++b;
	case Qt::MidButton:
		++b;
	case Qt::LeftButton:
		++b;
		break;
	default:
		b = 0;
	}
	if (!b || !m_tracking || !m_stream->videoPort)
		return;
	x11_rectangle_t rect;
	rect.x = pos.x();
	rect.y = pos.y();
	rect.w = 0;
	rect.h = 0;
	xine_port_send_gui_data(m_stream->videoPort, XINE_GUI_SEND_TRANSLATE_GUI_TO_VIDEO, &rect);
	if (m_stream->stream) {
		xine_input_data_t input;
		input.button = b;
		input.x = rect.x;
		input.y = rect.y;
		xine_event_t event;
		event.type = XINE_EVENT_INPUT_MOUSE_BUTTON;
		event.data = &input;
		event.data_length = sizeof(input);
		xine_event_send(stream()->stream, &event);
	}
}

}
