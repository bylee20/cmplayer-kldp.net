#include "generalwidget.h"
#include "general.h"
#include "../ui/ui_generalwidget.h"
#include <xine/informations.h>
#include <QFileDialog>

namespace Pref {

GeneralWidget::GeneralWidget(const General &general, QWidget *parent)
: QWidget(parent), ui(new Ui::Ui_GeneralWidget), m_general(new General) {
	*m_general = general;
	ui->setupUi(this);

	static Xine::Informations *info = Xine::Informations::get();

	ui->auto_add_combo->setCurrentIndex(m_general->autoAddFiles);
	ui->pause_check->setChecked(m_general->pauseWhenMinimized);
	ui->play_restored_check->setChecked(m_general->playWhenRestored);
	ui->restart_stopped_files_check->setChecked(m_general->playFromStoppedPoint);

	int index = -1;
	ui->vo_combo->addItems(info->videoOutputs());
	if ((index = info->videoOutputs().indexOf(m_general->videoOutput)) != -1)
		ui->vo_combo->setCurrentIndex(index);

	ui->ao_combo->addItems(info->audioOutputs());
	if ((index = info->audioOutputs().indexOf(m_general->audioOutput)) != -1)
		ui->ao_combo->setCurrentIndex(index);
	ui->amplification_spin->setValue(m_general->volumeAmplification);
	ui->reset_volume_check->setChecked(m_general->resetVolume);
	ui->volume_slider->setValue(m_general->initialVolume);
}

GeneralWidget::~GeneralWidget() {
	delete ui;
	delete m_general;
}

const General &GeneralWidget::general() const {
	m_general->autoAddFiles = General::AutoAddFiles(ui->auto_add_combo->currentIndex());
	m_general->pauseWhenMinimized = ui->pause_check->isChecked();
	m_general->playWhenRestored = ui->play_restored_check->isChecked();
	m_general->playFromStoppedPoint = ui->restart_stopped_files_check->isChecked();

	m_general->videoOutput = ui->vo_combo->currentText().trimmed();

	m_general->audioOutput = ui->ao_combo->currentText().trimmed();
	m_general->volumeAmplification = ui->amplification_spin->value();
	m_general->resetVolume = ui->reset_volume_check->isChecked();
	m_general->initialVolume = ui->volume_slider->value();

	return *m_general;
}

}
