#include "subviewdialog.h"
#include "videoplayer.h"
#include "subcompview.h"
#include <core/subtitle.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QDialogButtonBox>


struct SubViewDialog::Data {
	QList<SubCompView*> view;
	VideoPlayer *player;
	Core::Subtitle sub;
	double frameRate;
	QHBoxLayout *viewBox;
	QCheckBox *startEnd, *autoScroll;
};

SubViewDialog::SubViewDialog(VideoPlayer *player, QWidget *parent)
: QDialog(parent), d(new Data) {
	d->frameRate = -1.0;
	d->player = player;
	d->startEnd = new QCheckBox(tr("Show start/end point."), this);
	d->autoScroll = new QCheckBox(tr("Turn on autoscroll."), this);
	d->autoScroll->setChecked(true);
	QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(d->autoScroll);
	hbox->addWidget(d->startEnd);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	hbox->addWidget(button);
	QVBoxLayout *vbox = new QVBoxLayout(this);
	d->viewBox = new QHBoxLayout;
	vbox->addLayout(d->viewBox);
	vbox->addLayout(hbox);

	connect(button, SIGNAL(rejected()), this, SLOT(reject()));
	connect(d->startEnd, SIGNAL(toggled(bool)), this, SLOT(setStartEndVisible(bool)));
	connect(d->player, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
}

SubViewDialog::~SubViewDialog() {
	delete d;
}

void SubViewDialog::slotTick(int time) {
	if (isVisible()) {
		const bool scroll = d->autoScroll->isChecked();
		for (int i=0; i<d->view.size(); ++i)
			d->view[i]->setCurrent(time, d->player->frameRate(), scroll);
	}
}

void SubViewDialog::showCurrentSubtitle() {
	d->sub = d->player->subtitle();
	d->frameRate = d->player->frameRate();

	while (d->view.size() > d->sub.count())
		delete d->view.takeLast();
	while (d->view.size() < d->sub.count()) {
		SubCompView *view = new SubCompView(this);
		view->setStartEndVisible(d->startEnd->isChecked());
		d->viewBox->addWidget(view);
		d->view.append(view);
	}
	for (int i=0; i<d->sub.count(); ++i)
		d->view[i]->setComponent(d->sub[i]);
}

void SubViewDialog::setStartEndVisible(bool visible) {
	for (int i=0; i<d->view.size(); ++i)
		d->view[i]->setStartEndVisible(visible);
}
