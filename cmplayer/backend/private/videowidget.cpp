#include "videowidget.h"
#include "../videooutput.h"
#include <QResizeEvent>

namespace Backend {

VideoWidget::VideoWidget(VideoOutput *video)
: QWidget() {
	m_video = video;
}

QSize VideoWidget::sizeHint() const {
	return m_video->widgetSizeHint();
}

void VideoWidget::resizeEvent(QResizeEvent *event) {
	QSize size = static_cast<QResizeEvent*>(event)->size();
	m_video->updateVideo();
	emit m_video->widgetResized(size);
}

}
