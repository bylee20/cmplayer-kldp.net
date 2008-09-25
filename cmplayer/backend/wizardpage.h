#ifndef BACKEND_WIZARDPAGE_H
#define BACKEND_WIZARDPAGE_H

#include <QtGui/QWizardPage>

namespace Backend {

class WizardPage : public QWizardPage {
	Q_OBJECT
public:
	static int getUniqueId() {return ++uid;}
	WizardPage(QWidget *parent = 0);
	int id() const {return m_id;}
protected:
	void setWidget(QWidget *widget);
private:
	static int uid;
	const int m_id;
	QWidget *w;
};

}

#endif
