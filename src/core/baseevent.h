#ifndef BACKEND_BASEEVENT_H
#define BACKEND_BASEEVENT_H

#include <QtCore/QEvent>

namespace Core {

class BaseEvent : public QEvent {
public:
	enum Type {
		NewFrame = QEvent::User + 1,
		Duration,
		EndOfStream,
		UpdateInfo,
		UserType = QEvent::User + 100
	};
	BaseEvent(int type): QEvent(static_cast<QEvent::Type>(type)) {}
	int type() const {return static_cast<Type>(QEvent::type());}
};

class DurationEvent : public Core::BaseEvent {
	public:
		DurationEvent(int duration): BaseEvent(Duration), m_duration(duration) {}
		int duration() const {return m_duration;}
	private:
		int m_duration;
};

class EndOfStreamEvent : public Core::BaseEvent {
	public:
		EndOfStreamEvent(): BaseEvent(EndOfStream) {}
};

class UpdateInfoEvent : public Core::BaseEvent {
	public:
		UpdateInfoEvent(): BaseEvent(UpdateInfo) {}
};

}

#endif
