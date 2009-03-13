#include "prefdialog.h"
#include "state.h"
#include "playlistmodel.h"
#include "mainwindow.h"
#include "recentinfo.h"
#include "playinfowidget.h"
#include "encodingfiledialog.h"
#include "pref.h"
#include "helper.h"
#include "dockwidget.h"
#include "videoplayer.h"
#include "menu.h"
#include "sliders.h"
#include "abrepeatdialog.h"
#include "translator.h"
#include <core/info.h>
#include <core/backendiface.h>
#include <core/playlist.h>
#include <core/subtitle.h>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMenuBar>
#include <QtGui/QApplication>
#include <QtCore/QSet>
#include <QtCore/QDebug>
#include <cmath>
#include <QTime>

struct MainWindow::Data {
	Data(Menu &menu): dragPos(-1, -1), menu(menu) {}
	QWidget *control, *center;
	PlayInfoWidget *playInfo;
	PlayListModel *model;
	DockWidget *dock;
	QList<Core::Subtitle> subs;
	QList<int> subIdxes;
	QMenu *contextMenu;
	QSize prevWinSize;
	QPoint dragPos;
	QMap<ClickAction, QAction*> clickAction;
	QMap<WheelAction, ActionPair> wheelAction;
	RecentInfo *recent;
	bool pausedByHiding;
	VideoPlayer *player;
	Pref *pref;
	Menu &menu;
	ABRepeatDialog *repeater;
};

MainWindow::MainWindow()
: d(new Data(Menu::create(this))) {
	d->pausedByHiding = false;
	d->player = new VideoPlayer(this);
	d->repeater = new ABRepeatDialog(this);
	d->model = new PlayListModel(d->player);
	d->recent = RecentInfo::get();
	d->pref = Pref::get();
	
	setupUi();
	initIface();
	updateRecentActions(d->recent->sources());
	
	Menu &menu = d->menu;
	Menu &open = menu("open");
	connect(open["file"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open["dvd"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open("recent").g(), SIGNAL(triggered(const QUrl &)), this, SLOT(open(const QUrl &)));
	connect(open("recent")["clear"], SIGNAL(triggered()), d->recent, SLOT(clearStack()));
	Menu &screen = menu("screen");
	connect(screen("size").g(), SIGNAL(triggered(double)), this, SLOT(setVideoSize(double)));
	connect(screen("aspect").g(), SIGNAL(triggered(double)), d->player, SLOT(setAspectRatio(double)));
	connect(screen("crop").g(), SIGNAL(triggered(double)), d->player, SLOT(setCropRatio(double)));
	connect(screen("on top").g(), SIGNAL(triggered(QAction*)), this, SLOT(updateOnTop()));
	Menu &play = menu("play");
	connect(play["dvd menu"], SIGNAL(triggered()), d->player, SLOT(toggleDvdMenu()));
	connect(play["stop"], SIGNAL(triggered()), d->player, SLOT(stop()));
	connect(play("speed").g(), SIGNAL(triggered(int)), this, SLOT(setSpeed(int)));
	connect(play("speed")["reset"], SIGNAL(triggered()), d->player, SLOT(resetSpeed()));
	connect(play["prev"], SIGNAL(triggered()), d->model, SLOT(playPrevious()));
	connect(play["next"], SIGNAL(triggered()), d->model, SLOT(playNext()));
	connect(play["pause"], SIGNAL(triggered()), this, SLOT(togglePlayPause()));
	connect(play["list"], SIGNAL(triggered()), this, SLOT(toggleDockVisibility()));
	connect(play("engine").g(), SIGNAL(triggered(QAction*)), this, SLOT(setBackend(QAction*)));
	connect(play("repeat")["range"], SIGNAL(triggered()), this, SLOT(slotAbRange()));
	connect(play("repeat")["quit"], SIGNAL(triggered()), this, SLOT(slotAbQuit()));
	connect(play("repeat")["advanced"], SIGNAL(triggered()), this, SLOT(slotAbAdvanced()));
	connect(play("seek").g(), SIGNAL(triggered(int)), this, SLOT(seek(int)));
	Menu &video = menu("video");
	connect(video.g("prop"), SIGNAL(triggered(QAction*)), this, SLOT(setVideoProperty(QAction*)));
	connect(video("renderer").g(), SIGNAL(triggered(QAction*)), this, SLOT(setRenderer(QAction*)));
	Menu &audio = menu("audio");
	connect(audio.g("volume"), SIGNAL(triggered(int)), this, SLOT(setVolume(int)));
	connect(audio["mute"], SIGNAL(toggled(bool)), d->player, SLOT(setMuted(bool)));
	connect(audio.g("amp"), SIGNAL(triggered(int)), this, SLOT(setAmplifyingRate(int)));
	connect(audio("renderer").g(), SIGNAL(triggered(QAction*)), this, SLOT(setRenderer(QAction*)));
	connect(audio("track").g(), SIGNAL(triggered(const QString &))
			, d->player, SLOT(setCurrentTrack(const QString &)));
	Menu &sub = menu("subtitle");
	connect(sub("list")["hide"], SIGNAL(toggled(bool)), d->player, SLOT(setSubtitleHidden(bool)));
	connect(sub("list")["open"], SIGNAL(triggered()), this, SLOT(openSubFile()));
	connect(sub("list")["clear"], SIGNAL(triggered()), this, SLOT(clearSubs()));
	connect(sub("list").g(), SIGNAL(triggered(QAction*)), this, SLOT(slotSubtitle(QAction*)));
	connect(sub.g("pos"), SIGNAL(triggered(int)), this, SLOT(setSubtitlePos(int)));
	connect(sub.g("sync"), SIGNAL(triggered(int)), this, SLOT(setSyncDelay(int)));
	connect(menu["pref"], SIGNAL(triggered()), this, SLOT(showPrefDialog()));
// 	connect(menu["help"], SIGNAL(triggered()), this, SLOT(slotHelp()));
	connect(menu["exit"], SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, d->playInfo, SLOT(setState(Core::State)));
	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(d->player, SIGNAL(mutedChanged(bool)), audio["mute"], SLOT(setChecked(bool)));
	connect(d->player, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, d->playInfo, SLOT(setCurrentSource(const Core::MediaSource&)));
	connect(d->player, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, this, SLOT(autoLoadSubtitles()));
	connect(d->player, SIGNAL(backendChanged(const QString&)), this, SLOT(slotBackendChanged()));
	connect(d->player, SIGNAL(durationChanged(int)), d->playInfo, SLOT(setDuration(int)));
	connect(d->player, SIGNAL(tick(int)), d->playInfo, SLOT(setPlayTime(int)));
	connect(d->player, SIGNAL(tracksChanged(const QStringList&))
			, this, SLOT(slotTracksChanged(const QStringList&)));
	connect(d->player, SIGNAL(currentTrackChanged(const QString&))
			, this, SLOT(slotCurrentTrackChanged(const QString&)));
	connect(d->player, SIGNAL(spusChanged(const QStringList&))
			, this, SLOT(slotSpusChanged(const QStringList&)));
	connect(d->player, SIGNAL(currentSpuChanged(const QString&))
			, this, SLOT(slotCurrentSpuChanged(const QString&)));
	connect(d->player, SIGNAL(stopped(Core::MediaSource, int))
			, d->recent, SLOT(setStopped(const Core::MediaSource&, int)));
	connect(d->player, SIGNAL(finished(Core::MediaSource))
			, d->recent, SLOT(setFinished(const Core::MediaSource&)));
	connect(d->player, SIGNAL(customContextMenuRequested(const QPoint&))
			, this, SLOT(showContextMenu(const QPoint&)));
	connect(d->model, SIGNAL(currentRowChanged(int)), this, SLOT(updatePlayListInfo()));
	connect(d->model, SIGNAL(rowCountChanged(int)), this, SLOT(updatePlayListInfo()));
	connect(d->recent, SIGNAL(sourcesChanged(const RecentStack&))
			, this, SLOT(updateRecentActions(const RecentStack&)));
	connect(d->recent, SIGNAL(rememberCountChanged(int)), this, SLOT(updateRecentSize(int)));
	
	const BackendMap &backend = VideoPlayer::load();
	Menu &engine = play("engine");
	for (BackendMap::const_iterator it = backend.begin(); it != backend.end(); ++it)
		engine.addActionToGroupWithoutKey(it.key(), true)->setData(it.key());
	updatePref();
	loadState();
	
	const QStringList args = QCoreApplication::arguments();
	if (args.size() > 1) {
		QUrl url(args.last());
		if (url.scheme().isEmpty())
			url = QUrl::fromLocalFile(args.last());
		this->open(url);
	} else {
		d->model->setPlayList(d->recent->lastPlayList());
		d->model->setCurrentSource(d->recent->lastSource());
	}
}

MainWindow::~MainWindow() {
	d->player->stop();
	RecentInfo *recent = RecentInfo::get();
	saveState();
	recent->setLastSource(d->player->currentSource());
	recent->setLastPlayList(d->model->playList());
	RecentInfo::get()->save();
	delete d->model;
	delete d->player;
	delete d;
}

QWidget *MainWindow::createControl(QWidget *parent) {
	QWidget *control = new QWidget(parent);
	QVBoxLayout *vbox = new QVBoxLayout(control);
	vbox->setMargin(0);
	vbox->setSpacing(0);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->setMargin(0);
	hbox->setSpacing(0);
	vbox->addLayout(hbox);
#define addToolButton(act) {\
	QToolButton *tb = new QToolButton; tb->setAutoRaise(true); \
	tb->setFocusPolicy(Qt::NoFocus); tb->setDefaultAction(act); hbox->addWidget(tb);}
	Menu &play = d->menu("play");
	addToolButton(play["prev"]);
	addToolButton(play["pause"]);
	addToolButton(play["stop"]);
	addToolButton(play["next"]);
	hbox->addWidget(new SeekSlider(d->player));
	addToolButton(d->menu("audio")["mute"]);
	hbox->addWidget(new VolumeSlider(d->player));
#undef addToolButton
	vbox->addWidget(d->playInfo = new PlayInfoWidget);
	control->setFixedHeight(control->sizeHint().height());
	return control;
}

void MainWindow::saveState() {
	State state;
	const VideoPlayer::Map engines = d->player->engines();
	State::Map vmap, amap;
	for (VideoPlayer::Map::const_iterator it = engines.begin(); it != engines.end(); ++it) {
		const Core::PlayEngine *engine = it.value();
		if (engine) {
			const QString name = engine->info().name();
			vmap[name] = engine->videoRenderer();
			amap[name] = engine->audioRenderer();
		}
	}
	state[State::VideoRenderer] = vmap;
	state[State::AudioRenderer] = amap;
	state[State::ScreenAspectRatio] = d->player->aspectRatio();
	state[State::ScreenCrop] = d->player->cropRatio();
	state[State::ScreenOnTop] = d->menu("screen")("on top").g()->data();
	state[State::AudioVolume] = d->player->volume();
	state[State::AudioMuted] = d->player->isMuted();
	state[State::AudioAmp] = d->player->amplifyingRate();
	state[State::PlaySpeed] = d->player->speed();
	state[State::SubtitlePos] = d->player->subtitlePos();
	state[State::SubtitleSync] = d->player->syncDelay();
	state.save();
}

void MainWindow::loadState() {
	State state;
	state.load();
	d->menu("screen")("aspect").g()->trigger(state[State::ScreenAspectRatio]);
	d->menu("screen")("crop").g()->trigger(state[State::ScreenCrop]);
	d->menu("screen")("on top").g()->trigger(state[State::ScreenOnTop]);
	d->player->setVolume(state[State::AudioVolume].toInt());
	d->player->setMuted(state[State::AudioMuted].toBool());
	d->player->setAmplifyingRate(state[State::AudioAmp].toInt());
	d->player->setSpeed(state[State::PlaySpeed].toInt());
	d->player->setSubtitlePos(state[State::SubtitlePos].toInt());
	d->player->setSyncDelay(state[State::SubtitleSync].toInt());
}

void MainWindow::setupUi() {
	d->dock = new DockWidget(d->model, this);
	addDockWidget(Qt::RightDockWidgetArea, d->dock);
	d->dock->hide();
	
	d->center = new QWidget(this);
	setCentralWidget(d->center);
	d->control = createControl(d->center);
	QVBoxLayout *vbox = new QVBoxLayout(d->center);
	vbox->addWidget(d->player);
	vbox->addWidget(d->control);
	vbox->setMargin(0);
	vbox->setSpacing(0);
	
	setMouseTracking(true);
	setAcceptDrops(true);
	d->center->setMouseTracking(true);

}

void MainWindow::showContextMenu(const QPoint &pos) {
	d->contextMenu->exec(mapToGlobal(pos));
}

void MainWindow::initIface() {
	Menu &menu = d->menu;
	d->clickAction[OpenFile] = menu("open")["file"];
	d->clickAction[ToggleFullScreen] = menu("screen")("size")["full"];
	d->clickAction[TogglePlayPause] = menu("play")["pause"];
	d->clickAction[ToggleMute] = menu("audio")["mute"];
	d->clickAction[TogglePlayList] = menu("play")["list"];
	
#define ADD_PAIR(pair, m, a1, a2) ((pair) = qMakePair(((m)[(a1)]), ((m)[(a2)])))
	ADD_PAIR(d->wheelAction[Seek1], menu("play")("seek"), "forward1", "backward1");
	ADD_PAIR(d->wheelAction[Seek2], menu("play")("seek"), "forward2", "backward2");
	ADD_PAIR(d->wheelAction[Seek3], menu("play")("seek"), "forward3", "backward3");
	ADD_PAIR(d->wheelAction[NextPrevious], menu("play"), "next", "prev");
	ADD_PAIR(d->wheelAction[VolumeUpDown], menu("audio"), "volume up", "volume down");
	ADD_PAIR(d->wheelAction[AmpUpDown], menu("audio"), "amp up", "amp down");
#undef ADD_PAIR
	
	d->contextMenu = new QMenu(this);
	d->contextMenu->addAction(menu("open")["file"]);
	d->contextMenu->addMenu(menu.m("open"));
	d->contextMenu->addSeparator();
	d->contextMenu->addMenu(menu.m("screen"));
	d->contextMenu->addMenu(menu.m("play"));
	d->contextMenu->addMenu(menu.m("subtitle"));
	d->contextMenu->addMenu(menu.m("video"));
	d->contextMenu->addMenu(menu.m("audio"));
	d->contextMenu->addSeparator();
	d->contextMenu->addAction(menu["pref"]);
// 	d->contextMenu->addAction(menu["help"]);
// 	d->contextMenu->addAction(menu["about"]);
	d->contextMenu->addSeparator();
	d->contextMenu->addAction(menu["exit"]);
	
	addActions(d->contextMenu->actions());
	menuBar()->hide();
}

void MainWindow::setVideoSize(double rate) {
	if (rate < 0.) {
		const bool wasFull = isFullScreen();
		setFullScreen(!wasFull);
		d->menu("screen")("size")["full"]->setChecked(!wasFull);
	} else {
		if (isFullScreen())
			setFullScreen(false);
		resize(size() - d->player->size() + d->player->sizeHint()*std::sqrt(rate));
	}
}

void MainWindow::slotHelp() {

}

void MainWindow::clearSubs() {
	d->subs.clear();
	d->subIdxes.clear();
	QList<QAction*> acts = d->menu("subtitle")("list").g()->actions();
	for (int i=0; i<acts.size(); ++i)
		delete acts[i];
	if (d->player)
		d->player->setSubtitle(Core::Subtitle());
}

void MainWindow::updateSubtitle() {
	if (d->player) {
		QList<int> merge = d->subIdxes;
		const QStringList priority = d->pref->subtitlePriority();
		QList<int> order;
		QList<int> indexes = d->subIdxes;
		QList<int>::iterator it;
		for (int i=0; i<priority.size(); ++i) {
			it = indexes.begin();
			while(it != indexes.end()) {
				if (d->subs[*it].language().language() == priority[i]) {
					order.append(*it);
					it = indexes.erase(it);
				} else
					++it;
			}
		}
		order += indexes;
		Core::Subtitle sub;
		for (int i=0; i<order.size(); ++i)
			sub |= d->subs[order[i]];
		d->player->setSubtitle(sub);
	}
}

void MainWindow::slotSubtitle(QAction *action) {
	if (d->player->currentSource().isDisc()) {
		if (action->isChecked())
			d->player->setCurrentSpu(action->data().toString());
		else
			d->player->setCurrentSpu(QString::null);
	} else {
		const int idx = action->data().toInt();
		d->subIdxes.removeAll(idx);
		if (action->isChecked())
			d->subIdxes.append(idx);
		updateSubtitle();
	}
}

void MainWindow::autoLoadSubtitles() {
	clearSubs();
	const Core::MediaSource source = d->player->currentSource();
	d->menu("subtitle")("list").g()->setExclusive(source.isDisc());
	if (source.isLocalFile()) {
		const SubtitleAutoLoad autoLoad = d->pref->subtitleAutoLoad();
		if (autoLoad == NoAutoLoad)
			return;
		const SubtitleAutoSelect autoSelect = d->pref->subtitleAutoSelect();
		QStringList files;
		const QStringList nameFilter = QStringList() << "*.smi" << "*.srt";
		const QFileInfo file(source.filePath());
		const QDir dir = file.dir();
		const QFileInfoList all = dir.entryInfoList(nameFilter, QDir::Files, QDir::Name);
		const QString base = file.completeBaseName();
		const QString enc = d->pref->subtitleEncoding();
		QSet<QString> langs;
		for (int i=0; i<all.size(); ++i) {
			bool add = (autoLoad == SamePath);
			if (autoLoad == Matched)
				add = (base == all[i].completeBaseName());
			else
				add = all[i].fileName().contains(base);
			if (!add)
				continue;
			QList<Core::Subtitle> subs;
			if (!Core::Subtitle::parse(all[i].absoluteFilePath(), &subs, enc) || subs.isEmpty())
				continue;
			bool select = false;
			if (autoSelect == SameName)
				select = all[i].completeBaseName() == base;
			else if (autoSelect == AllLoaded)
				select = true;
			else if (autoSelect == EachLanguage) {
				const QString lang = d->subs[i].language().language();
				if ((select = (!langs.contains(lang))))
					langs.insert(lang);
			}
			if (select) {
				for (int j=0; j<subs.size(); ++j) {
					d->subIdxes.append(d->subs.size());
					d->subs.append(subs[j]);
				}
			} else
				d->subs += subs;
		}
		Menu &list = d->menu("subtitle")("list");
		for (int i=0; i<d->subs.size(); ++i) {
			QAction *action = list.addActionToGroupWithoutKey(d->subs[i].name(), true);
			action->setData(i);
			action->setChecked(d->subIdxes.contains(i));
		}
		updateSubtitle();
	}
}

void MainWindow::open() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (action && action->data().type() == QVariant::Url)
		open(action->data().toUrl());
	else {
		const QString filter = Helper::mediaExtensionFilter();
		const QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", filter);
		if (!filePath.isEmpty())
			open(QUrl::fromLocalFile(filePath));
	}
}

void MainWindow::open(const QUrl &url) {
	const Core::MediaSource source(url);
	Core::PlayList list;
	const AutoAddFiles mode = d->pref->autoAddFiles();
	if (source.isLocalFile() && mode != DoNotAddFiles) {
		static const QStringList filter
			= Core::Info::videoExtension().toNameFilter()
			+ Core::Info::audioExtension().toNameFilter();
		const QFileInfo file(source.filePath());
		const QFileInfoList files = file.dir().entryInfoList(filter
				, QDir::Files, QDir::Name);
		const QString fileName = file.fileName();
		bool prefix = false, suffix = false;
		QFileInfoList::const_iterator it = files.begin();
		for(; it != files.end(); ++it) {
			if (mode != AllFiles) {
				static QRegExp rxs("(\\D*)\\d+(.*)");
				static QRegExp rxt("(\\D*)\\d+(.*)");
				if (rxs.indexIn(fileName) == -1)
					continue;
				if (rxt.indexIn(it->fileName()) == -1)
					continue;
				if (!prefix && !suffix) {
					if (rxs.cap(1) == rxt.cap(1))
						prefix = true;
					else if (rxs.cap(2) == rxt.cap(2))
						suffix = true;
					else
						continue;
				} else if (prefix) {
					if (rxs.cap(1) != rxt.cap(1))
						continue;
				} else if (suffix) {
					if (rxs.cap(2) != rxt.cap(2))
						continue;
				}
			}
			list.append(QUrl::fromLocalFile(it->absoluteFilePath()));
		}
		if (list.isEmpty())
			list.append(source);
	} else
		list.append(source);
	d->model->setPlayList(list);
	d->model->play(list.indexOf(source));
	RecentInfo::get()->stackSource(source);
}

void MainWindow::togglePlayPause() {
	if (d->player->isPlaying())
		d->player->pause();
	else if (d->player->isPaused())
		d->player->play();
	else if (d->player->isStopped())
		d->model->play(d->model->currentRow());
}

void MainWindow::seek(int diff) {
	d->player->seek(diff, true, true);
}

void MainWindow::openSubFile() {
	const QString filter = tr("Subtitle Files") +' '+ Core::Info::subtitleExtension().toFilter();
	const QString enc = d->pref->subtitleEncoding();
	EncodingFileDialog dlg(this, tr("Open Subtitle"), QString(), filter, enc);
	dlg.setFileMode(QFileDialog::ExistingFiles);
	if (!dlg.exec())
		return;
	const QStringList files = dlg.selectedFiles();
	int idx = d->subs.size();
	Menu &list = d->menu("subtitle")("list");
	for (int i=0; i<files.size(); ++i) {
		QList<Core::Subtitle> subs;
		if (!Core::Subtitle::parse(files[i], &subs, dlg.encoding()) || subs.isEmpty())
			continue;
		for (int j=0; j<subs.size(); ++j, ++idx) {
			d->subIdxes.append(idx);
			d->subs.append(subs[j]);
			QAction *action = list.addActionToGroupWithoutKey(d->subs[idx].name(), true);
			action->setData(idx);
			action->setChecked(true);
		}
	}
}

void MainWindow::updateOnTop() {
	static bool setting = false;
	if (setting)
		return;
	setting = true;
	Menu &top = d->menu("screen")("on top");
	const bool onTop = !top["disable"]->isChecked() && (top["always"]->isChecked()
			|| (top["playing"]->isChecked() && d->player->isPlaying()));
	Qt::WindowFlags flags = windowFlags();
	const bool wasOnTop =  flags & Qt::WindowStaysOnTopHint;
	if (wasOnTop != onTop) {
		if (onTop)
			flags |= Qt::WindowStaysOnTopHint;
		else
			flags &= ~Qt::WindowStaysOnTopHint;
		QByteArray geo = saveGeometry();
		const bool wasVisible = isVisible();
		setWindowFlags(flags);
		restoreGeometry(geo);
		setVisible(wasVisible);
	}
	setting = false;
}

void MainWindow::updatePref() {
	Translator::load(d->pref->uiLanuage());
	const Core::MediaType media = d->player->currentSource().type();
	if (media != Core::Unknown)
		d->menu("play")("engine").g()->trigger(d->pref->backendName(media));
	d->player->setSubtitleStyle(d->pref->subtitleStyle());
	d->menu.updatePref();
	d->playInfo->setState(d->player->state());
	if (!d->player->isStopped()) {
		const bool paused = d->player->isPaused();
		const int time = d->player->currentTime();
		d->player->stop();
		d->player->play(time);
		if (paused)
			d->player->pause();
	}
}

void MainWindow::setFullScreen(bool full) {
	if (full == isFullScreen())
		return;
	d->control->setHidden(full);
	if (full) {
		d->prevWinSize = size();
		setWindowState(windowState() ^ Qt::WindowFullScreen);
// 		d->cursorTimer.start();
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		resize(d->prevWinSize);
// 		if (cursor().shape() == Qt::BlankCursor)
// 			unsetCursor();
	}
}

#define isInCenter() (d->player->geometry().contains(event->pos()))
#define isButton(b) (event->buttons() & (b))

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	if (!event->mimeData()->hasUrls())
		return;
	QList<QUrl> urls = event->mimeData()->urls();
	if (!urls.isEmpty()) {
		if (urls.size() > 1) {
			Core::PlayList pl;
			for (int i=0; i<urls.size(); ++i)
				pl.append(urls[i]);
			d->model->setPlayList(pl);
			d->model->play(0);
			RecentInfo::get()->stackSource(urls[0]);
		} else
			open(urls[0]);
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (!isInCenter())
		return;
	if (isButton(Qt::LeftButton) && !isFullScreen() && isInCenter())
		d->dragPos = event->globalPos() - frameGeometry().topLeft();
	else
		d->dragPos.setX(-1);
	if (isButton(Qt::MidButton)) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref->middleClickMap(), d->clickAction, 0);
		if (action)
			action->trigger();
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (isFullScreen()) {
		static const int h = d->control->height();
		QRect r = rect();
		r.setTop(r.height() - h);
		d->control->setVisible(r.contains(event->pos()));
	} else if (d->dragPos.x() >= 0 && isButton(Qt::LeftButton) && isInCenter())
		move(event->globalPos() - d->dragPos);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	QMainWindow::mouseDoubleClickEvent(event);
	if (isButton(Qt::LeftButton) && isInCenter()) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref->doubleClickMap(), d->clickAction, 0);
		if (action)
			action->trigger();
	}
}

void MainWindow::wheelEvent(QWheelEvent *event) {
	if (isInCenter() && event->delta()) {
		ActionPair pair(0, 0);
		pair = getTriggerAction(event->modifiers()
				, d->pref->wheelScrollMap(), d->wheelAction, pair);
		if (pair.first) {
			((event->delta() > 0) ? pair.first : pair.second)->trigger();
			event->accept();
			return;
		}
	}
	QMainWindow::wheelEvent(event);
}

// #undef GET_TRIGGER_ACTION

#undef isInCenter
#undef isButton

void MainWindow::setVideoProperty(QAction *action) {
	const QList<QVariant> data = action->data().toList();
	const Core::VideoProperty prop = Core::VideoProperty(data[0].toInt());
	const int value = d->player->videoProperty(prop) + data[1].toInt();
	d->player->setVideoProperty(prop, value);
	static const QString format("%2: %1%");
	QString msg = format.arg(Menu::toString(d->player->videoProperty(prop)));
	switch(prop) {
	case Core::Brightness:
		msg = msg.arg(tr("Brightness"));
		break;
	case Core::Saturation:
		msg = msg.arg(tr("Saturation"));
		break;
	case Core::Hue:
		msg = msg.arg(tr("Hue"));
		break;
	case Core::Contrast:
		msg = msg.arg(tr("Contrast"));
		break;
	}
	d->player->showMessage(msg);
}

void MainWindow::slotStateChanged(Core::State state, Core::State /*old*/) {
	if (state == Core::Playing) {
		d->menu("play")["pause"]->setIcon(QIcon(":/img/media-playback-pause.png"));
		d->menu("play")["pause"]->setText(tr("Pause"));
	} else {
		d->menu("play")["pause"]->setIcon(QIcon(":/img/media-playback-start.png"));
		d->menu("play")["pause"]->setText(tr("Play"));
	}
	if (state == Core::Paused && d->pausedByHiding)
		return;
	updateOnTop();
}

void MainWindow::updatePlayListInfo() {
	d->playInfo->setTrackNumber(d->model->currentRow() + 1, d->model->rowCount());
}

void MainWindow::toggleDockVisibility() {
	const bool visible = d->dock->isVisible();
	if (!d->dock->isFloating() && !isFullScreen()) {
		d->player->keepSize(true);
		QSize size = this->size();
		if (visible)
			size.rwidth() -= d->dock->width();
		else
			size.rwidth() += d->dock->width();
		resize(size);
	}
	d->dock->setVisible(!visible);
	d->player->keepSize(false);
}

void MainWindow::updateRecentSize(int size) {
	Menu &recent = d->menu("open")("recent");
	QAction *sprt = recent["seperator"];
	for(;;) {
		QList<QAction*> acts = recent.g()->actions();
		if (size == acts.size())
			break;
		if (size > acts.size()) {
			QAction *action = new QAction(&recent);
			recent.insertAction(sprt, action);
			recent.g()->addAction(action);
		} else
			delete acts.takeLast();
	}
}

void MainWindow::updateRecentActions(const RecentStack &stack) {
	ActionGroup *group = d->menu("open")("recent").g();
	if (group->actions().size() != stack.size())
		updateRecentSize(stack.size());
	QList<QAction*> acts = group->actions();
	for (int i=0; i<stack.size(); ++i) {
		QAction *act = acts[i];
		act->setData(stack[i].url());
		act->setText(stack[i].displayName());
		act->setVisible(stack[i].isValid());
	}
}

void MainWindow::setRenderer(QAction *action) {
	const QString renderer = action->data().toString();
	if (sender() == d->menu("video")("renderer").g()) {
		if (d->player->videoRenderer() != renderer)
			d->player->setVideoRenderer(renderer);
	} else if (sender() == d->menu("audio")("renderer").g()) {
		if (d->player->audioRenderer() != renderer)
			d->player->setAudioRenderer(renderer);
	}
}

void MainWindow::slotTracksChanged(const QStringList &tracks) {
	Menu &track = d->menu("audio")("track");
	track.g()->clear();
	for (int i=0; i<tracks.size(); ++i)
		track.addActionToGroupWithoutKey(tracks[i], true)->setData(tracks[i]);
}

void MainWindow::slotCurrentTrackChanged(const QString &track) {
	d->menu("audio")("track").g()->trigger(track);
}

void MainWindow::slotSpusChanged(const QStringList &spus) {
	clearSubs();
	Menu &list = d->menu("subtitle")("list");
	for (int i=0; i<spus.size(); ++i)
		list.addActionToGroupWithoutKey(spus[i], true)->setData(spus[i]);
	list.g()->setExclusive(true);
}

void MainWindow::slotCurrentSpuChanged(const QString &spu) {
	d->menu("subtitle")("list").g()->trigger(spu);
}

void MainWindow::setBackend(QAction *action) {
	int time = -1;
	if (!d->player->isStopped()) {
		time = d->player->currentTime();
		d->player->stop();
	}
	d->player->setBackend(action->data().toString());
	if (time != -1)
		d->player->play(time);
}

void MainWindow::slotBackendChanged() {
	Menu &vMenu = d->menu("video")("renderer");
	Menu &aMenu = d->menu("audio")("renderer");
	vMenu.g()->clear();
	aMenu.g()->clear();
// 	if (!d->player->engine())
// 		return;
	const Core::Info &info = d->player->engine()->info();
	d->menu("play")("engine").g()->trigger(info.name());
	const QStringList audios = info.audioRenderer();
	const QStringList videos = info.videoRenderer();
	for (int i=0; i<videos.size(); ++i)
		vMenu.addActionToGroupWithoutKey(videos[i], true)->setData(videos[i]);
	for (int i=0; i<audios.size(); ++i)
		aMenu.addActionToGroupWithoutKey(audios[i], true)->setData(audios[i]);
	State state;
	const QString video = state[State::VideoRenderer].toMap()[info.name()].toString();
	if (!video.isEmpty())
		vMenu.g()->trigger(video);
	const QString audio = state[State::AudioRenderer].toMap()[info.name()].toString();
	if (!audio.isEmpty())
		aMenu.g()->trigger(audio);
}

void MainWindow::showPrefDialog() {
	PrefDialog dlg(this);
	if (dlg.exec())
		updatePref();
}

void MainWindow::showEvent(QShowEvent *event) {
	QMainWindow::showEvent(event);
	if (d->pausedByHiding && d->player && d->player->isPaused())
		d->player->play();
	d->pausedByHiding = false;
}

void MainWindow::hideEvent(QHideEvent *event) {
	QMainWindow::hideEvent(event);
	if (d->pref->pauseWhenMinimized() && d->player && d->player->isPlaying()) {
		d->pausedByHiding = true;
		d->player->pause();
	}
}

void MainWindow::slotAbRange() {


}

void MainWindow::slotAbQuit() {

}

void MainWindow::slotAbAdvanced() {

}

#define DEC_DIFF_SETTER_MSG(setter, getter, msg, factor, sign)\
void MainWindow::setter(int diff) {d->player->setter(d->player->getter() + diff);\
d->player->showMessage(msg.arg(Menu::toString(d->player->getter()*factor, sign)));}

DEC_DIFF_SETTER_MSG(setSyncDelay, syncDelay, MainWindow::tr("Subtitle Sync: %1sec."), 0.001, true)
DEC_DIFF_SETTER_MSG(setVolume, volume, MainWindow::tr("Volume: %1%"), 1, false)
DEC_DIFF_SETTER_MSG(setSubtitlePos, subtitlePos, MainWindow::tr("Subtitle Position: %1%"), 1, false)
DEC_DIFF_SETTER_MSG(setSpeed, speed, MainWindow::trUtf8("Speed: \303\227%1"), 0.01, false)
DEC_DIFF_SETTER_MSG(setAmplifyingRate, amplifyingRate, MainWindow::tr("Amp.: %2% (Max.: %1%)")
		.arg(qRound(d->player->engine()->info().maximumAmplifyingRate()*100.0)), 1, false)

#undef DEC_DIFF_SETTER_MSG

