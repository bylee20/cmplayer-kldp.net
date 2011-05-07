#include "aboutdialog.hpp"
#include <QtCore/QFile>
#include "ui_aboutdialog.h"
#include "info.hpp"
#include <QtCore/QDate>
#include <QtGui/QTextBrowser>
#include <QtCore/QStringBuilder>

typedef QLatin1Char _LC;
typedef QLatin1String _LS;

struct AboutDialog::Data {
	Ui::AboutDialog ui;
};

AboutDialog::AboutDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
#define UI_LABEL_ARG(label, arg) d->ui.label->setText(d->ui.label->text().arg)
	UI_LABEL_ARG(version, arg(Info::version()));
	UI_LABEL_ARG(copyright, arg(QDate::currentDate().year()));
	UI_LABEL_ARG(contacts, arg("<a href=\"http://xylosper.net\">http://xylosper.net</a><br>")
		.arg("<a href=\"mailto:darklin20@gmail.com\">darklin20@gmail.com</a><br>")
		.arg("<a href=\"http://kldp.net/projects/cmplayer\">http://kldp.net/projects/cmplayer</a>"));
#undef UI_LABEL_ARG
	d->ui.license->setText(
		"This program is free software; "
		"you can redistribute it and/or modify it under the terms of "
		"the GNU General Public License as published by the Free Software Foundation; "
		"either version 2 of the License, or (at your option) any later version.<br><br>"

		"This program is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
		"See the GNU General Public License for more details.<br><br>"

		"You should have received a copy of the GNU General Public License along with this program; "
		"if not, see <a href=\"http://www.gnu.org/licenses\">http://www.gnu.org/licenses</a>.<br><br>"

		"Exception:<br>"
		"libchardet made by JoungKyun.Kim is distributed under Mozilla Public License(MPL).");
	connect(d->ui.view_license, SIGNAL(clicked()), this, SLOT(showFullLicense()));

	setFixedHeight(400);
	setFixedWidth(width());
}

AboutDialog::~AboutDialog() {
	delete d;
}

void AboutDialog::showFullLicense() {
	QDialog dlg(this);
	QTextBrowser *text = new QTextBrowser(&dlg);
	QPushButton *close = new QPushButton(tr("Close"), &dlg);
	QVBoxLayout *vbox = new QVBoxLayout(&dlg);
	vbox->addWidget(text);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	hbox->addWidget(close);
	hbox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	vbox->addLayout(hbox);
	connect(close, SIGNAL(clicked()), &dlg, SLOT(accept()));

	QFile file(":/gpl.html");
	file.open(QFile::ReadOnly);
	text->setHtml(QLatin1String(file.readAll()));
	dlg.resize(500, 400);
	dlg.exec();
}


#include "ui_opendvddialog.h"
#include <QtCore/QFileInfo>

struct OpenDVDDialog::Data {
	Ui::OpenDVDDialog ui;
	QPushButton *ok;
};

OpenDVDDialog::OpenDVDDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
	d->ok = d->ui.buttonBox->button(QDialogButtonBox::Ok);
	d->ok->setEnabled(false);
	connect(d->ui.device, SIGNAL(editTextChanged(QString)), this, SLOT(checkDevice(QString)));
}

OpenDVDDialog::~OpenDVDDialog() {
	delete d;
}

void OpenDVDDialog::setDevices(const QStringList &devices) {
	d->ui.device->clear();
	d->ui.device->addItems(devices);
}

void OpenDVDDialog::checkDevice(const QString &device) {
	const QFileInfo info(device);
	const bool exists = info.exists();
	d->ok->setEnabled(exists);
	if (exists)
		d->ui.available->setText(tr("Selected device is available."));
	else {
		d->ui.available->setText(_LS("<font color='red'>")
			% tr("Selected device doesn't exists.") % _LS("</font>"));
	}
}

QString OpenDVDDialog::device() const {
	return d->ui.device->currentText();
}
