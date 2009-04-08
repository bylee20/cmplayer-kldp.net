#include "videocolorwidget.h"
#include <QtGui/QSlider>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <core/colorproperty.h>
#include <QtCore/QMap>
#include <QtGui/QLabel>
#include <QtCore/QDebug>
#include <core/utility.h>
#include "videoplayer.h"
#include "verticaltext.h"
#include <QtGui/QCheckBox>

struct EqSlider::Data {
	int id;
	VerticalText *name;
	QSlider *slider;
	QLabel *value;
};

EqSlider::EqSlider(int id, const QString &name, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->id = id;
	d->name = new VerticalText(this, name);
	d->slider = new QSlider(Qt::Vertical, this);
	d->value = new QLabel(this);
	
	d->slider->setTickPosition(QSlider::TicksRight);
	d->value->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	
	QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(d->name, 0, 0, 1, 1);
	grid->addWidget(d->slider, 0, 1, 1, 1);
	grid->addWidget(d->value, 1, 0, 1, 2);
	
	d->value->setText(QString::number(d->slider->value()) + "%");
	connect(d->slider, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
}

void EqSlider::slotValueChanged(int value) {
	d->value->setText(QString::number(value) + "%");
	emit valueChanged(value);
}


int EqSlider::id() const {
	return d->id;
}

int EqSlider::value() const {
	return d->slider->value();
}

EqSlider::~EqSlider() {
	delete d;
}

void EqSlider::setRange(int min, int max) {
	d->slider->setRange(min, max);
}

void EqSlider::setValue(int value) {
	d->slider->setValue(value);
}

struct VideoColorWidget::Data {
	Core::ColorProperty prop;
	QMap<Core::ColorProperty::Value, EqSlider*> slider;
	VideoPlayer *player;
};

VideoColorWidget::VideoColorWidget(VideoPlayer *player, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->player = player;
	QVBoxLayout *vbox = new QVBoxLayout(this);
	QHBoxLayout *hbox = new QHBoxLayout;
	addSet(Core::ColorProperty::Brightness, "Brightness", hbox);
	addSet(Core::ColorProperty::Contrast, "Contrast", hbox);
	addSet(Core::ColorProperty::Saturation, "Saturation", hbox);
	addSet(Core::ColorProperty::Hue, "Hue", hbox);
	vbox->addLayout(hbox);
	updateValues();
	connect(d->player, SIGNAL(colorPropertyChanged()), this, SLOT(updateValues()));
	
}

VideoColorWidget::~VideoColorWidget() {
	delete d;
}

void VideoColorWidget::addSet(int p, const QString &name, QHBoxLayout *hbox) {
	const Core::ColorProperty::Value prop = static_cast<Core::ColorProperty::Value>(p);
	EqSlider *slider = new EqSlider(p, name, this);
	slider->setRange(-100, 100);
	d->slider[prop] = slider;
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(applyValue(int)));
	hbox->addWidget(slider);
}

void VideoColorWidget::updateValues() {
	QMap<Core::ColorProperty::Value, EqSlider*>::iterator it;
	for (it = d->slider.begin(); it != d->slider.end(); ++it) {
		it.value()->setValue(d->player->colorProperty(it.key()));
	}
}

void VideoColorWidget::applyValue(int value) {
	EqSlider *slider = qobject_cast<EqSlider*>(sender());
	if (slider) {
		const Core::ColorProperty::Value prop
				= static_cast<Core::ColorProperty::Value>(slider->id());
		d->player->setColorProperty(prop, value);
		
	}
}
