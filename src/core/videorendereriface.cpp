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

}
