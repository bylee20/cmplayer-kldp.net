#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <QtCore/QEvent>
#include "videoframe.hpp"

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
	Type type() const {return static_cast<Type>(QEvent::type());}
};

class NewFrameEvent : public BaseEvent {
public:
	NewFrameEvent(const VideoFrame &frame): BaseEvent(NewFrame), m_frame(frame) {}
	const VideoFrame &frame() const {return m_frame;}
private:
	VideoFrame m_frame;
};

#endif // EVENTS_HPP
