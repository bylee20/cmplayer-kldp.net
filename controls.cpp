#include "controls.hpp"
#include "playengine.hpp"
#include <QtGui/QAction>
#include <QtCore/QDebug>
#include "audiocontroller.hpp"

struct Button::Data {
	QAction *action;
};

Button::Button(QWidget *parent)
: QToolButton(parent), d(new Data) {
	init();
}

Button::Button(const QIcon &icon, QWidget *parent)
: QToolButton(parent), d(new Data) {
	init();
	setIcon(icon);
}

Button::Button(const QString &text, QWidget *parent)
: QToolButton(parent), d(new Data) {
	init();
	setText(text);
}

Button::~Button() {
	delete d;
}

void Button::init() {
	d->action = 0;
	setObjectName("flat");
	setFocusPolicy(Qt::NoFocus);
	setAutoRaise(true);
}

void Button::setIconSize(int extent) {
	QToolButton::setIconSize(QSize(extent, extent));
}

void Button::setAction(QAction *action, bool update) {
	if (d->action == action)
		return;
	if (!(d->action = action))
		return;
	setCheckable(action->isCheckable());
	if (isCheckable()) {
		connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleAction(bool)));
		connect(action, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
	} else
		connect(this, SIGNAL(clicked()), action, SLOT(trigger()));
	if (update) {
		if (action->icon().isNull())
			setText(action->text());
		else {
			setIcon(action->icon());
			setToolTip(action->text());
		}
	}
}

void Button::setBlock(bool block) {
	setObjectName(block ? "block" : "flat");
}

void Button::toggleAction(bool checked) {
	if (d->action && d->action->isChecked() != checked)
		d->action->trigger();
}


//#include "PlayEngine.h"
#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QStyle>
#include <QtCore/QDebug>

JumpSlider::JumpSlider(QWidget *parent)
: QSlider(Qt::Horizontal, parent) {
	setFocusPolicy(Qt::NoFocus);
	setSingleStep(1);
	setPageStep(1);
}

void JumpSlider::mousePressEvent(QMouseEvent *event) {
	const qint64 range = maximum() - minimum();
	const qint64 width = this->width();
	const qint64 newVal = static_cast<qint64>(event->x()) * range / width;
	const qint64 metric = qApp->style()->pixelMetric(QStyle::PM_SliderLength);
	const qint64 sub = (metric * range)/width;
	if (qAbs(newVal - value()) > sub)
		setValue(newVal);
	else
		QSlider::mousePressEvent(event);
}

SeekSlider::SeekSlider(PlayEngine *engine, QWidget *parent)
: JumpSlider(parent), engine(engine), tick(false) {
	setRange(0, engine->duration());
	setValue(engine->position());
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	connect(engine, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	connect(engine, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	connect(engine, SIGNAL(durationChanged(int)), this, SLOT(setDuration(int)));
	setEnabled(engine->isSeekable());
}

void SeekSlider::seek(int time) {
	if (!tick && engine)
		engine->seek(time);//, false, false);
}

void SeekSlider::setDuration(int duration) {
	setRange(0, duration);
}

void SeekSlider::slotTick(int time) {
	tick = true;
	setValue(time);
	tick = false;
}

VolumeSlider::VolumeSlider(PlayEngine *engine, QWidget *parent)
: JumpSlider(parent) {
	setMaximumWidth(70);
	setRange(0, 100);
	setValue(engine->audio()->volume());
	connect(this, SIGNAL(valueChanged(int)), engine->audio(), SLOT(setVolume(int)));
	connect(engine->audio(), SIGNAL(volumeChanged(int)), this, SLOT(setValue(int)));
}


