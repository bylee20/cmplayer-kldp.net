#include "wizardpage.h"
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

namespace Backend {

int WizardPage::uid = 50;

WizardPage::WizardPage(QWidget *parent)
: QWizardPage(parent), m_id(getUniqueId()) {}

void WizardPage::setWidget(QWidget *widget) {
	if (w != widget) {
		w = widget;
		if (layout())
			delete layout();
		QVBoxLayout *vbox = new QVBoxLayout;
		vbox->addWidget(w);
		vbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
		setLayout(vbox);
	}
}

}
