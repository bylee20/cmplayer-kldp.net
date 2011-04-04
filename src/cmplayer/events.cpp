#include "events.hpp"
#include "videoframe.hpp"
#include <QtCore/QDebug>

VideoPrepareEvent::VideoPrepareEvent(const VideoFormat *format)
: Event(VideoPrepare) {
	m_format = new VideoFormat;
	m_format->fourcc = format->fourcc;
	m_format->fps = format->fps;
	m_format->height = format->height;
	m_format->planeCount = format->planeCount;
	m_format->sar = format->sar;
	m_format->width = format->width;
	for (int i=0; i<m_format->planeCount; ++i) {
		m_format->planes[i] = format->planes[i];
	}
}

VideoPrepareEvent::~VideoPrepareEvent() {
	delete m_format;
}

VideoFrameEvent::VideoFrameEvent(const ::VideoFrame &frame)
: Event(VideoFrame), m_length(frame.length()), m_data(new uchar[m_length]) {
	memcpy(m_data, frame.data(), m_length);
}
