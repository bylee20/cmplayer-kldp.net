#ifndef PREFOSDWIDGET_H
#define PREFOSDWIDGET_H

#include <QWidget>

class QLabel;

namespace Core{class OsdStyle;}

class PrefOsdWidget : public QWidget {
	Q_OBJECT
public:
	PrefOsdWidget(QWidget *parent = 0);
	~PrefOsdWidget();
	void setStyle(const Core::OsdStyle &style);
	const Core::OsdStyle &style() const;
private slots:
	void updateFont(const QFont &font);
	static void setColor(QLabel *label, const QColor &color);
	void slotFont();
	void slotColor();
private:
	struct Data;
	Data *d;
};

#endif
