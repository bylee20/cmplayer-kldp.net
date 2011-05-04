#ifndef MENU_HPP
#define MENU_HPP

#include "global.hpp"
#include "actiongroup.hpp"
#include "enums.hpp"
#include "colorproperty.hpp"
#include <QtGui/QMenu>
#include <QtCore/QHash>

class Menu : public QMenu {
	Q_OBJECT
public:
	struct WheelActionPair {
		WheelActionPair(QAction *up, QAction *down): up(up), down(down) {}
		WheelActionPair(): up(0), down(0) {}
		bool isNull() const {return !up || !down;}
		QAction *up, *down;
	};
	typedef QMap<Enum::ClickAction, QAction*> ClickActionMap;
	typedef QMap<Enum::WheelAction, WheelActionPair> WheelActionMap;

	inline Menu &operator() (const QString &key) const {return *m(key);}
	inline QAction *operator[] (const QString &key) const {return a(key);}
	inline ActionGroup *g(const QString &key = "") const {return m_group[key];}
	inline QAction *a(const QString &key) const {return m_act[key];}
	inline Menu *m(const QString &key) const {return m_menu[key];}
	inline Menu *addMenu(const QString &k) {Menu *m = new Menu(k, this); QMenu::addMenu(m); return (m_menu[k] = m);}
	inline QAction *addActionToGroup(const QString &k, bool ch = false, const QString &g = "") {
		QAction *a = addAction(k, ch); addGroup(g)->addAction(a); return a;
	}
	inline QAction *addAction(const QString &key, bool ch = false) {
		QAction *a = QMenu::addAction(key); a->setCheckable(ch);
		return m_act[key] = acts[keys[a] = (m_unique + "." + key)] = a;
	}
	inline QAction *addActionToGroupWithoutKey(const QString &name, bool ch = false, const QString &g = "") {
		QAction *a = QMenu::addAction(name); a->setCheckable(ch); addGroup(g)->addAction(a); return a;
	}
	inline ActionGroup *addGroup(const QString &name) {
		ActionGroup *g = m_group.value(name, 0); return g ? g : (m_group[name] = new ActionGroup(this));
	}

	inline static Menu &root() {Q_ASSERT(m_root != 0); return *m_root;}
	inline static QString key(QAction *action) {return keys.value(action);}
	static QAction *clickAction(const Enum::ClickAction &a) {return m_click[a];}
	static QAction *wheelAction(const Enum::WheelAction &a, bool up) {return up ? m_wheel[a].up : m_wheel[a].down;}

	static void saveShortcut();
	static void loadShortcut();
	static void updatePref();

	static void init();
	static void fin();
private:
	Menu(const QString &key)
	: QMenu(0), m_key(key), m_upper(0) {
		addGroup("");
		m_unique = key;
	}
	Menu(const QString &key, Menu *parent)
	: QMenu(parent), m_key(key), m_upper(parent) {
		addGroup("");
		m_unique = parent->m_unique + "." + key;
	}
	static Menu *m_root;
	static QHash<QAction*, QString> keys;
	static QHash<QString, QAction*> acts;
	static ClickActionMap m_click;
	static WheelActionMap m_wheel;

	template<typename N>
	inline static void setActionAttr(QAction *act, const QVariant &data
			, const QString &text, N textValue, bool sign = true) {
		act->setData(data);
		act->setText(text.arg(toString(textValue, sign)));
	}

	inline static void setActionStep(QAction *plus, QAction *minus
			, const QString &text, int value, double textRate = -1.0) {
		if (textRate < 0) {
			plus->setText(text.arg(toString(value)));
			minus->setText(text.arg(toString(-value)));
		} else {
			plus->setText(text.arg(toString(value*textRate)));
			minus->setText(text.arg(toString(-value*textRate)));
		}
		plus->setData(value);
		minus->setData(-value);
	}

	inline static void setVideoPropStep(Menu &menu, const QString &key
			, ColorProperty::Value prop, const QString &text, int step) {
		setActionAttr(menu[key + "+"], QList<QVariant>() << prop << step, text, step);
		setActionAttr(menu[key + "-"], QList<QVariant>() << prop << -step, text, -step);
	}

	QHash<QString, ActionGroup*> m_group;
	QHash<QString, QAction*> m_act;
	QHash<QString, Menu*> m_menu;
	QString m_key;
	QString m_unique;
	Menu *m_upper;
};

#endif // MENU_HPP
