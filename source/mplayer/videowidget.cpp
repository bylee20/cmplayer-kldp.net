#include <QEvent>
#include <QPainter>
#include <QTimer>
#include "videowidget.h"
#include "playengine.h"
#include "videooutput.h"

namespace MPlayer {

VideoWidget::VideoWidget(VideoOutput *video, QWidget *parent)
: QWidget(parent), m_video(video), m_timer(new QTimer(this)) {
	m_timer->setSingleShot(true);
	m_timer->setInterval(3000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(hideCursor()));
}

void VideoWidget::mouseMoveEvent(QMouseEvent *event) {
	QWidget::mouseMoveEvent(event);
	m_timer->stop();
	if (cursor().shape() != Qt::ArrowCursor)
		setCursor(Qt::ArrowCursor);
	m_timer->start();
}

QSize VideoWidget::sizeHint() const {
	return m_video->widgetSizeHint();
}

void VideoWidget::hideCursor() {
	if (cursor().shape() != Qt::BlankCursor)
		setCursor(Qt::BlankCursor);
}

}
