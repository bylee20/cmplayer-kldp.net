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

// 	d->ui.encoding_combo->addItems(encs);

// 	static const QRegExp rxEncoding(".* \\(" + d->sub.encoding().toUpper() + "\\)");
// 	const int idx = encs.indexOf(rxEncoding);
	d->ui.encoding_combo->setEncoding(d->sub.encoding());
// 	d->ui.encoding_combo->setEditText(idx == -1 ? d->sub.encoding() : encs[idx]);
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
// 	QString encoding = d->ui.encoding_combo->currentText().trimmed();
// 	static QRegExp rxEnc(".* \\((.*)\\)");
	d->sub.setEncoding(d->ui.encoding_combo->encoding());
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
