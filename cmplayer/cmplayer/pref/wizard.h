#ifndef PREF_WIZARD_H
#define PREF_WIZARD_H

#include <QtGui/QWizard>

namespace Pref {

class Wizard : public QWizard {
	Q_OBJECT
public:
	Wizard(QWidget *parent = 0);
	~Wizard();
private:
	QWizardPage *createIntroPage();
	QWizardPage *createBackendPage();
	struct Data;
	Data *d;
};

}

#endif
