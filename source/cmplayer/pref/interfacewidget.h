#ifndef PREFERENCESPREFINTERFACEWIDGET_H
#define PREFERENCESPREFINTERFACEWIDGET_H

#include <QWidget>

class QTreeWidgetItem;
class ActionCollection;

namespace Pref {

class Interface;
namespace Ui {class Ui_InterfaceWidget;}

class InterfaceWidget : public QWidget {
	Q_OBJECT
public:
	InterfaceWidget(const Interface &interface, QWidget *parent = 0);
	~InterfaceWidget();
	const Interface &interface() const;
private slots:
	void getShortcut(QTreeWidgetItem *item);
private:
	Ui::Ui_InterfaceWidget *ui;
	mutable Interface *m_interface;
	const ActionCollection *const m_ac;
};

}

#endif
