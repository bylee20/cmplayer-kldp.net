#include "configwidget.h"
#include "private/ui_configwidget.h"
#include "info.h"

namespace Backend {

struct ConfigWidget::Data {
	Ui::Ui_ConfigWidget ui;
};

ConfigWidget::ConfigWidget(const Info &info, QWidget *parent)
: QWidget(parent) {
	d = new Data;
	d->ui.setupUi(this);
	d->ui.ao_combo->addItems(info.audioDrivers());
	d->ui.volume_amp_widget->setVisible(info.supportsVolumeAmplification());
	d->ui.volume_amp_spin->setRange(0, qRound(info.maximumVolumeAmplification()*100));
	d->ui.vo_combo->addItems(info.videoDrivers());
}

ConfigWidget::~ConfigWidget() {
	delete d;
}

void ConfigWidget::setVolumeAmplification(double volAmp) {
	d->ui.volume_amp_spin->setValue(qRound(volAmp*100));
}

void ConfigWidget::setVideoDriver(const QString &driver) {
	const int idx = d->ui.vo_combo->findText(driver);
	if (idx != -1)
		d->ui.vo_combo->setCurrentIndex(idx);
}

void ConfigWidget::setAudioDriver(const QString &driver) {
	const int idx = d->ui.ao_combo->findText(driver);
	if (idx != -1)
		d->ui.ao_combo->setCurrentIndex(idx);
}

double ConfigWidget::volumeAmplification() const {
	return static_cast<double>(d->ui.volume_amp_spin->value())/100.0;
}

QString ConfigWidget::videoDriver() const {
	return d->ui.vo_combo->currentText();
}

QString ConfigWidget::audioDriver() const {
	return d->ui.ao_combo->currentText();
}

}
