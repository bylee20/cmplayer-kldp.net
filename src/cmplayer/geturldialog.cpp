#include "geturldialog.h"
#include "encodingcombobox.h"
#include "ui_geturldialog.h"
#include <core/playlist.h>
#include <core/info.h>
#include <QtCore/QFileInfo>

struct GetUrlDialog::Data {
	Ui::GetUrlDialog ui;
};

GetUrlDialog::GetUrlDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
}

GetUrlDialog::~GetUrlDialog() {
	delete d;
}

void GetUrlDialog::accept() {
	QDialog::accept();
}

QUrl GetUrlDialog::url() const {
	return QUrl(d->ui.url->text().trimmed());
}

bool GetUrlDialog::isPlaylist() const {
	const QString suffix = QFileInfo(url().path()).suffix();
	return suffix.compare("pls", Qt::CaseInsensitive) == 0;
}

Core::Playlist GetUrlDialog::playlist() const {
	if (!isPlaylist())
		return Core::Playlist();
	Core::Playlist list;
	list.load(url(), d->ui.enc->encoding());
	return list;
}

void GetUrlDialog::showEvent(QShowEvent *event) {
	d->ui.retranslateUi(this);
	QDialog::showEvent(event);
}

QString GetUrlDialog::encoding() const {
	return d->ui.enc->encoding();
}
