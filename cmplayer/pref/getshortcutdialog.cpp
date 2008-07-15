#include "getshortcutdialog.h"
#include "ui/ui_getshortcutdialog.h"
#include <QKeyEvent>

namespace Pref {

GetShortcutDialog::GetShortcutDialog(QWidget *parent)
: QDialog(parent), ui(new Ui::Ui_GetShortcutDialog), m_getting(false), m_pressing(false), m_curIdx(0) {
	ui->setupUi(this);
	connect(ui->begin_button, SIGNAL(toggled(bool)), this, SLOT(setGetting(bool)));
	ui->key_edit->installEventFilter(this);
	for (int i=0; i<MaxKeyCount; ++i)
		m_codes[i] = 0;
	ui->begin_button->toggle();
}

GetShortcutDialog::~GetShortcutDialog() {
	delete ui;
}

const QKeySequence &GetShortcutDialog::key() const {
	return m_key;
}

void GetShortcutDialog::setKey(const QKeySequence &key) {
	m_key = key;
	ui->key_edit->setText(key.toString());
}

void GetShortcutDialog::setGetting(bool on) {
	m_getting = on;
	if (on) {
		m_curIdx = 0;
		ui->key_edit->setText("");
		m_key = QKeySequence();
	}
}

bool GetShortcutDialog::eventFilter(QObject *obj, QEvent *event) {
	if (m_getting && obj == ui->key_edit && event->type() == QEvent::KeyPress) {
		getShortcut(static_cast<QKeyEvent *>(event));
		return true;
	} else
		return QDialog::eventFilter(obj, event);
}

void GetShortcutDialog::keyPressEvent(QKeyEvent *event) {
	QDialog::keyPressEvent(event);
	if (m_getting)
		getShortcut(event);
}

void GetShortcutDialog::getShortcut(QKeyEvent *event) {
	if (m_curIdx >= 0 && m_curIdx < MaxKeyCount) {
		m_codes[m_curIdx] =  + event->key();
		if (!(event->modifiers() & Qt::KeypadModifier))
			m_codes[m_curIdx] += event->modifiers();
	}
}

void GetShortcutDialog::keyReleaseEvent(QKeyEvent *event) {
	QDialog::keyReleaseEvent(event);
	if (m_getting && m_codes[m_curIdx]) {
		m_key = QKeySequence(m_codes[0], m_codes[1], m_codes[2], m_codes[3]);
		ui->key_edit->setText(m_key.toString());
		++m_curIdx;
	}
}

}

