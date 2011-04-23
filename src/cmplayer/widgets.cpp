#include "widgets.hpp"
#include <QtGui/QDoubleSpinBox>
#include "qtcolorpicker.hpp"
#include <QtGui/QColorDialog>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>

struct FontOptionWidget::Data {
	QToolButton *bold, *italic, *underline, *strikeout;
	QToolButton *make_button(const QString &toolTip, const QIcon &icon, QWidget *parent) {
		QToolButton *button = new QToolButton(parent);
		button->setIcon(icon);
		button->setToolTip(toolTip);
		button->setCheckable(true);
		return button;
	}
};

FontOptionWidget::FontOptionWidget(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->bold = d->make_button(tr("Bold"), QIcon(":/img/format-text-bold.png"), this);
	d->italic = d->make_button(tr("Italic"), QIcon(":/img/format-text-italic.png"), this);
	d->underline = d->make_button(tr("Underline"), QIcon(":/img/format-text-underline.png"), this);
	d->strikeout = d->make_button(tr("Strike Out"), QIcon(":/img/format-text-strikethrough.png"), this);
	QHBoxLayout *hbox = new QHBoxLayout(this);
	hbox->addWidget(d->bold);
	hbox->addWidget(d->italic);
	hbox->addWidget(d->underline);
	hbox->addWidget(d->strikeout);
	hbox->setMargin(0);
}

FontOptionWidget::~FontOptionWidget() {
	delete d;
}

void FontOptionWidget::set(bool bold, bool italic, bool underline, bool strikeout) {
	d->bold->setChecked(bold);
	d->italic->setChecked(italic);
	d->underline->setChecked(underline);
	d->strikeout->setChecked(strikeout);
}

bool FontOptionWidget::bold() const {
	return d->bold->isChecked();
}

bool FontOptionWidget::italic() const {
	return d->italic->isChecked();
}

bool FontOptionWidget::underline() const {
	return d->underline->isChecked();
}

bool FontOptionWidget::strikeOut() const {
	return d->strikeout->isChecked();
}

///////////////////////////////////////////////////////////////////////////////////////

struct ColorSelectWidget::Data {
	QtSolution::QtColorPicker *color;
	QLabel *alphaLabel;
	QDoubleSpinBox *alpha;
	bool hasAlpha;
};

ColorSelectWidget::ColorSelectWidget(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->color = new QtSolution::QtColorPicker(this);
	d->color->setStandardColors();
	d->hasAlpha = false;
	d->alpha = new QDoubleSpinBox(this);
	d->alpha->setRange(0.0, 100.0);
	d->alpha->setSuffix("%");
	d->alpha->setDecimals(1);
	d->alpha->setAccelerated(true);
	d->alphaLabel = new QLabel(tr("Transparency"));
	QHBoxLayout *hbox = new QHBoxLayout(this);
	hbox->addWidget(d->color);
	hbox->addWidget(d->alphaLabel);
	hbox->addWidget(d->alpha);
	hbox->setMargin(0);

	d->alpha->hide();
	d->alphaLabel->hide();

}

ColorSelectWidget::~ColorSelectWidget() {
	delete d;
}

void ColorSelectWidget::setColor(const QColor &color, bool hasAlpha) {
	d->hasAlpha = hasAlpha;
	d->alpha->setVisible(d->hasAlpha);
	d->alpha->setValue(d->hasAlpha ? color.alphaF()*100.0 : 100.0);
	d->alphaLabel->setVisible(d->hasAlpha);
	d->color->setCurrentColor(color.rgb());
}

QColor ColorSelectWidget::color() const {
	QColor color = d->color->currentColor();
	color.setAlphaF(d->hasAlpha ? d->alpha->value()/100.0 : 1.0);
	return color;
}

