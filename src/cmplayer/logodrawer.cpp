#include "logodrawer.hpp"
#include <QtGui/QPainter>

LogoDrawer::LogoDrawer() {
	m_logo.load(":/img/cmplayer512.png");
	QLinearGradient grad(0.5, 1.0, 0.75, 0.13);
	grad.setColorAt(0.0, qRgb(51, 131, 230));
	grad.setColorAt(1.0, qRgb(110, 202, 247));
	m_bgBrush = QBrush(grad);

	grad = QLinearGradient(0.3, 0.1, 0.5, 0.9);
	grad.setColorAt(0.0, Qt::white);
	grad.setColorAt(1.0, Qt::transparent);
	m_lightBrush = QBrush(grad);

	const double oh = 0.6;
	m_lightPath.moveTo(0.0, 0.0);
	m_lightPath.lineTo(0.0, oh*0.8);
	m_lightPath.cubicTo(0.1, oh*0.9, 0.3, oh, 0.4, oh);
	m_lightPath.cubicTo(0.6, oh, 0.8, oh*0.9, 1.0, oh*0.6);
	m_lightPath.lineTo(1.0, 0.0);
	m_lightPath.closeSubpath();
}

LogoDrawer::~LogoDrawer() {
}

void LogoDrawer::draw(QPainter *painter, const QRect &bg) {
	const double w = bg.width();
	const double h = bg.height();

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(w, h);
	painter->setPen(Qt::NoPen);
	painter->fillRect(bg, m_bgBrush);
	painter->setOpacity(0.2);
	painter->setBrush(m_lightBrush);
	painter->drawPath(m_lightPath);
	painter->restore();

	const int len = qMin(qRound(qMin(w, h)*0.7), m_logo.width());
	QRect rect;
	rect.setX((w-len)*0.5 + 0.5);
	rect.setY((h-len)*0.5 + 0.5);
	rect.setWidth(len + 0.5);
	rect.setHeight(len + 0.5);
	if (len != m_logo.width())
		painter->drawPixmap(rect, m_logo.scaled(len, len
				, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	else
		painter->drawPixmap(rect, m_logo);
}
