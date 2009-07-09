#ifndef XVIDEO_IMAGE_H
#define XVIDEO_IMAGE_H

#include <QtCore/QList>
#include <core/videoframe.h>

extern "C" {
#include <X11/Xlib.h>
#include <X11/extensions/Xvlib.h>
}

class QRect;

namespace Core {

}

namespace XVideo {

class Image {
public:
	enum Format {NoFormat = -1, XvRGB32 = 0x3, XvYUY2 = 0x32595559, XvI420 = 0x30323449, XvYV12 = 0x32315659};
	typedef QList<Format> Formats;
	Image(Display *display/* = 0*/, int port/* = -1*/): m_display(display), m_port(port), m_xv(0) {}
	~Image() {free();}
	bool setVideoFrame(const Core::VideoFrame &frame);
	int width() const {return m_frame.info().size.width();}
	int height() const {return m_frame.info().size.height();}
	const QSize &size() const {return m_frame.info().size;}
	bool isNull() const {return !m_xv || m_frame.info().size.isEmpty();}
	inline static void setAvailableFormats(const Formats &formats) {
		Image::formats = formats;
	}
	void put(Drawable drawable, GC gc, const QRect &r);
	XvImage *xv() {return m_xv;}
private:
	void free() {if (m_xv) {XFree(m_xv); m_xv = 0;}}
	static Format toXvFormat(Core::VideoFrame::Type type) {
		switch(type) {
		case Core::VideoFrame::I420:
			return XvI420;
		case Core::VideoFrame::YV12:
			return XvYV12;
		case Core::VideoFrame::YUY2:
			return XvYUY2;
		case Core::VideoFrame::RGB:
			return XvRGB32;
		default:
			return NoFormat;
		}
	}
	static Formats formats;
	Display *m_display;
	int m_port;
	XvImage *m_xv;
	QByteArray m_data;
	Core::VideoFrame m_frame;
};

}

#endif // IMAGE_H
