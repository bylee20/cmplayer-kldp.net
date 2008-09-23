#ifndef PREFERENCESGENERALWIDGET_H
#define PREFERENCESGENERALWIDGET_H

#include <QtGui/QWidget>

namespace Pref {

class General;

class GeneralWidget : public QWidget {
	Q_OBJECT
public:
	GeneralWidget(const General &general, QWidget *parent);
	~GeneralWidget();
	const General &general() const;
private:
	struct Data;
	Data *d;
};

}

#endif
