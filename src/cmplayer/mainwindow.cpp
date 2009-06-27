#include "pref_dialog.h"
#include "subviewdialog.h"
#include "controlwidget.h"
#include "checkdialog.h"
#include "state.h"
#include "playlistmodel.h"
#include "mainwindow.h"
#include "recentinfo.h"
#include "playinfowidget.h"
#include "encodingfiledialog.h"
#include "pref.h"
#include "helper.h"
#include "videoplayer.h"
#include "menu.h"
#include "sliders.h"
#include "abrepeatdialog.h"
#include "translator.h"
#include "geturldialog.h"
#include "aboutdialog.h"
#include "snapshotdialog.h"
#include "application.h"
#include <core/info.h>
#include <core/charsetdetector.h>
#include <core/backendiface.h>
#include <core/playlist.h>
#include <core/subtitle.h>
#include <core/abrepeater.h>
#include <core/utility.h>
#include <core/mediainfo.h>
#include <QtCore/QMultiMap>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtCore/QTimer>
#include <QtCore/QSet>
#include <QtCore/QDebug>
#include <QtCore/QLinkedList>
#include <cmath>
#include <QTime>
#include "toolbox.h"
#include "dragcharm.h"
#include "backendmanager.h"

struct MainWindow::Data {
	Data(Menu &menu): pref(Pref::get()), menu(menu) {}
	QWidget *center;
	ControlWidget *control;
	PlaylistModel *model;
	ToolBox *toolBox;
	Core::Subtitle sub;
	QList<int> subIdxes;
	QSize prevWinSize;
	RecentInfo *recent;
	bool pausedByHiding, changingOnTop, changingSubtitle, showToolBox;
	VideoPlayer *player;
	const Pref &pref;
	Menu &menu;
	ABRepeatDialog *repeater;
	SnapshotDialog *snapshot;
	QTimer hider;
	QSystemTrayIcon *tray;
	DragCharm dragCharm;
	QRect toolRect;
	SubViewDialog *subViewer;
};

MainWindow::MainWindow(const Core::Mrl &mrl)
: QMainWindow(0/*, Qt::FramelessWindowHint*/) {
	commonInitialize();
	openMrl(mrl);
}

MainWindow::MainWindow()
: QMainWindow(0/*, Qt::FramelessWindowHint*/) {
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

	d->showToolBox = d->changingSubtitle = d->changingOnTop = d->pausedByHiding = false;
	d->player = new VideoPlayer(this);
	d->repeater = new ABRepeatDialog(this);
	d->snapshot = new SnapshotDialog(this);
	d->subViewer = new SubViewDialog(d->player, this);

	d->snapshot->setVideoPlayer(d->player);
	
	d->model = new PlaylistModel(d->player);
	d->recent = RecentInfo::get();
	
	QApplication::setWindowIcon(defaultIcon());
	d->tray = new QSystemTrayIcon(defaultIcon(), this);
	
	setupUi();
	updateRecentActions(d->recent->sources());
	
	Menu &menu = d->menu;
	Menu &open = menu("open");
	connect(open["file"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open["url"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open["dvd"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open("recent").g(), SIGNAL(triggered(Core::Mrl)), this, SLOT(openMrl(Core::Mrl)));
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
	connect(play["list"], SIGNAL(triggered()), this, SLOT(toggleToolBoxVisibility()));
	connect(play("engine").g(), SIGNAL(triggered(const QString &))
			, d->player, SLOT(setBackend(const QString &)));
	connect(play("repeat").g(), SIGNAL(triggered(int)), this, SLOT(slotRepeat(int)));
	connect(play("seek").g(), SIGNAL(triggered(int)), this, SLOT(seek(int)));
	Menu &video = menu("video");
	connect(video.g("color"), SIGNAL(triggered(QAction*))
			, this, SLOT(setColorProperty(QAction*)));
	connect(video("renderer").g(), SIGNAL(triggered(QAction*))
			, this, SLOT(setRenderer(QAction*)));
	Menu &audio = menu("audio");
	connect(audio.g("volume"), SIGNAL(triggered(int)), this, SLOT(setVolume(int)));
	connect(audio["mute"], SIGNAL(toggled(bool)), this, SLOT(setMuted(bool)));
	connect(audio.g("amp"), SIGNAL(triggered(int))
			, this, SLOT(setAmplifyingRate(int)));
	connect(audio("renderer").g(), SIGNAL(triggered(QAction*))
			, this, SLOT(setRenderer(QAction*)));
	connect(audio("track").g(), SIGNAL(triggered(const QString &))
			, d->player, SLOT(setCurrentTrack(const QString &)));
	Menu &sub = menu("subtitle");
	connect(sub("list")["hide"], SIGNAL(toggled(bool))
			, d->player, SLOT(setSubtitleHidden(bool)));
	connect(sub("list")["open"], SIGNAL(triggered()), this, SLOT(openSubFile()));
	connect(sub("list")["clear"], SIGNAL(triggered()), this, SLOT(clearSubs()));
	connect(sub("list").g(), SIGNAL(triggered(QAction*))
			, this, SLOT(slotSubtitle(QAction*)));
	connect(sub["viewer"], SIGNAL(triggered()), this, SLOT(openSubViewer()));
	connect(sub.g("pos"), SIGNAL(triggered(int)), this, SLOT(setSubtitlePos(int)));
	connect(sub.g("sync"), SIGNAL(triggered(int)), this, SLOT(setSyncDelay(int)));
	connect(menu["pref"], SIGNAL(triggered()), this, SLOT(showPrefDialog()));
	connect(menu["about"], SIGNAL(triggered()), this, SLOT(showAbout()));
// 	connect(menu["help"], SIGNAL(triggered()), this, SLOT(slotHelp()));
	connect(menu["exit"], SIGNAL(triggered()), qApp, SLOT(quit()));
	
	connect(d->player, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, this, SLOT(updateWindowTitle()));
	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(d->player, SIGNAL(mutedChanged(bool))
			, audio["mute"], SLOT(setChecked(bool)));
	connect(d->player, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, this, SLOT(autoLoadSubtitles()));
	connect(d->player, SIGNAL(backendChanged(const QString&))
			, this, SLOT(slotBackendChanged()));

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
	connect(d->toolBox, SIGNAL(hidingRequested())
			, this, SLOT(toggleToolBoxVisibility()));
	connect(d->model, SIGNAL(currentRowChanged(int))
			, this, SLOT(updatePlaylistInfo()));
	connect(d->model, SIGNAL(rowCountChanged(int))
			, this, SLOT(updatePlaylistInfo()));
	connect(d->recent, SIGNAL(sourcesChanged(const RecentStack&))
			, this, SLOT(updateRecentActions(const RecentStack&)));
	connect(d->recent, SIGNAL(rememberCountChanged(int))
			, this, SLOT(updateRecentSize(int)));
	connect(&d->hider, SIGNAL(timeout()), this, SLOT(hideCursor()));
	d->hider.setSingleShot(true);
	connect(d->tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason))
			, this, SLOT(slotTrayActivated(QSystemTrayIcon::ActivationReason)));
// 	titleBar()->connect(this);
	
	const BackendMap &backend = BackendManager::load();
	Menu &engine = play("engine");
	for (BackendMap::const_iterator it = backend.begin(); it != backend.end(); ++it)
		engine.addActionToGroupWithoutKey(it.key(), true)->setData(it.key());
	loadState();
	updatePref();
	
	d->dragCharm.activate(this);
// 	d->dragCharm.setBorder(7);
}

ControlWidget *MainWindow::createControl(QWidget *parent) {
	ControlWidget *control = new ControlWidget(d->player, parent);
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
	return control;
}

void MainWindow::updateWindowTitle() {
	const Core::MediaSource source = d->player->currentSource();
	QString title = source.mrl().location();
	if (title.isEmpty())
		title = "CMPlayer";
	else
		title += " - CMPlayer";
	setWindowTitle(title);
// 	titleBar()->setTitle(title);
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
	state[State::BackendName] = d->menu("play")("engine").g()->data();
	state[State::ScreenAspectRatio] = d->player->aspectRatio();
	state[State::ScreenCrop] = d->player->cropRatio();
	state[State::ScreenOnTop] = d->menu("screen")("on top").g()->data();
	state[State::AudioVolume] = d->player->volume();
	state[State::AudioMuted] = d->player->isMuted();
	state[State::AudioAmp] = d->player->amplifyingRate();
	state[State::PlaySpeed] = d->player->speed();
	state[State::SubtitlePos] = d->player->subtitlePos();
	state[State::SubtitleSync] = d->player->syncDelay();
	if (d->toolBox->isVisible())
		d->toolRect = d->toolBox->geometry();
	state[State::ToolBoxRect] = d->toolRect;
	if (d->control->currentLayout() == ControlWidget::TwoLine)
		state[State::PanelLayout] = QString("TwoLine");
	else
		state[State::PanelLayout] = QString("OneLine");
	state.save();
}

void MainWindow::loadState() {
	State state;
	state.load();
	const QString name = state[State::BackendName].toString();
	if (BackendManager::map().contains(name))
		d->menu("play")("engine").g()->trigger(name);
	else {
		QList<QAction*> actions = d->menu("play")("engine").g()->actions();
		if (!actions.isEmpty())
			actions[0]->trigger();
	}
	d->menu("screen")("aspect").g()->trigger(state[State::ScreenAspectRatio]);
	d->menu("screen")("crop").g()->trigger(state[State::ScreenCrop]);
	d->menu("screen")("on top").g()->trigger(state[State::ScreenOnTop]);
	d->player->setVolume(state[State::AudioVolume].toInt());
	d->player->setMuted(state[State::AudioMuted].toBool());
	d->player->setAmplifyingRate(state[State::AudioAmp].toInt());
	d->player->setSpeed(state[State::PlaySpeed].toInt());
	d->player->setSubtitlePos(state[State::SubtitlePos].toInt());
	d->player->setSyncDelay(state[State::SubtitleSync].toInt());
	d->toolRect = state[State::ToolBoxRect].toRect();
	if (state[State::PanelLayout].toString() == "TwoLine")
		d->control->changeLayout(ControlWidget::TwoLine);
	else
		d->control->changeLayout(ControlWidget::OneLine);
}

QIcon MainWindow::defaultIcon() {
	return QIcon(":/img/cmplayer-icon.png");
}

void MainWindow::setupUi() {
	d->toolBox = new ToolBox(d->player, d->model, this);
	d->toolBox->addAction(d->menu("play")["list"]);
	d->player->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	d->center = new QWidget(this);
	d->center->setMouseTracking(true);
	d->control = createControl(d->center);
	QVBoxLayout *vbox = new QVBoxLayout(d->center);
// 	vbox->addWidget(titleBar());
	vbox->addWidget(d->player);
	vbox->addWidget(d->control);
	vbox->setContentsMargins(0, 0, 0, 0);
// 	vbox->setContentsMargins(3, 3, 3, 3);
	vbox->setSpacing(0);
	
	updateWindowTitle();
	menuBar()->hide();
	setMouseTracking(true);
	setAcceptDrops(true);
	setCentralWidget(d->center);
	setWindowIcon(defaultIcon());
// 	titleBar()->setIcon(defaultIcon());
// 	titleBar()->addButton(QIcon(":/img/arrow-down-gray.png"), this, SLOT(showMinimized()));
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
	d->subViewer->showCurrentSubtitle();
//	d->subViewer->setSubtitle(d->sub);
}

void MainWindow::updateSubtitle() {
	QList<int> merge = d->subIdxes;
	const QStringList priority = d->pref.subtitlePriority;
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
	if (d->subViewer->isVisible())
		d->subViewer->showCurrentSubtitle();
//		d->subViewer->setSubtitle(d->sub);
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
	if (!source.isLocalFile())
		return;
	d->sub = loadSubtitle(source);
	d->subIdxes = selectSubtitle(source, d->sub);
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

Core::Subtitle MainWindow::loadSubtitle(const Core::MediaSource &source) {
	if (!source.isLocalFile() || d->pref.subtitleAutoLoad == NoAutoLoad)
		return Core::Subtitle();
	const QStringList filter = Core::Info::subtitleExtension().toNameFilter();
	const QFileInfo file(source.fileName());
	const QFileInfoList all = file.dir().entryInfoList(filter, QDir::Files, QDir::Name);
	const QString base = file.completeBaseName();
	Core::Subtitle subtitle;
	for (int i=0; i<all.size(); ++i) {
		if (d->pref.subtitleAutoLoad != SamePath) {
			if (d->pref.subtitleAutoLoad == Matched) {
				if (base != all[i].completeBaseName())
					continue;
			} else if (!all[i].fileName().contains(base))
				continue;
		}
		const QString filePath = all[i].absoluteFilePath();
		QString encoding;
		if (d->pref.useSubtitleEncodingAutoDetection)
			encoding = Core::CharsetDetector::detect(filePath
					, d->pref.subtitleEncodingConfidence*0.01);
		if (encoding.isEmpty())
			encoding = d->pref.subtitleEncoding;
		Core::Subtitle sub;
		if (sub.load(all[i].absoluteFilePath(), encoding))
			subtitle += sub;
	}
	return subtitle;
}

QList<int> MainWindow::selectSubtitle(const Core::MediaSource &source, const Core::Subtitle &sub) {
	QList<int> indexes;
	if (!source.isLocalFile() || sub.isEmpty())
		return indexes;
	QSet<QString> langSet;
	const QString base = QFileInfo(source.fileName()).completeBaseName();
	for (int i=0; i<sub.size(); ++i) {
		const QFileInfo file(sub[i].fileName());
		bool select = false;
		if (d->pref.subtitleAutoSelect == SameName) {
			select = QFileInfo(sub[i].fileName()).completeBaseName() == base;
		} else if (d->pref.subtitleAutoSelect == AllLoaded) {
			select = true;
		} else if (d->pref.subtitleAutoSelect == EachLanguage) {
			const QString lang = d->sub[i].language().id();
			if ((select = (!langSet.contains(lang))))
				langSet.insert(lang);
		}
		if (select)
			indexes.push_back(i);
	}
	if (d->pref.subtitleAutoSelect == SameName
			&& indexes.size() > 1 && !d->pref.subtitleExtension.isEmpty()) {
		int index = -1;
		for (int i=0; i<indexes.size(); ++i) {
			const QString suffix = QFileInfo(sub[indexes[i]].fileName()).suffix().toLower();
			if (d->pref.subtitleExtension == suffix) {
				index = indexes[i];
				break;
			}
		}
		if (index != -1) {
			indexes.clear();
			indexes.push_back(index);
		}
	}
	return indexes;
}

Core::Mrl MainWindow::getMrlFromCommandLine() {
	const QStringList args = QApplication::arguments();
	if (args.size() > 1)
		return Core::Mrl(args.last());
	return Core::Mrl();
}

void MainWindow::open() {
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;
	if (action->data().type() == QVariant::Url)
		openMrl(action->data().toUrl());
	else {
		const int key = action->data().toInt();
		if (key == 'f') {
			State s;
			const QString filter = Helper::mediaExtensionFilter();
			const QString filePath = QFileDialog::getOpenFileName(this
					, tr("Open File"), s[State::LastOpenFile].toString(), filter);
			if (!filePath.isEmpty()) {
				const Core::Mrl mrl = Core::Mrl::fromLocalFile(filePath);
				s[State::LastOpenFile] = mrl.location();
				openMrl(mrl);
			}
		} else if (key == 'u') {
			GetUrlDialog dlg(this);
			if (dlg.exec())
				openMrl(dlg.url(), dlg.encoding());
		}

	}
}

Core::Playlist MainWindow::open(const Core::MediaSource &source) {
	Core::Playlist list;
	const AutoAddFiles mode = d->pref.autoAddFiles;
	if (source.isLocalFile() && mode != DoNotAddFiles) {
		const QFileInfo file(source.fileName());
		QStringList filter
			= Core::Info::videoExtension().toNameFilter()
			+ Core::Info::audioExtension().toNameFilter();
		if (!filter.contains(file.suffix()))
			filter += ("*." + file.suffix());
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
			list.append(Core::Mrl::fromLocalFile(it->absoluteFilePath()));
		}
		if (list.isEmpty())
			list.append(source);
	} else
		list.append(source);
	RecentInfo::get()->stackSource(source);
	return list;
}

void MainWindow::openMrl(const Core::Mrl &mrl, const QString &enc) {
	const bool isPlaylist = mrl.isPlaylist();
	Core::Playlist list;
	if (isPlaylist)
		list.load(mrl, enc);
	else
		list = open(mrl);
	d->model->setPlaylist(list);
	d->model->play(isPlaylist ? 0 : list.indexOf(mrl));
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
	QString enc = d->pref.subtitleEncoding;
	const QStringList file = EncodingFileDialog::getOpenFileNames(this
			, tr("Open Subtitle"), QString(), filter, &enc);
	if (!file.isEmpty())
		appendSubtitle(file, true, enc);
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
	Translator::load(d->pref.locale);
	app()->setStyle(d->pref.windowStyle);
	d->player->setSubtitleStyle(d->pref.subtitleStyle);
	d->player->setVolumeNormalized(d->pref.normalizeVolume);
	d->player->setUseSoftwareEqualizer(d->pref.useSoftwareEqualizer);
	d->menu.updatePref();
	d->control->setState(d->player->state());
	if (!d->player->isStopped()) {
		const bool paused = d->player->isPaused();
		const int time = d->player->currentTime();
		d->player->stop();
		d->player->play(time);
		if (paused)
			d->player->pause();
	}
	d->tray->setVisible(d->pref.enableSystemTray);
}

void MainWindow::setFullScreen(bool full) {
	if (full == isFullScreen())
		return;
	d->control->setHidden(full);
// 	titleBar()->setHidden(full);
// 	const int m = full ? 0 : 3;
// 	d->center->layout()->setContentsMargins(m, m, m, m);
	if (full) {
		d->prevWinSize = size();
		setWindowState(windowState() ^ Qt::WindowFullScreen);
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		resize(d->prevWinSize);
	}
}

#define IS_IN_CENTER (d->player->geometry().contains(d->player->mapFrom(this, event->pos())))
#define IS_BUTTON(b) (event->buttons() & (b))

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::appendSubtitle(const QStringList &file, bool check, const QString &enc) {
	Menu &list = d->menu("subtitle")("list");
	int idx = d->sub.size();
	for (int i=0; i<file.size(); ++i) {
		Core::Subtitle sub;
		if (sub.load(file[i], enc)) {
			for (int j=0; j<sub.size(); ++j, ++idx) {
				d->subIdxes.append(idx);
				d->sub.append(sub[j]);
				QAction *action = list.addActionToGroupWithoutKey(
						d->sub[idx].name(), true);
				action->setData(idx);
				if (check) {
					d->changingSubtitle = true;
					action->setChecked(true);
				}
			}
		}
	}
	if (d->changingSubtitle) {
		d->changingSubtitle = false;
		updateSubtitle();
	}
}

void MainWindow::dropEvent(QDropEvent *event) {
	if (!event->mimeData()->hasUrls())
		return;
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;
	qSort(urls);
	Core::Playlist playlist;
	QStringList subList;
	for (int i=0; i<urls.size(); ++i) {
		const QString suffix = QFileInfo(urls[i].path()).suffix().toLower();
		if (suffix == "pls") {
			Core::Playlist list;
			list.load(urls[i]);
			playlist += list;
		} else if (Core::Info::subtitleExtension().contains(suffix)) {
			subList << urls[i].toLocalFile();
		} else if (Core::Info::videoExtension().contains(suffix)
				|| Core::Info::audioExtension().contains(suffix)) {
			playlist.append(urls[i]);
		}
	}
	if (!playlist.isEmpty()) {
		d->model->append(playlist);
		d->model->play(d->model->row(playlist.first()));
	} else if (!subList.isEmpty())
		appendSubtitle(subList, true, d->pref.subtitleEncoding);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (!IS_IN_CENTER)
		return;
	if (IS_BUTTON(Qt::MidButton)) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref.middleClickMap, d->menu.clickAction(), 0);
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
	}
	if (IS_IN_CENTER && d->pref.hideCursor && (!d->pref.hideInFullScreen || full))
		d->hider.start(d->pref.hideDelay);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	QMainWindow::mouseDoubleClickEvent(event);
	if (IS_BUTTON(Qt::LeftButton) && IS_IN_CENTER) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref.doubleClickMap, d->menu.clickAction(), 0);
		if (action)
			action->trigger();
	}
}

void MainWindow::wheelEvent(QWheelEvent *event) {
	if (IS_IN_CENTER && event->delta()) {
		Menu::WheelActionPair pair;
		pair = getTriggerAction(event->modifiers()
				, d->pref.wheelScrollMap, d->menu.wheelAction(), pair);
		if (!pair.isNull()) {
			((event->delta() > 0) ? pair.up : pair.down)->trigger();
			event->accept();
			return;
		}
	}
	QMainWindow::wheelEvent(event);
}

#undef IS_IN_CENTER
#undef IS_BUTTON

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
		Core::Utility::setScreensaverDisabled(d->pref.disableScreensaver);
		d->menu("play")["pause"]->setIcon(QIcon(":/img/media-playback-pause.png"));
		d->menu("play")["pause"]->setText(tr("Pause"));
	} else {
		Core::Utility::setScreensaverDisabled(false);
		d->menu("play")["pause"]->setIcon(QIcon(":/img/media-playback-start.png"));
		d->menu("play")["pause"]->setText(tr("Play"));
	}
	if (state == Core::Paused && d->pausedByHiding)
		return;
	updateOnTop();
}

void MainWindow::updatePlaylistInfo() {
	d->control->setTrackNumber(d->model->currentRow() + 1, d->model->rowCount());
}

void MainWindow::toggleToolBoxVisibility() {
	QAction *act = qobject_cast<QAction*>(sender());
	const bool visible = d->toolBox->isVisible();
	if (act && act->isChecked() == visible)
		return;
	if (visible)
		d->toolRect = d->toolBox->geometry();//QRect(d->toolBox->pos(), d->toolBox->size());
	else if (!d->toolRect.isNull())
		d->toolBox->setGeometry(d->toolRect);
		
	d->toolBox->setVisible(!visible);
	if (!act)
		d->menu("play")["list"]->setChecked(!visible);
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
		act->setData(stack[i].mrl().url());
		act->setText(stack[i].displayName());
		act->setVisible(stack[i].isValid());
	}
}

void MainWindow::setRenderer(QAction *action) {
	const QString renderer = action->data().toString();
	ActionGroup *group = static_cast<ActionGroup*>(sender());
	bool set = false;
	if (group == d->menu("video")("renderer").g()) {
		if (!(set = d->player->setVideoRenderer(renderer)))
			group->setChecked(d->player->videoRenderer(), true);
	} else if (sender() == d->menu("audio")("renderer").g()) {
		if (!(set = d->player->setAudioRenderer(renderer)))
			group->setChecked(d->player->audioRenderer(), true);
	} else
		return;
	if (!set)
		QMessageBox::warning(this, tr("Change Renderer")
				, tr("Failed in changing renderer. "
				"The renderer fell back into previous one."));
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

void MainWindow::slotBackendChanged() {
	d->repeater->hide();
	Menu &vMenu = d->menu("video")("renderer");
	Menu &aMenu = d->menu("audio")("renderer");
	vMenu.g()->clear();
	aMenu.g()->clear();
	const Core::Info *info = d->player->info();
	if (info) {
		d->menu("play")("engine").g()->trigger(info->name());
		const QStringList audios = info->audioRenderer();
		const QStringList videos = info->videoRenderer();
		for (int i=0; i<videos.size(); ++i)
			vMenu.addActionToGroupWithoutKey(videos[i], true)->setData(videos[i]);
		for (int i=0; i<audios.size(); ++i)
			aMenu.addActionToGroupWithoutKey(audios[i], true)->setData(audios[i]);
		State state;
		State::Map vMap = state[State::VideoRenderer].toMap();
		const QString video = vMap[info->name()].toString();
		if (video.isEmpty()) {
			vMenu.g()->setChecked(d->player->videoRenderer(), true);
			vMap[info->name()] = d->player->videoRenderer();
			state[State::VideoRenderer] = vMap;
		} else
			vMenu.g()->trigger(video);
		State::Map aMap = state[State::AudioRenderer].toMap();
		const QString audio = aMap[info->name()].toString();
		if (audio.isEmpty()) {
			aMenu.g()->setChecked(d->player->audioRenderer(), true);
			aMap[info->name()] = d->player->audioRenderer();
			state[State::AudioRenderer] = aMap;
		} else
			aMenu.g()->trigger(audio);
	}
}

void MainWindow::showPrefDialog() {
	Pref::Dialog dlg(this);
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
	if (d->showToolBox) {
		d->toolBox->show();
		d->showToolBox = false;
	}
}

void MainWindow::hideEvent(QHideEvent *event) {
	QMainWindow::hideEvent(event);
	updatePauseMinimized();
	if (d->toolBox->isVisible()) {
		d->toolBox->hide();
		d->showToolBox = true;
	}
}

void MainWindow::updatePauseMinimized() {
	if (d->changingOnTop || !d->pref.pauseMinimized)
		return;
	if (d->pref.pauseVideoOnly && !d->player->hasVideo())
		return;
	if (!d->player || !d->player->isPlaying())
		return;
	d->pausedByHiding = true;
	d->player->pause();
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
DEC_DIFF_SETTER_MSG(setSubtitlePos, subtitlePos
		, MainWindow::tr("Subtitle Position: %1%"), 1, false)
DEC_DIFF_SETTER_MSG(setAmplifyingRate, amplifyingRate, MainWindow::tr("Amp.: %1%"), 1, false)

#undef DEC_DIFF_SETTER_MSG

void MainWindow::showAbout() {
	AboutDialog dlg;
	dlg.exec();
}

void MainWindow::hideCursor() {
	if (cursor().shape() != Qt::BlankCursor)
		setCursor(Qt::BlankCursor);
}

void MainWindow::changeEvent(QEvent *event) {
	QMainWindow::changeEvent(event);
	if (event->type() == QEvent::WindowStateChange && (windowState() & Qt::WindowMinimized))
		updatePauseMinimized();
}

void MainWindow::paintEvent(QPaintEvent */*event*/) {
// 	QPainter painter(this);
// 	drawBackground(&painter, this);
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (d->pref.enableSystemTray && d->pref.hideClosed) {
		hide();
		State state;
		if (state[State::TrayFirst].toBool()) {
			CheckDialog dlg(this);
			dlg.setChecked(true);
			dlg.setLabelText(tr("CMPlayer will be running in the system tray "
					"when the window closed.<br>"
					"You can change this behavior in the preferences.<br>"
					"If you want to exit CMPlayer, please use 'Exit' menu."));
			dlg.setCheckBoxText(tr("Do not display this message again"));
			dlg.exec();
			state[State::TrayFirst] = !dlg.isChecked();
		}
		event->ignore();
	} else {
		event->accept();
		qApp->quit();
	}
}

void MainWindow::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger)
		setVisible(!isVisible());
	else if (reason == QSystemTrayIcon::Context)
		d->menu.contextMenu()->exec(QCursor::pos());
}

void MainWindow::showMessage(const QString &text) {
	d->player->showMessage(text);
	d->control->showMessage(text);
}

void MainWindow::takeSnapshot() {
	d->snapshot->take();
	d->snapshot->show();
	
}

void MainWindow::setMuted(bool muted) {
	if (muted)
		d->menu("audio")["mute"]->setIcon(QIcon(":/img/irc-unvoice.png"));
	else
		d->menu("audio")["mute"]->setIcon(QIcon(":/img/irc-voice.png"));
	d->player->setMuted(muted);
}

void MainWindow::openSubViewer() {
	if (!d->subViewer->isVisible()) {
		d->subViewer->showCurrentSubtitle();
		d->subViewer->adjustSize();
		d->subViewer->show();
	}
}

// void MainWindow::slotCurrentSourceChanged(const Core::MediaSource &source) {
// 	
// }
