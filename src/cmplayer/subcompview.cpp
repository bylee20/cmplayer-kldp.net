#include "subcompview.h"
#include <QtGui/QTreeWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtCore/QMap>
#include <QtGui/QVBoxLayout>
#include <QtGui/QCheckBox>

struct SubCompView::Data {
	Core::Subtitle::Component::Base base;
	QTreeWidgetItem *current, *selected, *hovered;
	QMap<int, QTreeWidgetItem*> item;
	QTreeWidget *tree;
	QLabel *name;
};

SubCompView::SubCompView(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->base = Core::Subtitle::Component::Time;
	d->current = d->hovered = d->selected = 0;

	d->tree = new QTreeWidget(this);
	d->tree->setMouseTracking(true);
	d->tree->setSelectionMode(QTreeWidget::NoSelection);

	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setText(Text, tr("Text"));
	item->setText(Start, tr("Start"));
	item->setText(End, tr("End"));
	d->tree->setHeaderItem(item);
	d->tree->setRootIsDecorated(false);

	d->name = new QLabel(this);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->name);
	vbox->addWidget(d->tree);

	connect(d->tree, SIGNAL(itemEntered(QTreeWidgetItem*,int))
		, this, SLOT(slotEntered(QTreeWidgetItem*,int)));
}

SubCompView::~SubCompView() {
	delete d;
}

void SubCompView::setComponent(const Core::Subtitle::Component &comp) {
	d->item.clear();
	d->tree->clear();
	d->current = d->hovered = d->selected = 0;

	d->base = comp.base();
	d->name->setText(comp.name());
	QTreeWidgetItem *prev = 0;
	for (CompConstIt it = comp.begin(); it != comp.end(); ++it) {
		if (prev) {
			prev->setText(End, QString::number(it.key()));
			prev = 0;
		}
		static QRegExp rxTag("<\\s*[a-zA-Z][^>]*>");
		QString text = it.value();
		if (text.remove(rxTag).remove('\n').trimmed().isEmpty())
			continue;
		QTreeWidgetItem *item = new QTreeWidgetItem;
		item->setText(Start, QString::number(it.key()));
		item->setText(Text, QString(it.value()).remove(rxTag));
		d->item[it.key()] = item;
		d->tree->addTopLevelItem(item);
		prev = item;
	}
	d->tree->resizeColumnToContents(Text);
	d->tree->resizeColumnToContents(Start);
	d->tree->resizeColumnToContents(End);
}

void SubCompView::slotEntered(QTreeWidgetItem *item, int column) {
	if (!item || column != Text)
		return;
	if (d->hovered && d->selected != d->hovered) {
		d->tree->removeItemWidget(d->hovered, Text);
		d->hovered = 0;
	}
	QLineEdit *edit = new QLineEdit(item->text(Text), this);
	edit->setAutoFillBackground(true);
	edit->setReadOnly(true);
	edit->setFrame(false);
	edit->setContentsMargins(0, -1, 0, -1);
	d->tree->setItemWidget(item, Text, edit);
	d->hovered = item;
	connect(edit, SIGNAL(selectionChanged()), this, SLOT(slotTextSelected()));
}

void SubCompView::slotTextSelected() {
	if (d->selected != d->hovered && d->selected)
		d->tree->removeItemWidget(d->selected, Text);
	d->selected = d->hovered;
}

void SubCompView::setCurrent(int time, double frameRate, bool scroll) {
	if (d->item.isEmpty() || time < 0 )
		return;
	int key = time;
	if (d->base == Core::Subtitle::Component::Frame) {
		if (frameRate < 0.0)
			return;
		key = qRound(time*0.001*frameRate);
	}
	QMap<int, QTreeWidgetItem*>::const_iterator it = d->item.upperBound(key);
	if (it != d->item.begin())
		--it;
	QTreeWidgetItem *item = it.value();
	if (item != d->current) {
		static const QBrush nb(qRgb(200, 200, 255));
		static const QBrush ob = item->background(0);
		if (d->current) {
			d->current->setBackground(Start, ob);
			d->current->setBackground(End, ob);
			d->current->setBackground(Text, ob);
		}
		d->current = item;
		item->setBackground(Start, nb);
		item->setBackground(End, nb);
		item->setBackground(Text, nb);
		if (scroll)
			d->tree->scrollToItem(item, QTreeWidget::PositionAtCenter);
	}
}

void SubCompView::setStartEndVisible(bool visible) {
	d->tree->setColumnHidden(Start, !visible);
	d->tree->setColumnHidden(End, !visible);
}
