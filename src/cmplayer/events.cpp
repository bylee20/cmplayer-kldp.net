#include "events.hpp"
#include "videoframe.hpp"

VideoFrameEvent::VideoFrameEvent(const ::VideoFrame &frame)
: Event(VideoFrame), m_length(frame.length()), m_data(new uchar[m_length]) {
	memcpy(m_data, frame.data(), m_length);
}
