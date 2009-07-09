#include "image.h"
#include <QtCore/QRect>

namespace XVideo {

Image::Formats Image::formats;

bool Image::setVideoFrame(const Core::VideoFrame &frame) {
	if (m_port < 0 || !m_display)
		return false;
	const Format format = toXvFormat(frame.info().type);
	if (format == NoFormat || !formats.contains(format))
		return false;
	m_frame = frame;
	if (!m_xv || m_xv->id != format) {
		if (m_xv) free();
		m_xv = XvCreateImage(m_display, m_port, format, 0, 0, 0);
	}
	if (!m_xv)
		return false;
	if (m_xv->width != frame.info().size.width())
		m_xv->width = frame.info().size.width();
	if (m_xv->height != frame.info().size.height())
		m_xv->height = frame.info().size.height();
	m_frame = frame;
	m_xv->data = m_frame.data();
	m_xv->data_size = m_frame.size();
	return true;
}

void Image::put(Drawable drawable, GC gc, const QRect &r) {
	if (!m_xv)
		return;
	XvPutImage(m_display, m_port, drawable, gc, m_xv
		, 0, 0, m_xv->width, m_xv->height, r.x(), r.y(), r.width(), r.height());
}

}
