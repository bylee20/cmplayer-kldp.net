#ifndef PREFERENCESGENERALWIDGET_H
#define PREFERENCESGENERALWIDGET_H

#include <QWidget>

namespace Pref {

class General;
namespace Ui {class Ui_GeneralWidget;}

class GeneralWidget : public QWidget {
	Q_OBJECT
public:
	GeneralWidget(const General &general, QWidget *parent);
	~GeneralWidget();
	const General &general() const;
private:
	Ui::Ui_GeneralWidget *ui;
	mutable General *m_general;
};

}

#endif
