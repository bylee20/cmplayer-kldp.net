#include "wizard.h"
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <backend/manager.h>
#include <backend/wizardpage.h>
#include "wizardpages.h"
#include <backend/config.h>

namespace Pref {

struct Wizard::Data {
	enum {Intro = 0, Backend, Subtitle, Finish};
	Data(): backends(Backend::Manager::get()->loadAll()), selectedPages(0) {}
	const Backend::BackendList backends;
	QMap<Backend::BackendObject*, QList<Backend::WizardPage*> > pages;
	QList<Backend::WizardPage*> *selectedPages;
// 	int introId, backendId, endId;
};

Wizard::Wizard(QWidget *parent)
: QWizard(parent) {
	d = new Data;
	setPage(Data::Intro, new IntroPage);
	setPage(Data::Backend, new BackendPage);
	setPage(Data::Subtitle, new SubtitlePage);
	setPage(Data::Finish, new FinishPage);
	for (int i=0; i<d->backends.size(); ++i) {
		Backend::Config *config = d->backends[i]->config();
		QList<Backend::WizardPage*> pages = config->wizard();
		for (int j=0; j<pages.size(); ++j)
			setPage(pages[j]->id(), pages[j]);
		d->pages[d->backends[i]] = pages;
	}
	setWindowTitle(trUtf8("설정 마법사"));
	setPixmap(QWizard::LogoPixmap, QPixmap(":/img/tools-wizard.png"));
	setStartId(Data::Intro);
	setWizardStyle(ModernStyle);
}

Wizard::~Wizard() {
	delete d;
}

int Wizard::nextId() const {
	const int id = currentId();
	switch (id) {
	case Data::Intro:
		return Data::Backend;
	case Data::Backend: {
		d->selectedPages = 0;
		Backend::BackendObject *obj = static_cast<BackendPage*>(currentPage())->backend();
		if (obj) {
			d->selectedPages = &d->pages[obj];
			if (!d->selectedPages->isEmpty())
				return d->selectedPages->first()->id();
		}
		return Data::Subtitle;
	} case Data::Subtitle:
		return Data::Finish;
	case Data::Finish:
		return -1;
	default:
		if (!d->selectedPages || d->selectedPages->isEmpty())
			return -1;
		for (int i=0; i<d->selectedPages->size()-1; ++i) {
			if (d->selectedPages->at(i)->id() == id)
				return d->selectedPages->at(i+1)->id();
		}
		return Data::Subtitle;
	}
}

}
