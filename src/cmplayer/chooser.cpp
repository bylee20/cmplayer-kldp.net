#include "chooser.h"
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

struct Chooser::Data {
	QButtonGroup *group;
	QVBoxLayout *vbox;
};

Chooser::Chooser(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->group = new QButtonGroup(this);
	d->vbox = new QVBoxLayout(this);
	d->vbox->setMargin(0);
}

Chooser::~Chooser() {
	delete d;
}

void Chooser::clear() {
	QList<QAbstractButton*> buttons = d->group->buttons();
	for (int i=0; i < buttons.size(); ++i) {
		d->group->removeButton(buttons[i]);
		delete buttons[i];
	}
}

void Chooser::setChoices(const QStringList choices, int chosen) {
	clear();
	for (int i=0; i<choices.size(); ++i) {
		QRadioButton *button = new QRadioButton(choices[i], this);
		d->group->addButton(button, i);
		d->vbox->addWidget(button);
	}
	if (0 <= chosen && chosen < choices.size())
		d->group->button(chosen)->setChecked(true);
}

void Chooser::setChosenText(const QString &text) {
	foreach(QAbstractButton* button, d->group->buttons()) {
		if (button->text() == text) {
			button->setChecked(true);
			return;
		}
	}
}

int Chooser::chosenIndex() const {
	return d->group->checkedId();
}

QString Chooser::chosenText() const {
	const QAbstractButton *button = d->group->checkedButton();
	return button ? button->text() : QString();
}
