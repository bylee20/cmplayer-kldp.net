#include "osdwidget.h"
#include <QtGui/QColor>
#include <QtGui/QFontDialog>
#include <QtCore/QDebug>

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
	QVector<QRgb> clut = Backend::OsdStyle::Clut::get()->clut();
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

Backend::OsdStyle OsdWidget::style() const {
	Backend::OsdStyle style;
	style.font = ui.font_label->font();
	style.text_color	= ui.text_color_combo->itemData(ui.text_color_combo->currentIndex()
			, Qt::BackgroundRole).value<QColor>();
	style.border_color = ui.border_color_combo->itemData(ui.border_color_combo->currentIndex()
			, Qt::BackgroundRole).value<QColor>();
	style.scale = static_cast<Backend::OsdStyle::Scale>(ui.scale_combo->currentIndex());
	style.size = ui.size_spin->value()/100.0;
	style.high_quality = ui.high_quality_check->isChecked();
	return style;
}

void OsdWidget::setStyle(const Backend::OsdStyle &style) {
	QFont font = style.font;
	font.setPixelSize(this->font().pixelSize());
	font.setPointSize(this->font().pointSize());
	ui.font_label->setFont(font);
	ui.font_label->setText(font.family());

	ui.text_color_combo->setCurrentIndex(Backend::OsdStyle::Clut::get()->clut().indexOf(style.text_color.rgba()));
	ui.border_color_combo->setCurrentIndex(Backend::OsdStyle::Clut::get()->clut().indexOf(style.border_color.rgba()));

	ui.scale_combo->setCurrentIndex(style.scale);
	ui.size_spin->setValue(style.size*100.0);

	ui.high_quality_check->setChecked(style.high_quality);
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
