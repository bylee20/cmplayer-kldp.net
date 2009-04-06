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
	Button *open, *save, *add, *erase, *up, *down, *clear;
	QWidget *buttons;
	QCheckBox *shutdownCheck;
	ShutdownDialog *shutdownDlg;
	PlaylistModel *model;
	PlaylistView *view;
	bool checking, adding;
	bool identified;
};

PlaylistWidget::PlaylistWidget(PlaylistModel *model, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->open = new Button(QIcon(":/img/document-open.png"), this);
	d->save = new Button(QIcon(":/img/document-save.png"), this);
	d->add = new Button(QIcon(":/img/list-add.png"), this);
	d->erase = new Button(QIcon(":/img/list-remove.png"), this);
	d->up = new Button(QIcon(":/img/arrow-up.png"), this);
	d->down = new Button(QIcon(":/img/arrow-down.png"), this);
	d->clear = new Button(QIcon(":/img/edit-clear-list.png"), this);
	d->shutdownCheck = new QCheckBox(this);
	d->shutdownDlg = new ShutdownDialog(this);
	d->view = new PlaylistView(this);
	d->model = model;
	d->view->setModel(d->model);
	d->checking = d->adding = d->identified = false;

	connect(d->open, SIGNAL(clicked()), this, SLOT(open()));
	connect(d->add, SIGNAL(clicked()), this, SLOT(add()));
	connect(d->erase, SIGNAL(clicked()), this, SLOT(erase()));
	connect(d->up, SIGNAL(clicked()), this, SLOT(up()));
	connect(d->down, SIGNAL(clicked()), this, SLOT(down()));
	connect(d->clear, SIGNAL(clicked()), d->model, SLOT(clear()));
	connect(d->view, SIGNAL(doubleClicked(const QModelIndex&)),
		  this, SLOT(slotDblClicked(const QModelIndex&)));
	connect(d->shutdownCheck, SIGNAL(toggled(bool)), this, SLOT(checkRoot(bool)));
	connect(d->save, SIGNAL(clicked()), this, SLOT(save()));
	connect(d->model, SIGNAL(playlistFinished()), this, SLOT(checkShutdown()));
	connect(d->model, SIGNAL(rowCountChanged(int)), this, SLOT(adjustCellSize()));

	d->buttons = new QWidget;
	QHBoxLayout *hbox = new QHBoxLayout(d->buttons);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	hbox->addWidget(d->open);
	hbox->addWidget(d->save);
	hbox->addWidget(d->add);
	hbox->addWidget(d->erase);
	hbox->addWidget(d->up);
	hbox->addWidget(d->down);
	hbox->addWidget(d->clear);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	
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
	d->open->setToolTip(tr("Open"));
	d->save->setToolTip(tr("Save"));
	d->add->setToolTip(tr("Add"));
	d->erase->setToolTip(tr("Erase"));
	d->up->setToolTip(tr("Up"));
	d->down->setToolTip(tr("Down"));
	d->clear->setToolTip(tr("Clear"));
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
	Core::Playlist list;
	list.load(file, enc);
	d->model->setPlaylist(list);
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

int PlaylistWidget::controlHeight() const {
	return d->open->rect().bottom()+2;
}

QWidget *PlaylistWidget::buttons() {
	return d->buttons;
}
