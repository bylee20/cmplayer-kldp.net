#ifndef OSDSTYLE_HPP
#define OSDSTYLE_HPP

#include <QtGui/QWidget>

class QSettings;

class OsdStyle {
public:
	class Widget;
	enum Scale {FitToWidth, FitToHeight, FitToDiagonal};
	OsdStyle(): bgColor(Qt::black), fgColor(Qt::white) {
		alignment = Qt::AlignCenter;
		borderWidth = 0.05;
		textSize = 0.03;
		scale = FitToDiagonal;
	}
	void save(QSettings *set, const QString &group) const;
	void load(QSettings *set, const QString &group);
	QFont font;
	QColor bgColor, fgColor;
	double borderWidth, textSize;
	Qt::Alignment alignment;
	Scale scale;
};

class QLabel;

class OsdStyle::Widget : public QWidget {
	Q_OBJECT
public:
	Widget(QWidget *parent = 0);
	~Widget();
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const;
private slots:
	void updateFont(const QFont &font);
	static void setColor(QLabel *label, const QColor &color);
	void slotFont();
	void slotColor();
private:
	struct Data;
	Data *d;

};

#endif // OSDSTYLE_HPP
