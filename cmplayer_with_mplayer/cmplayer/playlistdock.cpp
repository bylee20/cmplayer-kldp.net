#include "playlistdock.h"
#include "playlistdock_p.h"
#include "ui/ui_playlistwidget.h"
#include "playlistmodel.h"
#include <mplayer/informations.h>
#include <mplayer/playlist.h>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

ShutdownDialog::ShutdownDialog(QWidget *parent)
: QDialog(parent) {
	QLabel *label = new QLabel(this);
	label->setText(trUtf8("시스템 종료를 위한 명령어를 선택 혹은 입력해주세요."));
	combo = new QComboBox(this);
	combo->setEditable(true);
	combo->addItems(QStringList() << "shutdown -h now" << "halt");
	combo->setCurrentIndex(0);
	QPushButton *ok = new QPushButton(trUtf8("확인(&O)"), this);
	ok->setDefault(true);
	QPushButton *cancel = new QPushButton(trUtf8("취소(&C)"), this);
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
		QMessageBox::warning(this, trUtf8("자동종료"), trUtf8("종료 명령어가 비어있습니다."));
	else
		QDialog::accept();
}

struct PlayListDock::Data {
	Data(PlayListModel *model, PlayListDock *parent)
	: p(parent), model(model), checking(false)
	, adding(false) {
		QWidget *w = new QWidget(p);
		ui.setupUi(w);
		p->setWidget(w);
		p->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		dlg = new ShutdownDialog(p);

		connect(ui.open_button, SIGNAL(clicked()), p, SLOT(open()));
		connect(ui.add_button, SIGNAL(clicked()), p, SLOT(add()));
		connect(ui.erase_button, SIGNAL(clicked()), p, SLOT(erase()));
		connect(ui.up_button, SIGNAL(clicked()), p, SLOT(up()));
		connect(ui.down_button, SIGNAL(clicked()), p, SLOT(down()));
		connect(ui.clear_button, SIGNAL(clicked()), model, SLOT(clear()));
		connect(ui.close_button, SIGNAL(clicked()), p, SLOT(hide()));
		connect(ui.list, SIGNAL(activated(const QModelIndex&)),
				p, SLOT(slotActivated(const QModelIndex&)));
		connect(ui.shutdown_check, SIGNAL(toggled(bool)), p, SLOT(checkRoot(bool)));
		connect(ui.save_button, SIGNAL(clicked()), p, SLOT(save()));

		connect(model, SIGNAL(playListFinished()), p, SLOT(checkShutdown()));
		connect(model, SIGNAL(rowCountChanged(int)), p, SLOT(adjustCellSize()));
		identified = false;
		ui.list->setModel(model);
		p->setWindowTitle(trUtf8("재생 목록"));
	}
	void move(bool up) {
		const int row = ui.list->currentIndex().row();
		int target = up ? row-1 : row + 1;
		if (model->swap(row, target))
			ui.list->setCurrentIndex(model->index(target, 0));
	}
	PlayListDock *p;
	Ui::Ui_PlayListWidget ui;
	PlayListModel *model;
	bool checking, adding;
	bool identified;
	ShutdownDialog *dlg;
};

PlayListDock::PlayListDock(PlayListModel *model, QWidget *parent)
: QDockWidget(parent), d(new Data(model, this)) {

}

PlayListDock::~PlayListDock() {
	delete d;
}

void PlayListDock::checkRoot(bool checked) {
	if (!checked)
		return;
	static QProcess proc;
	proc.start("whoami");
	if (!proc.waitForFinished(1000))
		proc.kill();
	static QRegExp rxRoot("^root\\r*\\n*$");
	if (rxRoot.indexIn(proc.readAllStandardOutput()) == -1) {
		QMessageBox::warning(this, trUtf8("자동종료"), trUtf8("루트권한으로 CMPlayer를 실행해야합니다."));
		d->ui.shutdown_check->setChecked(false);
	} else
		d->ui.shutdown_check->setChecked(d->dlg->exec());
}

void PlayListDock::up() {
	d->move(true);
}

void PlayListDock::down() {
	d->move(false);
}

void PlayListDock::open() {
	QString file = QFileDialog::getOpenFileName(this, trUtf8("파일 열기"), QString(),
			trUtf8("재생 목록") + " (*.pls)");
	if (file.isEmpty())
		return;
	MPlayer::PlayList pl;
	pl.load(file);
	d->model->setPlayList(pl);
}

void PlayListDock::save() {
	QString file = QFileDialog::getSaveFileName(this, trUtf8("파일 저장"), QString(),
			trUtf8("재생 목록") + " (*.pls)");
	if (!file.isEmpty()) {
		if (QFileInfo(file).suffix().compare("pls", Qt::CaseInsensitive) != 0)
			file += ".pls";
		d->model->playList().save(file);
	}
}

void PlayListDock::setAutoShutdown(bool shut) {
	d->ui.shutdown_check->setChecked(shut);
}

void PlayListDock::add() {
	static const MPlayer::Informations *info = MPlayer::Informations::get();
	static const QString Filter = trUtf8("비디오 파일") +' ' + info->videoExtensions().toFilter() + ";;"
			+ trUtf8("음악 파일") + ' ' + info->audioExtensions().toFilter() + ";;"
			+ trUtf8("모든 파일") + ' ' + "(*.*)";
	QStringList files = QFileDialog::getOpenFileNames(this, trUtf8("파일 열기"), QString(), Filter);
	MPlayer::PlayList pl;
	for (int i=0; i<files.size(); ++i)
		pl.append(MPlayer::MediaSource(files[i]));
	d->model->addSources(pl);
}

void PlayListDock::erase() {
	const int row = d->ui.list->currentIndex().row();
	if (row < 0)
		return;
	d->model->remove(row);
	d->ui.list->setCurrentIndex(d->model->index(row, 0));
}

void PlayListDock::checkShutdown() {
	if (!d->ui.shutdown_check->isChecked())
		return;
	QStringList command = d->dlg->command();
	if (QProcess::startDetached(command[0], command.mid(1)))
		qApp->quit();
}

void PlayListDock::slotActivated(const QModelIndex &index) {
	if (index.isValid())
		d->model->play(index.row());
}

void PlayListDock::adjustCellSize() {
	d->ui.list->resizeColumnsToContents();
	d->ui.list->resizeRowsToContents();
}

void PlayListDock::showEvent(QShowEvent *event) {
	QDockWidget::showEvent(event);
	adjustCellSize();
}

