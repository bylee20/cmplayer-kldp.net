#include "jumpslider.h"
#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QStyle>

namespace Backend {

JumpSlider::JumpSlider(QWidget *parent)
: QSlider(parent) {
	setOrientation(Qt::Horizontal);
	setFocusPolicy(Qt::NoFocus);
}

void JumpSlider::mousePressEvent(QMouseEvent *event) {
	const qint64 range = maximum() - minimum();
	const qint64 width = this->width();
	const qint64 newVal = static_cast<qint64>(event->x()) * range / width;
	const qint64 metric = qApp->style()->pixelMetric(QStyle::PM_SliderLength);
	const qint64 sub = (metric * range)/width;
	if (qAbs(newVal - value()) > sub)
		setValue(newVal);
	else
		QSlider::mousePressEvent(event);
}

}
