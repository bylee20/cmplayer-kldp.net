#include "selectwidget.h"
#include "ui_selectwidget.h"
#include "backend.h"

namespace Backend {

struct SelectWidget::Data {
	Data(): backends(Backend::Manager::get()->loadAll()), idx(-1) {}
	Ui::SelectWidget ui;
	const BackendList &backends;
	int idx;
};

SelectWidget::SelectWidget(QWidget *parent)
: QWidget(parent) {
	d = new Data;
	d->ui.setupUi(this);
	for (int i=0; i<d->backends.size(); ++i) {
		const Backend::BackendObject *obj = d->backends[i];
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(0, obj->name() + '(' + obj->fileName() + ')');
		item->setData(0, Qt::UserRole, i);
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

void SelectWidget::setBackend(BackendObject *obj) {
	const int idx = d->backends.indexOf(obj);
	if (idx != -1) {
		QTreeWidgetItem *item = d->ui.list->topLevelItem(idx);
		d->ui.list->setCurrentItem(item);
	}
}

void SelectWidget::slotSelected() {
	QList<QTreeWidgetItem*> items = d->ui.list->selectedItems();
	if (items.isEmpty()) {
		d->idx = -1;
		d->ui.backend_label->setText(trUtf8("없음"));
		emit selected(0);
	} else {
		d->idx = items[0]->data(0, Qt::UserRole).toInt();
		Backend::BackendObject *obj = d->backends[d->idx];
		d->ui.backend_label->setText(obj->name());
		d->ui.info_label->setText(obj->description());
		emit selected(obj);
	}
}

const BackendList &SelectWidget::backends() const {
	return d->backends;
}

void SelectWidget::slotActivated(QTreeWidgetItem *item) {
	if (item) {
		const int idx = item->data(0, Qt::UserRole).toInt();
		emit activated(d->backends[idx]);
	}
}

Backend::BackendObject *SelectWidget::backend() const {
	return (d->idx == -1) ? 0 : d->backends[d->idx];
}

}
