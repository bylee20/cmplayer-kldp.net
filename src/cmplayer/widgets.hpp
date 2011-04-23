#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <QtGui/QWidget>

class FontOptionWidget : public QWidget {
	Q_OBJECT
public:
	FontOptionWidget(QWidget *parent = 0);
	~FontOptionWidget();
	void set(bool bold, bool italic, bool underline, bool strikeout);
	bool bold() const;
	bool italic() const;
	bool underline() const;
	bool strikeOut() const;
private:
	struct Data;
	Data *d;
};

class ColorSelectWidget : public QWidget {
	Q_OBJECT
public:
	ColorSelectWidget(QWidget *parent = 0);
	~ColorSelectWidget();
	void setColor(const QColor &color, bool hasAlpha);
	QColor color() const;
private:
	struct Data;
	Data *d;
};

#endif // WIDGETS_HPP
