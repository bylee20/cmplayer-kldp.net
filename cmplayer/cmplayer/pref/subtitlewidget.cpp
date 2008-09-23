#include "subtitle.h"
#include "subtitlewidget.h"
#include "ui_subtitlewidget.h"

namespace Pref {

struct SubtitleWidget::Data {
	Ui::Ui_SubtitleWidget ui;
	mutable Subtitle sub;
};

SubtitleWidget::SubtitleWidget(const Subtitle &subtitle, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->sub = subtitle;
	d->ui.setupUi(this);

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

	d->ui.encoding_combo->addItems(encs);

	static const QRegExp rxEncoding(".* \\(" + d->sub.encoding().toUpper() + "\\)");
	const int idx = encs.indexOf(rxEncoding);
	d->ui.encoding_combo->setEditText(idx == -1 ? d->sub.encoding() : encs[idx]);
	d->ui.init_pos_spin->setValue(d->sub.initialPos());
	d->ui.auto_load_combo->setCurrentIndex(d->sub.autoLoad());
	d->ui.auto_select_combo->setCurrentIndex(d->sub.autoSelect());
	if (d->sub.priority().size()) {
		QString priority = d->sub.priority()[0];
		for (int i=1; i<d->sub.priority().size(); ++i)
			priority += ',' + d->sub.priority()[i];
		d->ui.priority_edit->setText(priority);
	}
	d->ui.osd_widget->setStyle(d->sub.osdStyle());
}

SubtitleWidget::~SubtitleWidget() {
	delete d;
}

const Subtitle &SubtitleWidget::subtitle() const {
	QString encoding = d->ui.encoding_combo->currentText().trimmed();
	static QRegExp rxEnc(".* \\((.*)\\)");
	d->sub.setEncoding((rxEnc.indexIn(encoding) != -1) ? rxEnc.cap(1) : encoding);
	d->sub.setInitialPos(d->ui.init_pos_spin->value());
	d->sub.setPriority(d->ui.priority_edit->text().split(','));
	int idx = d->ui.auto_load_combo->currentIndex();
	d->sub.setAutoLoad(static_cast<Subtitle::AutoLoad>(idx));
	idx = d->ui.auto_select_combo->currentIndex();
	d->sub.setAutoSelect(static_cast<Backend::SubtitleOutput::AutoSelect>(idx));
	d->sub.setOsdStyle(d->ui.osd_widget->style());
	return d->sub;
}

}
