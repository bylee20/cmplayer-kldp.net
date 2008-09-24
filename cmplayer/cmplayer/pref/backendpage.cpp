#include "backendpage.h"
#include "ui_backendpage.h"
#include "backend.h"
#include <backend/info.h>

namespace Pref {

struct BackendPage::Data {
	Data(): map(Backend::Manager::get()->loadAll()) {}
	Ui::BackendPage ui;
	const Backend::FactoryMap map;
};

BackendPage::BackendPage(QWidget *parent)
: QWidget(parent) {
	d = new Data;
	d->ui.setupUi(this);
	Backend::FactoryMap::const_iterator it = d->map.begin();
	for (; it != d->map.end(); ++it) {
		const Backend::FactoryIface *f = it.value();
		Backend::Info *info = f->info();
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(0, info->name() + '(' + it.key() + ')');
		item->setData(0, Qt::UserRole, it.key());
		d->ui.list->addTopLevelItem(item);
	}
	d->ui.info_label->hide();
	adjustSize();
	connect(d->ui.list, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelected()));
	connect(d->ui.list, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int))
			, this, SLOT(slotActivated(QTreeWidgetItem*)));
}

BackendPage::~BackendPage() {
	delete d;
}

void BackendPage::slotSelected() {
	QList<QTreeWidgetItem*> items = d->ui.list->selectedItems();
	if (items.isEmpty()) {
		d->ui.backend_label->setText(trUtf8("없음"));
	} else {
		const QString key = items[0]->data(0, Qt::UserRole).toString();
		const Backend::FactoryIface *f = d->map[key];
		d->ui.backend_label->setText(f->info()->name());
		d->ui.info_label->setText(f->info()->description());
	}
}

const Backend::FactoryMap &BackendPage::factories() const {
	return d->map;
}

void BackendPage::slotActivated(QTreeWidgetItem *item) {
	if (item) {
		const QString fileName = item->data(0, Qt::UserRole).toString();
		emit backendSelected(fileName);
		emit backendSelected(d->map[fileName]);
	}
}

}
