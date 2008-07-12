#ifndef ACTIONCOLLECTION_H
#define ACTIONCOLLECTION_H

#include <QString>
#include <QMap>

class QMenu;			class QAction;

class ActionCollection {
public:
	static ActionCollection *get() {static ActionCollection obj; return &obj;}
	void addAction(const QString &name, QAction *action);
	void addAction(QAction *action);
	void addActions(const QList<QAction*> &actions);
	void addActions(QMenu *menu);
	inline QList<QAction *> actions() const {return m_actions.values();}
	inline QString name(QAction *action) const {return m_names.value(action);}
	inline QAction *action(const QString &name) const {return m_actions.value(name);}
	inline void setAction(const QString &name, QAction *action) {
		if (m_actions.contains(name)) {m_names.remove(m_actions[name]);} addAction(name, action);}
	void setName(QAction *action, const QString &name);
	QAction* take(QAction *action);
	inline void setWholeMenu(QMenu *menu) {m_wholeMenu = menu;}
	inline QMenu *wholeMenu() const {return m_wholeMenu;}
private:
	ActionCollection();
	QMap<QAction*, QString> m_names;
	QMap<QString, QAction*> m_actions;
	QMenu *m_wholeMenu;
};

#endif
