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
	connect(d->ui.quit, SIGNAL(clicked()), this, SLOT(quit()));
	setStopped();
}

ABRepeatDialog::~ABRepeatDialog() {
	delete d;
}

void ABRepeatDialog::start() {
	if (d->repeater) {
		const int a = Core::Utility::timeToMSecs(d->ui.a->time());
		const int b = Core::Utility::timeToMSecs(d->ui.b->time());
		const int dif = qRound(d->ui.add->value()*1000);
		const int times = d->ui.times->value();
		if (d->repeater->repeat(a - dif, b+ dif, times > 0 ? times : -1))
			setStopped(false);
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
		disconnect(d->repeater, 0, this, 0);
	}
	if ((d->repeater = repeater)) {
		if (d->repeater->hasA())
			d->ui.a->setTime(Core::Utility::msecsToTime(d->repeater->a()));
		if (d->repeater->hasB())
			d->ui.b->setTime(Core::Utility::msecsToTime(d->repeater->b()));
		if (d->repeater->times() > 0)
			d->ui.times->setValue(d->repeater->times());
		else
			d->ui.times->setValue(0);
		setRest(d->repeater->restTimes());
		connect(d->repeater, SIGNAL(repeated(int)), this, SLOT(setRest(int)));
		connect(d->repeater, SIGNAL(stopped()), this, SLOT(setStopped()));
		setStopped(!d->repeater->isRepeating());
	}
}

void ABRepeatDialog::quit() {
	if (d->repeater)
		d->repeater->stop();
}

void ABRepeatDialog::hideEvent(QHideEvent */*event*/) {
	quit();
	setRepeater(0);
}

void ABRepeatDialog::showEvent(QShowEvent */*event*/) {
	d->ui.retranslateUi(this);
}

void ABRepeatDialog::setRest(int rest) {
	if (rest < 0)
		d->ui.rest->setText(tr("Infinite Repetition"));
	else
		d->ui.rest->setText(tr("Rest: %1 times").arg(rest));
}

void ABRepeatDialog::setStopped(bool stopped) {
	d->ui.start->setEnabled(stopped);
	d->ui.quit->setEnabled(!stopped);
}
