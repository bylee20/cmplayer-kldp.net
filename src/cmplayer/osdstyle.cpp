#include "osdstyle.hpp"
#include "ui_osdstyle_widget.h"
#include <QtGui/QColorDialog>
#include <QtGui/QFontDialog>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

void OsdStyle::save(QSettings *set, const QString &group) const {
	set->beginGroup(group);
	set->setValue("font", font);
	set->setValue("color_bg", color_bg);
	set->setValue("color_fg", color_fg);
	set->setValue("border_width", border_width);
	set->setValue("text_scale", text_scale);
	set->setValue("alignment", int(alignment));
	if (auto_size == FitToWidth)
		set->setValue("auto_size", "FitToWidth");
	else if (auto_size == FitToHeight)
		set->setValue("auto_size", "FitToHeight");
	else
		set->setValue("auto_size", "FitToDiagonal");
	set->endGroup();
}

void OsdStyle::load(QSettings *set, const QString &group) {
	set->beginGroup(group);
	font = set->value("font", font).value<QFont>();
	color_bg = set->value("color_bg", color_bg).value<QColor>();
	color_fg = set->value("color_fg", color_fg).value<QColor>();
	border_width = set->value("border_width", border_width).toDouble();
	text_scale = set->value("text_scale", text_scale).toDouble();
	alignment = Qt::Alignment(set->value("alignment", int(alignment)).toInt());
	const QString size = set->value("auto_size", "FitToDiagonal").toString();
	if (size == "FitToWidth")
		auto_size = FitToWidth;
	else if (size == "FitToHeight")
		auto_size = FitToHeight;
	else
		auto_size = FitToDiagonal;
	set->endGroup();
}

struct OsdStyle::Widget::Data {
	Ui::OsdStyle_Widget ui;
	OsdStyle style;
};

OsdStyle::Widget::Widget(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->ui.setupUi(this);
	d->ui.fgColorLabel->setAutoFillBackground(true);
	d->ui.bgColorLabel->setAutoFillBackground(true);
	d->ui.scale->addItem(tr("Fit to Diagonal"), OsdStyle::FitToDiagonal);
	d->ui.scale->addItem(tr("Fit to Height"), OsdStyle::FitToHeight);
	d->ui.scale->addItem(tr("Fit to Width"), OsdStyle::FitToWidth);
	setStyle(d->style);
	connect(d->ui.fontButton, SIGNAL(clicked()), this, SLOT(slotFont()));
	connect(d->ui.fgColorButton, SIGNAL(clicked()), this, SLOT(slotColor()));
	connect(d->ui.bgColorButton, SIGNAL(clicked()), this, SLOT(slotColor()));
}

OsdStyle::Widget::~Widget() {
	delete d;
}

void OsdStyle::Widget::slotColor() {
	QLabel *label = 0;
	QColor *before = 0;
	if (sender() == d->ui.fgColorButton) {
		label = d->ui.fgColorLabel;
		before = &d->style.color_fg;
	} else if (sender() == d->ui.bgColorButton) {
		label = d->ui.bgColorLabel;
		before = &d->style.color_bg;
	} else
		return;
	bool ok = false;
	const QRgb rgba = QColorDialog::getRgba(before->rgba(), &ok, this);
	if (ok) {
		before->setRgba(rgba);
		setColor(label, *before);
	}
}

void OsdStyle::Widget::slotFont() {
	bool ok = false;
	const QFont font = QFontDialog::getFont(&ok, d->style.font, this, tr("Select Font"), QFontDialog::DontUseNativeDialog);
	if (ok)
		updateFont(font);
}

void OsdStyle::Widget::updateFont(const QFont &font) {
	d->style.font = font;
	d->style.font.setPointSize(this->font().pointSize());
	d->ui.fontLabel->setFont(d->style.font);
	d->ui.fontLabel->setText(d->style.font.family());
}

void OsdStyle::Widget::setStyle(const OsdStyle &style) {
	d->style = style;
	updateFont(d->style.font);
	setColor(d->ui.fgColorLabel, d->style.color_fg);
	setColor(d->ui.bgColorLabel, d->style.color_bg);
	d->ui.scale->setCurrentIndex(d->ui.scale->findData(d->style.auto_size));
	d->ui.size->setValue(d->style.text_scale*100.);
}

void OsdStyle::Widget::setColor(QLabel *label, const QColor &color) {
	QPalette p = label->palette();
	p.setColor(QPalette::Window, color);
	p.setColor(QPalette::Button, color);
#define INVERT(value) (255 - value)
	const QColor inverted(INVERT(color.red()), INVERT(color.green()), INVERT(color.blue()));
#undef INVERT
	p.setColor(QPalette::WindowText, inverted);
	p.setColor(QPalette::ButtonText, inverted);
	label->setPalette(p);
	label->setText(color.name());
}

const OsdStyle &OsdStyle::Widget::style() const {
	const int scale = d->ui.scale->currentIndex();
	if (scale != -1)
		d->style.auto_size = (OsdStyle::AutoSize)(d->ui.scale->itemData(scale).toInt());
	d->style.text_scale = d->ui.size->value()/100.;
	return d->style;
}
