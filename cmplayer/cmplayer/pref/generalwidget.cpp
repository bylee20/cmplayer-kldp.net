#include "generalwidget.h"
#include "general.h"
#include "ui_generalwidget.h"
#include <QtGui/QFileDialog>

namespace Pref {

struct GeneralWidget::Data {
	Ui::Ui_GeneralWidget ui;
	General general;
};

GeneralWidget::GeneralWidget(const General &general, QWidget *parent)
: QWidget(parent) {
	d = new Data;
	d->general = general;
	d->ui.setupUi(this);
	d->ui.auto_add_combo->setCurrentIndex(d->general.autoAddFiles());
	d->ui.pause_check->setChecked(d->general.pauseWhenMinimized());
	d->ui.play_restored_check->setChecked(d->general.playWhenRestored());
	d->ui.restart_stopped_files_check->setChecked(d->general.playFromStoppedPoint());
	d->ui.reset_volume_check->setChecked(d->general.resetVolume());
	d->ui.volume_slider->setValue(d->general.initialVolume());
}

GeneralWidget::~GeneralWidget() {
	delete d;
}

const General &GeneralWidget::general() const {
	d->general.setAutoAddFiles(General::AutoAddFiles(d->ui.auto_add_combo->currentIndex()));
	d->general.setPauseWhenMinimized(d->ui.pause_check->isChecked());
	d->general.setPlayWhenRestored(d->ui.play_restored_check->isChecked());
	d->general.setPlayFromStoppedPoint(d->ui.restart_stopped_files_check->isChecked());
	d->general.setResetVolume(d->ui.reset_volume_check->isChecked());
	d->general.setInitialVolume(d->ui.volume_slider->value());
	return d->general;
}

}
