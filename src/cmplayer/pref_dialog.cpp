#include "pref_dialog.hpp"
#include "pref_widget.hpp"
#include <QtGui/QToolBar>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QActionGroup>

struct Pref::Dialog::Data {
	Widget *widget;
};

Pref::Dialog::Dialog(QWidget *parent)
 : QDialog(parent), d(new Data) {
	QToolBar *tb = new QToolBar(this);
	d->widget = new Widget(this);
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->setSpacing(0);
	vbox->addWidget(tb);
	vbox->addWidget(d->widget);
	QActionGroup *g = new QActionGroup(this);
	for (int i=0; i<d->widget->pageCount(); ++i) {
		QAction *act = tb->addAction(d->widget->pageIcon(i), d->widget->pageName(i));
		act->setData(i);
		act->setCheckable(true);
		g->addAction(act);
	}
	g->actions()[d->widget->currentPage()]->setChecked(true);
	connect(g, SIGNAL(triggered(QAction*)), this, SLOT(setCurrentPage(QAction*)));
	tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	QDialogButtonBox *dbb = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	dbb->setContentsMargins(11, 0, 11, 11);
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(dbb);
	hbox->setMargin(0);
	vbox->addLayout(hbox);
	setLayout(vbox);

	setFixedSize(sizeHint());
}

void Pref::Dialog::setCurrentPage(QAction *action) {
	if (!action || !action->isChecked())
		return;
	d->widget->setCurrentPage(action->data().toInt());
}


void Pref::Dialog::accept() {
	d->widget->apply();
	QDialog::accept();
}
