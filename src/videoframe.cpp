#include "videoframe.hpp"

//static inline void fourcc_to_char(quint32 fcc, char *psz_fourcc ) {
//	memcpy( psz_fourcc, &fcc, 4 );
//}

VideoFrame::VideoFrame(quint32 fourcc, int width, int height)
: m_size(width, height) {
	switch (fourcc) {
	case FOURCC('Y', 'V', '1', '2'):
		m_type = YV12;
		m_data.resize(width*height*3/2);
		m_offset << 0 << width*height << width*height*5/4;
		break;
	case FOURCC('R', 'V', '1', '6'):
		m_type = RV16;
		m_data.resize(width*height*2);
		m_offset << 0;
	default:
		m_type = Unknown;
		break;
	}
}
