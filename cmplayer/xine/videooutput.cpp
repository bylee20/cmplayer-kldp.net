#include "utility.h"
#include "xinepost.h"
#include "videooutput.h"
#include "xinestream.h"
#include "videowidget.h"
#include <QResizeEvent>
#include <QPaintEvent>
#include <QWidget>
#include <QPainter>

namespace Xine {

VideoOutput::VideoOutput(XineStream *stream) {
	m_stream = stream;
	m_port = 0;
	m_driver = "auto";
	m_ar = AR_Auto;
	m_cr = CR_Off;
	m_widget = new QWidget;
	m_visual = new QWidget(m_widget);
	m_visual->move(0, 0);
	m_widget->setMouseTracking(true);
	m_video = new VideoWidget(this, m_visual);
	m_widget->setMouseTracking(true);
	m_video->setMouseTracking(true);
	m_visual->setMouseTracking(true);
	m_video->move(0, 0);

	m_video->setAutoFillBackground(true);
	m_video->setAttribute(Qt::WA_NoSystemBackground);
	m_video->setAttribute(Qt::WA_StaticContents);
	m_video->setAttribute(Qt::WA_PaintOnScreen);
	m_video->setAttribute(Qt::WA_PaintUnclipped);
	m_widget->installEventFilter(this);
	m_widget->resize(400, 300);
	m_widget->setMinimumSize(40, 30);
	m_video->setMinimumSize(10, 10);
	m_visual->setMinimumSize(10, 10);
	m_expanded = false;
	m_videoSize.scale(10, 10, Qt::IgnoreAspectRatio);
}

bool VideoOutput::eventFilter(QObject *obj, QEvent *event) {
	if (obj == m_widget) {
		QEvent::Type type = event->type();
		switch(type) {
		case QEvent::Resize: {
			QSize size = static_cast<QResizeEvent*>(event)->size();
			updateVisual();
			emit widgetResized(size);
			return false;
		} case QEvent::Paint: {
			QPainter p(m_widget);
			p.fillRect(m_widget->rect(), Qt::black);
			return true;
		} default:
			return false;
		}
	}
	return false;
}

void VideoOutput::setDriver(const QString &driver) {
	if (m_driver == driver)
		return;
	m_driver = driver;
	if (m_stream->isOpen()) {
		m_stream->close();
		m_stream->open();
	}
}

void VideoOutput::setFullScreen(bool fs) {
	if (fs == m_fullScreen)
		return;
	if (m_fullScreen = fs) {
		m_visual->resize(Utility::desktopSize());
		m_video->resize(Utility::desktopSize());
		m_visual->move(0, 0);
		m_video->move(0, 0);
		emit sizeUpdated();
	} else
		updateVisual();
}

QSize VideoOutput::widgetSizeHint() const {
	QSize hint = m_videoSize;
	switch (m_ar) {
	case AR_4_3:
		hint.setWidth(hint.height()*(4.0/3.0));
		break;
	case AR_16_9:
		hint.setWidth(hint.height()*(16.0/9.0));
		break;
	case AR_211_100:
		hint.setWidth(hint.height()*(2.11/1.0));
		break;
	default:
		break;
	}
	switch (m_cr) {
	case CR_4_3:
		hint.setHeight(hint.width()/(4.0/3.0));
		break;
	case CR_16_9:
		hint.setHeight(hint.width()/(16.0/9.0));
		break;
	case CR_211_100:
		hint.setHeight(hint.width()/(2.11/1.0));
		break;
	default:
		break;
	}
	return hint;
}

void VideoOutput::setBrightness(int value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_BRIGHTNESS, qBound(0, value, 65535));
}

void VideoOutput::setContrast(int value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_CONTRAST, qBound(0, value, 65535));
}

void VideoOutput::setHue(int value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_HUE, qBound(0, value, 65535));
}

void VideoOutput::setSaturation(int value) {
	if(m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_SATURATION, qBound(0, value, 65535));
}

int VideoOutput::brightness() const {
	return m_stream->isOpen() ? xine_get_param(m_stream->stream(), XINE_PARAM_VO_BRIGHTNESS) : 32768;
}

int VideoOutput::contrast() const {
	return m_stream->isOpen() ? xine_get_param(m_stream->stream(), XINE_PARAM_VO_CONTRAST) : 32768;
}

int VideoOutput::hue() const {
	return m_stream->isOpen() ? xine_get_param(m_stream->stream(), XINE_PARAM_VO_HUE) : 32768;
}

int VideoOutput::saturation() const {
	return m_stream->isOpen() ? xine_get_param(m_stream->stream(), XINE_PARAM_VO_SATURATION) : 32768;
}

void *VideoOutput::visual() {
	return &m_video->m_visual;
}

void VideoOutput::updateVisual() {
	if (m_fullScreen)
		return;
	QSizeF widget = m_widget->size();
	QSizeF visual(1.0, 1.0), video(1.0, 1.0);
	if (isExpanded()) {
		video.setWidth(Utility::desktopRatio());
		visual.setWidth(ratio(m_videoSize));
		visual.scale(widget, Qt::KeepAspectRatio);
		video.scale(visual, Qt::KeepAspectRatioByExpanding);
	} else {
		if (m_cr != CR_Off) {
			visual = widgetSizeHint();
			video = m_videoSize;
			visual.scale(widget, Qt::KeepAspectRatio);
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
		} else
			video = visual = widget;
	}
	m_visual->resize(visual.toSize());
	m_video->resize(video.toSize());
	QPoint pos;
	pos.setX((widget.width() - visual.width())/2);
	pos.setY((widget.height() - visual.height())/2);
	m_visual->move(pos);
	pos.setX((visual.width() - video.width())/2);
	pos.setY((visual.height() - video.height())/2);
	m_video->move(pos);
	emit sizeUpdated();
}

void VideoOutput::setAspectRatio(AspectRatio ar) {
	int val = XINE_VO_ASPECT_AUTO;
	m_ar = ar;
	switch (ar) {
	case AR_4_3:
		val = XINE_VO_ASPECT_4_3;
		break;
	case AR_16_9:
		val = XINE_VO_ASPECT_ANAMORPHIC;
		break;
	case AR_211_100:
		val = XINE_VO_ASPECT_DVB;
		break;
	default:
		val = XINE_VO_ASPECT_AUTO;
		m_ar = AR_Auto;
		break;
	}
	if (!isExpanded())
		xine_set_param(m_stream->stream(), XINE_PARAM_VO_ASPECT_RATIO, val);
	else
		updateVisual();
}

void VideoOutput::crop(CropRatio cr) {
	m_cr = cr;
	updateVisual();
}

QSize VideoOutput::visualSize(bool scaled) const {
	if (scaled) {
		if (m_cr != CR_Off) {
			return widgetSizeHint();
		} else {
			QSize size = m_visual->size();
			size.scale(m_videoSize, Qt::KeepAspectRatio);
			return size;
		}
	} else {
		if (m_cr != CR_Off) {
			QSize size = widgetSizeHint();
			size.scale(m_video->size(), Qt::KeepAspectRatio);
			return size;
		} else {
			return m_visual->size();
		}
	}
}

QRect VideoOutput::osdRect(bool forScaled) const {
	if (forScaled && !isExpanded())
		return QRect(QPoint(0, 0), m_videoSize);
	QPoint pos = m_video->mapTo(m_visual, QPoint(0, 0));
	QSize size(qMin(m_visual->width(), m_video->width()), qMin(m_visual->height(), m_video->height()));
	if (pos.y() < 0)
		pos.setY(-pos.y());
	if (forScaled) {
		QSizeF video = m_video->size();
		double videoRatio = ratio(m_videoSize);
		static const double desktopRatio = Utility::desktopRatio();
		if (videoRatio > desktopRatio)
			video.setHeight(video.height()/(videoRatio/desktopRatio));
		else
			video.setWidth(video.width()*videoRatio/desktopRatio);
		double scale = video.width()/m_videoSize.width();
		pos /= scale;
		size /= scale;
	}
	return QRect(pos, size);
}

int VideoOutput::visualBottom() const {
	return m_visual->height() - m_video->y();
}

void VideoOutput::updateSizeInfo() {
	if (!m_stream->hasVideo())
		return;
	QSize size;
	size.setWidth(xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_VIDEO_WIDTH));
	size.setHeight(xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_VIDEO_HEIGHT));
	updateSizeInfo(size);
}

void VideoOutput::updateSizeInfo(const QSize &size) {
	if (m_videoSize != size) {
		m_videoSize = size;
		emit widgetSizeHintChanged(widgetSizeHint());
	}
}

void VideoOutput::expand(bool expanded) {
	if (m_expanded == expanded)
		return;
	if (m_expanded = expanded) {
		XinePost *post = m_stream->addVideoPost("expand");
		if (post) {
			XinePost::DoubleParam *param
					= static_cast<XinePost::DoubleParam*>(post->parameters()->value("aspect"));
			if (!(m_expanded = param != 0))
				return;
			param->setValue(Utility::desktopRatio());
		} else
			m_expanded = false;
	} else
		m_stream->removeVideoPost("expand");
	updateVisual();
}

}
