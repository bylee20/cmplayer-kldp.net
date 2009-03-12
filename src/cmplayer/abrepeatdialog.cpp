#include "abrepeatdialog.h"
#include "ui_abrepeatdialog.h"
#include <core/abrepeater.h>
#include <core/utility.h>

struct ABRepeatDialog::Data {
	Ui::ABRepeatDialog ui;
	Core::ABRepeater *repeater;
};

ABRepeatDialog::ABRepeatDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->repeater = 0;
	d->ui.setupUi(this);
	d->ui.a->setDisplayFormat("h:mm:ss.zzz");
	d->ui.b->setDisplayFormat("h:mm:ss.zzz");
	connect(d->ui.aTime, SIGNAL(clicked()), this, SLOT(getAFromTime()));
	connect(d->ui.bTime, SIGNAL(clicked()), this, SLOT(getBFromTime()));
	connect(d->ui.aSub, SIGNAL(clicked()), this, SLOT(getAFromSubtitle()));
	connect(d->ui.bSub, SIGNAL(clicked()), this, SLOT(getBFromSubtitle()));
	connect(d->ui.start, SIGNAL(clicked()), this, SLOT(start()));
}

ABRepeatDialog::~ABRepeatDialog() {
	delete d;
}

void ABRepeatDialog::start() {
	if (d->repeater) {
		const int a = Core::Utility::timeToMSecs(d->ui.a->time());
		const int b = Core::Utility::timeToMSecs(d->ui.b->time());
		const int dif = qRound(d->ui.add->value()*1000);
		d->repeater->repeat(a - dif, b+ dif, d->ui.times->value());
	}
}

void ABRepeatDialog::getAFromTime() {
	if (d->repeater)
		d->ui.a->setTime(Core::Utility::msecsToTime(d->repeater->setAToCurrentTime()));
}

void ABRepeatDialog::getAFromSubtitle() {
	if (d->repeater)
		d->ui.a->setTime(Core::Utility::msecsToTime(d->repeater->setAToSubtitleTime()));
}

void ABRepeatDialog::getBFromTime() {
	if (d->repeater)
		d->ui.b->setTime(Core::Utility::msecsToTime(d->repeater->setBToCurrentTime()));
}

void ABRepeatDialog::getBFromSubtitle() {
	if (d->repeater)
		d->ui.b->setTime(Core::Utility::msecsToTime(d->repeater->setBToSubtitleTime()));
}

void ABRepeatDialog::setRepeater(Core::ABRepeater *repeater) {
	if (d->repeater && d->repeater->isRepeating()) {
		d->repeater->stop();
		disconnect(d->ui.quit, SIGNAL(clicked()), d->repeater, SLOT(stop()));
	}
	d->repeater = repeater;
	connect(d->ui.quit, SIGNAL(clicked()), repeater, SLOT(stop()));
}
