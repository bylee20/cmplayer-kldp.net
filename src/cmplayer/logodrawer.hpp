#ifndef LOGOVIEW_HPP
#define LOGOVIEW_HPP

#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <QtGui/QPainterPath>

class LogoDrawer {
public:
	LogoDrawer();
	~LogoDrawer();
	void draw(QPainter *painter, const QRect &rect);
private:
	QPixmap m_logo;
	QBrush m_bgBrush, m_lightBrush;
	QPainterPath m_lightPath;
};

#endif // LOGOVIEW_HPP
