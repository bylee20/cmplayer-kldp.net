
#include "subtitle.h"
#include "subtitlewidget.h"
#include "../ui/ui_subtitlewidget.h"

namespace Pref {

SubtitleWidget::SubtitleWidget(const Subtitle &subtitle, QWidget *parent)
: QWidget(parent), ui(new Ui::Ui_SubtitleWidget), m_subtitle(new Subtitle) {
	*m_subtitle = subtitle;
	ui->setupUi(this);

	static const QStringList encs = QStringList() << "UTF-8" << "Unicode"
		<< QObject::trUtf8("서유럽언어") + " (ISO-8859-1)"
		<< QObject::trUtf8("서유럽언어(유로)") + " (ISO-8859-15)"
		<< QObject::trUtf8("슬라브어/중앙유럽언어") + " (ISO-8859-2)"
		<< QObject::trUtf8("슬라브어/중앙유럽언어(Windows)") + " (CP1250)"
		<< QObject::trUtf8("에스페란토, 갈리시아어, 몰타어, 터키어") + " (ISO-8859-3)"
		<< QObject::trUtf8("구발트어 문자셋") + " (ISO-8859-4)"
		<< QObject::trUtf8("키릴문자") + " (ISO-8859-5)"
		<< QObject::trUtf8("키릴문자(Windows)") + " (CP1251)"
		<< QObject::trUtf8("아랍어") + " (ISO-8859-6)"
		<< QObject::trUtf8("현대 그리스어") + " (ISO-8859-7)"
		<< QObject::trUtf8("터키어") + " (ISO-8859-9)"
		<< QObject::trUtf8("발트어") + " (ISO-8859-13)"
		<< QObject::trUtf8("켈트어") + " (ISO-8859-14)"
		<< QObject::trUtf8("히브리어 문자셋") + " (ISO-8859-8)"
		<< QObject::trUtf8("러시아어") + " (KOI8-R)"
		<< QObject::trUtf8("우크라이나어, 벨로루시어") + " (KOI8-U/RU)"
		<< QObject::trUtf8("간체자 중국어 문자셋") + " (CP936)"
		<< QObject::trUtf8("번체자 중국어 문자셋") + " (BIG5)"
		<< QObject::trUtf8("일본어 문자셋") + " (SHIFT-JIS)"
		<< QObject::trUtf8("한국어 문자셋") + " (CP949)"
		<< QObject::trUtf8("타이 문자셋") + " (CP874)";

	ui->encoding_combo->addItems(encs);

	const int index = encs.indexOf(QRegExp(".* \\(" + m_subtitle->encoding.toUpper() + "\\)"));
	ui->encoding_combo->setEditText(index == -1 ? m_subtitle->encoding : encs[index]);
	ui->init_pos_spin->setValue(m_subtitle->initialPos);
	ui->display_on_margin_check->setChecked(m_subtitle->displayOnMarginWhenFullScreen);
	ui->auto_load_combo->setCurrentIndex(m_subtitle->autoLoad);
	ui->auto_select_combo->setCurrentIndex(m_subtitle->autoSelect);
	if (m_subtitle->priority.size()) {
		QString priority = m_subtitle->priority[0];
		for (int i=1; i<m_subtitle->priority.size(); ++i)
			priority += ',' + m_subtitle->priority[i];
		ui->priority_edit->setText(priority);
	}
	ui->osd_widget->setStyle(m_subtitle->osdStyle);
}

SubtitleWidget::~SubtitleWidget() {
	delete m_subtitle;
	delete ui;
}

const Subtitle &SubtitleWidget::subtitle() const {
	QString encoding = ui->encoding_combo->currentText().trimmed();
	static QRegExp rxEnc(".* \\((.*)\\)");
	m_subtitle->encoding = (rxEnc.indexIn(encoding) != -1) ? rxEnc.cap(1) : encoding;
	m_subtitle->initialPos = ui->init_pos_spin->value();
	m_subtitle->priority = ui->priority_edit->text().split(',');
	m_subtitle->autoLoad = static_cast<Subtitle::AutoLoad>(ui->auto_load_combo->currentIndex());
	m_subtitle->autoSelect =
			static_cast<Xine::SubtitleOutput::AutoSelect>(ui->auto_select_combo->currentIndex());
	m_subtitle->displayOnMarginWhenFullScreen = ui->display_on_margin_check->isChecked();
	m_subtitle->osdStyle = ui->osd_widget->style();
	return *m_subtitle;
}

}
