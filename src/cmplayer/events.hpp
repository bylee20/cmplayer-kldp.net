#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <QtCore/QEvent>

class Event : public QEvent {
public:
	enum Type {Reopen = QEvent::User + 1, VideoFrame, };
	Event(Type type): QEvent(static_cast<QEvent::Type>(type)) {}
	Type type() const {return static_cast<Type>(QEvent::type());}
};

class ReopenEvent: public Event {
public:
	ReopenEvent(): Event(Reopen) {}
};

class VideoFrame;

class VideoFrameEvent : public Event {
public:
	VideoFrameEvent(const ::VideoFrame &frame);
	uchar *data() const {return m_data;}
	int length() const {return m_length;}
private:
	int m_length;
	uchar *m_data;
};

#endif // EVENTS_HPP
