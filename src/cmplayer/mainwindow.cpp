#include "timelineosdrenderer.hpp"
#include "playlistview.hpp"
#include "historyview.hpp"
#include "playlistmodel.hpp"
#include "aboutdialog.hpp"
#include "subtitlerenderer.hpp"
#include "charsetdetector.hpp"
#include "snapshotdialog.hpp"
#include "subtitle_parser.hpp"
#include "audiocontroller.hpp"
#include "controlwidget.hpp"
#include "pref_dialog.hpp"
#include "application.hpp"
#include "recentinfo.hpp"
#include "abrepeater.hpp"
#include "mainwindow.hpp"
#include "playengine.hpp"
#include "translator.hpp"
#include "videorenderer.hpp"
#include "appstate.hpp"
#include "playlist.hpp"
#include "dialogs.hpp"
#include "toolbox.hpp"
#include "libvlc.hpp"
#include "menu.hpp"
#include "info.hpp"
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <qmath.h>

#ifdef Q_WS_MAC
void qt_mac_set_dock_menu(QMenu *menu);
#endif

struct MainWindow::Data {
	Data(Menu &menu): menu(menu), pref(Pref::get()) {}
	bool moving, changingSub, pausedByHiding, dontShowMsg;
	Menu &menu;			const Pref &pref;
	ControlWidget *control;		QWidget *center;
	PlayEngine *engine;		VideoRenderer *video;
	SubtitleRenderer *subtitle;	AudioController *audio;
	TimeLineOsdRenderer *timeLine;	TextOsdRenderer *message;
	Subtitle subLoaded;		QList<int> subSelected;
	QPoint prevPos;			QTimer *hider;
	RecentInfo *recent;		ABRepeater *ab;
	PlaylistView *playlist;		HistoryView *history;
//	FavoritesView *favorite;
#ifndef Q_WS_MAC
	QSystemTrayIcon *tray;
#endif
};

QIcon MainWindow::defaultIcon() {
	static QIcon icon;
	static bool init = false;
	if (!init) {
		icon.addFile(":/img/cmplayer16.png", QSize(16, 16));
		icon.addFile(":/img/cmplayer22.png", QSize(22, 22));
		icon.addFile(":/img/cmplayer24.png", QSize(24, 24));
		icon.addFile(":/img/cmplayer32.png", QSize(32, 32));
		icon.addFile(":/img/cmplayer64.png", QSize(64, 64));
		icon.addFile(":/img/cmplayer128.png", QSize(128, 128));
		icon.addFile(":/img/cmplayer256.png", QSize(256, 256));
		icon.addFile(":/img/cmplayer512.png", QSize(512, 512));
		init = true;
	}
	return icon;
}

MainWindow::MainWindow() {
	LibVLC::init();
	d = new Data(Menu::create(this));
	d->pausedByHiding = d->dontShowMsg = false;
	d->changingSub = d->moving = false;
	d->engine = LibVLC::engine();
	d->audio = LibVLC::audio();
	d->video = LibVLC::video();
	d->subtitle = new SubtitleRenderer;
	d->subtitle->setOsd(new TextOsdRenderer);
	d->timeLine = new TimeLineOsdRenderer;
	d->message = new TextOsdRenderer(Qt::AlignTop | Qt::AlignLeft);
	d->ab = new ABRepeater(d->engine, d->subtitle);
	d->control = createControlWidget();
	d->center = createCentralWidget(d->video, d->control);
	d->recent = &RecentInfo::get();
	d->hider = new QTimer(this);
	d->playlist = new PlaylistView(d->engine, this);
	d->history = new HistoryView(d->engine, this);
#ifndef Q_WS_MAC
	d->tray = new QSystemTrayIcon(defaultIcon(), this);
#endif

	d->hider->setSingleShot(true);
	d->video->addOsd(d->subtitle->osd());
	d->video->addOsd(d->timeLine);
	d->video->addOsd(d->message);

	setMouseTracking(true);
	setCentralWidget(d->center);
	setWindowTitle(QString("CMPlayer %1").arg(Info::version()));
	setAcceptDrops(true);
	d->video->setAcceptDrops(false);
	d->center->setAcceptDrops(false);
	d->control->setAcceptDrops(false);

	Menu &menu = d->menu;
	Menu &open = menu("open");		Menu &play = menu("play");
	Menu &video = menu("video");		Menu &audio = menu("audio");
	Menu &sub = menu("subtitle");		Menu &tool = menu("tool");
	Menu &win = menu("window");		Menu &help = menu("help");

	CONNECT(open["file"], triggered(), this, openFile());
	CONNECT(open["url"], triggered(), this, openUrl());
	CONNECT(open["dvd"], triggered(), this, openDvd());
	CONNECT(open("recent").g(), triggered(QString), this, openLocation(QString));
	CONNECT(open("recent")["clear"], triggered(), d->recent, clear());

	CONNECT(play["stop"], triggered(), d->engine, stop());
	CONNECT(play("speed").g(), triggered(int), this, setSpeed(int));
	CONNECT(play["pause"], triggered(), this, togglePlayPause());
	CONNECT(play("repeat").g(), triggered(int), this, doRepeat(int));
	CONNECT(play["prev"], triggered(), d->playlist, playPrevious());
	CONNECT(play["next"], triggered(), d->playlist, playNext());
	CONNECT(play("seek").g(), triggered(int), this, seek(int));
	CONNECT(play("title").g(), triggered(QAction*), this, setTitle(QAction*));
	CONNECT(play("chapter").g(), triggered(QAction*), this, setChapter(QAction*));

	CONNECT(video("track").g(), triggered(QAction*), this, setVideoTrack(QAction*));
	CONNECT(video("aspect").g(), triggered(double), d->video, setAspectRatio(double));
	CONNECT(video("crop").g(), triggered(double), d->video, setCropRatio(double));
	CONNECT(video["snapshot"], triggered(), this, takeSnapshot());
	CONNECT(video.g("color"), triggered(QAction*), this, setColorProperty(QAction*));

	CONNECT(audio("track").g(), triggered(QAction*), this, setAudioTrack(QAction*));
	CONNECT(audio.g("volume"), triggered(int), this, setVolume(int));
	CONNECT(audio["mute"], toggled(bool), this, setMuted(bool));
	CONNECT(audio.g("amp"), triggered(int), this, setAmp(int));
	CONNECT(audio["volnorm"], toggled(bool), this, setVolumeNormalized(bool));

	CONNECT(sub("list")["hide"], toggled(bool), d->subtitle, setHidden(bool));
	CONNECT(sub("list")["open"], triggered(), this, openSubFile());
	CONNECT(sub("list")["clear"], triggered(), this, clearSubtitles());
	CONNECT(sub("list").g(), triggered(QAction*), this, updateSubtitle(QAction*));
	CONNECT(sub("spu").g(), triggered(QAction*), this, setSPU(QAction*));
	CONNECT(sub.g("pos"), triggered(int), this, moveSubtitle(int));
	CONNECT(sub.g("sync"), triggered(int), this, setSyncDelay(int));

	CONNECT(tool["playlist"], triggered(), d->playlist, toggle());
	CONNECT(tool["history"], triggered(), d->history, toggle());
	CONNECT(tool["subtitle"], triggered(), d->subtitle->view(), toggle());
	CONNECT(tool["pref"], triggered(), this, setPref());

	CONNECT(win.g("sot"), triggered(int), this, updateStaysOnTop());
	CONNECT(win.g("size"), triggered(double), this, setVideoSize(double));

	CONNECT(help["about"], triggered(), this, about());
	CONNECT(menu["exit"], triggered(), qApp, quit());

	CONNECT(&play, aboutToShow(), this, checkPlayMenu());
	CONNECT(&play("title"), aboutToShow(), this, checkTitleMenu());
	CONNECT(&play("chapter"), aboutToShow(), this, checkChapterMenu());
	CONNECT(&video, aboutToShow(), this, checkVideoMenu());
	CONNECT(&video("track"), aboutToShow(), this, checkVideoTrackMenu());
	CONNECT(&audio, aboutToShow(), this, checkAudioMenu());
	CONNECT(&audio("track"), aboutToShow(), this, checkAudioTrackMenu());
	CONNECT(&sub, aboutToShow(), this, checkSubtitleMenu());
	CONNECT(&sub("spu"), aboutToShow(), this, checkSPUMenu());

	CONNECT(d->engine, mrlChanged(Mrl), this, updateMrl(Mrl));
	CONNECT(d->engine, stateChanged(MediaState,MediaState)
		, this, updateState(MediaState,MediaState));
	CONNECT(d->engine, tick(int), d->subtitle, render(int));
	CONNECT(d->video, customContextMenuRequested(const QPoint&)
		, this, showContextMenu(const QPoint&));
	CONNECT(d->video, frameRateChanged(double), d->subtitle, setFrameRate(double));
	CONNECT(d->audio, mutedChanged(bool), audio["mute"], setChecked(bool));
	CONNECT(d->audio, volumeNormalizedChanged(bool), audio["volnorm"], setChecked(bool));

	CONNECT(d->recent, openListChanged(QList<Mrl>), this, updateRecentActions(QList<Mrl>));
	CONNECT(d->hider, timeout(), this, hideCursor());
	CONNECT(d->history, playRequested(Mrl), this, openMrl(Mrl));
#ifndef Q_WS_MAC
	CONNECT(d->tray, activated(QSystemTrayIcon::ActivationReason)
		, this, handleTray(QSystemTrayIcon::ActivationReason));
#endif

	loadState();
	applyPref();

	d->playlist->setPlaylist(d->recent->lastPlaylist());
	d->engine->setMrl(d->recent->lastMrl());
	updateRecentActions(d->recent->openList());

#ifdef Q_WS_MAC
//	qt_mac_set_dock_menu(&d->menu);
	QMenuBar *mb = app()->globalMenuBar();
	mb->addMenu(&open);
	mb->addMenu(&play);
	mb->addMenu(&video);
	mb->addMenu(&audio);
	mb->addMenu(&sub);
	mb->addMenu(&tool);
	mb->addMenu(&win);
	mb->addMenu(&help);
#endif
}

MainWindow::~MainWindow() {
	d->engine->stop();
	d->recent->setLastPlaylist(d->playlist->playlist());
	d->recent->setLastMrl(d->engine->mrl());
	d->recent->save();
	saveState();
	delete d->subtitle;
	LibVLC::release();
	delete d;
}

void MainWindow::checkPlayMenu() {
	Menu &menu = d->menu("play");
	menu("title").setEnabled(d->engine->titleCount() > 0);
	menu("chapter").setEnabled(d->engine->chapterCount() > 0);
}

void MainWindow::checkSubtitleMenu() {
	d->menu("subtitle")("spu").setEnabled(d->engine->spuCount() > 0);
}

void MainWindow::checkVideoMenu() {
	d->menu("video")("track").setEnabled(d->engine->videoTrackCount() > 0);
}

void MainWindow::checkAudioMenu() {
	d->menu("audio")("track").setEnabled(d->engine->audioTrackCount() > 0);
}

#define DEF_TRACK_MENU_FUNC(prop, cap, mm, msg) \
void MainWindow::check##cap##Menu() {\
	const TrackList tracks = d->engine->prop##s();\
	Menu &menu = d->menu mm;\
	menu.g()->clear();\
	if (tracks.isEmpty())\
		return;\
	for (int i=0; i<tracks.size(); ++i)\
		menu.addActionToGroupWithoutKey(tracks[i].name, true)->setData(i);\
	QAction *act = menu.g()->actions().value(d->engine->current##cap##Id());\
	if (act)\
		act->setChecked(true);\
}\
\
void MainWindow::set##cap(QAction *act) {\
	d->engine->setCurrent##cap(act->data().toInt());\
	showMessage(tr(msg), act->text());\
}

DEF_TRACK_MENU_FUNC(videoTrack, VideoTrack, ("video")("track"), "Current Video Track")
DEF_TRACK_MENU_FUNC(audioTrack, AudioTrack, ("audio")("track"), "Current Audio Track")
DEF_TRACK_MENU_FUNC(spu, SPU, ("subtitle")("spu"), "Current Subtitle Track")
DEF_TRACK_MENU_FUNC(title, Title, ("play")("title"), "Current Title")
DEF_TRACK_MENU_FUNC(chapter, Chapter, ("play")("chapter"), "Current Chapter")

void MainWindow::openLocation(const QString &loc) {
	openMrl(Mrl(loc));
}

void MainWindow::loadState() {
	d->dontShowMsg = true;
	AppState as;
	as.load();

	d->menu("video")("aspect").g()->trigger(as[AppState::AspectRatio]);
	d->menu("video")("crop").g()->trigger(as[AppState::Crop]);
	d->menu("window").g("sot")->trigger(StaysOnTopEnum::value(as[AppState::StaysOnTop].toString()));

	d->audio->setVolume(as[AppState::Volume].toInt());
	d->audio->setMuted(as[AppState::Muted].toBool());
	d->audio->setPreAmp(as[AppState::Amp].toDouble());
	d->audio->setVolumeNormalized(as[AppState::VolNorm].toBool());

	d->engine->setSpeed(as[AppState::PlaySpeed].toDouble());
	d->subtitle->setPos(as[AppState::SubPos].toDouble());
	d->subtitle->setDelay(as[AppState::SubSync].toInt());

	d->dontShowMsg = false;
}

StaysOnTop MainWindow::staysOnTopMode() const {
	const int data = d->menu("window").g("sot")->checkedAction()->data().toInt();
	switch (data) {
	case OnTopPlaying:
		return OnTopPlaying;
	case AlwaysOnTop:
		return AlwaysOnTop;
	case DontStayOnTop:
		return DontStayOnTop;
	default:
		return OnTopPlaying;
	}
}

void MainWindow::saveState() {
	AppState as;
	as[AppState::AspectRatio] = d->video->aspectRatio();
	as[AppState::Crop] = d->video->cropRatio();
	as[AppState::Volume] = d->audio->volume();
	as[AppState::VolNorm] = d->audio->isVolumeNormalized();
	as[AppState::Muted] = d->audio->isMuted();
	as[AppState::Amp] = d->audio->preAmp();
	as[AppState::PlaySpeed] = d->engine->speed();
	as[AppState::SubPos] = d->subtitle->pos();
	as[AppState::SubSync] = d->subtitle->delay();
	as[AppState::StaysOnTop] = StaysOnTopEnum::name(staysOnTopMode());
	as.save();
}

ControlWidget *MainWindow::createControlWidget() {
	ControlWidget *w = new ControlWidget(d->engine, 0);
	Menu &play = d->menu("play");
	w->connectMute(d->menu("audio")["mute"]);
	w->connectPlay(play["pause"]);
	w->connectPrevious(play["prev"]);
	w->connectNext(play["next"]);
	w->connectForward(play("seek")["forward1"]);
	w->connectBackward(play("seek")["backward1"]);
	return w;
}

QWidget *MainWindow::createCentralWidget(QWidget *video, QWidget *control) {
	QWidget *w = new QWidget(this);
	w->setMouseTracking(true);

	QVBoxLayout *vbox = new QVBoxLayout(w);
	vbox->addWidget(video);
	vbox->addWidget(control);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->setSpacing(0);
	return w;
}

void MainWindow::updateRecentActions(const QList<Mrl> &list) {
	Menu &recent = d->menu("open")("recent");
	ActionGroup *group = recent.g();
	const int diff = group->actions().size() - list.size();
	if (diff < 0) {
		QList<QAction*> acts = recent.actions();
		QAction *sprt = acts[acts.size()-2];
		for (int i=0; i<-diff; ++i) {
			QAction *action = new QAction(&recent);
			recent.insertAction(sprt, action);
			recent.g()->addAction(action);
		}
	} else if (diff > 0) {
		QList<QAction*> acts = recent.g()->actions();
		for (int i=0; i<diff; ++i)
			delete acts.takeLast();
	}
	QList<QAction*> acts = group->actions();
	for (int i=0; i<list.size(); ++i) {
		QAction *act = acts[i];
		act->setData(list[i].toString());
		act->setText(list[i].displayName());
		act->setVisible(!list[i].isEmpty());
	}
}

void MainWindow::openMrl(const Mrl &mrl) {
	openMrl(mrl, QString());
}

void MainWindow::openMrl(const Mrl &mrl, const QString &enc) {
	if (mrl == d->engine->mrl()) {
		if (!d->engine->isPlaying())
			d->engine->play();
	} else {
		d->playlist->load(mrl, enc);
		if (!mrl.isPlaylist()) {
			d->playlist->play(mrl);
			if (!mrl.isDVD())
				RecentInfo::get().stack(mrl);
		}
	}
}

void MainWindow::openFile() {
	AppState as;
	const QString filter = Info::mediaExtFilter();
	const QString dir = QFileInfo(as[AppState::LastOpenFile].toString()).absolutePath();
	const QString file = QFileDialog::getOpenFileName(this, tr("Open File"), dir, filter);
	if (!file.isEmpty()) {
		openMrl(Mrl(file));
		as[AppState::LastOpenFile] = file;
	}
}

void MainWindow::openDvd() {
	OpenDVDDialog dlg(this);
	dlg.setDevices(app()->devices());
	if (dlg.exec()) {
		const Mrl mrl(QLatin1String("dvd://") + dlg.device());
		openMrl(mrl);
	}
}

void MainWindow::openUrl() {
//	GetUrlDialog dlg(this);
//	if (dlg.exec())
//		openMrl(dlg.url(), dlg.encoding());
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	int width = d->center->width();
	int height = d->center->height();
	if (!isFullScreen())
		height -= d->control->height();
	showMessage(QString("%1x%2").arg(width).arg(height), 1000);
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
	if (mrl.isLocalFile()) {
		doSubtitleAutoLoad();
		doSubtitleAutoSelection();
	} else
		clearSubtitles();
	Menu &list = d->menu("subtitle")("list");
	d->changingSub = true;
	for (int i=0; i<d->subLoaded.size(); ++i) {
		QAction *action = list.addActionToGroupWithoutKey(d->subLoaded[i].name(), true);
		action->setData(i);
		action->setChecked(d->subSelected.contains(i));
	}
	d->changingSub = false;
	updateSubtitle();
	const int row = d->playlist->model()->currentRow() + 1;
	if (row > 0)
		d->control->setTrackNumber(row, d->playlist->model()->rowCount());
}

void MainWindow::clearSubtitles() {
	d->subLoaded.clear();
	d->subSelected.clear();
	QList<QAction*> acts = d->menu("subtitle")("list").g()->actions();
	for (int i=0; i<acts.size(); ++i)
		delete acts[i];
	d->subtitle->setSubtitle(d->subLoaded);
}

void MainWindow::openSubFile() {
	const QString filter = tr("Subtitle Files") +' '+ Info::subtitleExt().toFilter();
	QString enc = d->pref.subtitleEncoding;
	QString dir;
	if (d->engine->mrl().isLocalFile())
		dir = QFileInfo(d->engine->mrl().toLocalFile()).absolutePath();
	const QStringList files = EncodingFileDialog::getOpenFileNames(this
			, tr("Open Subtitle"), dir, filter, &enc);
	if (!files.isEmpty())
		appendSubFiles(files, true, enc);
}

void MainWindow::appendSubFiles(const QStringList &files, bool checked, const QString &enc) {
	if (files.isEmpty())
		return;
	Menu &list = d->menu("subtitle")("list");
	int idx = d->subLoaded.size();
	for (int i=0; i<files.size(); ++i) {
		Subtitle sub;
		if (!sub.load(files[i], enc))
			continue;
		for (int j=0; j<sub.size(); ++j, ++idx) {
			d->subSelected.append(idx);
			d->subLoaded.append(sub[j]);
			QAction *action = list.addActionToGroupWithoutKey(
					d->subLoaded[idx].name(), true);
			action->setData(idx);
			if (checked) {
				d->changingSub = true;
				action->setChecked(true);
			}
		}
	}
	if (d->changingSub) {
		d->changingSub = false;
		updateSubtitle();
	}
}

void MainWindow::updateSubtitle(QAction *action) {
	if (d->changingSub)
		return;
	const int idx = action->data().toInt();
	d->subSelected.removeAll(idx);
	if (action->isChecked())
		d->subSelected.append(idx);
	updateSubtitle();
}

void MainWindow::doSubtitleAutoLoad() {
	clearSubtitles();
	const Mrl mrl = d->engine->mrl();
	// needs local file checking!
	if (d->pref.subtitleAutoLoad == NoAutoLoad)
		return;
	const QStringList filter = Info::subtitleNameFilter();
	const QFileInfo fileInfo(mrl.toLocalFile());
	const QFileInfoList all = fileInfo.dir().entryInfoList(filter, QDir::Files, QDir::Name);
	const QString base = fileInfo.completeBaseName();
	for (int i=0; i<all.size(); ++i) {
		if (d->pref.subtitleAutoLoad != SamePath) {
			if (d->pref.subtitleAutoLoad == Matched) {
				if (base != all[i].completeBaseName())
					continue;
			} else if (!all[i].fileName().contains(base))
				continue;
		}
		Subtitle sub;
		if (sub.load(all[i].absoluteFilePath(), d->pref.subtitleEncoding)) {
			d->subLoaded += sub;
		}
	}
}

void MainWindow::doSubtitleAutoSelection() {
	const Mrl mrl = d->engine->mrl();
	d->subSelected.clear();
	if (d->subLoaded.isEmpty() || !mrl.isLocalFile())
		return;
	QSet<QString> langSet;
	const QString base = QFileInfo(mrl.toLocalFile()).completeBaseName();
	for (int i=0; i<d->subLoaded.size(); ++i) {
		bool select = false;
		if (d->pref.subtitleAutoSelect == SameName) {
			select = QFileInfo(d->subLoaded[i].fileName()).completeBaseName() == base;
		} else if (d->pref.subtitleAutoSelect == AllLoaded) {
			select = true;
		} else if (d->pref.subtitleAutoSelect == EachLanguage) {
			const QString lang = d->subLoaded[i].language().id();
			if ((select = (!langSet.contains(lang))))
				langSet.insert(lang);
		}
		if (select)
			d->subSelected.push_back(i);
	}
	if (d->pref.subtitleAutoSelect == SameName
			&& d->subSelected.size() > 1 && !d->pref.subtitleExtension.isEmpty()) {
		int index = -1;
		for (int i=0; i<d->subSelected.size(); ++i) {
			const QString fileName = d->subLoaded[d->subSelected[i]].fileName();
			const QString suffix = QFileInfo(fileName).suffix().toLower();
			if (d->pref.subtitleExtension == suffix) {
				index = d->subSelected[i];
				break;
			}
		}
		if (index != -1) {
			d->subSelected.clear();
			d->subSelected.push_back(index);
		}
	}
}

void MainWindow::updateSubtitle() {
	const QStringList priority = d->pref.subtitlePriority;
	QList<int> order;
	QList<int> indexes = d->subSelected;
	QList<int>::iterator it;
	for (int i=0; i<priority.size(); ++i) {
		it = indexes.begin();
		while(it != indexes.end()) {
			if (d->subLoaded[*it].language().id() == priority[i]) {
				order.append(*it);
				it = indexes.erase(it);
			} else
				++it;
		}
	}
	order += indexes;
	Subtitle subtitle;
	for (int i=0; i<order.size(); ++i)
		subtitle.append(d->subLoaded[order[i]]);
	d->subtitle->setSubtitle(subtitle);
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

void MainWindow::showMessage(const QString &cmd, double value, const QString &unit, bool sign, int last) {
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
	if (d->dontShowMsg)
		return;
	d->message->showText(message, last);
}

void MainWindow::setVolume(int diff) {
	if (!diff)
		return;
	const int volume = qBound(0, d->audio->volume() + diff, 100);
	d->audio->setVolume(volume);
	showMessage(tr("Volume"), volume, "%");
}

void MainWindow::setMuted(bool muted) {
	d->audio->setMuted(muted);
	showMessage(tr("Mute"), muted);
}

void MainWindow::setFullScreen(bool full) {
	if (full == isFullScreen())
		return;
	d->moving = false;
	d->prevPos = QPoint();
	d->control->setHidden(full);
	if (full) {
		app()->setAlwaysOnTop(this, false);
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		if (d->pref.hideCursor)
			d->hider->start(d->pref.hideDelay);
		d->video->setFixedRenderSize(size());
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		d->hider->stop();
		if (cursor().shape() == Qt::BlankCursor)
			unsetCursor();
		d->video->setFixedRenderSize(QSize());
		updateStaysOnTop();
	}
}

void MainWindow::setVideoSize(double rate) {
	if (rate < 0) {
		const bool wasFull = isFullScreen();
		setFullScreen(!wasFull);
	} else {
		if (isFullScreen())
			setFullScreen(false);
		resize(size() - d->video->size() + d->video->sizeHint()*qSqrt(rate));
	}
}

void MainWindow::updateState(MediaState state, MediaState old) {
	if (old == state)
		return;
	if (state == PlayingState) {
		app()->setScreensaverDisabled(d->pref.disableScreensaver);
		d->menu("play")["pause"]->setText(tr("Pause"));
	} else {
		app()->setScreensaverDisabled(false);
		d->menu("play")["pause"]->setText(tr("Play"));
	}
	d->video->setLogoMode(state == StoppedState);
	updateStaysOnTop();
}

void MainWindow::setSpeed(int diff) {
	int newSpeed = 100;
	if (diff)
		newSpeed = d->engine->speed()*100.0 + diff + 0.5;
	d->engine->setSpeed(qBound(0.1, newSpeed*0.01, 10.0));
	showMessage(tr("Speed"), QString::fromUtf8("\303\227%1").arg(d->engine->speed()));
}

void MainWindow::setAmp(int amp) {
	const int newAmp = qBound(0, qRound(d->audio->preAmp()*100 + amp), 1000);
	d->audio->setPreAmp(newAmp*0.01);
	showMessage(tr("Amp"), newAmp, "%");
}

void MainWindow::doRepeat(int key) {
	QString ex;
/*	if (key == 'a') {
		if (d->repeater->isHidden()) {
			d->repeater->setRepeater(repeater);
			d->repeater->show();
		}
	} else */
	if (key == 'r') {
		if (d->engine->state() == StoppedState)
			return;
		if (!d->ab->hasA()) {
			const int at = d->ab->setAToCurrentTime();
			QString a = msecsToString(at, "h:mm:ss.zzz");
			a.chop(2);
			ex = tr("Set A to %1").arg(a);
		} else if (!d->ab->hasB()) {
			const int at = d->ab->setBToCurrentTime();
			if ((at - d->ab->a()) < 100) {
				ex = tr("Range is too short!");
				d->ab->setB(-1);
			} else {
				QString b = msecsToString(at, "h:mm:ss.zzz");
				b.chop(2);
				ex = tr("Set B to %1. Start to repeat!").arg(b);
				d->ab->start();
			}
		}
	} else if (key == 'q') {
		d->ab->stop();
		d->ab->setA(-1);
		d->ab->setB(-1);
		ex = tr("Quit repeating");
	} else if (key == 's') {
		d->ab->setAToSubtitleTime();
		d->ab->setBToSubtitleTime();
		ex = tr("Repeat current subtitle");
		d->ab->start();
	}
	showMessage(tr("A-B Repeat"), ex);
}

void MainWindow::moveSubtitle(int dy) {
	int newPos = qBound(0, qRound(d->subtitle->pos()*100.0 + dy), 100);
	d->subtitle->setPos(newPos*0.01);
	showMessage(tr("Subtitle Position"), newPos, "%");
}

PlayEngine *MainWindow::engine() const {
	return d->engine;
}

#define IS_IN_CENTER (d->video->geometry().contains(d->video->mapFrom(this, event->pos())))
#define IS_BUTTON(b) (event->buttons() & (b))

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (!IS_IN_CENTER)
		return;
	if (IS_BUTTON(Qt::LeftButton) && !isFullScreen()) {
		d->moving = true;
		d->prevPos = event->globalPos();
	}
	if (IS_BUTTON(Qt::MidButton)) {
		QAction *action = getTriggerAction(event->modifiers()
				, d->pref.middleClickMap, d->menu.clickAction(), 0);
		if (action)
			action->trigger();
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	d->hider->stop();
	if (cursor().shape() == Qt::BlankCursor)
		unsetCursor();
	QMainWindow::mouseMoveEvent(event);
	const bool full = isFullScreen();
	if (full) {
		if (d->moving) {
			d->moving = false;
			d->prevPos = QPoint();
		}
		static const int h = d->control->height();
		QRect rect = this->rect();
		rect.setTop(rect.height() - h);
		d->control->setVisible(rect.contains(event->pos()));
		if (d->pref.hideCursor)
			d->hider->start(d->pref.hideDelay);
	} else {
		if (d->moving) {
			const QPoint pos = event->globalPos();
			move(this->pos() + pos - d->prevPos);
			d->prevPos = pos;
		}
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
	if (d->moving) {
		d->moving = false;
		d->prevPos = QPoint();
	}
	QMainWindow::mouseReleaseEvent(event);
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

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	if (!event->mimeData()->hasUrls())
		return;
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;
	qSort(urls);
	Playlist playlist;
	QStringList subList;
	for (int i=0; i<urls.size(); ++i) {
		const QString suffix = QFileInfo(urls[i].path()).suffix().toLower();
		if (Info::playlistExt().contains(suffix)) {
			Playlist list;
			list.load(urls[i].toString());
			playlist += list;
		} else if (Info::subtitleExt().contains(suffix)) {
			subList << urls[i].toLocalFile();
		} else if (Info::videoExt().contains(suffix)
				|| Info::audioExt().contains(suffix)) {
			playlist.append(urls[i].toString());
		}
	}
	if (!playlist.isEmpty()) {
		d->playlist->append(playlist);
		d->playlist->play(playlist.first());
	} else if (!subList.isEmpty())
		appendSubFiles(subList, true, d->pref.subtitleEncoding);
}

void MainWindow::setSyncDelay(int diff) {
	int delay = diff ? d->subtitle->delay() + diff : 0;
	d->subtitle->setDelay(delay);
	showMessage("Subtitle Sync", delay*0.001, "sec", true);

}

void MainWindow::setPref() {
	Pref::Dialog dlg(this);
	if (dlg.exec())
		applyPref();
}

void MainWindow::showEvent(QShowEvent *event) {
	QMainWindow::showEvent(event);
	if (d->pausedByHiding && d->engine && d->engine->isPaused()) {
		d->engine->play();
		d->pausedByHiding = false;
	}
}

void MainWindow::hideEvent(QHideEvent *event) {
	QMainWindow::hideEvent(event);
	if (!d->pref.pauseMinimized || !d->engine)
		return;
	if (!d->engine->isPlaying() || (d->pref.pauseVideoOnly && !d->engine->hasVideo()))
		return;
	d->pausedByHiding = true;
	d->engine->pause();
}

void MainWindow::hideCursor() {
	if (cursor().shape() != Qt::BlankCursor)
		setCursor(Qt::BlankCursor);
}

void MainWindow::handleTray(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger)
		setVisible(!isVisible());
	else if (reason == QSystemTrayIcon::Context)
		d->menu.contextMenu()->exec(QCursor::pos());
}

void MainWindow::applyPref() {
	Subtitle::Parser::setMsPerCharactor(d->pref.msPerChar);
	Translator::load(d->pref.locale);
	app()->setStyle(d->pref.windowStyle);
	d->subtitle->osd()->setStyle(d->pref.subtitleStyle);
	d->menu.updatePref();
#ifndef Q_WS_MAC
	d->tray->setVisible(d->pref.enableSystemTray);
#endif
	d->control->setState(d->engine->state());
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (d->pref.enableSystemTray && d->pref.hideClosed) {
		hide();
		AppState as;
		if (as[AppState::TrayFirst].toBool()) {
			CheckDialog dlg(this);
			dlg.setChecked(true);
			dlg.setLabelText(tr("CMPlayer will be running in the system tray "
					"when the window closed.<br>"
					"You can change this behavior in the preferences.<br>"
					"If you want to exit CMPlayer, please use 'Exit' menu."));
			dlg.setCheckBoxText(tr("Do not display this message again"));
			dlg.exec();
			as[AppState::TrayFirst] = !dlg.isChecked();
		}
		event->ignore();
	} else {
		event->accept();
		qApp->quit();
	}
}

void MainWindow::showMessage(const QString &cmd, bool value, int last) {
	showMessage(cmd, value ? tr("On") : tr("Off"), last);
}

void MainWindow::setVolumeNormalized(bool norm) {
	d->audio->setVolumeNormalized(norm);
	showMessage(tr("Normalize Volume"), norm);
}

void MainWindow::setTempoScaled(bool scaled) {
	d->audio->setTempoScaled(scaled);
	showMessage(tr("Autoscale Pitch"), scaled);
}

void MainWindow::setColorProperty(QAction *action) {
	const QList<QVariant> data = action->data().toList();
	const ColorProperty::Value prop = ColorProperty::Value(data[0].toInt());
	ColorProperty color = d->video->colorProperty();
	color.setValue(prop, color.value(prop) + data[1].toInt()*0.01);
	d->video->setColorProperty(color);
	QString cmd;
	switch(prop) {
	case ColorProperty::Brightness:
		cmd = tr("Brightness");
		break;
	case ColorProperty::Saturation:
		cmd = tr("Saturation");
		break;
	case ColorProperty::Hue:
		cmd = tr("Hue");
		break;
	case ColorProperty::Contrast:
		cmd = tr("Contrast");
		break;
	default:
		return;
	}
	const double v = d->video->colorProperty()[prop]*100.0;
	const int value = v + (v < 0 ? -0.5 : 0.5);
	showMessage(cmd, value, "%", true);
}

void MainWindow::updateStaysOnTop() {
	const int data = d->menu("window").g("sot")->checkedAction()->data().toInt();
	bool onTop = false;
	if (!isFullScreen()) {
		switch (data) {
		case AlwaysOnTop:
			onTop = true;
			break;
		case DontStayOnTop:
			onTop = false;
			break;
		default: // OnTopPlaying:
			onTop = d->engine->isPlaying();
			break;
		}
	}
	app()->setAlwaysOnTop(this, onTop);
}

void MainWindow::takeSnapshot() {
	static SnapshotDialog *dlg = new SnapshotDialog(this);
	dlg->setVideoRenderer(d->video);
	dlg->setSubtitleRenderer(d->subtitle);
	dlg->take();
	dlg->adjustSize();
	dlg->show();
}

void MainWindow::about() {
	AboutDialog dlg(this);
	dlg.exec();
}


