#include "utility.h"
#include "videooutput.h"
#include <QtGui/QResizeEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include "private/videowidget.h"
#include "playengine.h"
#include <QtCore/QDebug>

namespace Backend {

VideoOutput::VideoOutput(PlayEngine *engine)
: AVOutput(engine) {
	m_aspectRatio = AspectRatioAuto;
	m_cropRatio = CropOff;
	m_widget = 0;
	
	QPalette p;
	p.setColor(QPalette::Window, Qt::black);
	p.setColor(QPalette::Button, Qt::black);
	m_widget = new VideoWidget(this);
	m_visual = new QWidget(m_widget);
	m_visual->move(0, 0);
	m_widget->setMouseTracking(true);
	m_video = new QWidget(m_visual);
	m_widget->setMouseTracking(true);
	m_video->setMouseTracking(true);
	m_visual->setMouseTracking(true);
	m_video->move(0, 0);

	m_video->setAutoFillBackground(true);
	m_widget->setAutoFillBackground(true);
	m_visual->setAutoFillBackground(true);
// 	m_video->setAttribute(Qt::WA_NoSystemBackground);
	m_video->setAttribute(Qt::WA_StaticContents);
	m_video->setAttribute(Qt::WA_PaintOnScreen);
	m_video->setAttribute(Qt::WA_PaintUnclipped);
	m_widget->resize(400, 300);
	m_video->resize(400, 300);
	m_visual->resize(400, 300);
	m_widget->setMinimumSize(100, 0);
	m_video->setMinimumSize(10, 10);
	m_visual->setMinimumSize(10, 10);
	
	m_widget->setPalette(p);
	m_visual->setPalette(p);
	m_video->setPalette(p);
	m_expanded = false;
	updateVideoSize(QSize(10, 10));
	connect(engine, SIGNAL(started()), this, SLOT(update()));
}

void VideoOutput::setFullScreen(bool fs) {
	if (fs == m_fullScreen)
		return;
	m_fullScreen = fs;
	updateVideo();
}

// QRectF VideoOutput::videoRect() const {
// 	if (!isExpanded())
// 		return QRect(QPoint(0, 0), videoSize());
// 	QWidget *w = internalWidget();
// 	QRectF rect(QPointF(0.0, 0.0), w->size());
// 	if (videoRatio() > Utility::desktopRatio())
// 		rect.setHeight(rect.height()/videoOverDesktop());
// 	else
// 		rect.setWidth(rect.width()*videoOverDesktop());
// 	rect.moveTopLeft(QPoint((w->width()-rect.width())/2, (w->height()-rect.height())/2));
// 	return rect;
// }

// QRect VideoOutput::osdRect(bool forScaled) const {
// 	if (forScaled && !isExpanded())
// 		return QRect(QPoint(0, 0), widgetSizeHint());
// 	QPoint pos = internalWidget()->mapTo(visualWidget(), QPoint(0, 0));
// 	QSize size(qMin(visualWidget()->width(), internalWidget()->width()), qMin(visualWidget()->height(), internalWidget()->height()));
// 	if (pos.y() < 0)
// 		pos.setY(-pos.y());
// 	if (pos.x() < 0)
// 		pos.setX(-pos.x());
// 	if (forScaled) {
// 		const double scale = videoRect().width()/widgetSizeHint().width();
// 		pos /= scale;
// 		size /= scale;
// 	}
// 	return QRect(pos, size);
// }

QSize VideoOutput::widgetSizeHint() const {
	QSize hint = m_videoSize;
	if (m_aspectRatio > 0.0)
		hint.setWidth(hint.height()*m_aspectRatio);
	if (m_cropRatio > 0.0)
		hint.setHeight(hint.width()/m_cropRatio);
	return hint;
}

void VideoOutput::setBrightness(double value) {
	value = qBound(-1.0, value, 1.0);
	if (!isSame(value, m_brightness)) {
		updateBrightness(value);
		m_brightness = value;
	}
}

void VideoOutput::setContrast(double value) {
	value = qBound(-1.0, value, 1.0);
	if (!isSame(value, m_contrast)) {
		updateContrast(value);
		m_contrast = value;
	}
}

void VideoOutput::setHue(double value) {
	value = qBound(-1.0, value, 1.0);
	if (!isSame(value, m_hue)) {
		updateHue(value);
		m_hue = value;
	}
}

void VideoOutput::setSaturation(double value) {
	value = qBound(-1.0, value, 1.0);
	if (!isSame(value, m_saturation)) {
		updateSaturation(value);
		m_saturation = value;
	}
}

void VideoOutput::updateVideo() {
	QSizeF widget = m_fullScreen ? Utility::desktopSize() : m_widget->size();
	QSizeF visual(1.0, 1.0), video(1.0, 1.0);
	const double aspect = aspectRatioF();
	const double widgetRatio = ratio(widget);
	static const double desktopRatio = Utility::desktopRatio();
	if (isExpanded()) {
// 		if (m_fullScreen)
// 			visual = widget;
// 		else {
			visual.setWidth(m_videoRatio);
			visual.rwidth() *= (aspect/m_videoRatio);
			
			if (widgetRatio < ratio(visual)) {
				if (widgetRatio < desktopRatio) {
					visual.setWidth(desktopRatio * visual.height());
					visual.scale(widget, Qt::KeepAspectRatio);
				} else
					visual = widget;
			} else
				visual.scale(widget, Qt::KeepAspectRatio);
// 		}
		video.setWidth(desktopRatio);
		video.rwidth() *= (aspect/m_videoRatio);
		if (ratio(visual) > ratio(video))
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
		else
			video.scale(visual, Qt::KeepAspectRatio);
		if (m_cropRatio > 0.0) {
			visual.setWidth(visual.height()*m_cropRatio);
			visual.scale(widget, Qt::KeepAspectRatio);
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
			if (m_cropRatio < m_videoRatio)
				video *= (m_videoRatio / m_cropRatio);
		}
	} else {
		video.setWidth(aspect);
		if (m_cropRatio > 0.0) {
			visual.setWidth(m_cropRatio);
			visual.scale(widget, Qt::KeepAspectRatio);
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
		} else {
			visual = video;
			visual.scale(widget, Qt::KeepAspectRatio);
			video = visual;
		}
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

void VideoOutput::setAspectRatio(double ratio) {
	if (!isSame(ratio, m_aspectRatio)) {
		m_aspectRatio = ratio;
		updateVideo();
	}
}

void VideoOutput::crop(double ratio) {
	if (!isSame(ratio, m_cropRatio)) {
		m_cropRatio = ratio;
		updateVideo();
	}
}

void VideoOutput::updateVideoSize(const QSize &size) {
	if (m_videoSize != size) {
		m_videoRatio = ratio(m_videoSize = size);
		qDebug() << m_videoRatio << Utility::desktopRatio();
		qDebug() << (m_videoOverDesktop = m_videoRatio/Utility::desktopRatio());
		emit widgetSizeHintChanged(widgetSizeHint());
	}
}

bool VideoOutput::expand(bool expanded) {
	if (m_expanded == expanded)
		return true;
	if (!updateExpand(expanded))
		return false;
	m_expanded = expanded;
	updateVideo();
	return true;
}

void VideoOutput::update() {
	updateVideo();
	updateBrightness(m_brightness);
	updateHue(m_hue);
	updateSaturation(m_saturation);
	updateContrast(m_contrast);
}

}
