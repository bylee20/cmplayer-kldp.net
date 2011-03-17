#ifndef VIDEOFRAME_HPP
#define VIDEOFRAME_HPP
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QByteArray>
#include <QtCore/QSize>
#include <vlc/vlc.h>
#include <QtCore/QList>

#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
#define FOURCC( a, b, c, d ) \
	( ((quint32)d) | ( ((quint32)c) << 8 ) \
		| ( ((quint32)b) << 16 ) | ( ((quint32)a) << 24 ) )
#define TWOCC( a, b ) \
	( (quint32)(b) | ( (quint32)(a) << 8 ) )
#else
#define FOURCC( a, b, c, d ) \
	( ((quint32)a) | ( ((quint32)b) << 8 ) \
	   | ( ((quint32)c) << 16 ) | ( ((quint32)d) << 24 ) )
#define TWOCC( a, b ) \
	( (quint32)(a) | ( (quint32)(b) << 8 ) )
#endif

class VideoFrame {
public:
	// do not change the order!!
	struct Plane {
		uchar *data;
		int dataPitch, dataLines;
		int framePitch, frameLines;
	};
	enum Type {Unknown = 0, YV12, RV16};
	VideoFrame();
	VideoFrame(quint32 fourcc, int width, int height);
	~VideoFrame();
	void alloc(Plane *hints, int dataLength);
	void free();
	uchar *data() {return m_data;}
	const Plane &plane(int idx) const {return m_planes[idx];}
	const QSize &size() const {return m_size;}
	int length() const {return m_length;}
	int planeCount() const {return m_planeCount;}
	int width() const {return m_size.width();}
	int height() const {return m_size.height();}
	Type type() const {return m_type;}
	bool isEmpty() const {return m_length <= 0;}
	void copy(const VideoFrame &frame);
	void attach(const VideoFrame &frame);
private:
	VideoFrame &operator= (const VideoFrame &rhs);
	VideoFrame(const VideoFrame &rhs);
	Type m_type;
	QSize m_size;

	uchar *m_data;
	int m_length;
	Plane *m_planes;
	int m_planeCount;
};

class VideoFrameEvent : public QEvent {
public:
	static const int Id = QEvent::User + 1;
	VideoFrameEvent(const VideoFrame &frame)
	: QEvent((Type)Id) {
		m_frame = new VideoFrame(frame.type(), frame.width(), frame.height());
		m_frame->copy(frame);
	}
	const VideoFrame *frame() const {return m_frame;}
	VideoFrame *frame() {return m_frame;}
private:
	VideoFrame *m_frame;
};


#endif // VIDEOFRAME_HPP
