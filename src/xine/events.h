#ifndef XINE_EVENTS_H
#define XINE_EVENTS_H

#include <core/baseevent.h>
#include <QtCore/QEvent>

namespace Xine {

enum EventType {
	BusCall = Core::BaseEvent::UserType + 1,
	SetCursor,
	NewOverlay
};

class SetCursorEvent : public Core::BaseEvent {
public:
	SetCursorEvent(Qt::CursorShape shape): BaseEvent(SetCursor), m_shape(shape) {}
	Qt::CursorShape shape() const {return m_shape;}
private:
	Qt::CursorShape m_shape;
};

}

#endif
