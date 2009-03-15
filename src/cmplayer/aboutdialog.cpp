#include "aboutdialog.h"
#include "videoplayer.h"
#include "ui_aboutdialog.h"
#include <core/backendiface.h>
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
	const BackendMap map = VideoPlayer::backend();
	BackendMap::const_iterator it = map.begin();
	for (; it != map.end(); ++it) {
		const Core::Info *info = it.value()->info();
		const QStringList engine = QStringList()
				<< info->name() << info->compileVersion()
				<< info->runtimeVersion();
		QTreeWidgetItem *item = new QTreeWidgetItem(engine);
		d->ui.engineTree->addTopLevelItem(item);
	}
}

AboutDialog::~AboutDialog() {
	delete d;
}


