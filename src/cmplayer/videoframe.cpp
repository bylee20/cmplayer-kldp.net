#include "videoframe.hpp"
#include <stdlib.h>

//static inline void fourcc_to_char(quint32 fcc, char *psz_fourcc ) {
//	memcpy( psz_fourcc, &fcc, 4 );
//}

VideoFrame::VideoFrame()
: m_type(Unknown), m_data(0), m_length(0), m_planes(0), m_planeCount(0) {}

VideoFrame::VideoFrame(quint32 fourcc, int width, int height)
: m_type(Unknown), m_size(width, height), m_data(0), m_length(0), m_planes(0), m_planeCount(0) {
	switch (fourcc) {
	case FOURCC('I', '4', '2', '0'):
	case FOURCC('Y', 'V', '1', '2'):
		m_type = YV12;
		m_planeCount = 3;
		break;
	case FOURCC('R', 'V', '1', '6'):
		m_type = RV16;
		m_planeCount = 1;
		break;
	default:
		m_type = Unknown;
		break;
	}
	if (m_planeCount > 0) {
		m_planes = reinterpret_cast<Plane*>(malloc(sizeof(Plane)*m_planeCount));
		memset(m_planes, 0, sizeof(Plane)*m_planeCount);
	}
}

VideoFrame::~VideoFrame() {

}

void VideoFrame::free() {
	if (m_planes)
		::free(m_planes);
	if (m_data)
		::free(m_data);
}

void VideoFrame::alloc(Plane *hints, int dataLength) {
	if (dataLength != m_length) {
		m_length = dataLength;
		::free(m_data);
		m_data = reinterpret_cast<uchar*>(malloc(dataLength));
	}
	uchar *buffer = m_data;
	for (int i=0; i<m_planeCount; ++i) {
		hints[i].data = buffer;
		m_planes[i] = hints[i];
		buffer += hints[i].dataPitch*hints[i].dataLines;
	}
}

void VideoFrame::copy(const VideoFrame &frame) {
	m_type = frame.m_type;
	m_size = frame.m_size;
	m_length = frame.m_length;
	m_planeCount = frame.m_planeCount;

	m_data = (uchar*)malloc(m_length);
	memcpy(m_data, frame.m_data, m_length);
	m_planes = (Plane*)malloc(m_planeCount*sizeof(Plane));
	memcpy(m_planes, frame.m_planes, sizeof(Plane)*m_planeCount);
}

void VideoFrame::attach(const VideoFrame &frame) {
	m_type = frame.m_type;
	m_size = frame.m_size;
	m_length = frame.m_length;
	m_planeCount = m_planeCount;

	m_data = frame.m_data;
	m_planes = frame.m_planes;
}
