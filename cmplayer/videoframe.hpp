#ifndef VIDEOFRAME_HPP
#define VIDEOFRAME_HPP

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
	enum Type {Unknown = 0, YV12, RV16};
	VideoFrame() {m_type = Unknown;}
	VideoFrame(quint32 fourcc, int width, int height);
	uchar *data() {return (uchar*)m_data.data();}
	const uchar *constData() const {return (const uchar*)m_data.constData();}
	const QSize &size() const {return m_size;}
	int length() const {return m_data.length();}
	int offset(int idx) {return m_offset[idx];}
	int planeCount() const {return m_offset.size();}
	uchar *plane(int idx) {return data() + offset(idx);}
	int width() const {return m_size.width();}
	int height() const {return m_size.height();}
	Type type() const {return m_type;}
	bool isEmpty() const {return m_size.isEmpty() || m_size.isNull();}
private:
	QByteArray m_data;
	Type m_type;
	QSize m_size;
	QList<int> m_offset;
};

class VideoFrameEvent : public QEvent {
public:
	static const int Id = QEvent::User + 1;
	VideoFrameEvent(const VideoFrame &frame)
	: QEvent((Type)Id), m_frame(frame) {
	}
	const VideoFrame &frame() const {return m_frame;}
private:
	VideoFrame m_frame;
};


#endif // VIDEOFRAME_HPP
