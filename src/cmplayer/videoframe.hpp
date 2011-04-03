#ifndef VIDEOFRAME_HPP
#define VIDEOFRAME_HPP

#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QByteArray>
#include <QtCore/QSize>
#include <QtCore/QList>
#include "avmisc.hpp"


class VideoFrame {
public:
	typedef FramePlane Plane;
	enum Type {Unknown = 0, I420, YV12, RV16};
	static QString fourccToString(uint32_t fourcc);
	VideoFrame();
	VideoFrame(const VideoFormat &format);
	~VideoFrame();
	uchar *data() const {return m_data;}
	uchar *data(int idx) const {return m_data + m_offset[idx];}
	int dataPitch(int idx = 0) const {return m_planes[idx].dataPitch;}
	int dataLines(int idx = 0) const {return m_planes[idx].dataLines;}
	int framePitch(int idx = 0) const {return m_planes[idx].framePitch;}
	int frameLines(int idx = 0) const {return m_planes[idx].frameLines;}
	int offset(int idx) const {return m_offset[idx];}
	const QSize &size() const {return m_size;}
	int length() const {return m_length;}
	int planeCount() const {return m_planeCount;}
	int width() const {return m_size.width();}
	int height() const {return m_size.height();}
	Type type() const {return m_type;}
	bool isEmpty() const {return m_length <= 0;}
	bool isPlanar() const {return m_type == I420 || m_type == YV12;}
	VideoFrame &operator= (const VideoFrame &rhs);
	VideoFrame(const VideoFrame &rhs);
	void copy(const VideoFrame &frame);
	void copyData(const VideoFrame &frame);
	void setData(uchar *data) {delete[] m_data; m_data = data;}
private:
	inline static int planeDataLength(const Plane &plane) {
		return plane.dataPitch*plane.dataLines;
	}
	Type m_type;
	QSize m_size;

	uchar *m_data;
	int m_length;
	Plane m_planes[VIDEO_FRAME_MAX_PLANE_COUNT];
	int m_offset[VIDEO_FRAME_MAX_PLANE_COUNT];
	int m_planeCount;
};

#endif // VIDEOFRAME_HPP
