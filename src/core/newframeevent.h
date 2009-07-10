#ifndef CORE_NEWFRAMEEVENT_H
#define CORE_NEWFRAMEEVENT_H

#include <QtCore/QList>
#include "videoframe.h"
#include "baseevent.h"

namespace Core {

class NewFrameEvent : public BaseEvent {
public:
	NewFrameEvent(const VideoFrame &frame): BaseEvent(NewFrame), m_frame(frame) {}
	const VideoFrame &frame() const {return m_frame;}
private:
	VideoFrame m_frame;
};
}

#endif
