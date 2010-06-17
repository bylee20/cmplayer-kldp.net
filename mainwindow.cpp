#include "mainwindow.hpp"
#include "subtitlerenderer.hpp"
#include <QtGui/QStackedWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include "controlwidget.hpp"
#include "menu.hpp"
#include "playengine.hpp"
#include "nativevideorenderer.hpp"
#include "timelineosdrenderer.hpp"
#include "textosdrenderer.hpp"
#include "audiocontroller.hpp"

struct MainWindow::Data {
	Data(Menu &menu): menu(menu) {}
	QStackedWidget *stack;
	Menu &menu;
	PlayEngine *engine;
	SubtitleRenderer *subtitle;
	TimeLineOsdRenderer *timeLine;
	TextOsdRenderer *message;
};

MainWindow::MainWindow(): d(new Data(Menu::create(this))) {
	d->stack = new QStackedWidget(this);
	d->engine = new PlayEngine;
	d->subtitle = new SubtitleRenderer;
	d->timeLine = new TimeLineOsdRenderer;
	d->message = new TextOsdRenderer(Qt::AlignTop | Qt::AlignLeft);
	d->engine->renderer()->setContextMenuPolicy(Qt::CustomContextMenu);
	d->stack->addWidget(d->engine->renderer());
	setupUi();
	d->menu.updatePref();


	Menu &menu = d->menu;
	Menu &open = d->menu("open");
	connect(open["file"], SIGNAL(triggered()), this, SLOT(openFile()));
//	connect(open["url"], SIGNAL(triggered()), this, SLOT(open()));
//	connect(open["dvd"], SIGNAL(triggered()), this, SLOT(open()));
//	connect(open("recent").g(), SIGNAL(triggered(Core::Mrl)), this, SLOT(openMrl(Core::Mrl)));
//	connect(open("recent")["clear"], SIGNAL(triggered()), d->recent, SLOT(clearStack()));
	Menu &screen = menu("screen");
//	connect(screen("size").g(), SIGNAL(triggered(double)), this, SLOT(setVideoSize(double)));
//	connect(screen("aspect").g(), SIGNAL(triggered(double)), d->engine, SLOT(setAspectRatio(double)));
//	connect(screen("crop").g(), SIGNAL(triggered(double)), d->engine, SLOT(setCropRatio(double)));
//	connect(screen("on top").g(), SIGNAL(triggered(QAction*)), this, SLOT(updateOnTop()));
//	connect(screen["snapshot"], SIGNAL(triggered()), this, SLOT(takeSnapshot()));
	Menu &play = menu("play");
//	connect(play["dvd menu"], SIGNAL(triggered()), d->engine, SLOT(toggleDvdMenu()));
	connect(play["stop"], SIGNAL(triggered()), d->engine, SLOT(stop()));
//	connect(play("speed").g(), SIGNAL(triggered(int)), this, SLOT(setSpeed(int)));
//	connect(play["prev"], SIGNAL(triggered()), d->model, SLOT(playPrevious()));
//	connect(play["next"], SIGNAL(triggered()), d->model, SLOT(playNext()));
	connect(play["pause"], SIGNAL(triggered()), this, SLOT(togglePlayPause()));
//	connect(play["list"], SIGNAL(triggered()), this, SLOT(toggleToolBoxVisibility()));
//	connect(play("engine").g(), SIGNAL(triggered(const QString &))
//			, d->engine, SLOT(setBackend(const QString &)));
//	connect(play("repeat").g(), SIGNAL(triggered(int)), this, SLOT(slotRepeat(int)));
	connect(play("seek").g(), SIGNAL(triggered(int)), this, SLOT(seek(int)));
	Menu &video = menu("video");
//	connect(video.g("color"), SIGNAL(triggered(QAction*))
//			, this, SLOT(setColorProperty(QAction*)));
//	connect(video("renderer").g(), SIGNAL(triggered(QAction*))
//			, this, SLOT(setRenderer(QAction*)));
	Menu &audio = menu("audio");
	connect(audio.g("volume"), SIGNAL(triggered(int)), this, SLOT(setVolume(int)));
	connect(audio["mute"], SIGNAL(toggled(bool)), this, SLOT(setMuted(bool)));
//	connect(audio.g("amp"), SIGNAL(triggered(int))
//			, this, SLOT(setAmplifyingRate(int)));
//	connect(audio("renderer").g(), SIGNAL(triggered(QAction*))
//			, this, SLOT(setRenderer(QAction*)));
//	connect(audio("track").g(), SIGNAL(triggered(const QString &))
//			, d->engine, SLOT(setCurrentTrack(const QString &)));
	Menu &sub = menu("subtitle");
//	connect(sub("list")["hide"], SIGNAL(toggled(bool))
//			, d->engine, SLOT(setSubtitleHidden(bool)));
//	connect(sub("list")["open"], SIGNAL(triggered()), this, SLOT(openSubFile()));
//	connect(sub("list")["clear"], SIGNAL(triggered()), this, SLOT(clearSubs()));
//	connect(sub("list").g(), SIGNAL(triggered(QAction*))
//			, this, SLOT(slotSubtitle(QAction*)));
//	connect(sub["viewer"], SIGNAL(triggered()), this, SLOT(openSubViewer()));
//	connect(sub.g("pos"), SIGNAL(triggered(int)), this, SLOT(setSubtitlePos(int)));
//	connect(sub.g("sync"), SIGNAL(triggered(int)), this, SLOT(setSyncDelay(int)));
//	connect(menu["pref"], SIGNAL(triggered()), this, SLOT(showPrefDialog()));
//	connect(menu["about"], SIGNAL(triggered()), this, SLOT(showAbout()));
//// 	connect(menu["help"], SIGNAL(triggered()), this, SLOT(slotHelp()));
//	connect(menu["exit"], SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(updateMrl(Mrl)));
//	connect(d->engine, SIGNAL(stateChanged(Core::State, Core::State))
//			, this, SLOT(slotStateChanged(Core::State, Core::State)));
//	connect(d->engine, SIGNAL(stateChanged(Core::State, Core::State))
//			, this, SLOT(slotStateChanged(Core::State, Core::State)));
//	connect(d->engine, SIGNAL(mutedChanged(bool))
//			, audio["mute"], SLOT(setChecked(bool)));
//	connect(d->engine, SIGNAL(currentSourceChanged(const Core::MediaSource&))
//			, this, SLOT(autoLoadSubtitles()));
//	connect(d->engine, SIGNAL(backendChanged(const QString&))
//			, this, SLOT(slotBackendChanged()));
//
//	connect(d->engine, SIGNAL(tracksChanged(const QStringList&))
//			, this, SLOT(slotTracksChanged(const QStringList&)));
//	connect(d->engine, SIGNAL(currentTrackChanged(const QString&))
//			, this, SLOT(slotCurrentTrackChanged(const QString&)));
//	connect(d->engine, SIGNAL(spusChanged(const QStringList&))
//			, this, SLOT(slotSpusChanged(const QStringList&)));
//	connect(d->engine, SIGNAL(currentSpuChanged(const QString&))
//			, this, SLOT(slotCurrentSpuChanged(const QString&)));
	connect(d->engine->renderer(), SIGNAL(customContextMenuRequested(const QPoint&))
			, this, SLOT(showContextMenu(const QPoint&)));
//	connect(d->toolBox, SIGNAL(hidingRequested())
//			, this, SLOT(toggleToolBoxVisibility()));
//	connect(d->model, SIGNAL(currentRowChanged(int))
//			, this, SLOT(updatePlaylistInfo()));
//	connect(d->model, SIGNAL(rowCountChanged(int))
//			, this, SLOT(updatePlaylistInfo()));
//	connect(d->recent, SIGNAL(sourcesChanged(const RecentStack&))
//			, this, SLOT(updateRecentActions(const RecentStack&)));
//	connect(d->recent, SIGNAL(rememberCountChanged(int))
//			, this, SLOT(updateRecentSize(int)));
//	connect(&d->hider, SIGNAL(timeout()), this, SLOT(hideCursor()));
//	d->hider.setSingleShot(true);
//	connect(d->tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason))
//			, this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));

	d->engine->renderer()->addOsdRenderer(d->subtitle->osd());
	d->engine->renderer()->addOsdRenderer(d->timeLine);
	d->engine->renderer()->addOsdRenderer(d->message);
	connect(d->engine->renderer(), SIGNAL(frameRateChanged(double))
		, d->subtitle, SLOT(setFrameRate(double)));
	connect(d->engine, SIGNAL(tick(int)), d->subtitle, SLOT(render(int)));
}

MainWindow::~MainWindow() {
	delete d->subtitle;
	delete d->timeLine;
	delete d->message;
	delete d->engine;
	delete d;
}

void MainWindow::setupUi() {
	QWidget *center = new QWidget(this);
	center->setMouseTracking(true);

	ControlWidget *control = new ControlWidget(d->engine, center);
	Menu &play = d->menu("play");
	control->connectMute(d->menu("audio")["mute"]);
	control->connectPlay(play["pause"]);
	control->connectStop(play["stop"]);
	control->connectPrevious(play["prev"]);
	control->connectNext(play["next"]);
	control->connectForward(play("seek")["forward1"]);
	control->connectBackward(play("seek")["backward1"]);
	control->connectOpen(d->menu("open")["file"]);
	control->connectFullScreen(d->menu("screen")("size")["full"]);
	control->connectPlaylist(d->menu("play")["list"]);

	QVBoxLayout *vbox = new QVBoxLayout(center);
	vbox->addWidget(d->stack);
	vbox->addWidget(control);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->setSpacing(0);

	menuBar()->hide();
	setMouseTracking(true);
	setCentralWidget(center);
}

void MainWindow::openFile() {
//	const QString filter = Helper::mediaExtensionFilter();
//	const QString filePath = QFileDialog::getOpenFileName(this
//			, tr("Open File"), s[State::LastOpenFile].toString(), filter);
//	if (!filePath.isEmpty()) {
//		const Core::Mrl mrl = Core::Mrl::fromLocalFile(filePath);
//		s[State::LastOpenFile] = mrl.location();
//		openMrl(mrl);
//	}
	const QString file = QFileDialog::getOpenFileName(this
				, tr("Open File")/*, s[State::LastOpenFile].toString(), filter*/);
	d->engine->stop();
	d->engine->setMrl(Mrl::fromLocalFile(file));
	d->engine->play();
}

void MainWindow::togglePlayPause() {
	if (d->engine->state() == PlayingState)
		d->engine->pause();
	else
		d->engine->play();
}

void MainWindow::showContextMenu(const QPoint &pos) {
	d->menu.contextMenu()->exec(mapToGlobal(pos));
}

void MainWindow::updateMrl(const Mrl &mrl) {
	const QStringList filter = QStringList() << "*.smi";
//	if (!source.isLocalFile() || d->pref.subtitleAutoLoad == NoAutoLoad)
//		return Core::Subtitle();
//	const QStringList filter = Core::Info::subtitleExtension().toNameFilter();
	const QFileInfo fileInfo(mrl.url().toLocalFile());
	const QFileInfoList all = fileInfo.dir().entryInfoList(filter, QDir::Files, QDir::Name);
	const QString base = fileInfo.completeBaseName();
	QString subFile;
	for (int i=0; i<all.size(); ++i) {
		if (all[i].completeBaseName() == base) {
			subFile = all[i].absoluteFilePath();
			break;
		}
	}
	if (subFile.isEmpty())
		return;
	const Subtitle sub = Subtitle::parse(subFile, "euc-kr");
	if (sub.isEmpty())
		return;
	d->subtitle->setSubtitle(sub);

//	const Core::MediaSource source = d->player->currentSource();
//	d->menu("subtitle")("list").g()->setExclusive(source.isDisc());
//	if (!source.isLocalFile())
//		return;
//	d->sub = loadSubtitle(source);
//	d->subIdxes = selectSubtitle(source, d->sub);
//	Menu &list = d->menu("subtitle")("list");
//	d->changingSubtitle = true;
//	for (int i=0; i<d->sub.size(); ++i) {
//		QAction *action = list.addActionToGroupWithoutKey(d->sub[i].name(), true);
//		action->setData(i);
//		action->setChecked(d->subIdxes.contains(i));
//	}
//	d->changingSubtitle = false;
//	updateSubtitle();
}

void MainWindow::seek(int diff) {
	if (!diff || d->engine->state() == StoppedState)
		return;
	const int target = qBound(0, d->engine->position() + diff, d->engine->duration());
	if (d->engine->seek(target)) {
		d->timeLine->show(target, d->engine->duration());
		showMessage(tr("Seeking"), diff/1000, tr("sec"), true);
	}
}

void MainWindow::showMessage(const QString &cmd, int value, const QString &unit, bool sign, int last) {
	if (!sign || value < 0)
		showMessage(cmd, QString::number(value) + unit, last);
	else
		showMessage(cmd, QString::fromUtf8(value ? "+" : "±")
			+ QString::number(value) + unit, last);
}

void MainWindow::showMessage(const QString &cmd, const QString &description, int last) {
	showMessage(cmd + " : " + description, last);
}

void MainWindow::showMessage(const QString &message, int last) {
	d->message->showText(message, last);
}

void MainWindow::setVolume(int diff) {
	if (!diff)
		return;
	const int volume = qBound(0, d->engine->audio()->volume() + diff, 100);
	d->engine->audio()->setVolume(volume);
	showMessage(tr("Volume"), volume, "%");
}

void MainWindow::setMuted(bool muted) {
	d->engine->audio()->setMuted(muted);
	showMessage(tr("Mute"), muted ? tr("On") : tr("Off"));
}
