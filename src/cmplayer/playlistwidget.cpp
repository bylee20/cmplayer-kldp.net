#include "playlistwidget.h"
#include "playlistmodel.h"
#include "playlistview.h"
#include "button.h"
#include "encodingfiledialog.h"
#include "helper.h"
#include <core/playlist.h>
#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>

PlaylistWidget::ShutdownDialog::ShutdownDialog(QWidget *parent)
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

void PlaylistWidget::ShutdownDialog::accept() {
	if (combo->currentText().trimmed().isEmpty())
		QMessageBox::warning(this, tr("Automatic Shutdown")
				, tr("Command for Shutdown is Empty."));
	else
		QDialog::accept();
}

QStringList PlaylistWidget::ShutdownDialog::command() const {
	return combo->currentText().trimmed().split(" ");
}


struct PlaylistWidget::Data {
	QMenu *context;
	QAction *open, *save, *add, *erase, *up, *down, *clear;
	QCheckBox *shutdownCheck;
	ShutdownDialog *shutdownDlg;
	PlaylistModel *model;
	PlaylistView *view;
	bool checking, adding;
	bool identified;
};

PlaylistWidget::PlaylistWidget(PlaylistModel *model, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->shutdownCheck = new QCheckBox(this);
	d->shutdownDlg = new ShutdownDialog(this);
	d->view = new PlaylistView(this);
	d->model = model;
	d->view->setModel(d->model);
	d->checking = d->adding = d->identified = false;
	setContextMenuPolicy(Qt::CustomContextMenu);
	d->context = new QMenu(this);
	d->open = d->context->addAction("");
	d->save = d->context->addAction("");
	d->add = d->context->addAction("");
	d->erase = d->context->addAction("");
	d->up = d->context->addAction("");
	d->down = d->context->addAction("");
	d->clear = d->context->addAction("");
	
	connect(d->open, SIGNAL(triggered()), this, SLOT(open()));
	connect(d->save, SIGNAL(triggered()), this, SLOT(save()));
	connect(d->add, SIGNAL(triggered()), this, SLOT(add()));
	connect(d->erase, SIGNAL(triggered()), this, SLOT(erase()));
	connect(d->up, SIGNAL(triggered()), this, SLOT(up()));
	connect(d->down, SIGNAL(triggered()), this, SLOT(down()));
	connect(d->clear, SIGNAL(triggered()), d->model, SLOT(clear()));
	connect(d->view, SIGNAL(doubleClicked(const QModelIndex&)),
		  this, SLOT(slotDblClicked(const QModelIndex&)));
	connect(d->shutdownCheck, SIGNAL(toggled(bool)), this, SLOT(checkRoot(bool)));

	connect(d->model, SIGNAL(playlistFinished()), this, SLOT(checkShutdown()));
	connect(d->model, SIGNAL(rowCountChanged(int)), this, SLOT(adjustCellSize()));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&))
			, this, SLOT(showContextMenu(const QPoint&)));
	
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->addWidget(d->view);
	vbox->addWidget(d->shutdownCheck);
	
	retranslateUi();
}

PlaylistWidget::~PlaylistWidget() {
	delete d;
}

void PlaylistWidget::retranslateUi() {
	d->open->setText(tr("Open"));
	d->save->setText(tr("Save"));
	d->add->setText(tr("Add"));
	d->erase->setText(tr("Erase"));
	d->up->setText(tr("Up"));
	d->down->setText(tr("Down"));
	d->clear->setText(tr("Clear"));
	d->shutdownCheck->setText(tr("Shutdown when the list finished"));
}

void PlaylistWidget::checkRoot(bool checked) {
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
		d->shutdownCheck->setChecked(false);
	} else
		d->shutdownCheck->setChecked(d->shutdownDlg->exec());
}

void PlaylistWidget::up() {
	move(true);
}

void PlaylistWidget::down() {
	move(false);
}

void PlaylistWidget::open() {
	QString enc;
	const QString file = EncodingFileDialog::getOpenFileName(this, tr("Open File")
			, QString(), tr("Playlist") + " (*.pls)", &enc);
	if (!file.isEmpty()) {
		Core::Playlist list;
		list.load(file, enc);
		d->model->setPlaylist(list);
	}
}

void PlaylistWidget::save() {
	QString file = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
			tr("Playlist") + " (*.pls)");
	if (!file.isEmpty()) {
		if (QFileInfo(file).suffix().compare("pls", Qt::CaseInsensitive) != 0)
			file += ".pls";
		d->model->playlist().save(file);
	}
}

void PlaylistWidget::setAutoShutdown(bool shut) {
	d->shutdownCheck->setChecked(shut);
}

void PlaylistWidget::add() {
	const QString filter = Helper::mediaExtensionFilter();
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Open File"), QString(), filter);
	Core::Playlist pl;
	for (int i=0; i<files.size(); ++i)
		pl.append(Core::MediaSource(QUrl::fromLocalFile(files[i])));
	d->model->append(pl);
}

void PlaylistWidget::erase() {
	QList<int> rows = d->view->selectedRows();
	if (rows.isEmpty())
		return;
	qSort(rows);
	for (int i=rows.size() - 1; i >= 0; --i)
		d->model->remove(rows[i]);
	d->view->setCurrentIndex(d->model->index(rows.first(), 0));
}

void PlaylistWidget::checkShutdown() {
	if (!d->shutdownCheck->isChecked())
		return;
	QStringList command = d->shutdownDlg->command();
	if (QProcess::startDetached(command[0], command.mid(1)))
		qApp->quit();
}

void PlaylistWidget::slotDblClicked(const QModelIndex &index) {
	if (index.isValid())
		d->model->play(index.row());
}

void PlaylistWidget::adjustCellSize() {
	d->view->resizeColumnsToContents();
	d->view->resizeRowsToContents();
}

void PlaylistWidget::move(bool up) {
	const int row = d->view->currentIndex().row();
	int target = up ? row-1 : row + 1;
	if (d->model->swap(row, target))
		d->view->setCurrentIndex(d->model->index(target, 0));
}

void PlaylistWidget::showContextMenu(const QPoint &pos) {
	d->context->exec(mapToGlobal(pos));
}
