#include "osdstyle.hpp"
#include "ui_osdstyle_widget.h"
#include <QtGui/QColorDialog>
#include <QtGui/QFontDialog>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

void OsdStyle::save(QSettings *set, const QString &group) const {
	set->beginGroup(group);
	set->setValue("font", font);
	set->setValue("bgColor", bgColor);
	set->setValue("fgColor", fgColor);
	set->setValue("borderWidth", borderWidth);
	set->setValue("textSize", textSize);
	set->setValue("alignment", int(alignment));
	if (scale == FitToWidth)
		set->setValue("scale", "FitToWidth");
	else if (scale == FitToHeight)
		set->setValue("scale", "FitToHeight");
	else
		set->setValue("scale", "FitToDiagonal");
	set->endGroup();
}

void OsdStyle::load(QSettings *set, const QString &group) {
	set->beginGroup(group);
	font = set->value("font", font).value<QFont>();
	bgColor = set->value("bgColor", bgColor).value<QColor>();
	fgColor = set->value("fgColor", fgColor).value<QColor>();
	borderWidth = set->value("borderWidth", borderWidth).toDouble();
	textSize = set->value("textSize", textSize).toDouble();
	alignment = Qt::Alignment(set->value("alignment", int(alignment)).toInt());
	const QString scale = set->value("scale", "FitToDiagonal").toString();
	if (scale == "FitToWidth")
		this->scale = FitToWidth;
	else if (scale == "FitToHeight")
		this->scale = FitToHeight;
	else
		this->scale = FitToDiagonal;
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
		before = &d->style.fgColor;
	} else if (sender() == d->ui.bgColorButton) {
		label = d->ui.bgColorLabel;
		before = &d->style.bgColor;
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
	setColor(d->ui.fgColorLabel, d->style.fgColor);
	setColor(d->ui.bgColorLabel, d->style.bgColor);
	d->ui.scale->setCurrentIndex(d->ui.scale->findData(d->style.scale));
	d->ui.size->setValue(d->style.textSize*100.);
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
		d->style.scale = (OsdStyle::Scale)(d->ui.scale->itemData(scale).toInt());
	d->style.textSize = d->ui.size->value()/100.;
	return d->style;
}
