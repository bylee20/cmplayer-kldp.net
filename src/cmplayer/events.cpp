#include "events.hpp"
#include "videoframe.hpp"

VideoPrepareEvent::VideoPrepareEvent(const VideoFormat *format)
: Event(VideoPrepare) {
	m_format = (VideoFormat*)malloc(sizeof(VideoFormat));
	memcpy(m_format, format, sizeof(VideoFormat));
}

VideoPrepareEvent::~VideoPrepareEvent() {
	free(m_format);
}

VideoFrameEvent::VideoFrameEvent(const ::VideoFrame &frame)
: Event(VideoFrame), m_length(frame.length()), m_data(new uchar[m_length]) {
	memcpy(m_data, frame.data(), m_length);
}
