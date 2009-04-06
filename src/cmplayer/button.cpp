#include "button.h"
#include <QtGui/QAction>
#include <QtCore/QDebug>

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
