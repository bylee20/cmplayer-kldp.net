#ifndef GST_EVENTS_H
#define GST_EVENTS_H

#include <QtCore/QEvent>
#include <gst/gst.h>
#include <core/baseevent.h>

namespace Gst {

enum EventType {
	BusCall = Core::BaseEvent::UserType + 1,
	StateChange,
	NotifyStreamInfo
};

class StateChangeEvent : public Core::BaseEvent {
public:
	StateChangeEvent(int state, int old)
	: BaseEvent(StateChange), m_state(state), m_old(old) {}
	int state() const {return m_state;}
	int oldState() const {return m_old;}
private:
	int m_state, m_old;
};

class NotifyStreamInfoEvent : public Core::BaseEvent {
public:
	NotifyStreamInfoEvent(): BaseEvent(NotifyStreamInfo) {}
};

}

#endif
