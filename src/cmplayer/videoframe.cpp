#include "videoframe.hpp"
#include <stdlib.h>

QString VideoFrame::fourccToString(quint32 fcc) {
	char str[5] = {0};
	memcpy(str, &fcc, 4);
	return QString::fromAscii(str);
}

VideoFrame::VideoFrame()
: m_type(Unknown), m_data(0), m_length(0), m_planeCount(0) {}

VideoFrame::VideoFrame(const VideoFormat &format) {
	m_size = QSize(format.width, format.height);
	m_planeCount = format.planeCount;
	switch (format.fourcc) {
	case FOURCC('Y', 'V', '1', '2'):
		m_type = YV12;
		break;
	case FOURCC('I', '4', '2', '0'):
		m_type = I420;
		break;
	case FOURCC('Y', 'U', 'Y', '2'):
	case FOURCC('Y', 'U', 'Y', 'V'):
		m_type = YUY2;
		break;
	case FOURCC('R', 'V', '1', '6'):
		m_type = RV16;
		break;
	default:
		m_type = Unknown;
		break;
	}
	m_length = 0;
	if (m_planeCount > 0) {
		for (int i=0; i<m_planeCount; ++i) {
			m_planes[i] = format.planes[i];
			m_offset[i] = m_length;
			m_length += planeDataLength(m_planes[i]);
		}
		if (m_length > 0)
			m_data = new uchar[m_length];
	}
}

VideoFrame::~VideoFrame() {
	delete [] m_data;
}

void VideoFrame::copy(const VideoFrame &frame) {
	m_type = frame.m_type;
	m_size = frame.m_size;
	m_planeCount = frame.m_planeCount;
	for (int i=0; i<m_planeCount; ++i) {
		m_planes[i] = frame.m_planes[i];
		m_offset[i] = frame.m_offset[i];
	}
	if (m_length != frame.m_length) {
		delete [] m_data;
		m_length = frame.m_length;
		m_data = new uchar[m_length];
	}
	copyData(frame);
}

void VideoFrame::copyData(const VideoFrame &frame) {
	Q_ASSERT(m_length == frame.m_length);
	memcpy(m_data, frame.m_data, m_length);
}

VideoFrame &VideoFrame::operator= (const VideoFrame &rhs) {
	if (this != &rhs)
		copy(rhs);
	return *this;
}

VideoFrame::VideoFrame(const VideoFrame &rhs)
: m_type(Unknown), m_data(0), m_length(0), m_planeCount(0) {
	copy(rhs);
}

