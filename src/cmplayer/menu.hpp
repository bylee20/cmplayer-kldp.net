#ifndef MENU_HPP
#define MENU_HPP

#include "mrl.hpp"
#include "global.hpp"
#include <QtGui/QMenu>
#include <QtGui/QActionGroup>
#include <QtCore/QHash>

class ActionGroup : public QActionGroup {
	Q_OBJECT
public:
	ActionGroup(QObject *parent = 0)
	: QActionGroup(parent) {
		connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitData(QAction*)));
	}
	void setChecked(const QVariant &data, bool checked) {
		const QList<QAction*> acts = actions();
		for (int i=0; i<acts.size(); ++i) {
			if (acts[i]->data() == data) {
				acts[i]->setChecked(checked);
				return;
			}
		}
	}
	void trigger(const QVariant &data) {
		const QList<QAction*> acts = actions();
		for (int i=0; i<acts.size(); ++i) {
			if (acts[i]->data() == data) {
				acts[i]->trigger();
				return;
			}
		}
	}
	QVariant data() const {
		const QAction *act = checkedAction();
		return act ? act->data() : QVariant();
	}
	void clear() {
		QList<QAction*> acts = actions();
		for (int i=0; i<acts.size(); ++i) {
			removeAction(acts[i]);
			delete acts[i];
		}
	}
signals:
	void triggered(const QVariant &data);
	void triggered(int data);
	void triggered(double data);
	void triggered(const QString &data);
	void triggered(const Mrl &data);
private slots:
	void emitData(QAction *action);
};

class Menu : public QMenu {
	Q_OBJECT
public:
	struct WheelActionPair {
		WheelActionPair(QAction *up, QAction *down): up(up), down(down) {}
		WheelActionPair(): up(0), down(0) {}
		bool isNull() const {return !up || !down;}
		QAction *up, *down;
	};
	typedef QMap<ClickAction, QAction*> ClickActionMap;
	typedef QMap<WheelAction, WheelActionPair> WheelActionMap;
	template <typename N>
	static QString toString(N value, bool sign = true) {
		if (!sign || value < 0)
			return QString::number(value);
		else {
			QString s = value ? "+" : QString::fromUtf8("±");
			return s += QString::number(value);
		}
	}
	static Menu &get() {return *obj;}
	static Menu &create(QWidget *parent);
	static QString key(QAction *action) {return keys.value(action);}
	static void saveShortcut();
	static void loadShortcut();
	static void updatePref();
	~Menu() {}
	Menu &operator() (const QString &key) const {return *m(key);}
	QAction *operator[] (const QString &key) const {return a(key);}
	Menu *addMenu(const QString &key) {
		Menu *menu = new Menu(key, this);
		QMenu::addMenu(menu);
		return (m_menu[key] = menu);
	}
	QAction *addActionToGroup(const QString &key
			, bool checkable = false, const QString &group = "") {
		QAction *action = addAction(key, checkable);
		addGroup(group)->addAction(action);
		return action;
	}
	QAction *addAction(const QString &key, bool checkable = false) {
		QAction *action = QMenu::addAction(key);
		action->setCheckable(checkable);
		const QString k = m_unique + "." + key;
		keys[action] = k;
		acts[k] = action;
		return m_act[key] = action;
	}
	QAction *addActionToGroupWithoutKey(const QString &name
			, bool checkable = false, const QString &group = "") {
		QAction *action = QMenu::addAction(name);
		action->setCheckable(checkable);
		addGroup(group)->addAction(action);
		return action;
	}
	ActionGroup *addGroup(const QString &name) {
		QHash<QString, ActionGroup*>::iterator it = m_group.find(name);
		if (it == m_group.end())
			it = m_group.insert(name, new ActionGroup(this));
		return it.value();
	}
	ActionGroup *g(const QString &key = "") const {return m_group[key];}
	QAction *a(const QString &key) const {return m_act[key];}
	Menu *m(const QString &key) const {return m_menu[key];}
	QMenu *contextMenu() const {return m_context;}
	const ClickActionMap &clickAction() const {return m_click;}
	const WheelActionMap &wheelAction() const {return m_wheel;}
private:
	Menu(const QString &key, QWidget *parent)
	: QMenu(parent), m_key(key), m_upper(0) {
		addGroup("");
		m_unique = key;
	}
	Menu(const QString &key, Menu *parent)
	: QMenu(parent), m_key(key), m_upper(parent) {
		addGroup("");
		m_unique = parent->m_unique + "." + key;
	}
	static Menu *obj;
	QMenu *m_context;
	static QHash<QAction*, QString> keys;
	static QHash<QString, QAction*> acts;
	QHash<QString, ActionGroup*> m_group;
	QHash<QString, QAction*> m_act;
	QHash<QString, Menu*> m_menu;
	QMap<ClickAction, QAction*> m_click;
	QMap<WheelAction, WheelActionPair> m_wheel;
	QString m_key;
	QString m_unique;
	Menu *m_upper;
};

#endif // MENU_HPP
