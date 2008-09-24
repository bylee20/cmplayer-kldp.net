#include "wizardpage.h"

namespace Backend {

int WizardPage::uid = 0;

WizardPage::WizardPage(QWidget *parent)
: QWizardPage(parent), m_id(getUniqueId()) {}

}
