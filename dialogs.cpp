#include "dialogs.hpp"
#include "controls.hpp"
#include "playlist.hpp"
#include "info.hpp"
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QKeyEvent>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QDialogButtonBox>
#include "appstate.hpp"
#include <QtGui/QCompleter>

static QDialogButtonBox *makeButtonBox(QDialog *dlg) {
	QDialogButtonBox *dbb = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dlg);
	dlg->connect(dbb, SIGNAL(accepted()), SLOT(accept()));
	dlg->connect(dbb, SIGNAL(rejected()), SLOT(reject()));
	return dbb;
}

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

/*******************************************************************************************/

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

/*************************************************************************/

EncodingFileDialog::EncodingFileDialog(QWidget *parent, const QString &caption
		, const QString &directory, const QString &filter, const QString &encoding)
: QFileDialog(parent, caption, directory, filter), combo(new EncodingComboBox(this)) {
	QGridLayout *grid = qobject_cast<QGridLayout*>(layout());
	if (grid) {
		const int row = grid->rowCount();
		grid->addWidget(new QLabel("Encoding:", this), row, 0, 1, 1);
		grid->addWidget(combo, row, 1, 1, grid->columnCount()-1);
	}
	if (!encoding.isEmpty())
		setEncoding(encoding);
}

void EncodingFileDialog::setEncoding(const QString &encoding) {
	combo->setEncoding(encoding);
}

QString EncodingFileDialog::encoding() const {
	return combo->encoding();
}

QString EncodingFileDialog::getOpenFileName(QWidget *parent
		, const QString &caption, const QString &dir
		, const QString &filter, QString *enc) {
	const QStringList files = getOpenFileNames(parent
			, caption, dir, filter, enc, ExistingFile);
	return files.isEmpty() ? QString() : files[0];
}

QStringList EncodingFileDialog::getOpenFileNames(QWidget *parent
		, const QString &caption
		, const QString &dir
		, const QString &filter
		, QString *enc, FileMode mode) {
	EncodingFileDialog dlg(parent, caption, dir, filter);
	if (enc && !enc->isEmpty())
		dlg.setEncoding(*enc);
	dlg.setFileMode(mode);
	if (dlg.exec()) {
		if (enc)
			*enc = dlg.encoding();
		return dlg.selectedFiles();
	}
	return QStringList();
}

/******************************************************************************/

struct GetUrlDialog::Data {
	QLineEdit *url;
	EncodingComboBox *enc;
	QCompleter *c;
};

GetUrlDialog::GetUrlDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	AppState as;
	d->c = new QCompleter(as[AppState::OpenUrlList].toStringList(), this);
	d->url = new QLineEdit(this);
	d->url->setCompleter(d->c);
	d->enc = new EncodingComboBox(this);
	d->enc->setEncoding(as[AppState::UrlEncoding].toString());

	QVBoxLayout *vbox = new QVBoxLayout(this);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel(tr("URL"), this));
	hbox->addWidget(d->url);
	vbox->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel(tr("Encoding for Playlist"), this));
	hbox->addWidget(d->enc);
	hbox->addWidget(makeButtonBox(this));
	vbox->addLayout(hbox);
}

GetUrlDialog::~GetUrlDialog() {
	delete d;
}

void GetUrlDialog::accept() {
	AppState as;
	QStringList list = as[AppState::OpenUrlList].toStringList();
	const QString url = d->url->text().trimmed();
	if (!list.contains(url)) {
		list << url;
		as[AppState::OpenUrlList] = list;
	}
	as[AppState::UrlEncoding] = d->enc->encoding();
	QDialog::accept();
}

QUrl GetUrlDialog::url() const {
	return QUrl(d->url->text().trimmed());
}

//bool GetUrlDialog::isPlaylist() const {
//	const QString suffix = QFileInfo(url().path()).suffix();
//	return suffix.compare("pls", Qt::CaseInsensitive) == 0;
//}

//Playlist GetUrlDialog::playlist() const {
//	if (!isPlaylist())
//		return Playlist();
//	Playlist list;
//	list.load(url(), d->enc->encoding());
//	return list;
//}

QString GetUrlDialog::encoding() const {
	return d->enc->encoding();
}

