#include "osdwidget.h"
#include <QColor>
#include <QFontDialog>
#include <xine/xineosd_clut.h>

namespace Pref {

OsdWidget::OsdWidget(QWidget *parent)
: QWidget(parent) {
	ui.setupUi(this);
	connect(ui.font_button, SIGNAL(clicked()), this, SLOT(changeFont()));
	connect(ui.text_color_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTextColor(int)));
	connect(ui.border_color_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBorderColor(int)));
	initColorComboBox(ui.text_color_combo);
	initColorComboBox(ui.border_color_combo);
	changeTextColor(ui.text_color_combo->currentIndex());
	changeBorderColor(ui.border_color_combo->currentIndex());
}

OsdWidget::~OsdWidget() {

}

void OsdWidget::initColorComboBox(QComboBox *combo) {
	QVector<QRgb> clut = Xine::XineOsd::Clut::get()->clut();
	for (int i=0; i<clut.size(); ++i) {
		QColor color(clut[i]);
		color.setAlpha(qAlpha(clut[i]));
		combo->addItem(color.name() + "-" + QString::number(qRound(color.alphaF()*100)) + "%");
		combo->setItemData(i, color, Qt::BackgroundRole);
	}
}

void OsdWidget::setColor(QWidget *widget, const QColor &color) {
	QPalette p = widget->palette();
	p.setColor(QPalette::Button, color);
	widget->setPalette(p);
}

Xine::XineOsd::Style OsdWidget::style() const {
	Xine::XineOsd::Style style;
	style.font = ui.font_label->font();
	style.textColor	= ui.text_color_combo->itemData(ui.text_color_combo->currentIndex()
			, Qt::BackgroundRole).value<QColor>();
	style.borderColor = ui.border_color_combo->itemData(ui.border_color_combo->currentIndex()
			, Qt::BackgroundRole).value<QColor>();
	style.scale = static_cast<Xine::XineOsd::Style::Scale>(ui.scale_combo->currentIndex());
	style.size = ui.size_spin->value()/100.0;
	style.highQuality = ui.high_quality_check->isChecked();
	return style;
}

void OsdWidget::setStyle(const Xine::XineOsd::Style &style) {
	QFont font = style.font;
	font.setPixelSize(this->font().pixelSize());
	font.setPointSize(this->font().pointSize());
	ui.font_label->setFont(font);
	ui.font_label->setText(font.family());

	ui.text_color_combo->setCurrentIndex(Xine::XineOsd::Clut::get()->clut().indexOf(style.textColor.rgba()));
	ui.border_color_combo->setCurrentIndex(Xine::XineOsd::Clut::get()->clut().indexOf(style.borderColor.rgba()));

	ui.scale_combo->setCurrentIndex(style.scale);
	ui.size_spin->setValue(style.size*100.0);

	ui.high_quality_check->setChecked(style.highQuality);
}

void OsdWidget::changeFont() {
	bool ok = false;
	QFont font = QFontDialog::getFont(&ok, ui.font_label->font(), this);
	if (ok) {
		font.setPixelSize(this->font().pixelSize());
		font.setPointSize(this->font().pointSize());
		ui.font_label->setFont(font);
		ui.font_label->setText(font.family());
	}
}

void OsdWidget::changeTextColor(int index) {
	QColor color = ui.text_color_combo->itemData(index, Qt::BackgroundRole).value<QColor>();
	setColor(ui.text_color_combo, color);
}

void OsdWidget::changeBorderColor(int index) {
	QColor color = ui.border_color_combo->itemData(index, Qt::BackgroundRole).value<QColor>();
	setColor(ui.border_color_combo, color);
}

}
