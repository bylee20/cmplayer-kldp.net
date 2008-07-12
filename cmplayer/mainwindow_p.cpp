#include "mainwindow_p.h"
#include "actioncollection.h"
#include "playmenubar.h"
#include "playlistdock.h"
#include "playlistmodel.h"
#include "recentinfo.h"
#include "helper.h"
#include "selecttitledialog.h"
#include "pref/preferences.h"
#include "pref/general.h"
#include "pref/interface.h"
#include "pref/subtitle.h"
#include <mplayer/utility.h>
#include <mplayer/playengine.h>
#include <mplayer/videooutput.h>
#include <mplayer/subtitleoutput.h>
#include <mplayer/audiooutput.h>
#include <mplayer/informations.h>
#include <mplayer/playlist.h>
#include <mplayer/volumeslider.h>
#include <mplayer/seekslider.h>
#include <mplayer/abrepeater.h>
#include <mplayer/mediainfo.h>
#include <mplayer/dvdinfo.h>
#include <QToolButton>
#include <QDir>
#include <QLayout>
#include <QFileInfo>
#include <QUrl>

MainWindow::Data::Data(MainWindow *p)
: p(p), dragMove(false), repeating(false), pausedByHiding(false), resizedByAct(false)
, changingOnTop(false), staysOnTop(NotStayOnTop), pref(Pref::get())
, engine(new MPlayer::PlayEngine(p)), video(new MPlayer::VideoOutput(p))
, audio(new MPlayer::AudioOutput(p)), subout(new MPlayer::SubtitleOutput(p))
, model(new PlayListModel(engine, p)), info(MPlayer::Informations::get())
, recent(RecentInfo::get()), pmb(new PlayMenuBar(p)), onTopActions(new QActionGroup(p))
, videoSizeActions(new QActionGroup(p)), videoAspectActions(new QActionGroup(p))
, videoCropActions(new QActionGroup(p)), subListActions(new QActionGroup(p))
, seekChapterActions(new QActionGroup(p)), seekTitleActions(new QActionGroup(0))
, dock(new PlayListDock(model, p)), repeater(engine->repeater()) {
	ui.setupUi(p);
}

MainWindow::Data::~Data() {
	checkClose();
}

void MainWindow::Data::init() {
	createConnections();

	ui.video_on_top_only_playing_action->setData(OnlyPlaying);
	ui.video_on_top_off_action->setData(NotStayOnTop);
	ui.video_on_top_always_action->setData(AlwaysOnTop);
	onTopActions->addAction(ui.video_on_top_always_action);
	onTopActions->addAction(ui.video_on_top_only_playing_action);
	onTopActions->addAction(ui.video_on_top_off_action);

	ui.video_size_50_action->setData(0.5);
	ui.video_size_100_action->setData(1.0);
	ui.video_size_150_action->setData(1.5);
	ui.video_size_200_action->setData(2.0);
	ui.video_size_250_action->setData(2.5);
	ui.video_size_300_action->setData(3.0);
	ui.video_size_350_action->setData(3.5);
	ui.video_size_400_action->setData(4.0);
	ui.video_size_user_action->setData(-1.0);
	videoSizeActions->addAction(ui.video_size_50_action);
	videoSizeActions->addAction(ui.video_size_100_action);
	videoSizeActions->addAction(ui.video_size_150_action);
	videoSizeActions->addAction(ui.video_size_200_action);
	videoSizeActions->addAction(ui.video_size_250_action);
	videoSizeActions->addAction(ui.video_size_300_action);
	videoSizeActions->addAction(ui.video_size_350_action);
	videoSizeActions->addAction(ui.video_size_400_action);
	videoSizeActions->addAction(ui.video_size_user_action);

	ui.video_aspect_auto_action->setData(MPlayer::VideoOutput::AspectRatioAuto);
	ui.video_aspect_widget_action->setData(MPlayer::VideoOutput::AspectRatioWidget);
	ui.video_aspect_4_3_action->setData(static_cast<double>(4) / 3);
	ui.video_aspect_16_9_action->setData(static_cast<double>(16) / 9);
	ui.video_aspect_2_35_1_action->setData(2.35 / 1);
	videoAspectActions->addAction(ui.video_aspect_auto_action);
	videoAspectActions->addAction(ui.video_aspect_widget_action);
	videoAspectActions->addAction(ui.video_aspect_4_3_action);
	videoAspectActions->addAction(ui.video_aspect_16_9_action);
	videoAspectActions->addAction(ui.video_aspect_2_35_1_action);

	ui.video_crop_off_action->setData(MPlayer::VideoOutput::CropOff);
	ui.video_crop_4_3_action->setData(static_cast<double>(4) / 3);
	ui.video_crop_16_9_action->setData(static_cast<double>(16) / 9);
	ui.video_crop_2_35_1_action->setData(2.35 / 1);
	videoCropActions->addAction(ui.video_crop_off_action);
	videoCropActions->addAction(ui.video_crop_4_3_action);
	videoCropActions->addAction(ui.video_crop_16_9_action);
	videoCropActions->addAction(ui.video_crop_2_35_1_action);

	subListActions->addAction(ui.subtitle_disable_action);
	subListActions->setExclusive(false);

	model->setPlayList(recent->lastPlayList());
	model->setCurrentRow(model->row(recent->lastSource()));

	setupGUI();

	registerActions();
}

void MainWindow::Data::setupGUI() {
	video->widget()->setContextMenuPolicy(Qt::CustomContextMenu);
	p->setMouseTracking(true);
	p->setAcceptDrops(true);
	p->setCentralWidget(video->widget());
	p->setWindowTitle(trUtf8("CMPlayer") + ' ' + Helper::version());

	QMenuBar *bar = p->menuBar();
	bar->insertSeparator(ui.help_menu->menuAction());
	bar->hide();

	p->addDockWidget(Qt::RightDockWidgetArea, dock);
	dock->hide();

	QList<QWidget *> tools;
#define addToolButton(act) {\
	QToolButton *tb = new QToolButton(pmb); tb->setAutoRaise(true); \
	tb->setFocusPolicy(Qt::NoFocus); tb->setDefaultAction(act); tools.append(tb);}
	addToolButton(ui.play_previous_action);
	addToolButton(ui.play_pause_action);
	addToolButton(ui.play_stop_action);
	addToolButton(ui.play_next_action);
	tools.append(new MPlayer::SeekSlider(engine, pmb));
	addToolButton(ui.audio_mute_action);
	tools.append(new MPlayer::VolumeSlider(audio, pmb));
#undef makeToolButton
	pmb->init(tools);
	ui.play_bar->layout()->setMargin(0);
	ui.play_bar->layout()->setSpacing(0);
	ui.play_bar->addWidget(pmb);

	p->resize(300, 200);
}

void MainWindow::Data::createConnections() {
	connect(ui.open_file_action, SIGNAL(triggered()), p, SLOT(openFile()));
	connect(ui.open_dvd_action, SIGNAL(triggered()), p, SLOT(openDVD()));
	connect(ui.recent_clear_action, SIGNAL(triggered()), p, SLOT(clearRecentFiles()));
	connect(ui.file_exit_action, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(ui.play_show_playlist_action, SIGNAL(triggered()), p, SLOT(togglePlayListVisibility()));
	connect(ui.play_pause_action, SIGNAL(triggered()), p, SLOT(playPause()));
	connect(ui.play_stop_action, SIGNAL(triggered()), engine, SLOT(stop()));
	connect(ui.play_previous_action, SIGNAL(triggered()), model, SLOT(playPrevious()));
	connect(ui.play_next_action, SIGNAL(triggered()), model, SLOT(playNext()));
	connect(ui.play_forward_action, SIGNAL(triggered()), p, SLOT(forward()));
	connect(ui.play_forward_more_action, SIGNAL(triggered()), p, SLOT(forwardMore()));
	connect(ui.play_forward_much_more_action, SIGNAL(triggered()), p, SLOT(forwardMuchMore()));
	connect(ui.play_backward_action, SIGNAL(triggered()), p, SLOT(backward()));
	connect(ui.play_backward_more_action, SIGNAL(triggered()), p, SLOT(backwardMore()));
	connect(ui.play_backward_much_more_action, SIGNAL(triggered()), p, SLOT(backwardMuchMore()));
	connect(ui.play_ab_select_action, SIGNAL(triggered()), p, SLOT(selectABSection()));
	connect(ui.play_ab_stop_action, SIGNAL(triggered()), engine->repeater(), SLOT(stop()));
	connect(ui.play_ab_advance_action, SIGNAL(triggered()), p, SLOT(showABRepeatDialog()));
	connect(ui.play_speed_restore_action, SIGNAL(triggered()), p, SLOT(restoreSpeed()));
	connect(ui.play_speed_increase_action, SIGNAL(triggered()), p, SLOT(increaseSpeed()));
	connect(ui.play_speed_decrease_action, SIGNAL(triggered()), p, SLOT(decreaseSpeed()));
	connect(ui.play_speed_double_action, SIGNAL(triggered()), p, SLOT(doubleSpeed()));
	connect(ui.play_speed_half_action, SIGNAL(triggered()), p, SLOT(halfSpeed()));
	connect(seekTitleActions, SIGNAL(triggered(QAction*)), p, SLOT(seekDVDTitle(QAction*)));
	connect(seekChapterActions, SIGNAL(triggered(QAction*)), p, SLOT(seekDVDChapter(QAction*)));

	connect(ui.video_fullscreen_action, SIGNAL(toggled(bool)), p, SLOT(setFullScreen(bool)));
	connect(onTopActions, SIGNAL(triggered(QAction*)), p, SLOT(changeStaysOnTop(QAction*)));
	connect(videoSizeActions, SIGNAL(triggered(QAction*)), p, SLOT(changeVideoSize(QAction*)));
	connect(videoAspectActions, SIGNAL(triggered(QAction*)), p, SLOT(changeAspectRatio(QAction*)));
	connect(videoCropActions, SIGNAL(triggered(QAction *)), p, SLOT(crop(QAction *)));
	connect(ui.video_equalizer_action, SIGNAL(triggered()), p, SLOT(showEqualizer()));

	connect(ui.audio_volume_up_action, SIGNAL(triggered()), p, SLOT(increaseVolume()));
	connect(ui.audio_volume_down_action, SIGNAL(triggered()), p, SLOT(decreaseVolume()));
	connect(ui.audio_mute_action, SIGNAL(toggled(bool)), audio, SLOT(setMuted(bool)));

	connect(subListActions, SIGNAL(triggered(QAction*)), p, SLOT(changeCurrentSubtitles(QAction*)));
	connect(ui.subtitle_clear_action, SIGNAL(triggered()), subout, SLOT(clear()));
	connect(ui.subtitle_add_action, SIGNAL(triggered()), p, SLOT(addSubtitles()));
	connect(ui.subtitle_step_up_action, SIGNAL(triggered()), subout, SLOT(moveUp()));
	connect(ui.subtitle_step_down_action, SIGNAL(triggered()), subout, SLOT(moveDown()));
	connect(ui.subtitle_sync_increase_action, SIGNAL(triggered()), p, SLOT(increaseSyncDelay()));
	connect(ui.subtitle_sync_decrease_action, SIGNAL(triggered()), p, SLOT(decreaseSyncDelay()));

//	connect(ui.option_wizard_action, SIGNAL(triggered()), p, SLOT(executeWizard()));
	connect(ui.option_preferences_action, SIGNAL(triggered()), p, SLOT(showPreferencesDialog()));

	connect(ui.about_qt_action, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.about_cmp_action, SIGNAL(triggered()), p, SLOT(showAboutDialog()));

	MPlayer::link(engine, video);
	MPlayer::link(engine, audio);
	MPlayer::link(engine, subout);

	connect(engine, SIGNAL(totalTimeChanged(qint64)), pmb, SLOT(setTotalTime(qint64)));
	connect(engine, SIGNAL(tick(qint64)), pmb, SLOT(setCurrentTime(qint64)));
	connect(engine, SIGNAL(stateChanged(MPlayer::State, MPlayer::State)),
			p, SLOT(slotStateChanged(MPlayer::State)));
	connect(engine, SIGNAL(started()), p, SLOT(slotStarted()));
	connect(engine, SIGNAL(speedChanged(double)), p, SLOT(updateSpeed(double)));
	connect(engine, SIGNAL(aboutToFinished()), p, SLOT(updateFinished()));
	connect(engine, SIGNAL(stopped(qint64)), p, SLOT(updateStopped(qint64)));

	connect(model, SIGNAL(currentRowChanged(int)), p, SLOT(updatePlayText()));
	connect(model, SIGNAL(rowCountChanged(int)), p, SLOT(updatePlayText()));

	connect(video, SIGNAL(widgetResized(const QSize&)), p, SLOT(slotResized()));
	connect(video, SIGNAL(widgetSizeHintChanged(const QSize&)), ui.video_size_100_action, SLOT(trigger()));
	connect(video->widget(), SIGNAL(customContextMenuRequested(const QPoint&)),
			p, SLOT(showMenu(const QPoint&)));

	connect(audio, SIGNAL(mutedChanged(bool)), ui.audio_mute_action, SLOT(setChecked(bool)));

	connect(subout, SIGNAL(syncDelayChanged(int)), p, SLOT(updateSyncDelay(int)));
	connect(subout, SIGNAL(availableSubtitlesChanged(const QStringList&)),
			p, SLOT(updateSubtitles(const QStringList&)));
	connect(subout, SIGNAL(selectedSubtitlesChanged(const QList<int>&)),
			p, SLOT(updateCurrentSubtitleIndexes(const QList<int>&)));

	connect(recent, SIGNAL(sourcesChanged(const RecentStack&)), p, SLOT(updateRecentActions(const RecentStack&)));
	connect(recent, SIGNAL(rememberCountChanged(int)), p, SLOT(updateRecentSize(int)));

	connect(dock, SIGNAL(visibilityChanged(bool)), p, SLOT(adjustSizeForDock(bool)));
}


void MainWindow::Data::registerActions() {
	mouseClickActions[Pref::Interface::ToggleFullScreen] = ui.video_fullscreen_action;
	mouseClickActions[Pref::Interface::TogglePlayPause] = ui.play_pause_action;
	wheelScrollActions[Pref::Interface::ForwardBackward]
			= qMakePair(ui.play_forward_action, ui.play_backward_action);
	wheelScrollActions[Pref::Interface::ForwardBackwardMore]
			= qMakePair(ui.play_forward_more_action, ui.play_backward_more_action);
	wheelScrollActions[Pref::Interface::ForwardBackwardMuchMore]
			= qMakePair(ui.play_forward_much_more_action, ui.play_backward_much_more_action);
	wheelScrollActions[Pref::Interface::VolumeUpDown]
			= qMakePair(ui.audio_volume_up_action, ui.audio_volume_down_action);

	ActionCollection *ac = ActionCollection::get();
	QList<QAction *> acts = p->menuBar()->actions();
	ac->addActions(acts);
	QMenu *wholeMenu = new QMenu(p);
	wholeMenu->addActions(acts);
	ac->setWholeMenu(wholeMenu);
	p->addActions(ac->actions());

	contextMenu = new QMenu(p);
	contextMenu->addMenu(ui.file_open_menu);
	contextMenu->addSeparator();
	contextMenu->addMenu(ui.video_on_top_menu);
	contextMenu->addMenu(ui.video_size_menu);
	contextMenu->addMenu(ui.video_aspect_menu);
	contextMenu->addMenu(ui.video_crop_menu);
	contextMenu->addAction(ui.video_equalizer_action);
	contextMenu->addSeparator();
	seekDVDAction = contextMenu->addMenu(ui.seek_dvd_menu);
	seekDVDAction->setVisible(false);
	contextMenu->addAction(ui.play_show_playlist_action);
	contextMenu->addMenu(ui.play_speed_menu);
	contextMenu->addMenu(ui.play_ab_menu);
	contextMenu->addSeparator();
	contextMenu->addMenu(ui.subtitle_select_menu);
	contextMenu->addAction(ui.subtitle_add_action);
	contextMenu->addMenu(ui.subtitle_pos_menu);
	contextMenu->addMenu(ui.subtitle_sync_menu);
	contextMenu->addSeparator();
	contextMenu->addAction(ui.option_preferences_action);
//		contextMenu->addAction(ui.option_wizard_action);
	contextMenu->addSeparator();
	contextMenu->addAction(ui.about_qt_action);
	contextMenu->addAction(ui.about_cmp_action);
	contextMenu->addSeparator();
	contextMenu->addAction(ui.file_exit_action);

	const QMap<QString, QKeySequence> &keys = Pref::get()->interface().shortcuts;
	for (QMap<QString, QKeySequence>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
		QAction *act = ac->action(it.key());
		if (act)
			act->setShortcut(it.value());
	}
	p->updateRecentActions(recent->sources());
}

void MainWindow::Data::updateStaysOnTop() {
	if (p->isFullScreen() || p->isHidden())
		return;
	Qt::WindowFlags f = p->windowFlags();
	QPoint oldPos = p->pos();
	bool wasOnTop =  f & Qt::WindowStaysOnTopHint;
	bool wasVisible = p->isVisible();
	bool wasActivated = p->isActiveWindow();
	bool isOnTop = staysOnTop == AlwaysOnTop || (staysOnTop == OnlyPlaying && engine->isPlaying());
	if (wasOnTop != isOnTop) {
		changingOnTop = true;
		if (isOnTop)
			f |= Qt::WindowStaysOnTopHint;
		else
			f &= ~Qt::WindowStaysOnTopHint;
		p->setWindowFlags(f);
		p->setVisible(wasVisible);
		if (wasActivated)
			p->activateWindow();
		p->move(oldPos);
		changingOnTop = false;
	}
}

QMenu *MainWindow::Data::findMenuIn(QMenu *menu, const QString &title) {
	QList<QAction*> acts = menu->actions();
	for (int i=0; i<acts.size(); ++i) {
		if (acts[i]->menu()) {
			if (acts[i]->text() != title) {
				QMenu *m = findMenuIn(acts[i]->menu(), title);
				if (m)
				return m;
			} else
				return acts[i]->menu();
		}
	}
	return 0;
}
void MainWindow::Data::initSubtitles() {
	const Pref::Subtitle &subtitle = pref->subtitle();
	QStringList files;
	const MPlayer::MediaSource source = engine->currentSource();
	if (subtitle.autoLoad != Pref::Subtitle::NoAutoLoad && source.isLocalFile()) {
		static const QStringList NameFilter = info->subtitleFilter().toNameFilter();
		QFileInfo file(source.filePath());
		QDir dir = file.dir();
		QFileInfoList all = dir.entryInfoList(NameFilter, QDir::Files, QDir::Name);
		QString base = file.baseName();
		for (int i=0; i<all.size(); ++i) {
			bool add = subtitle.autoLoad == Pref::Subtitle::SamePath;
			if (subtitle.autoLoad == Pref::Subtitle::Matched)
				add = base == QFileInfo(all[i]).baseName();
			else
				add = all[i].fileName().contains(base);
			if (add)
				files.append(all[i].absoluteFilePath());
		}
		subout->load(files);
	} else if (source.isDisc())
		subout->loadDisc();
}

void MainWindow::Data::checkClose() {
	static bool done = false;
	if (done)
		return;
	engine->stop();
	recent->setLastPlayList(model->playList());
	recent->setLastSource(engine->currentSource());
	recent->save();
	done = true;
}

void MainWindow::Data::clearSeekDVDMenu() {
	QList<QAction*> acts = seekTitleActions->actions();
	for (int i=0; i<acts.size(); ++i) {
		seekTitleActions->removeAction(acts[i]);
		delete acts[i];
	}
	acts = seekChapterActions->actions();
	for (int i=0; i<acts.size(); ++i) {
		seekChapterActions->removeAction(acts[i]);
		delete acts[i];
	}
	seekDVDAction->setVisible(false);
}

void MainWindow::Data::resetSeekDVDMenu() {
	clearSeekDVDMenu();
	MPlayer::MediaSource source = engine->currentSource();
	if (source.isDisc() && source.gotInfo()) {
		for (QVector<MPlayer::DVDInfo::Title>::const_iterator it = source.info().dvd().titles.begin()
				; it != source.info().dvd().titles.end(); ++it) {
			QAction *act = ui.seek_title_menu->addAction(trUtf8("타이틀 %1").arg(it->number));
			act->setData(it->number);
			seekTitleActions->addAction(act);
		}
		seekDVDAction->setVisible(true);
		int title = source.titleNumber();
		if (title < 1)
			return;
		const QVector<qint64> &chapters = source.info().dvd().titles[title-1].chapters;
		for (int i=0; i<chapters.size(); ++i) {
			QAction *act = ui.seek_chapter_menu->addAction(QString::number(i+1)
					+ ' ' + MPlayer::Utility::msecsToString(chapters[i]));
			act->setData(i+1);
			seekChapterActions->addAction(act);
		}
	}
}

void MainWindow::Data::open(MPlayer::MediaSource source) {
	engine->stop();
	if (source.isLocalFile() || source.isDisc())
		source.getInfo();
	MPlayer::PlayList list;
	const Pref::General &general = pref->general();
	int idx = 0;
	if (source.isLocalFile() && general.autoAddFiles != Pref::General::DoNotAddFiles) {
		static const QStringList NameFilter = info->videoExtensions().toNameFilter()
				+ info->audioExtensions().toNameFilter();
		const QFileInfo file(source.filePath());
		QString fileName = file.fileName();
		QFileInfoList files=file.dir().entryInfoList(NameFilter, QDir::Files, QDir::Name);
		bool all = general.autoAddFiles == Pref::General::AllFiles;
		bool prefix = false, suffix = false;
		for(QFileInfoList::const_iterator it = files.begin(); it != files.end(); ++it) {
			if (!all) {
				static QRegExp rxs("(\\D*)\\d+(.*)");
				static QRegExp rxt("(\\D*)\\d+(.*)");
				if (rxs.indexIn(fileName) == -1 || rxt.indexIn(it->fileName()) == -1)
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
			list.append(MPlayer::MediaSource(it->absoluteFilePath()));
		}
		idx = list.indexOf(source);
	} else if (source.isDisc() && source.getInfo()) {
		const MPlayer::DVDInfo &dvd = source.info().dvd();
		if (dvd.titles.isEmpty())
			return;
		if (dvd.titles.size() > 1) {
			SelectTitleDialog dlg(dvd, p);
			if (!dlg.exec())
				return;
			idx = dlg.titleNumber() - 1;
		}
		if (general.addAllDVDTitles) {
			for (int i=0; i<dvd.titles.size(); ++i)
				list.append(MPlayer::MediaSource(QUrl("dvd://" + QString::number(i+1))));
		} else
			list.append(MPlayer::MediaSource(QUrl("dvd://" + QString::number(idx+1))));
	} else
		list.append(source);
	model->setPlayList(list);
	model->play(idx);
	recent->stackSource(source);
}

