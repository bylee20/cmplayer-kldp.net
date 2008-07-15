#include "generalwidget.h"
#include "general.h"
#include <ui/ui_generalwidget.h>
//#include <mplayer/informations.h>
#include <QFileDialog>

namespace Pref {

GeneralWidget::GeneralWidget(const General &general, QWidget *parent)
: QWidget(parent), ui(new Ui::Ui_GeneralWidget), m_general(new General) {
	*m_general = general;
	ui->setupUi(this);

	connect(ui->find_mplayer_button, SIGNAL(clicked()), this, SLOT(findMPlayer()));

//	static MPlayer::Informations *info = MPlayer::Informations::get();

	ui->mplayer_path_edit->setText(m_general->mplayerPath);
	ui->auto_add_combo->setCurrentIndex(m_general->autoAddFiles);
	ui->add_all_titles_check->setChecked(m_general->addAllDVDTitles);
	ui->pause_check->setChecked(m_general->pauseWhenMinimized);
	ui->play_restored_check->setChecked(m_general->playWhenRestored);
	ui->additional_options_edit->setText(m_general->additionalOptions.join(" "));
	ui->restart_stopped_files_check->setChecked(m_general->playFromStoppedPoint);

	int index = -1;
//	ui->vo_combo->addItems(info->videoOutputs());
//	if ((index = info->videoOutputs().indexOf(m_general->videoOutput)) == -1)
//		ui->vo_combo->setEditText(m_general->videoOutput);
//	else
//		ui->vo_combo->setCurrentIndex(index);
	ui->software_equalizer_check->setChecked(m_general->useSoftwareEqualizer);

//	ui->ao_combo->addItems(info->audioOutputs());
//	if ((index = info->audioOutputs().indexOf(m_general->audioOutput)) == -1)
//		ui->ao_combo->setEditText(m_general->audioOutput);
//	else
//		ui->ao_combo->setCurrentIndex(index);
	ui->software_volume_check->setChecked(m_general->useSoftwareVolume);
	ui->amplification_spin->setValue(m_general->volumeAmplification);
	ui->reset_volume_check->setChecked(m_general->resetVolume);
	ui->volume_slider->setValue(m_general->initialVolume);
	ui->auto_pitch_check->setChecked(m_general->autoPitch);
}

GeneralWidget::~GeneralWidget() {
	delete ui;
	delete m_general;
}

void GeneralWidget::findMPlayer() {
	QString path = QFileDialog::getOpenFileName(this);
	if (!path.isEmpty())
		ui->mplayer_path_edit->setText(path);
}

const General &GeneralWidget::general() const {
	m_general->mplayerPath = ui->mplayer_path_edit->text().trimmed();
	m_general->autoAddFiles = General::AutoAddFiles(ui->auto_add_combo->currentIndex());
	m_general->addAllDVDTitles = ui->add_all_titles_check->isChecked();
	m_general->pauseWhenMinimized = ui->pause_check->isChecked();
	m_general->playWhenRestored = ui->play_restored_check->isChecked();
	m_general->additionalOptions = ui->additional_options_edit->text().trimmed().split(' ');
	m_general->playFromStoppedPoint = ui->restart_stopped_files_check->isChecked();

	m_general->videoOutput = ui->vo_combo->currentText().trimmed();
	m_general->useSoftwareEqualizer = ui->software_equalizer_check->isChecked();

	m_general->audioOutput = ui->ao_combo->currentText().trimmed();
	m_general->useSoftwareVolume = ui->software_volume_check->isChecked();
	m_general->volumeAmplification = ui->amplification_spin->value();
	m_general->resetVolume = ui->reset_volume_check->isChecked();
	m_general->initialVolume = ui->volume_slider->value();
	m_general->autoPitch = ui->auto_pitch_check->isChecked();

	return *m_general;
}

}
