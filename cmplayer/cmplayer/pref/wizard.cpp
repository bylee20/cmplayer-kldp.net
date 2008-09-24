#include "wizard.h"
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <backend/manager.h>
#include "backendpage.h"

namespace Pref {

struct Wizard::Data {
	Backend::FactoryMap map;
};

Wizard::Wizard(QWidget *parent)
: QWizard(parent) {
	d = new Data;
	
	addPage(createIntroPage());
	addPage(createBackendPage());
	
	setWindowTitle(trUtf8("설정 마법사"));
	setPixmap(QWizard::LogoPixmap, QPixmap(":/img/tools-wizard.png"));
}

Wizard::~Wizard() {
	delete d;
}

QWizardPage *Wizard::createIntroPage() {
	QWizardPage *page = new QWizardPage;
	page->setTitle(trUtf8("설정 마법사 시작"));
	QLabel *label = new QLabel(trUtf8("이 마법사를 통해 "
			"CMPlayer를 이용하는데 있어서 꼭 필요하거나 중요한 사항들을 설정하게 될 것입니다."));
	label->setWordWrap(true);
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(label);
	page->setLayout(vbox);
	return page;
}

QWizardPage *Wizard::createBackendPage() {
	QWizardPage *page = new QWizardPage;
	page->setTitle(trUtf8("재생 엔진 선택"));
	page->setSubTitle(trUtf8("멀티미디어 재생을 위한 엔진을 선택해주세요."));
	BackendPage *w = new BackendPage;
	d->map = w->factories();
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(w);
	page->setLayout(vbox);
	return page;
}

}
