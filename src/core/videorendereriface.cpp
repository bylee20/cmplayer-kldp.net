#include "videorendereriface.h"
#include "videoframe.h"
#include <QtGui/QWidget>
#include <QtCore/QPointF>
#include <QtGui/QResizeEvent>
#include <cmath>

namespace Core {

VideoRendererIface::VideoRendererIface(QWidget *target) {
	m_crop = m_aspect = -1.0;
	m_videoRatio = m_aspectF = 1.0;
	m_target = target;
	m_target->setMouseTracking(true);
}

VideoRendererIface::~VideoRendererIface() {}

void VideoRendererIface::rerender() {
	m_target->update();
}

QSize VideoRendererIface::widgetSizeHint() const {
	double k = (m_crop > 0.0) ? m_crop : ((m_aspect > 0.0) ? m_aspect : -1.0);
	if (k > 0.0) {
		const double height = std::sqrt(m_videoSize.width() * m_videoSize.height() / k);
		return QSize(k*height, height);
	} else
		return m_videoSize;
}

void VideoRendererIface::updateVideoSize(const QSize &size) {
	if (size != m_videoSize) {
		m_videoRatio = ratio(m_videoSize = size);
		m_aspectF = m_aspect > 0. ? m_aspect : m_videoRatio;
		calculate();
	}
}

void VideoRendererIface::updateFrameInfo(const VideoFrame::Info &info, bool setVideo) {
	if (m_frame == info)
		return;
	m_frame = info;
	if (setVideo) {
		QSize size = info.size;
		if (info.pixelAspectRatio > 1.)
			size.rwidth() *= info.pixelAspectRatio;
		else
			size.rheight() /= info.pixelAspectRatio;
		updateVideoSize(size);
	} else
		calculate();

}

void VideoRendererIface::setAspectRatio(double ratio) {
	if (qAbs(m_aspect - ratio) > 1.e-5) {
		m_aspect = ratio;
		m_aspectF = m_aspect > 0. ? m_aspect : m_videoRatio;
		calculate();
		rerender();
	}
}

void VideoRendererIface::setCropRatio(double ratio) {
	if (qAbs(m_crop - ratio) > 1.e-5) {
		m_crop = ratio;
		calculate();
		rerender();
	}
}

QImage VideoRendererIface::grabCurrentFrame() const {
	return QImage();
}

QImage VideoRendererIface::imageFromYuy2(const QByteArray &yuy2, int width, int height) {
	return imageFromYv12(yuy2ToYv12(yuy2, width, height), width, height);
}

QImage VideoRendererIface::imageFromYv12(const QByteArray &yv12, int width, int height) {
	const QByteArray rgb = yv12ToRgb(yv12, width, height);
	return QImage(reinterpret_cast<const uchar*>(rgb.data()), width, height
			, QImage::Format_RGB32).copy();
}

/************************************************************
 *   Helpers to convert yuy and yv12 frames to rgb          *
 *   code from gxine modified for 32bit output              *
 *   Copyright (C) 2000-2003 the xine project               *
 ************************************************************/

QByteArray VideoRendererIface::yuy2ToYv12(const QByteArray &yuy2, int width, int height) {
	QByteArray yv12(width*height*2, '\0');
	const uint8_t *buffer = reinterpret_cast<const uint8_t*>(yuy2.data());
	uint8_t *y = reinterpret_cast<uint8_t*>(yv12.data());
	uint8_t *u = y + width*height;
	uint8_t *v = y + width*height*5/4;
	const int w2 = width/2;
	for (int i=0; i<height; i +=2 ) {
		for (int j=0; j<w2; ++j) {
			// packed YUV 422 is: Y[i] U[i] Y[i+1] V[i]
			*(y++) = *(buffer++);
			*(u++) = *(buffer++);
			*(y++) = *(buffer++);
			*(v++) = *(buffer++);
		}
		// down sampling
		for (int j=0; j<w2; ++j) {
			// skip every second line for U and V
			*(y++) = *(buffer++);
			++buffer;
			*(y++) = *(buffer++);
			++buffer;
		}
	}
	return yv12;
}

QByteArray VideoRendererIface::yv12ToRgb(const QByteArray &yv12, int width, int height) {
	// Create rgb data from yv12
	const uint8_t *src_y = reinterpret_cast<const uint8_t*>(yv12.data());
	const uint8_t *src_u = src_y + width*height;
	const uint8_t *src_v = src_y + width*height*5/4;
	const int uv_width  = width / 2;
	const int uv_height = height / 2;
	QByteArray rgb(width*height*4, '\0');
	uint8_t *pRgb = reinterpret_cast<uint8_t*>(rgb.data());
	for (int i=0; i<height; ++i) {
		// calculate u & v rows
		const int sub_i_uv = ((i * uv_height) / height);
		for (int j = 0; j < width; ++j) {
			// calculate u & v columns
			const int sub_j_uv = ((j * uv_width) / width);
			
			/***************************************************
			*
			*  Colour conversion from
			*    http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
			*
			*  Thanks to Billy Biggs <vektor@dumbterm.net>
			*  for the pointer and the following conversion.
			*
			*   R' = [ 1.1644         0    1.5960 ]   ([ Y' ]   [  16 ])
			*   G' = [ 1.1644   -0.3918   -0.8130 ] * ([ Cb ] - [ 128 ])
			*   B' = [ 1.1644    2.0172         0 ]   ([ Cr ]   [ 128 ])
			*
			*  Where in xine the above values are represented as
			*
			*   Y' == image->y
			*   Cb == image->u
			*   Cr == image->v
			*
			***************************************************/

			const int y = src_y[(i * width) + j] - 16;
			const int u = src_u[(sub_i_uv * uv_width) + sub_j_uv] - 128;
			const int v = src_v[(sub_i_uv * uv_width) + sub_j_uv] - 128;

			const int r = qBound(0, (int)((1.1644 * (double)y) + (1.5960 * (double)v)), 255);
			const int g = qBound(0, (int)((1.1644 * (double)y) - (0.3918 * (double)u) - (0.8130 * (double)v)), 255);
			const int b = qBound(0, (int)((1.1644 * (double)y) + (2.0172 * (double)u)), 255);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
			pRgb[(i * width + j) * 4 + 0] = b;
			pRgb[(i * width + j) * 4 + 1] = g;
			pRgb[(i * width + j) * 4 + 2] = r;
			pRgb[(i * width + j) * 4 + 3] = 0;
#else
			pRgb[(i * width + j) * 4 + 3] = b;
			pRgb[(i * width + j) * 4 + 2] = g;
			pRgb[(i * width + j) * 4 + 1] = r;
			pRgb[(i * width + j) * 4 + 0] = 0;
#endif
		}
	}
	return rgb;
}

}
