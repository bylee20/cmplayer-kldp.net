// #include "actioncollection.h"
// #include <QtGui/QAction>
// #include <QtGui/QMenu>
// 
// ActionCollection::ActionCollection() {}
// 
// ActionCollection *ActionCollection::get() {
// 	static ActionCollection obj;
// 	return &obj;
// }
// 
// void ActionCollection::addAction(const QString &name, QAction *action) {
// 	if (action->menu())
// 		addActions(action->menu());
// 	else if (!action->isSeparator()) {
// 		m_actions[name] = action;
// 		m_names[action] = name;
// 	}
// }
// 
// void ActionCollection::addAction(QAction *action) {
// 	addAction(action->text(), action);
// }
// 
// void ActionCollection::addActions(const QList<QAction*> &actions) {
// 	for (int i=0; i<actions.size(); ++i)
// 		addAction(actions[i]);
// }
// 
// void ActionCollection::addActions(QMenu *menu) {
// 	QAction *action = menu->menuAction();
// 	QString name = action->text();
// 	m_actions[name] = action;
// 	m_names[action] = name;
// 	addActions(menu->actions());
// }
// 
// void ActionCollection::setName(QAction *action, const QString &name) {
// 	if (m_names.contains(action)) {
// 		m_actions.remove(m_names[action]);
// 		m_actions[name] = action;
// 		m_names[action] = name;
// 	}
// }
// 
// QAction* ActionCollection::take(QAction *action) {
// 	if (m_names.contains(action)) {
// 		m_actions.remove(m_names[action]);
// 		m_names.remove(action);
// 	}
// 	return action;
// }
