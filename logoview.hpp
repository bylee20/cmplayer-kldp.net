#ifndef LOGOVIEW_HPP
#define LOGOVIEW_HPP

#include <QtGui/QWidget>

class RichString;

class LogoView : public QWidget {
	Q_OBJECT
public:
	LogoView(QWidget *parent);
	~LogoView();
	QSize sizeHint() const {return QSize(600, 450);}
	void showText(const RichString &text, int last = 2500);
	void showTimeLine(int pos, int duration, int last = 2500);
private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	struct Data;
	Data *d;
};

#endif // LOGOVIEW_HPP
