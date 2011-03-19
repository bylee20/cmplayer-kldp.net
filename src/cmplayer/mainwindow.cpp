#include "timelineosdrenderer.hpp"
#include "framebufferobjectoverlay.hpp"
#include "screensavermanager.hpp"
#include "subtitlerenderer.hpp"
#include "charsetdetector.hpp"
#include "subtitle_parser.hpp"
#include "textosdrenderer.hpp"
#include "audiocontroller.hpp"
#include "controlwidget.hpp"
#include "playlistview.hpp"
#include "subtitleview.hpp"
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
#include "pref.hpp"
#include "info.hpp"
#include <QtGui/QMouseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QMenuBar>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <qmath.h>

struct MainWindow::Data {
	Data(Menu &menu): menu(menu), pref(Pref::get()) {}
	Menu &menu;
	VideoScreen *screen;
	const Pref &pref;
	ControlWidget *control;
	PlayEngine *engine;
	VideoRenderer *video;
	AudioController *audio;
	SubtitleRenderer *subtitle;
	TimeLineOsdRenderer *timeLine;
	TextOsdRenderer *message;
	Subtitle subLoaded;
	QList<int> subSelected;
	ABRepeater *ab;
	ToolBox *tool;
	bool moving, changingSub, pausedByHiding, dontShowMsg;
	QPoint prevPos;
	QTimer hider;
	QSystemTrayIcon *tray;
	TrackList spus, titles, chapters, aTracks, vTracks;
};

QIcon MainWindow::defaultIcon() {
	return QIcon(":/img/cmplayer.png");
}

MainWindow::MainWindow(): d(new Data(Menu::create(this))) {
	d->changingSub = d->moving = false;
	d->engine = LibVlc::engine();
	d->audio = LibVlc::audio();
	d->video = LibVlc::video();
	d->subtitle = new SubtitleRenderer;
	d->subtitle->setOsd(new TextOsdRenderer);
	d->timeLine = new TimeLineOsdRenderer;
	d->message = new TextOsdRenderer(Qt::AlignTop | Qt::AlignLeft);
	QApplication::setWindowIcon(defaultIcon());
	d->tray = new QSystemTrayIcon(defaultIcon(), this);
	d->dontShowMsg = false;
	setupUi();

	d->ab = new ABRepeater(d->engine, d->subtitle);
	d->pausedByHiding = false;
	RecentInfo &recent = RecentInfo::get();

	Menu &menu = d->menu;

	Menu &open = d->menu("open");
	connect(open["file"], SIGNAL(triggered()), this, SLOT(openFile()));
	connect(open["url"], SIGNAL(triggered()), this, SLOT(openUrl()));
	connect(open["dvd"], SIGNAL(triggered()), this, SLOT(openDvd()));
	connect(open("recent").g(), SIGNAL(triggered(QString)), this, SLOT(openLocation(QString)));
	connect(open("recent")["clear"], SIGNAL(triggered()), &recent, SLOT(clear()));

	Menu &play = menu("play");
	connect(play["stop"], SIGNAL(triggered()), d->engine, SLOT(stop()));
	connect(play("speed").g(), SIGNAL(triggered(int)), this, SLOT(setSpeed(int)));
	connect(play["pause"], SIGNAL(triggered()), this, SLOT(togglePlayPause()));
	connect(play("repeat").g(), SIGNAL(triggered(int)), this, SLOT(doRepeat(int)));
	connect(play("seek").g(), SIGNAL(triggered(int)), this, SLOT(seek(int)));
	connect(play("title").g(), SIGNAL(triggered(QAction*)), this, SLOT(setTitle(QAction*)));
	connect(play("chapter").g(), SIGNAL(triggered(QAction*)), this, SLOT(setChapter(QAction*)));

	Menu &video = menu("video");
	connect(video("track").g(), SIGNAL(triggered(QAction*)), this, SLOT(setVideoTrack(QAction*)));
	connect(video("size").g(), SIGNAL(triggered(double)), this, SLOT(setVideoSize(double)));
	connect(video("aspect").g(), SIGNAL(triggered(double)), d->video, SLOT(setAspectRatio(double)));
	connect(video("crop").g(), SIGNAL(triggered(double)), d->video, SLOT(setCropRatio(double)));
//	connect(screen["snapshot"], SIGNAL(triggered()), this, SLOT(takeSnapshot()));
	connect(video.g("color"), SIGNAL(triggered(QAction*)), this, SLOT(setColorProperty(QAction*)));

	Menu &audio = menu("audio");
	connect(audio("track").g(), SIGNAL(triggered(QAction*)), this, SLOT(setAudioTrack(QAction*)));
	connect(audio.g("volume"), SIGNAL(triggered(int)), this, SLOT(setVolume(int)));
	connect(audio["mute"], SIGNAL(toggled(bool)), this, SLOT(setMuted(bool)));
	connect(audio.g("amp"), SIGNAL(triggered(int)), this, SLOT(setAmp(int)));
	connect(audio["normalize-volume"], SIGNAL(toggled(bool)), this, SLOT(setVolumeNormalized(bool)));
	connect(d->audio, SIGNAL(mutedChanged(bool)), audio["mute"], SLOT(setChecked(bool)));
	connect(d->audio, SIGNAL(volumeNormalizedChanged(bool))
		, audio["normalize-volume"], SLOT(setChecked(bool)));

	Menu &sub = menu("subtitle");

	connect(sub("list")["hide"], SIGNAL(toggled(bool)), d->subtitle, SLOT(setHidden(bool)));
	connect(sub("list")["open"], SIGNAL(triggered()), this, SLOT(openSubFile()));
	connect(sub("list")["clear"], SIGNAL(triggered()), this, SLOT(clearSubtitles()));
	connect(sub("list").g(), SIGNAL(triggered(QAction*)), this, SLOT(updateSubtitle(QAction*)));
	connect(sub("spu").g(), SIGNAL(triggered(QAction*)), this, SLOT(setSPU(QAction*)));
	connect(sub.g("pos"), SIGNAL(triggered(int)), this, SLOT(moveSubtitle(int)));
	connect(sub.g("sync"), SIGNAL(triggered(int)), this, SLOT(setSyncDelay(int)));

	connect(menu["tool-box"], SIGNAL(triggered()), this, SLOT(toggleToolBox()));
	connect(menu["pref"], SIGNAL(triggered()), this, SLOT(setPref()));
//	connect(menu["about"], SIGNAL(triggered()), this, SLOT(showAbout()));
//// 	connect(menu["help"], SIGNAL(triggered()), this, SLOT(slotHelp()));
	connect(menu["exit"], SIGNAL(triggered()), qApp, SLOT(quit()));



	connect(&play, SIGNAL(aboutToShow()), this, SLOT(checkPlayMenu()));
	connect(&play("title"), SIGNAL(aboutToShow()), this, SLOT(checkTitleMenu()));
	connect(&play("chapter"), SIGNAL(aboutToShow()), this, SLOT(checkChapterMenu()));
	connect(&video, SIGNAL(aboutToShow()), this, SLOT(checkVideoMenu()));
	connect(&video("track"), SIGNAL(aboutToShow()), this, SLOT(checkVideoTrackMenu()));
	connect(&audio, SIGNAL(aboutToShow()), this, SLOT(checkAudioMenu()));
	connect(&audio("track"), SIGNAL(aboutToShow()), this, SLOT(checkAudioTrackMenu()));
	connect(&sub, SIGNAL(aboutToShow()), this, SLOT(checkSubtitleMenu()));
	connect(&sub("spu"), SIGNAL(aboutToShow()), this, SLOT(checkSPUMenu()));


	QMenuBar *mb = menuBar();
	mb->addMenu(&open);
	mb->addMenu(&play);
	mb->addMenu(&sub);
	mb->addMenu(&video);
	mb->addMenu(&audio);
	QMenu *m = mb->addMenu(tr("Tools"));
	m->addAction(tr("Preferences"), this, SLOT(setPref()));

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(updateMrl(Mrl)));
	connect(d->engine, SIGNAL(stateChanged(MediaState,MediaState))
		, this, SLOT(updateState(MediaState,MediaState)));

	connect(d->video, SIGNAL(customContextMenuRequested(const QPoint&))
		, this, SLOT(showContextMenu(const QPoint&)));
	connect(&recent, SIGNAL(openListChanged(QList<Mrl>))
		, this, SLOT(updateRecentActions(QList<Mrl>)));
	connect(&d->hider, SIGNAL(timeout()), this, SLOT(hideCursor()));
	d->hider.setSingleShot(true);
	connect(d->tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason))
		, this, SLOT(handleTray(QSystemTrayIcon::ActivationReason)));

	d->video->addOsd(d->subtitle->osd());
	d->video->addOsd(d->timeLine);
	d->video->addOsd(d->message);
	connect(d->video, SIGNAL(frameRateChanged(double))
		, d->subtitle, SLOT(setFrameRate(double)));
	connect(d->engine, SIGNAL(tick(int)), d->subtitle, SLOT(render(int)));

	loadState();
	applyPref();
	d->tool = new ToolBox(this);

	connect(play["prev"], SIGNAL(triggered()), d->tool->playlist(), SLOT(playPrevious()));
	connect(play["next"], SIGNAL(triggered()), d->tool->playlist(), SLOT(playNext()));
	connect(d->tool->playlist(), SIGNAL(finished()), this, SLOT(handleFinished()));
	connect(d->tool->history(), SIGNAL(playRequested(Mrl)), this, SLOT(openMrl(Mrl)));

	d->tool->playlist()->setPlaylist(recent.lastPlaylist());
	d->engine->setMrl(recent.lastMrl());
	updateRecentActions(recent.openList());

}

MainWindow::~MainWindow() {
	d->engine->stop();
	RecentInfo &recent = RecentInfo::get();
	recent.setLastPlaylist(d->tool->playlist()->playlist());
	recent.setLastMrl(d->engine->mrl());
	recent.save();
	saveState();

	delete d->subtitle;
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

	d->audio->setVolume(as[AppState::Volume].toInt());
	d->audio->setMuted(as[AppState::Muted].toBool());
	d->audio->setPreAmp(as[AppState::Amp].toDouble());
	d->audio->setVolumeNormalized(as[AppState::VolNorm].toBool());

	d->engine->setSpeed(as[AppState::PlaySpeed].toDouble());
	d->subtitle->setPos(as[AppState::SubPos].toDouble());
	d->subtitle->setDelay(as[AppState::SubSync].toInt());
	d->dontShowMsg = false;
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
	as.save();
}

void MainWindow::setupUi() {
	QWidget *center = new QWidget(this);

//	d->screen = new VideoScreen(d->engine->renderer(), center);

	d->control = new ControlWidget(d->engine, center);
	Menu &play = d->menu("play");
	d->control->connectMute(d->menu("audio")["mute"]);
	d->control->connectPlay(play["pause"]);
	d->control->connectPrevious(play["prev"]);
	d->control->connectNext(play["next"]);
	d->control->connectForward(play("seek")["forward1"]);
	d->control->connectBackward(play("seek")["backward1"]);
	d->control->connectOpen(d->menu("open")["file"]);
	d->control->connectFullScreen(d->menu("video")("size")["full"]);
	d->control->connectToolBox(d->menu["tool-box"]);
	d->control->connectPreference(d->menu["pref"]);
	QVBoxLayout *vbox = new QVBoxLayout(center);
	vbox->addWidget(d->video);
	vbox->addWidget(d->control);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->setSpacing(0);

	setMouseTracking(true);
	setCentralWidget(center);
	d->video->setMouseTracking(true);
	center->setMouseTracking(true);
	d->video->setContextMenuPolicy(Qt::CustomContextMenu);
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
	if (mrl == d->engine->mrl())
		return;
	d->tool->playlist()->load(mrl, enc);
	if (!mrl.isPlaylist()) {
		d->tool->playlist()->play(mrl);
		if (!mrl.isDVD())
			RecentInfo::get().stack(mrl);
	}
}

void MainWindow::openFile() {
	AppState as;
	const QString filter = Info::mediaExtensionFilter();
	const QString dir = QFileInfo(as[AppState::LastOpenFile].toString()).absolutePath();
	const QString file = QFileDialog::getOpenFileName(this, tr("Open File"), dir, filter);
	if (!file.isEmpty()) {
		openMrl(Mrl(file));
		as[AppState::LastOpenFile] = file;
	}
}

void MainWindow::openDvd() {
	const Mrl mrl("dvd:///dev/rdisk4");
	openMrl(mrl);
}

void MainWindow::openUrl() {
//	GetUrlDialog dlg(this);
//	if (dlg.exec())
//		openMrl(dlg.url(), dlg.encoding());
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

//	const bool dvd = mrl.isDVD();
//	d->menu("dvd-menu").menuAction()->setVisible(dvd);
//	d->menu("subtitle").menuAction()->setVisible(!dvd);
}

void MainWindow::clearSubtitles() {
	d->subLoaded.clear();
	d->subSelected.clear();
	QList<QAction*> acts = d->menu("subtitle")("list").g()->actions();
	for (int i=0; i<acts.size(); ++i)
		delete acts[i];
	d->subtitle->setSubtitle(d->subLoaded);
	d->tool->subtitle()->setSubtitle(d->subLoaded);
}

void MainWindow::openSubFile() {
	const QString filter = tr("Subtitle Files") +' '+ Info::subtitleExtension().toFilter();
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
//	if (d->player->currentSource().isDisc())
//		d->player->setCurrentSpu(action->isChecked()
//				? action->data().toString() : QString());
//	else {
		const int idx = action->data().toInt();
		d->subSelected.removeAll(idx);
		if (action->isChecked())
			d->subSelected.append(idx);
		updateSubtitle();
//	}
}

void MainWindow::doSubtitleAutoLoad() {
	clearSubtitles();
	const Mrl mrl = d->engine->mrl();
	// needs local file checking!
	if (d->pref.subtitleAutoLoad == NoAutoLoad)
		return;
	const QStringList filter = Info::subtitleExtension().toNameFilter();
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
		if (sub.load(all[i].absoluteFilePath(), d->pref.subtitleEncoding))
			d->subLoaded += sub;
	}
//	d->menu("subtitle")("list").g()->setExclusive(source.isDisc());
//	if (!source.isLocalFile())
//		return;
}

void MainWindow::doSubtitleAutoSelection() {
	const Mrl mrl = d->engine->mrl();
	d->subSelected.clear();
	// local file check need
	if (d->subLoaded.isEmpty())
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
	d->tool->subtitle()->setSubtitle(subtitle);
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
	d->control->setHidden(full);
//	d->screen->setSyncSize(!full);
	if (full) {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		if (d->pref.hideCursor)
			d->hider.start(d->pref.hideDelay);
		d->video->setFixedRenderSize(size());
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		d->hider.stop();
		if (cursor().shape() == Qt::BlankCursor)
			unsetCursor();
		d->video->setFixedRenderSize(QSize());
	}
}

void MainWindow::setVideoSize(double rate) {
	if (rate < 0) {
		const bool wasFull = isFullScreen();
		setFullScreen(!wasFull);
	} else {
		if (isFullScreen())
			setFullScreen(false);
//		QWidget *w = d->stack->currentWidget();
		resize(size() - d->video->size() + d->video->sizeHint()*qSqrt(rate));
	}
}

void MainWindow::updateState(MediaState state, MediaState old) {
	if (old == state)
		return;
	if (state == PlayingState) {
//		ScreensaverManager::setDisabled(d->pref.disableScreensaver);
		d->menu("play")["pause"]->setText(tr("Pause"));
	} else {
//		ScreensaverManager::setDisabled(false);
		d->menu("play")["pause"]->setText(tr("Play"));
	}
	if (state == StoppedState) {
//		d->stack->setCurrentWidget(d->logo);
	} else {
//		if (d->engine->hasVideo())
//			d->stack->setCurrentWidget(d->engine->renderer());
//		else
//			d->stack->setCurrentWidget(d->logo);
	}
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

void MainWindow::toggleToolBox() {
	d->tool->setVisible(!d->tool->isVisible());
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
	d->hider.stop();
	if (cursor().shape() == Qt::BlankCursor)
		unsetCursor();
	QMainWindow::mouseMoveEvent(event);
	const bool full = isFullScreen();
	if (full) {
		static const int h = d->control->height();
		QRect rect = this->rect();
		rect.setTop(rect.height() - h);
		d->control->setVisible(rect.contains(event->pos()));
		if (d->pref.hideCursor)
			d->hider.start(d->pref.hideDelay);
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
		if (Info::playlistExtension().contains(suffix)) {
			Playlist list;
			list.load(urls[i].toString());
			playlist += list;
		} else if (Info::subtitleExtension().contains(suffix)) {
			subList << urls[i].toLocalFile();
		} else if (Info::videoExtension().contains(suffix)
				|| Info::audioExtension().contains(suffix)) {
			playlist.append(urls[i].toString());
		}
	}
	if (!playlist.isEmpty()) {
		d->tool->playlist()->append(playlist);
//		d->model->play(d->model->row(playlist.first()));
	} else if (!subList.isEmpty())
		appendSubFiles(subList, true, d->pref.subtitleEncoding);
}

void MainWindow::handleFinished() {
//	d->stack->setCurrentWidget(d->logo);
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
	d->tray->setVisible(d->pref.enableSystemTray);
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
