#include "getshortcutdialog.h"
#include <QtGui/QKeyEvent>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>

struct GetShortcutDialog::Data {
	QLineEdit *edit;
	QPushButton *begin, *erase, *ok, *cancel;
	bool getting;
	bool pressing;
	int curIdx;
	int codes[MaxKeyCount];
	QKeySequence shortcut;
};

GetShortcutDialog::GetShortcutDialog(const QKeySequence &shortcut, QWidget *parent)
: QDialog(parent) {
	init();
	setShortcut(shortcut);
}

GetShortcutDialog::GetShortcutDialog(QWidget *parent)
: QDialog(parent) {
	init();
}

GetShortcutDialog::~GetShortcutDialog() {
	delete d;
}

void GetShortcutDialog::init() {
	d = new Data;
	d->getting = d->pressing = (d->curIdx = 0);
	d->edit = new QLineEdit(this);
	d->edit->setReadOnly(true);
	d->begin = new QPushButton(tr("Get Shortcut"), this);
	d->begin->setFocusPolicy(Qt::NoFocus);
	d->begin->setCheckable(true);
	d->begin->setDefault(false);
	d->erase = new QPushButton(tr("Erase"), this);
	d->erase->setFocusPolicy(Qt::NoFocus);
	d->erase->setAutoDefault(false);
	d->ok = new QPushButton(tr("Ok"), this);
	d->ok->setFocusPolicy(Qt::NoFocus);
	d->cancel = new QPushButton(tr("Cancel"), this);
	d->cancel->setAutoDefault(false);
	
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->edit);
	
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(d->begin);
	hbox->addWidget(d->erase);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	hbox->addWidget(d->ok);
	hbox->addWidget(d->cancel);
	
	vbox->addLayout(hbox);
	
	connect(d->begin, SIGNAL(toggled(bool)), this, SLOT(setGetting(bool)));
	connect(d->erase, SIGNAL(clicked()), this, SLOT(erase()));
	connect(d->ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(d->cancel, SIGNAL(clicked()), this, SLOT(reject()));
	d->edit->installEventFilter(this);
	for (int i=0; i<MaxKeyCount; ++i)
		d->codes[i] = 0;
	d->begin->toggle();
}

void GetShortcutDialog::erase() {
	setShortcut(QKeySequence());
}

const QKeySequence &GetShortcutDialog::shortcut() const {
	return d->shortcut;
}

void GetShortcutDialog::setShortcut(const QKeySequence &shortcut) {
	d->shortcut = shortcut;
	d->edit->setText(shortcut.toString());
}

void GetShortcutDialog::setGetting(bool on) {
	if ((d->getting = on)) {
		d->curIdx = 0;
		d->edit->clear();
		d->shortcut = QKeySequence();
	}
}

bool GetShortcutDialog::eventFilter(QObject *obj, QEvent *event) {
	if (d->getting && obj == d->edit && event->type() == QEvent::KeyPress) {
		getShortcut(static_cast<QKeyEvent *>(event));
		return true;
	} else
		return QDialog::eventFilter(obj, event);
}

void GetShortcutDialog::keyPressEvent(QKeyEvent *event) {
	QDialog::keyPressEvent(event);
	if (d->getting)
		getShortcut(event);
}

void GetShortcutDialog::getShortcut(QKeyEvent *event) {
	if (0 <= d->curIdx && d->curIdx < MaxKeyCount) {
		d->codes[d->curIdx] = event->key();
		if (!(event->modifiers() & Qt::KeypadModifier))
			d->codes[d->curIdx] += event->modifiers();
	}
}

void GetShortcutDialog::keyReleaseEvent(QKeyEvent *event) {
	QDialog::keyReleaseEvent(event);
	if (d->getting && d->codes[d->curIdx]) {
		d->shortcut = QKeySequence(d->codes[0], d->codes[1], d->codes[2], d->codes[3]);
		d->edit->setText(d->shortcut.toString());
		++d->curIdx;
	}
}
