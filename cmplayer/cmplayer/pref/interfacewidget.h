#ifndef PREF_INTERFACEWIDGET_H
#define PREF_INTERFACEWIDGET_H

#include <QtGui/QWidget>

class QTreeWidgetItem;

namespace Pref {

class Interface;

class InterfaceWidget : public QWidget {
	Q_OBJECT
public:
	InterfaceWidget(const Interface &interface, QWidget *parent = 0);
	~InterfaceWidget();
	const Interface &interface() const;
private slots:
	void getShortcut(QTreeWidgetItem *item);
private:
	struct Data;
	Data *d;
};

}

#endif
