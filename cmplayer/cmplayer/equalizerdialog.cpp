#include "equalizerdialog.h"
#include "ui_equalizerdialog.h"
#include <backend/videooutput.h>

struct EqualizerDialog::Data {
	static const int MaxValue = 100;
	Ui::Ui_EqualizerDialog ui;
	Backend::VideoOutput *video;
	int brightness;
	int contrast;
	int gamma;
	int hue;
	int saturation;
};

EqualizerDialog::EqualizerDialog(Backend::VideoOutput *video, QWidget *parent)
: QDialog(parent), d(new Data) {
	d->video = video;
	d->brightness = d->video->brightness();
	d->contrast = d->video->contrast();
	d->hue = d->video->hue();
	d->saturation = d->video->saturation();

	d->ui.setupUi(this);

	d->ui.brightness_spin->setValue(to200(d->brightness));
	d->ui.contrast_spin->setValue(to200(d->contrast));
	d->ui.hue_spin->setValue(to200(d->hue));
	d->ui.saturation_spin->setValue(to200(d->saturation));

	connect(d->ui.brightness_spin, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
	connect(d->ui.contrast_spin, SIGNAL(valueChanged(int)), this, SLOT(setContrast(int)));
	connect(d->ui.hue_spin, SIGNAL(valueChanged(int)), this, SLOT(setHue(int)));
	connect(d->ui.saturation_spin, SIGNAL(valueChanged(int)), this, SLOT(setSaturation(int)));
	connect(d->ui.reset_button, SIGNAL(clicked()), this, SLOT(resetValues()));
	connect(d->ui.default_button, SIGNAL(clicked()), this, SLOT(setValuesDefault()));
}

EqualizerDialog::~EqualizerDialog() {
	delete d;
}

void EqualizerDialog::setBrightness(int value) {
	d->video->setBrightness(to1(value));
	d->ui.brightness_spin->setValue(to200(d->video->brightness()));
}

void EqualizerDialog::setContrast(int value) {
	d->video->setContrast(to1(value));
	d->ui.contrast_spin->setValue(to200(d->video->contrast()));
}

void EqualizerDialog::setHue(int value) {
	d->video->setHue(to1(value));
	d->ui.hue_spin->setValue(to200(d->video->hue()));
}

void EqualizerDialog::setSaturation(int value) {
	d->video->setSaturation(to1(value));
	d->ui.saturation_spin->setValue(to200(d->video->saturation()));
}

void EqualizerDialog::showEvent(QShowEvent *event) {
	QDialog::showEvent(event);
	d->brightness = d->ui.brightness_spin->value();
	d->contrast = d->ui.contrast_spin->value();
	d->hue = d->ui.hue_spin->value();
	d->saturation = d->ui.saturation_spin->value();
}

void EqualizerDialog::resetValues() {
	d->ui.brightness_spin->setValue(d->brightness);
	d->ui.contrast_spin->setValue(d->contrast);
	d->ui.hue_spin->setValue(d->hue);
	d->ui.saturation_spin->setValue(d->saturation);
}

void EqualizerDialog::setValuesDefault() {
	d->ui.brightness_spin->setValue(0);
	d->ui.contrast_spin->setValue(0);
	d->ui.hue_spin->setValue(0);
	d->ui.saturation_spin->setValue(0);
}
