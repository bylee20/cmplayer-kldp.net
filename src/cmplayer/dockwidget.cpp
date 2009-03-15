#include "dockwidget.h"
#include "playlistmodel.h"
#include "encodingfiledialog.h"
#include "ui_playlistwidget.h"
#include <core/info.h>
#include <core/playlist.h>
#include <QtCore/QProcess>
#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include "helper.h"

ShutdownDialog::ShutdownDialog(QWidget *parent)
: QDialog(parent) {
	QLabel *label = new QLabel(this);
	label->setText(tr("Input or Select Command for Shutdown:"));
	combo = new QComboBox(this);
	combo->setEditable(true);
	combo->addItems(QStringList() << "shutdown -h now" << "halt");
	combo->setCurrentIndex(0);
	QPushButton *ok = new QPushButton(tr("&Ok"), this);
	ok->setDefault(true);
	QPushButton *cancel = new QPushButton(tr("&Cancel"), this);
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(label);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(combo);
	hbox->addWidget(ok);
	hbox->addWidget(cancel);
	vbox->addLayout(hbox);
	setLayout(vbox);
	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

QStringList ShutdownDialog::command() const {
	return combo->currentText().trimmed().split(" ");
}

void ShutdownDialog::accept() {
	if (combo->currentText().trimmed().isEmpty())
		QMessageBox::warning(this, tr("Automatic Shutdown"), tr("Command for Shutdown is Empty."));
	else
		QDialog::accept();
}

struct DockWidget::Data {
	void move(bool up) {
		const int row = ui.list->currentIndex().row();
		int target = up ? row-1 : row + 1;
		if (model->swap(row, target))
			ui.list->setCurrentIndex(model->index(target, 0));
	}
	Ui::PlaylistWidget ui;
	PlaylistModel *model;
	bool checking, adding;
	bool identified;
	ShutdownDialog *dlg;
};

DockWidget::DockWidget(PlaylistModel *model, QWidget *parent)
: QDockWidget(parent), d(new Data) {
	d->checking = d->adding = d->identified = false;
	d->model = model;
	setWidget(new QWidget(this));
	d->ui.setupUi(widget());
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	d->dlg = new ShutdownDialog(this);
	connect(d->ui.open_button, SIGNAL(clicked()), this, SLOT(open()));
	connect(d->ui.add_button, SIGNAL(clicked()), this, SLOT(add()));
	connect(d->ui.erase_button, SIGNAL(clicked()), this, SLOT(erase()));
	connect(d->ui.up_button, SIGNAL(clicked()), this, SLOT(up()));
	connect(d->ui.down_button, SIGNAL(clicked()), this, SLOT(down()));
	connect(d->ui.clear_button, SIGNAL(clicked()), d->model, SLOT(clear()));
	connect(d->ui.close_button, SIGNAL(clicked()), this, SLOT(hide()));
	connect(d->ui.list, SIGNAL(activated(const QModelIndex&)),
		this, SLOT(slotActivated(const QModelIndex&)));
	connect(d->ui.shutdown_check, SIGNAL(toggled(bool)), this, SLOT(checkRoot(bool)));
	connect(d->ui.save_button, SIGNAL(clicked()), this, SLOT(save()));
	connect(d->model, SIGNAL(playlistFinished()), this, SLOT(checkShutdown()));
	connect(d->model, SIGNAL(rowCountChanged(int)), this, SLOT(adjustCellSize()));
	d->ui.list->setModel(model);
	setWindowTitle(tr("Play List"));
}

DockWidget::~DockWidget() {
	delete d;
}

void DockWidget::checkRoot(bool checked) {
	if (!checked)
		return;
	static QProcess proc;
	proc.start("whoami");
	if (!proc.waitForFinished(1000))
		proc.kill();
	static QRegExp rxRoot("^root\\r*\\n*$");
	if (rxRoot.indexIn(proc.readAllStandardOutput()) == -1) {
		QMessageBox::warning(this, tr("Automatic Shutdown")
				, tr("You must run CMPlayer with root permission."));
		d->ui.shutdown_check->setChecked(false);
	} else
		d->ui.shutdown_check->setChecked(d->dlg->exec());
}

void DockWidget::up() {
	d->move(true);
}

void DockWidget::down() {
	d->move(false);
}

void DockWidget::open() {
	EncodingFileDialog dlg(this, tr("Open File")
			, QString(), tr("Play List") + " (*.pls)");
	dlg.setFileMode(QFileDialog::ExistingFile);
	if (!dlg.exec())
		return;
	const QStringList file = dlg.selectedFiles();
	if (file.isEmpty() || file[0].isEmpty())
		return;
	Core::Playlist list;
	list.load(file[0], dlg.encoding());
	d->model->setPlaylist(list);
}

void DockWidget::save() {
	QString file = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
			tr("Play List") + " (*.pls)");
	if (!file.isEmpty()) {
		if (QFileInfo(file).suffix().compare("pls", Qt::CaseInsensitive) != 0)
			file += ".pls";
		d->model->playlist().save(file);
	}
}

void DockWidget::setAutoShutdown(bool shut) {
	d->ui.shutdown_check->setChecked(shut);
}

void DockWidget::add() {
	const QString filter = Helper::mediaExtensionFilter();
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Open File"), QString(), filter);
	Core::Playlist pl;
	for (int i=0; i<files.size(); ++i)
		pl.append(QUrl::fromLocalFile(files[i]));
	d->model->append(pl);
}

void DockWidget::erase() {
	const int row = d->ui.list->currentIndex().row();
	if (row < 0)
		return;
	d->model->remove(row);
	d->ui.list->setCurrentIndex(d->model->index(row, 0));
}

void DockWidget::checkShutdown() {
	if (!d->ui.shutdown_check->isChecked())
		return;
	QStringList command = d->dlg->command();
	if (QProcess::startDetached(command[0], command.mid(1)))
		qApp->quit();
}

void DockWidget::slotActivated(const QModelIndex &index) {
	if (index.isValid())
		d->model->play(index.row());
}

void DockWidget::adjustCellSize() {
	d->ui.list->resizeColumnsToContents();
	d->ui.list->resizeRowsToContents();
}

void DockWidget::showEvent(QShowEvent *event) {
	QDockWidget::showEvent(event);
	adjustCellSize();
}

