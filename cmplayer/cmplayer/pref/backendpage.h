#ifndef PREFBACKENDPAGE_H
#define PREFBACKENDPAGE_H

#include <backend/wizardpage.h>

namespace Pref {

class BackendPage : public Backend::WizardPage {
	Q_OBJECT
public:
	BackendPage(QWidget *parent = 0);
	~BackendPage();
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
