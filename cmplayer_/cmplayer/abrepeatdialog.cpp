#include "abrepeatdialog.h"
#include "ui/ui_abrepeatdialog.h"
#include <xine/abrepeater.h>
#include <xine/utility.h>

struct ABRepeatDialog::Data {
	ABRepeatDialog *p;
	Ui::Ui_ABRepeatDialog ui;
	Xine::ABRepeater *repeater;
};

ABRepeatDialog::ABRepeatDialog(Xine::ABRepeater *repeater, QWidget *parent)
: QDialog(parent), d(new Data) {
	d->repeater = repeater;
	d->ui.setupUi(this);
	connect(d->ui.a_time_button, SIGNAL(clicked()), this, SLOT(getAFromTime()));
	connect(d->ui.b_time_button, SIGNAL(clicked()), this, SLOT(getBFromTime()));
	connect(d->ui.a_subtitle_button, SIGNAL(clicked()), this, SLOT(getAFromSubtitle()));
	connect(d->ui.b_subtitle_button, SIGNAL(clicked()), this, SLOT(getBFromSubtitle()));
	connect(d->ui.quit_button, SIGNAL(clicked()), repeater, SLOT(stop()));
	connect(d->ui.start_button, SIGNAL(clicked()), this, SLOT(start()));
}

ABRepeatDialog::~ABRepeatDialog() {
	delete d;
}

void ABRepeatDialog::start() {
	int a = Xine::Utility::timeToMSecs(d->ui.a_time_edit->time());
	int b = Xine::Utility::timeToMSecs(d->ui.b_time_edit->time());
	int dif = d->ui.add_time_spin->value()*1000;
	d->repeater->repeat(a - dif, b+ dif, d->ui.times_spin->value());
}

void ABRepeatDialog::getAFromTime() {
	d->ui.a_time_edit->setTime(Xine::Utility::msecsToTime(d->repeater->setAToCurrentTime()));
}

void ABRepeatDialog::getAFromSubtitle() {
	d->ui.a_time_edit->setTime(Xine::Utility::msecsToTime(d->repeater->setAToSubtitleTime()));
}

void ABRepeatDialog::getBFromTime() {
	d->ui.b_time_edit->setTime(Xine::Utility::msecsToTime(d->repeater->setBToCurrentTime()));
}

void ABRepeatDialog::getBFromSubtitle() {
	d->ui.b_time_edit->setTime(Xine::Utility::msecsToTime(d->repeater->setBToSubtitleTime()));
}

