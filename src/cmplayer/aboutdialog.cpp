#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <core/info.h>

struct AboutDialog::Data {
	Ui::AboutDialog ui;
};

AboutDialog::AboutDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
	QString version = d->ui.version->text();
	version = version.arg(Core::Info::coreVersion());
	version = version.arg(QT_VERSION_STR);
	version = version.arg(qVersion());
	d->ui.version->setText(version);
}

AboutDialog::~AboutDialog() {
	delete d;
}


