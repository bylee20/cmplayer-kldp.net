#include "backendwidget.h"
#include "ui_backendwidget.h"
#include <backend/info.h>
#include <backend/manager.h>
#include <backend/config.h>
#include <QtCore/QMap>
#include <QtGui/QRadioButton>
#include <QtGui/QButtonGroup>
#include "backend.h"

namespace Pref {

struct BackendWidget::Data {
	Ui::Ui_BackendWidget ui;
	Backend::FactoryMap factories;
	BackendPref pref;
};

BackendWidget::BackendWidget(QWidget *parent)
: QWidget(parent) {
	d = new Data;
	d->ui.setupUi(this);
	d->factories = Backend::Manager::get()->loadAll();
	foreach(QString fileName, d->factories.keys())
		addBackend(fileName);
}

BackendWidget::~BackendWidget() {
	delete d;
}

const BackendPref &BackendWidget::backend() const {
	d->pref.setFileName(d->factories.keys()[d->ui.combo->currentIndex()]);
	return d->pref;
}

void BackendWidget::addBackend(const QString &fileName) {
	Backend::FactoryIface *f = d->factories[fileName];
	Backend::Info *info = f->info();
	
	d->ui.combo->addItem(info->name());
	
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setText(0, info->name() + '(' + fileName + ')');
	d->ui.list->addTopLevelItem(item);
	
	d->ui.tabs->addTab(f->config()->widget(this), info->name());
}

void BackendWidget::save() {
	Backend::FactoryMap::iterator it = d->factories.begin();
	for (int i=1; i<d->ui.tabs->count() && it != d->factories.end(); ++i, ++it) {
		QWidget *w = d->ui.tabs->widget(i);
		it.value()->config()->update(w);
		it.value()->config()->save();
	}
}

}
