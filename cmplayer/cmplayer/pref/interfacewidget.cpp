#include "interface.h"
#include "../helper.h"
#include "interfacewidget.h"
#include "getshortcutdialog.h"
#include "../actioncollection.h"
#include "ui_interfacewidget.h"
#include <QtGui/QMenu>

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

struct InterfaceWidget::Data {
	Ui::Ui_InterfaceWidget ui;
	Interface iface;
	const ActionCollection *ac;
};

InterfaceWidget::InterfaceWidget(const Interface &interface, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->ac = ActionCollection::get();
	d->iface = interface;
	d->ui.setupUi(this);
	connect(d->ui.shortcut_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
			SLOT(getShortcut(QTreeWidgetItem*)));

	QList<QAction *> acts = d->ac->wholeMenu()->actions();
	for (int i=0; i<acts.size(); ++i)
		new MenuTreeItem(d->ui.shortcut_tree, acts[i]);

	static const QStringList MouseClickActions = Interface::mouseClickActions();
	static const QStringList WheelScrollActions = Interface::wheelScrollActions();
	d->ui.double_action_combo->addItems(MouseClickActions);
	d->ui.middle_action_combo->addItems(MouseClickActions);
	d->ui.scroll_action_combo->addItems(WheelScrollActions);
	d->ui.double_action_combo->setCurrentIndex(d->iface.doubleClickAction());
	d->ui.middle_action_combo->setCurrentIndex(d->iface.middleClickAction());
	d->ui.scroll_action_combo->setCurrentIndex(d->iface.wheelScrollAction());

	d->ui.seek_spin->setValue(d->iface.seekingStep()/1000);
	d->ui.seek_more_spin->setValue(d->iface.seekingMoreStep()/1000);
	d->ui.seek_much_more_spin->setValue(d->iface.seekingMuchMoreStep()/1000);
	d->ui.volume_spin->setValue(d->iface.volumeStep());
}

InterfaceWidget::~InterfaceWidget() {
	delete d;
}

const Interface &InterfaceWidget::interface() const {
	Interface::KeyMap map;
	for (int i=0; i<d->ui.shortcut_tree->topLevelItemCount(); ++i) {
		QTreeWidgetItem *item = d->ui.shortcut_tree->topLevelItem(i);
		static_cast<MenuTreeItem*>(item)->applyShortcut();
	}
	QList<QAction*> acts = d->ac->actions();
	for (int i=0; i<acts.size(); ++i)
		map[d->ac->name(acts[i])] = acts[i]->shortcut();
	d->iface.setShortcuts(map);

	int idx = d->ui.double_action_combo->currentIndex();
	d->iface.setDoubleClickAction(static_cast<Interface::MouseClickAction>(idx));
	idx = d->ui.middle_action_combo->currentIndex();
	d->iface.setMiddleClickAction(static_cast<Interface::MouseClickAction>(idx));
	idx = d->ui.scroll_action_combo->currentIndex();
	d->iface.setWheelScrollAction(static_cast<Interface::WheelScrollAction>(idx));

	d->iface.setSeekingStep(d->ui.seek_spin->value()*1000);
	d->iface.setSeekingMoreStep(d->ui.seek_more_spin->value()*1000);
	d->iface.setSeekingMuchMoreStep(d->ui.seek_much_more_spin->value()*1000);
	d->iface.setVolumeStep(d->ui.volume_spin->value());

	return d->iface;
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
