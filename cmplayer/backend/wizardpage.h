#ifndef BACKEND_WIZARDPAGE_H
#define BACKEND_WIZARDPAGE_H

#include <QtGui/QWizardPage>

namespace Backend {

class WizardPage : public QWizardPage {
	Q_OBJECT
public:
	static int getUniqueId() {return ++uid;}
	WizardPage(QWidget *parent);
	int id() const {return m_id;}
private:
	static int uid;
	const int m_id;
};

}

#endif
