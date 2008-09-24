#ifndef PREFBACKENDPAGE_H
#define PREFBACKENDPAGE_H

#include <QtGui/QWidget>
#include <backend/manager.h>

class QTreeWidgetItem;

namespace Pref {

class BackendPage : public QWidget {
	Q_OBJECT
public:
	BackendPage(QWidget *parent = 0);
	~BackendPage();
	const Backend::FactoryMap &factories() const;
signals:
	void backendSelected(const QString &fileName);
	void backendSelected(Backend::FactoryIface *factory);
private slots:
	void slotSelected();
	void slotActivated(QTreeWidgetItem *item);
private:
	struct Data;
	Data *d;
};

}

#endif
