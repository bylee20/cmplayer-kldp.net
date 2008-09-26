#include "selectwidget.h"
#include "ui_selectwidget.h"
#include "backend.h"
#include "info.h"

namespace Backend {

struct SelectWidget::Data {
	Data(): map(Backend::Manager::get()->loadAll()) {}
	Ui::SelectWidget ui;
	const Backend::FactoryMap map;
	QString selected;
};

SelectWidget::SelectWidget(QWidget *parent)
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

SelectWidget::~SelectWidget() {
	delete d;
}

void SelectWidget::slotSelected() {
	QList<QTreeWidgetItem*> items = d->ui.list->selectedItems();
	if (items.isEmpty()) {
		d->selected.clear();
		d->ui.backend_label->setText(trUtf8("없음"));
		emit selected(QString::null);
		emit selected(0);
	} else {
		d->selected = items[0]->data(0, Qt::UserRole).toString();
		Backend::FactoryIface *f = d->map[d->selected];
		d->ui.backend_label->setText(f->info()->name());
		d->ui.info_label->setText(f->info()->description());
		emit selected(d->selected);
		emit selected(f);
	}
}

const Backend::FactoryMap &SelectWidget::factories() const {
	return d->map;
}

void SelectWidget::slotActivated(QTreeWidgetItem *item) {
	if (item) {
		const QString fileName = item->data(0, Qt::UserRole).toString();
		emit activated(fileName);
		emit activated(d->map[fileName]);
	}
}


QString SelectWidget::selectedFileName() const {
	return d->selected;
}

Backend::FactoryIface *SelectWidget::selectedObject() const {
	return d->map[d->selected];
}

}
