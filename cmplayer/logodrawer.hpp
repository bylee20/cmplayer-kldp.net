#ifndef LOGOVIEW_HPP
#define LOGOVIEW_HPP

#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <QtGui/QPainterPath>

class RichString;

class LogoDrawer {
public:
	LogoDrawer();
	~LogoDrawer();
//	QSize sizeHint() const {return QSize(600, 450);}
//	void showText(const RichString &text, int last = 2500);
//	void showTimeLine(int pos, int duration, int last = 2500);
//private:
//	void paintEvent(QPaintEvent *event);
//	void resizeEvent(QResizeEvent *event);
	void draw(QPainter *painter, const QRect &rect);
private:
	QPixmap m_logo;
	QBrush m_bgBrush, m_lightBrush;
	QPainterPath m_lightPath;
};

#endif // LOGOVIEW_HPP
