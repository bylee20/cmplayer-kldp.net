#include "interface.h"
#include "../helper.h"
#include "interfacewidget.h"
#include "getshortcutdialog.h"
#include "../actioncollection.h"
#include "../ui/ui_interfacewidget.h"
#include <QMenu>

namespace Pref {

class MenuTreeItem : public QTreeWidgetItem {
public:
	enum Column {Discription = 0, Shortcut = 1};
	MenuTreeItem(int type = Type) : QTreeWidgetItem(type) {}
	MenuTreeItem(QTreeWidget *parent, QAction *action, int type = Type)
		: QTreeWidgetItem(parent, type) {init(action);}
	MenuTreeItem(QTreeWidget *parent, QMenu *menu, int type = Type)
		: QTreeWidgetItem(parent, type) {init(menu->menuAction());}
	MenuTreeItem(MenuTreeItem *parent, QAction *action, int type = Type)
		: QTreeWidgetItem(parent, type) {init(action);}
	MenuTreeItem(MenuTreeItem *parent, QMenu *menu, int type = Type)
		: QTreeWidgetItem(parent, type) {init(menu->menuAction());}
	void addChild(QAction *action){new MenuTreeItem(this, action);}
	void addChildren(QMenu *menu){new MenuTreeItem(this, menu);}
	void addChildren(const QList<QAction *> &actions){
		foreach(QAction *act, actions) addChild(act);}
	void setAction(QAction *action){takeChildren(); init(action);}
	QAction *action() const {return m_action;}
	bool isMenu() const{return m_action->menu() != 0;}
	bool isSeparator() const{return m_action->isSeparator();}
	const QKeySequence &key() const {return m_key;}
	void setKey(const QKeySequence &key) {
		m_key = key; setText(Shortcut, m_key.toString());}
	void applyShortcut() {
		for (int i=0; i<childCount(); ++i)
			static_cast<MenuTreeItem *>(child(i))->applyShortcut();
		m_action->setShortcut(m_key);}
	void addActionTo(QMenu *parent){
		if (childCount()) {
			QMenu *menu = parent->addMenu(m_action->text());
			menu->setIcon(m_action->icon());
			for (int i=0; i<childCount(); ++i)
				static_cast<MenuTreeItem *>(child(i))->addActionTo(menu);
		} else if (isSeparator())
			parent->addSeparator();
		else
			parent->addAction(m_action);
	}
private:
	void init(QAction *action) {
		static ActionCollection *ac = ActionCollection::get();
		m_action = action;
		setIcon(Discription, action->icon());
		if(m_action->menu()) {
			setText(Discription, action->text());
			addChildren(m_action->menu()->actions());
		} else {
			m_key = m_action->shortcut();
			setText(Discription,
			isSeparator() ? QObject::trUtf8("----구분선----") : ac->name(m_action));
			setText(Shortcut, m_key.toString());
		}
	}
	QAction *m_action;
	QKeySequence m_key;
};


InterfaceWidget::InterfaceWidget(const Interface &interface, QWidget *parent)
: QWidget(parent), ui(new Ui::Ui_InterfaceWidget), m_interface(new Interface)
, m_ac(ActionCollection::get()) {
	*m_interface = interface;
	ui->setupUi(this);
	connect(ui->shortcut_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
			SLOT(getShortcut(QTreeWidgetItem*)));

	QList<QAction *> acts = m_ac->wholeMenu()->actions();
	for (int i=0; i<acts.size(); ++i)
		new MenuTreeItem(ui->shortcut_tree, acts[i]);

	static const QStringList MouseClickActions = Interface::mouseClickActions();
	static const QStringList WheelScrollActions = Interface::wheelScrollActions();
	ui->double_action_combo->addItems(MouseClickActions);
	ui->middle_action_combo->addItems(MouseClickActions);
	ui->scroll_action_combo->addItems(WheelScrollActions);
	ui->double_action_combo->setCurrentIndex(m_interface->doubleClickAction);
	ui->middle_action_combo->setCurrentIndex(m_interface->middleClickAction);
	ui->scroll_action_combo->setCurrentIndex(m_interface->wheelScrollAction);

	ui->seek_spin->setValue(m_interface->seekingStep/1000);
	ui->seek_more_spin->setValue(m_interface->seekingMoreStep/1000);
	ui->seek_much_more_spin->setValue(m_interface->seekingMuchMoreStep/1000);
	ui->volume_spin->setValue(m_interface->volumeStep);
}

InterfaceWidget::~InterfaceWidget() {
	delete m_interface;
	delete ui;
}

const Interface &InterfaceWidget::interface() const {
	m_interface->shortcuts.clear();
	for (int i=0; i<ui->shortcut_tree->topLevelItemCount(); ++i)
		static_cast<MenuTreeItem*>(ui->shortcut_tree->topLevelItem(i))->applyShortcut();
	QList<QAction*> acts = m_ac->actions();
	for (int i=0; i<acts.size(); ++i)
		m_interface->shortcuts[m_ac->name(acts[i])] = acts[i]->shortcut();
//	acts = m_ac->customMenu()->actions();
//	for (int i=0; i<acts.size(); ++i)
//		m_ac->customMenu()->removeAction(acts[i]);

	m_interface->doubleClickAction = Interface::MouseClickAction(ui->double_action_combo->currentIndex());
	m_interface->middleClickAction = Interface::MouseClickAction(ui->middle_action_combo->currentIndex());
	m_interface->wheelScrollAction = Interface::WheelScrollAction(ui->scroll_action_combo->currentIndex());

	m_interface->seekingStep = ui->seek_spin->value()*1000;
	m_interface->seekingMoreStep = ui->seek_more_spin->value()*1000;
	m_interface->seekingMuchMoreStep = ui->seek_much_more_spin->value()*1000;
	m_interface->volumeStep = ui->volume_spin->value();

	return *m_interface;
}

void InterfaceWidget::getShortcut(QTreeWidgetItem *item) {
	if (!item)
		return;
	GetShortcutDialog dlg(this);
	MenuTreeItem *mi = static_cast<MenuTreeItem *>(item);
	if (!mi->isMenu() && !mi->isSeparator()) {
		dlg.setKey(mi->key());
		if (dlg.exec())
			mi->setKey(dlg.key());
	}
}

}
