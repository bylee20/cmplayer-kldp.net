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
#include "geturldialog.h"
#include "aboutdialog.h"
#include "snapshotdialog.h"
#include <core/info.h>
#include <core/backendiface.h>
#include <core/playlist.h>
#include <core/subtitle.h>
#include <core/abrepeater.h>
#include <core/utility.h>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMenuBar>
#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtCore/QSet>
#include <QtCore/QDebug>
#include <cmath>
#include <QTime>

struct MainWindow::Data {
	Data(Menu &menu): dragPos(-1, -1), menu(menu) {}
	QWidget *control, *center;
	PlayInfoWidget *playInfo;
	PlaylistModel *model;
	DockWidget *dock;
	Core::Subtitle sub;
	QList<int> subIdxes;
	QSize prevWinSize;
	QPoint dragPos;
	RecentInfo *recent;
	bool pausedByHiding, changingOnTop, changingSubtitle;
	VideoPlayer *player;
	Pref *pref;
	Menu &menu;
	ABRepeatDialog *repeater;
	SnapshotDialog *snapshot;
	QTimer hider;
	QSystemTrayIcon *tray;
};

MainWindow::MainWindow(const QUrl &url) {
	commonInitialize();
	open(url);
}

MainWindow::MainWindow() {
	commonInitialize();
	d->model->setPlaylist(d->recent->lastPlaylist());
	d->model->setCurrentSource(d->recent->lastSource());
}

MainWindow::~MainWindow() {
	d->player->stop();
	RecentInfo *recent = RecentInfo::get();
	saveState();
	recent->setLastSource(d->player->currentSource());
	recent->setLastPlaylist(d->model->playlist());
	RecentInfo::get()->save();
	delete d->model;
	delete d->player;
	delete d;
}

void MainWindow::commonInitialize() {
	d = new Data(Menu::create(this));
	
	d->changingSubtitle = d->changingOnTop = d->pausedByHiding = false;
	d->player = new VideoPlayer(this);
	d->repeater = new ABRepeatDialog(this);
	d->snapshot = new SnapshotDialog(this);
	d->snapshot->setVideoPlayer(d->player);
	
	d->model = new PlaylistModel(d->player);
	d->recent = RecentInfo::get();
	d->pref = Pref::get();
	
	QApplication::setWindowIcon(defaultIcon());
	d->tray = new QSystemTrayIcon(defaultIcon(), this);
	d->tray->show();
	
	setupUi();
	updateRecentActions(d->recent->sources());
	
	Menu &menu = d->menu;
	Menu &open = menu("open");
	connect(open["file"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open["url"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open["dvd"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open("recent").g(), SIGNAL(triggered(const QUrl &)), this, SLOT(open(const QUrl &)));
	connect(open("recent")["clear"], SIGNAL(triggered()), d->recent, SLOT(clearStack()));
	Menu &screen = menu("screen");
	connect(screen("size").g(), SIGNAL(triggered(double)), this, SLOT(setVideoSize(double)));
	connect(screen("aspect").g(), SIGNAL(triggered(double)), d->player, SLOT(setAspectRatio(double)));
	connect(screen("crop").g(), SIGNAL(triggered(double)), d->player, SLOT(setCropRatio(double)));
	connect(screen("on top").g(), SIGNAL(triggered(QAction*)), this, SLOT(updateOnTop()));
	connect(screen["snapshot"], SIGNAL(triggered()), this, SLOT(takeSnapshot()));
	Menu &play = menu("play");
	connect(play["dvd menu"], SIGNAL(triggered()), d->player, SLOT(toggleDvdMenu()));
	connect(play["stop"], SIGNAL(triggered()), d->player, SLOT(stop()));
	connect(play("speed").g(), SIGNAL(triggered(int)), this, SLOT(setSpeed(int)));
	connect(play["prev"], SIGNAL(triggered()), d->model, SLOT(playPrevious()));
	connect(play["next"], SIGNAL(triggered()), d->model, SLOT(playNext()));
	connect(play["pause"], SIGNAL(triggered()), this, SLOT(togglePlayPause()));
	connect(play["list"], SIGNAL(triggered()), this, SLOT(toggleDockVisibility()));
	connect(play("engine").g(), SIGNAL(triggered(QAction*)), this, SLOT(setBackend(QAction*)));
	connect(play("repeat").g(), SIGNAL(triggered(int)), this, SLOT(slotRepeat(int)));
	connect(play("seek").g(), SIGNAL(triggered(int)), this, SLOT(seek(int)));
	Menu &video = menu("video");
	connect(video.g("color"), SIGNAL(triggered(QAction*))
			, this, SLOT(setColorProperty(QAction*)));
	connect(video("renderer").g(), SIGNAL(triggered(QAction*))
			, this, SLOT(setRenderer(QAction*)));
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
	connect(menu["about"], SIGNAL(triggered()), this, SLOT(showAbout()));
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
	connect(d->player, SIGNAL(customContextMenuRequested(const QPoint&))
			, this, SLOT(showContextMenu(const QPoint&)));
	connect(d->dock, SIGNAL(hidingRequested()), this, SLOT(toggleDockVisibility()));
	connect(d->model, SIGNAL(currentRowChanged(int)), this, SLOT(updatePlaylistInfo()));
	connect(d->model, SIGNAL(rowCountChanged(int)), this, SLOT(updatePlaylistInfo()));
	connect(d->recent, SIGNAL(sourcesChanged(const RecentStack&))
			, this, SLOT(updateRecentActions(const RecentStack&)));
	connect(d->recent, SIGNAL(rememberCountChanged(int)), this, SLOT(updateRecentSize(int)));
	connect(&d->hider, SIGNAL(timeout()), this, SLOT(hideCursor()));
	d->hider.setSingleShot(true);
	connect(d->tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason))
			, this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));
	updatePref();
	loadState();
	
	const BackendMap &backend = VideoPlayer::backend();
	Menu &engine = play("engine");
	for (BackendMap::const_iterator it = backend.begin(); it != backend.end(); ++it)
		engine.addActionToGroupWithoutKey(it.key(), true)->setData(it.key());
	
	const QUrl url = getUrlFromCommandLine();
	if (url.isEmpty()) {
		d->model->setPlaylist(d->recent->lastPlaylist());
		d->model->setCurrentSource(d->recent->lastSource());
	} else
		this->open(url);
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

void MainWindow::updateWindowTitle() {
	setWindowTitle(QString("CMPlayer - %1").arg(Core::Info::coreVersion()));
}

void MainWindow::saveState() {
	State state;
	const VideoPlayer::Map engines = d->player->engines();
	State::Map vmap = state[State::VideoRenderer].toMap();
	State::Map amap = state[State::AudioRenderer].toMap();
	VideoPlayer::Map::const_iterator it = engines.begin();
	for (; it != engines.end(); ++it) {
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
	state[State::DockWidth] = d->dock->width();
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
	d->dock->setWidth(state[State::DockWidth].toInt());
}

QIcon MainWindow::defaultIcon() {
	return QIcon(":/img/cmplayer-icon.png");
}

void MainWindow::setupUi() {
	d->dock = new DockWidget(d->model, this);
	addDockWidget(Qt::RightDockWidgetArea, d->dock);
	d->dock->hide();
	d->player->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	d->center = new QWidget(this);
	d->center->setMouseTracking(true);
	d->control = createControl(d->center);
	QVBoxLayout *vbox = new QVBoxLayout(d->center);
	vbox->addWidget(d->player);
	vbox->addWidget(d->control);
	vbox->setMargin(0);
	vbox->setSpacing(0);
	
	updateWindowTitle();
	menuBar()->hide();
	setMouseTracking(true);
	setAcceptDrops(true);
	setCentralWidget(d->center);
	setWindowIcon(defaultIcon());
}

void MainWindow::showContextMenu(const QPoint &pos) {
	d->menu.contextMenu()->exec(mapToGlobal(pos));
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
	d->sub.clear();
	d->subIdxes.clear();
	QList<QAction*> acts = d->menu("subtitle")("list").g()->actions();
	for (int i=0; i<acts.size(); ++i)
		delete acts[i];
	d->player->setSubtitle(d->sub);
}

void MainWindow::updateSubtitle() {
	QList<int> merge = d->subIdxes;
	const QStringList priority = d->pref->subtitlePriority();
	QList<int> order;
	QList<int> indexes = d->subIdxes;
	QList<int>::iterator it;
	for (int i=0; i<priority.size(); ++i) {
		it = indexes.begin();
		while(it != indexes.end()) {
			if (d->sub[*it].language().id() == priority[i]) {
				order.append(*it);
				it = indexes.erase(it);
			} else
				++it;
		}
	}
	order += indexes;
	Core::Subtitle sub;
	for (int i=0; i<order.size(); ++i)
		sub.append(d->sub[order[i]]);
	d->player->setSubtitle(sub);
}

void MainWindow::slotSubtitle(QAction *action) {
	if (d->changingSubtitle)
		return;
	if (d->player->currentSource().isDisc())
		d->player->setCurrentSpu(action->isChecked()
				? action->data().toString() : QString());
	else {
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
			Core::Subtitle sub;
			if (!sub.load(all[i].absoluteFilePath(), enc))
				continue;
			bool select = false;
			if (autoSelect == SameName)
				select = all[i].completeBaseName() == base;
			else if (autoSelect == AllLoaded)
				select = true;
			else if (autoSelect == EachLanguage) {
				const QString lang = d->sub[i].language().id();
				if ((select = (!langs.contains(lang))))
					langs.insert(lang);
			}
			if (select) {
				for (int j=0; j<sub.size(); ++j) {
					d->subIdxes.append(d->sub.size());
					d->sub.append(sub[j]);
				}
			} else
				d->sub += sub;
		}
		Menu &list = d->menu("subtitle")("list");
		d->changingSubtitle = true;
		for (int i=0; i<d->sub.size(); ++i) {
			QAction *action = list.addActionToGroupWithoutKey(d->sub[i].name(), true);
			action->setData(i);
			action->setChecked(d->subIdxes.contains(i));
		}
		d->changingSubtitle = false;
		updateSubtitle();
	}
}

QUrl MainWindow::getUrlFromCommandLine() {
	const QStringList args = QApplication::arguments();
	if (args.size() > 1) {
		QUrl url(args.last());
		if (url.scheme().isEmpty())
			url = QUrl::fromLocalFile(QFileInfo(args.last()).absoluteFilePath());
		return url;
	} else
		return QUrl();
}

void MainWindow::open() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;
	if (action->data().type() == QVariant::Url)
		open(action->data().toUrl());
	else {
		const int key = action->data().toInt();
		if (key == 'f') {
			State s;
			const QString filter = Helper::mediaExtensionFilter();
			const QString filePath = QFileDialog::getOpenFileName(this
					, tr("Open File"), s[State::LastOpenFile].toString(), filter);
			if (!filePath.isEmpty()) {
				s[State::LastOpenFile] = QFileInfo(filePath).absolutePath();
				open(QUrl::fromLocalFile(filePath));
			}
		} else if (key == 'u') {
			GetUrlDialog dlg(this);
			if (dlg.exec())
				open(dlg.url(), dlg.encoding());
		}
	}
}

Core::Playlist MainWindow::open(const Core::MediaSource &source) {
	Core::Playlist list;
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
			const QUrl url = QUrl::fromLocalFile(it->absoluteFilePath());
			list.append(Core::MediaSource(url));
		}
		if (list.isEmpty())
			list.append(source);
	} else
		list.append(source);
	RecentInfo::get()->stackSource(source);
	return list;
}

void MainWindow::open(const QUrl &url, const QString &enc) {
	Core::Playlist list;
	const bool isPlaylist = (QFileInfo(url.path()).suffix().toLower() == "pls");
	if (isPlaylist)
		list.load(url, enc);
	else
		list = open(Core::MediaSource(url));
	d->model->setPlaylist(list);
	d->model->play(isPlaylist ? 0 : list.indexOf(Core::MediaSource(url)));
	show();
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
	QString enc = d->pref->subtitleEncoding();
	const QStringList files = EncodingFileDialog::getOpenFileNames(this, tr("Open Subtitle"), QString(), filter, &enc);
	if (files.isEmpty())
		return;
	int idx = d->sub.size();
	Menu &list = d->menu("subtitle")("list");
	for (int i=0; i<files.size(); ++i) {
		Core::Subtitle sub;
		if (sub.load(files[i], enc)) {
			for (int j=0; j<sub.size(); ++j, ++idx) {
				d->subIdxes.append(idx);
				d->sub.append(sub[j]);
				QAction *action = list.addActionToGroupWithoutKey(
						d->sub[idx].name(), true);
				action->setData(idx);
				d->changingSubtitle = true;
				action->setChecked(true);
			}
		}
	}
	if (d->changingSubtitle) {
		d->changingSubtitle = false;
		updateSubtitle();
	}
}

void MainWindow::updateOnTop() {
	if (d->changingOnTop)
		return;
	d->changingOnTop = true;
	Menu &top = d->menu("screen")("on top");
	const bool onTop = !top["disable"]->isChecked() && (top["always"]->isChecked()
			|| (top["playing"]->isChecked() && d->player->isPlaying()));
	Qt::WindowFlags flags = windowFlags();
	const bool wasOnTop =  isOnTop();
	if (wasOnTop != onTop) {
		if (onTop)
			flags |= Qt::WindowStaysOnTopHint;
		else
			flags &= ~Qt::WindowStaysOnTopHint;
		const bool wasVisible = isVisible();
		const QPoint p = pos();
		hide();
		setWindowFlags(flags);
		setVisible(wasVisible);
		if (pos() != p)
			move(p);
	}
	d->changingOnTop = false;
}

void MainWindow::updatePref() {
	Translator::load(d->pref->uiLanuage());
	const Core::MediaType media = d->player->currentSource().type();
	if (media != Core::Unknown)
		d->menu("play")("engine").g()->trigger(d->pref->backendName(media));
	d->player->setSubtitleStyle(d->pref->subtitleStyle());
	d->player->setVolumeNormalized(d->pref->isVolumeNormalized());
	d->player->setUseSoftwareEqualizer(d->pref->useSoftwareEqualizer());
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
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		resize(d->prevWinSize);
	}
}

#define IS_IN_CENTER (d->player->geometry().contains(event->pos()))
#define IS_BUTTON(b) (event->buttons() & (b))

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	if (!event->mimeData()->hasUrls())
		return;
	QList<QUrl> urls = event->mimeData()->urls();
	if (!urls.isEmpty()) {
// 		if (urls.size() > 1) {
// 			Core::Playlist pl;
// 			for (int i=0; i<urls.size(); ++i)
// 				pl.append(urls[i]);
// 			d->model->setPlaylist(pl);
// 			d->model->play(0);
// 			RecentInfo::get()->stackSource(urls[0]);
// 		} else
			open(urls[0]);
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (!IS_IN_CENTER)
		return;
	if (IS_BUTTON(Qt::LeftButton) && !isFullScreen() && IS_IN_CENTER)
		d->dragPos = event->globalPos() - frameGeometry().topLeft();
	else
		d->dragPos.setX(-1);
	if (IS_BUTTON(Qt::MidButton)) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref->middleClickMap(), d->menu.clickAction(), 0);
		if (action)
			action->trigger();
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	d->hider.stop();
	if (cursor().shape() == Qt::BlankCursor)
		unsetCursor();
	QMainWindow::mouseMoveEvent(event);
	const bool full = isFullScreen();
	if (full) {
		static const int h = d->control->height();
		QRect r = rect();
		r.setTop(r.height() - h);
		d->control->setVisible(r.contains(event->pos()));
	} else if (d->dragPos.x() >= 0 && IS_BUTTON(Qt::LeftButton) && IS_IN_CENTER)
		move(event->globalPos() - d->dragPos);
	if (IS_IN_CENTER && d->pref->hideCursor() && (!d->pref->hideInFullScreen() || full))
		d->hider.start(d->pref->hideDelay());
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	QMainWindow::mouseDoubleClickEvent(event);
	if (IS_BUTTON(Qt::LeftButton) && IS_IN_CENTER) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref->doubleClickMap(), d->menu.clickAction(), 0);
		if (action)
			action->trigger();
	}
}

void MainWindow::wheelEvent(QWheelEvent *event) {
	if (IS_IN_CENTER && event->delta()) {
		Menu::WheelActionPair pair;
		pair = getTriggerAction(event->modifiers()
				, d->pref->wheelScrollMap(), d->menu.wheelAction(), pair);
		if (!pair.isNull()) {
			((event->delta() > 0) ? pair.up : pair.down)->trigger();
			event->accept();
			return;
		}
	}
	QMainWindow::wheelEvent(event);
}

// #undef GET_TRIGGER_ACTION

#undef isInCenter
#undef isButton

void MainWindow::setColorProperty(QAction *action) {
	const QList<QVariant> data = action->data().toList();
	const Core::ColorProperty::Value prop = Core::ColorProperty::Value(data[0].toInt());
	const int value = d->player->colorProperty(prop) + data[1].toInt();
	d->player->setColorProperty(prop, value);
	static const QString format("%2: %1%");
	QString msg = format.arg(Menu::toString(d->player->colorProperty(prop)));
	switch(prop) {
	case Core::ColorProperty::Brightness:
		msg = msg.arg(tr("Brightness"));
		break;
	case Core::ColorProperty::Saturation:
		msg = msg.arg(tr("Saturation"));
		break;
	case Core::ColorProperty::Hue:
		msg = msg.arg(tr("Hue"));
		break;
	case Core::ColorProperty::Contrast:
		msg = msg.arg(tr("Contrast"));
		break;
	}
	showMessage(msg);
}

void MainWindow::setSpeed(int diff) {
	if (diff)
		d->player->setSpeed(d->player->speed() + diff);
	else
		d->player->resetSpeed();
	const QString speed = Menu::toString(d->player->speed()*0.01, false);
	showMessage(trUtf8("Speed: \303\227%1").arg(speed));
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

void MainWindow::updatePlaylistInfo() {
	d->playInfo->setTrackNumber(d->model->currentRow() + 1, d->model->rowCount());
}

void MainWindow::toggleDockVisibility() {
	static int frameWidth = -1;
	if (frameWidth < 0)
		frameWidth = (width() - (d->player->width() + d->dock->width()));
	const bool visible = d->dock->isVisible();
	if (!d->dock->isFloating() && !isFullScreen() && !isMaximized()) {
		int w = d->dock->frameGeometry().width();
		if (frameWidth >= 0)
			w += frameWidth;
		d->player->keepSize(true);
		if (visible) {
			d->dock->hide();
			resize(width() - w, height());
		} else {
			resize(width() + w, height());
			d->dock->show();
		}
		d->player->keepSize(false);
	} else
		d->dock->setVisible(!visible);
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
	d->repeater->hide();
	Menu &vMenu = d->menu("video")("renderer");
	Menu &aMenu = d->menu("audio")("renderer");
	vMenu.g()->clear();
	aMenu.g()->clear();
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
	if (video.isEmpty())
		vMenu.g()->trigger(d->player->videoRenderer());
	else
		vMenu.g()->trigger(video);
	const QString audio = state[State::AudioRenderer].toMap()[info.name()].toString();
	if (audio.isEmpty())
		aMenu.g()->trigger(d->player->audioRenderer());
	else
		aMenu.g()->trigger(audio);
}

void MainWindow::showPrefDialog() {
	PrefDialog dlg(this);
	if (dlg.exec())
		updatePref();
}

void MainWindow::showEvent(QShowEvent *event) {
	QMainWindow::showEvent(event);
	if (!d->changingOnTop) {
		if (d->pausedByHiding && d->player && d->player->isPaused())
			d->player->play();
		d->pausedByHiding = false;
	}
}

void MainWindow::hideEvent(QHideEvent *event) {
	QMainWindow::hideEvent(event);
	if (!d->changingOnTop && d->pref->pauseWhenMinimized()) {
		if (d->pref->pauseVideoOnly() && !d->player->hasVideo())
			return;
		if (d->player && d->player->isPlaying()) {
			d->pausedByHiding = true;
			d->player->pause();
		}
	}
}

void MainWindow::slotRepeat(int key) {
	Core::ABRepeater *repeater = d->player->repeater();
	if (!repeater || d->repeater->isVisible())
		return;
	const QString msg = tr("A-B Repeat: %1");
	if (key == 'a') {
		if (d->repeater->isHidden()) {
			d->repeater->setRepeater(repeater);
			d->repeater->show();
		}
	} else if (key == 'r') {
		if (d->player->isStopped())
			return;
		if (!repeater->hasA()) {
			const int at = repeater->setAToCurrentTime();
			QString a = Core::Utility::msecsToString(at, "h:mm:ss.zzz");
			a.chop(2);
			showMessage(msg.arg(tr("Set A to %1").arg(a)));
		} else if (!repeater->hasB()) {
			const int at = repeater->setBToCurrentTime();
			if ((at - repeater->a()) < 100) {
				d->player->showMessage(msg.arg(tr("Range is too short!")));
				repeater->setB(-1);
			} else {
				QString b = Core::Utility::msecsToString(at, "h:mm:ss.zzz");
				b.chop(2);
				showMessage(msg.arg(tr("Set B to %1. "
						"Start to repeat!").arg(b)));
				repeater->start();
			}
		}
	} else if (key == 'q') {
		repeater->stop();
		repeater->setA(-1);
		repeater->setB(-1);
		showMessage(msg.arg(tr("Quit repeating")));
	} else if (key == 's') {
		repeater->setAToSubtitleTime();
		repeater->setBToSubtitleTime();
		showMessage(msg.arg(tr("Repeat current subtitle")));
		repeater->start();
	}
}

void MainWindow::setSyncDelay(int diff) {
	if (diff)
		d->player->setSyncDelay(d->player->syncDelay() + diff);
	else
		d->player->setSyncDelay(0);
	const QString sync = Menu::toString(d->player->syncDelay()*0.001, true);
	showMessage(tr("Subtitle Sync: %1sec.").arg(sync));
}

#define DEC_DIFF_SETTER_MSG(setter, getter, msg, factor, sign)\
void MainWindow::setter(int diff) {d->player->setter(d->player->getter() + diff);\
showMessage(msg.arg(Menu::toString(d->player->getter()*factor, sign)));}

DEC_DIFF_SETTER_MSG(setVolume, volume, MainWindow::tr("Volume: %1%"), 1, false)
DEC_DIFF_SETTER_MSG(setSubtitlePos, subtitlePos, MainWindow::tr("Subtitle Position: %1%"), 1, false)
DEC_DIFF_SETTER_MSG(setAmplifyingRate, amplifyingRate, MainWindow::tr("Amp.: %2% (Max.: %1%)")
		.arg(qRound(d->player->engine()->info().maximumAmplifyingRate()*100.0)), 1, false)

#undef DEC_DIFF_SETTER_MSG

void MainWindow::showAbout() {
	AboutDialog dlg;
	dlg.exec();
}

void MainWindow::hideCursor() {
	if (cursor().shape() != Qt::BlankCursor)
		setCursor(Qt::BlankCursor);
}

void MainWindow::closeEvent(QCloseEvent *event) {
	hide();
	event->ignore();
}

void MainWindow::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger)
		setVisible(!isVisible());
	else if (reason == QSystemTrayIcon::Context)
		d->menu.contextMenu()->exec(QCursor::pos());
}

void MainWindow::showMessage(const QString &text) {
	d->player->showMessage(text);
	d->playInfo->showMessage(text);
}

void MainWindow::takeSnapshot() {
	d->snapshot->take();
	d->snapshot->show();
	
}
