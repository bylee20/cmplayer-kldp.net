#ifndef OSDWIDGET_H_
#define OSDWIDGET_H_

#include <QtGui/QWidget>
#include <backend/osdstyle.h>
#include "../ui/ui_osdwidget.h"

class QComboBox;

namespace Pref {

class OsdWidget : public QWidget {
	Q_OBJECT
public:
	OsdWidget(QWidget *parent = 0);
	~OsdWidget();
	Backend::OsdStyle style() const;
	bool highQuality() const;
	void setStyle(const Backend::OsdStyle &style);
	void setHighQuality(bool high);
private slots:
	void changeFont();
	void changeTextColor(int index);
	void changeBorderColor(int index);
private:
	static void setColor(QWidget *widget, const QColor &color);
	static void initColorComboBox(QComboBox *combo);
	Ui::Ui_OsdWidget ui;
};

}

#endif /* OSDWIDGET_H_ */
