#include "checkdialog.h"
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

struct CheckDialog::Data {
	QCheckBox *check;
	QLabel *label;
	QDialogButtonBox *button;
	QDialogButtonBox::StandardButton clicked;
};

CheckDialog::CheckDialog(QWidget *parent, QDialogButtonBox::StandardButtons buttons)
: QDialog(parent), d(new Data) {
	d->check = new QCheckBox(this);
	d->label = new QLabel(this);
// 	d->label->setWordWrap(true);
	d->button = new QDialogButtonBox(this);
	d->button->setCenterButtons(true);
	d->clicked = QDialogButtonBox::NoButton;

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->label);
	vbox->addWidget(d->check);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	hbox->addWidget(d->button);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	vbox->addLayout(hbox);
	connect(d->button, SIGNAL(clicked(QAbstractButton*))
		, this, SLOT(slotButtonClicked(QAbstractButton*)));
	setButtonBox(buttons);
}

CheckDialog::~CheckDialog() {
	delete d;
}

void CheckDialog::setLabelText(const QString &text) {
	d->label->setText(text);
}

void CheckDialog::setCheckBoxText(const QString &text) {
	d->check->setText(text);
}

void CheckDialog::setChecked(bool checked) {
	d->check->setChecked(checked);
}

bool CheckDialog::isChecked() const {
	return d->check->isChecked();
}

void CheckDialog::setButtonBox(QDialogButtonBox::StandardButtons buttons) {
	d->button->setStandardButtons(buttons);
}

int CheckDialog::exec() {
	QDialog::exec();
	return d->clicked;
}

void CheckDialog::slotButtonClicked(QAbstractButton *button) {
	d->clicked = d->button->standardButton(button);
	accept();
}
