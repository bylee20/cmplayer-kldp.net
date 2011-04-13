#ifndef OSDSTYLE_HPP
#define OSDSTYLE_HPP

#include <QtGui/QWidget>

class QSettings;

class OsdStyle {
public:
	class Widget;
	enum AutoSize {FitToWidth, FitToHeight, FitToDiagonal};
	OsdStyle(): color_bg(Qt::black), color_fg(Qt::white) {
		alignment = Qt::AlignCenter;
		border_width = 0.05;
		text_scale = 0.03;
		auto_size = FitToDiagonal;
	}
	void save(QSettings *set, const QString &group) const;
	void load(QSettings *set, const QString &group);
	QFont font;
	QColor color_bg, color_fg;
	double border_width, text_scale;
	Qt::Alignment alignment;
	AutoSize auto_size;
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
