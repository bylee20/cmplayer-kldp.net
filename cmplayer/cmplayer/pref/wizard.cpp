#include "wizard.h"
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <backend/manager.h>
#include <backend/wizardpage.h>
#include "wizardpages.h"
#include <backend/config.h>

namespace Pref {

struct Wizard::Data {
	enum {Intro = 0, Backend, Subtitle, End};
	Data(): map(Backend::Manager::get()->loadAll()), backendPages(0) {}
	const Backend::FactoryMap map;
	QMap<QString, QList<Backend::WizardPage*> > pages;
	QList<Backend::WizardPage*> *backendPages;
// 	int introId, backendId, endId;
};

Wizard::Wizard(QWidget *parent)
: QWizard(parent) {
	d = new Data;
	setPage(Data::Intro, new IntroPage);
	setPage(Data::Backend, new BackendPage);
	setPage(Data::Subtitle, new SubtitlePage);
	setPage(Data::End, new QWizardPage);
	Backend::FactoryMap::const_iterator it = d->map.constBegin();
	for (; it != d->map.constEnd(); ++it) {
		Backend::Config *config = it.value()->config();
		QList<Backend::WizardPage*> pages = config->wizard();
		for (int i=0; i<pages.size(); ++i)
			setPage(pages[i]->id(), pages[i]);
		d->pages[it.key()] = pages;
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
		d->backendPages = 0;
		QString fileName = static_cast<BackendPage*>(currentPage())->backend();
		if (fileName.isEmpty())
			return Data::Subtitle;
		d->backendPages = &d->pages[fileName];
		if (d->backendPages->isEmpty())
			return Data::Subtitle;
		return d->backendPages->first()->id();
	} case Data::Subtitle:
		return Data::End;
	case Data::End:
		return -1;
	default:
		if (!d->backendPages || d->backendPages->isEmpty())
			return -1;
		for (int i=0; i<d->backendPages->size()-1; ++i) {
			if (d->backendPages->at(i)->id() == id)
				return d->backendPages->at(i+1)->id();
		}
		return Data::Subtitle;
	}
}

}
