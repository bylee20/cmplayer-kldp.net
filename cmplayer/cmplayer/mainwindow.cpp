#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playmenubar.h"
#include "playlistmodel.h"
#include "equalizerdialog.h"
#include "abrepeatdialog.h"
#include "playlistdock.h"
#include "recentinfo.h"
#include "encodingfiledialog.h"
#include "actioncollection.h"
#include "helper.h"
#include "ui_aboutdialog.h"
#include "pref/general.h"
#include "pref/interface.h"
#include "pref/subtitle.h"
#include "pref/backend.h"
#include "pref/preferencesdialog.h"
#include "pref/preferences.h"
#include <backend/backendobject.h>
#include <backend/playengine.h>
#include <backend/audiooutput.h>
#include <backend/videooutput.h>
#include <backend/subtitleoutput.h>
#include <backend/mediasource.h>
#include <backend/abrepeater.h>
#include <backend/manager.h>
#include <backend/playlist.h>
#include <backend/volumeslider.h>
#include <backend/seekslider.h>
#include <backend/info.h>
#include <cmath>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtGui/QCloseEvent>
#include <QtCore/QFileInfo>

struct MainWindow::Data {
	Data(MainWindow *parent)
	: onTopActions(0), videoSizeActions(0), videoAspectActions(0)
	, videoCropActions(0), subListActions(0), subChannelsActions(0), audioTrackActions(0) {
		ui.setupUi(parent);
		repeating = pausedByHiding = resizedByAct = changingOnTop = closed = false;
		staysOnTop = NotStayOnTop;
		pref = Pref::get();
		backend = 0;
		engine = 0;
		audio = 0;
		video = 0;
		subtitle = 0;
		repeater = 0;
		model = new PlayListModel();
		recent = RecentInfo::get();
		QList<QAction*> acts;
		acts << ui.play_previous_action << ui.play_pause_action << ui.play_stop_action
				<< ui.play_next_action << 0 << ui.audio_mute_action << 0;
		pmb = new PlayMenuBar(acts, parent);
		dock = new PlayListDock(model, parent);
		contextMenu = new QMenu(parent);

	}
	~Data() {
		delete model;
		delete engine;
	}
	Ui::MainWindow ui;
	bool repeating, pausedByHiding, resizedByAct, changingOnTop, closed;
	StaysOnTop staysOnTop;
	QPoint dragPos;
	Pref::Preferences *pref;
	Backend::BackendObject *backend;
	Backend::PlayEngine *engine;
	Backend::AudioOutput *audio;
	Backend::VideoOutput *video;
	Backend::SubtitleOutput *subtitle;
	Backend::ABRepeater *repeater;
	PlayListModel *model;
	RecentInfo *recent;
	PlayMenuBar *pmb;
	PlayListDock *dock;
	QActionGroup onTopActions, videoSizeActions, videoAspectActions;
	QActionGroup videoCropActions, subListActions, subChannelsActions;
	QActionGroup audioTrackActions;
	QList<QAction *> recentActions;
	QMenu *contextMenu;
	QMap<int, QAction*> mouseClickActions;
	QMap<int, QPair<QAction*, QAction*> > wheelScrollActions;
// 	QAction *seekDVDAction;
	QTimer cursorTimer;
};

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent) {
	d = new Data(this);
	initGui();
	initIface();
	
	connect(d->ui.open_file_action, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(d->ui.open_dvd_action, SIGNAL(triggered()), this, SLOT(openDVD()));
	connect(d->ui.recent_clear_action, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));
	connect(d->ui.file_exit_action, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(d->ui.play_show_playlist_action, SIGNAL(triggered()), this, SLOT(togglePlayListVisibility()));
	connect(d->ui.play_pause_action, SIGNAL(triggered()), this, SLOT(playPause()));
	
	connect(d->ui.play_previous_action, SIGNAL(triggered()), d->model, SLOT(playPrevious()));
	connect(d->ui.play_next_action, SIGNAL(triggered()), d->model, SLOT(playNext()));
	connect(d->ui.play_forward_action, SIGNAL(triggered()), this, SLOT(forward()));
	connect(d->ui.play_forward_more_action, SIGNAL(triggered()), this, SLOT(forwardMore()));
	connect(d->ui.play_forward_much_more_action, SIGNAL(triggered()), this, SLOT(forwardMuchMore()));
	connect(d->ui.play_backward_action, SIGNAL(triggered()), this, SLOT(backward()));
	connect(d->ui.play_backward_more_action, SIGNAL(triggered()), this, SLOT(backwardMore()));
	connect(d->ui.play_backward_much_more_action, SIGNAL(triggered()), this, SLOT(backwardMuchMore()));
	connect(d->ui.play_ab_select_action, SIGNAL(triggered()), this, SLOT(selectABSection()));
	
	connect(d->ui.play_ab_advance_action, SIGNAL(triggered()), this, SLOT(showABRepeatDialog()));
	connect(d->ui.play_speed_restore_action, SIGNAL(triggered()), this, SLOT(restoreSpeed()));
	connect(d->ui.play_speed_increase_action, SIGNAL(triggered()), this, SLOT(increaseSpeed()));
	connect(d->ui.play_speed_decrease_action, SIGNAL(triggered()), this, SLOT(decreaseSpeed()));
	connect(d->ui.play_speed_double_action, SIGNAL(triggered()), this, SLOT(doubleSpeed()));
	connect(d->ui.play_speed_half_action, SIGNAL(triggered()), this, SLOT(halfSpeed()));

	connect(d->ui.video_fullscreen_action, SIGNAL(triggered()), this, SLOT(slotFullScreen()));
	connect(&d->onTopActions, SIGNAL(triggered(QAction*)), this, SLOT(changeStaysOnTop(QAction*)));
	connect(&d->videoSizeActions, SIGNAL(triggered(QAction*)), this, SLOT(changeVideoSize(QAction*)));
	connect(&d->videoAspectActions, SIGNAL(triggered(QAction*)), this, SLOT(changeAspectRatio(QAction*)));
	connect(&d->videoCropActions, SIGNAL(triggered(QAction *)), this, SLOT(crop(QAction *)));
	connect(d->ui.video_equalizer_action, SIGNAL(triggered()), this, SLOT(showEqualizer()));

	connect(d->ui.audio_volume_up_action, SIGNAL(triggered()), this, SLOT(increaseVolume()));
	connect(d->ui.audio_volume_down_action, SIGNAL(triggered()), this, SLOT(decreaseVolume()));
	connect(&d->audioTrackActions, SIGNAL(triggered(QAction*)), this, SLOT(changeCurrentAuidoTrack(QAction*)));

	connect(&d->subChannelsActions, SIGNAL(triggered(QAction*)), this, SLOT(changeCurrentSubChannel(QAction*)));
	connect(&d->subListActions, SIGNAL(triggered(QAction*)), this, SLOT(changeCurrentSubtitles(QAction*)));
	connect(d->ui.subtitle_add_action, SIGNAL(triggered()), this, SLOT(addSubtitles()));
	connect(d->ui.subtitle_sync_increase_action, SIGNAL(triggered()), this, SLOT(increaseSyncDelay()));
	connect(d->ui.subtitle_sync_decrease_action, SIGNAL(triggered()), this, SLOT(decreaseSyncDelay()));

//	connect(d->ui.option_wizard_action, SIGNAL(triggered()), this, SLOT(executeWizard()));
	connect(d->ui.option_preferences_action, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

	connect(d->ui.about_qt_action, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(d->ui.about_cmp_action, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
	connect(d->ui.dummy, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));
	
	connect(d->model, SIGNAL(currentRowChanged(int)), this, SLOT(updatePlayText()));
	connect(d->model, SIGNAL(rowCountChanged(int)), this, SLOT(updatePlayText()));
	connect(d->recent, SIGNAL(sourcesChanged(const RecentStack&)), this, SLOT(updateRecentActions(const RecentStack&)));
	connect(d->recent, SIGNAL(rememberCountChanged(int)), this, SLOT(updateRecentSize(int)));

	connect(d->dock, SIGNAL(visibilityChanged(bool)), this, SLOT(adjustSizeForDock(bool)));

	connect(&d->cursorTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));
	
	setBackendActionsEnabled(false);
	d->ui.video_on_top_only_playing_action->setData(OnlyPlaying);
	d->ui.video_on_top_off_action->setData(NotStayOnTop);
	d->ui.video_on_top_always_action->setData(AlwaysOnTop);
	d->onTopActions.addAction(d->ui.video_on_top_always_action);
	d->onTopActions.addAction(d->ui.video_on_top_only_playing_action);
	d->onTopActions.addAction(d->ui.video_on_top_off_action);

	d->ui.video_size_50_action->setData(0.5);
	d->ui.video_size_100_action->setData(1.0);
	d->ui.video_size_150_action->setData(1.5);
	d->ui.video_size_200_action->setData(2.0);
	d->ui.video_size_250_action->setData(2.5);
	d->ui.video_size_300_action->setData(3.0);
	d->ui.video_size_350_action->setData(3.5);
	d->ui.video_size_400_action->setData(4.0);
	d->videoSizeActions.addAction(d->ui.video_size_50_action);
	d->videoSizeActions.addAction(d->ui.video_size_100_action);
	d->videoSizeActions.addAction(d->ui.video_size_150_action);
	d->videoSizeActions.addAction(d->ui.video_size_200_action);
	d->videoSizeActions.addAction(d->ui.video_size_250_action);
	d->videoSizeActions.addAction(d->ui.video_size_300_action);
	d->videoSizeActions.addAction(d->ui.video_size_350_action);
	d->videoSizeActions.addAction(d->ui.video_size_400_action);

	d->ui.video_aspect_auto_action->setData(Backend::VideoOutput::AspectRatioAuto);
	d->ui.video_aspect_4_3_action->setData(4.0/3.0);
	d->ui.video_aspect_16_9_action->setData(16.0/9.0);
	d->ui.video_aspect_211_100_action->setData(2.11/1.0);
	d->videoAspectActions.addAction(d->ui.video_aspect_auto_action);
	d->videoAspectActions.addAction(d->ui.video_aspect_4_3_action);
	d->videoAspectActions.addAction(d->ui.video_aspect_16_9_action);
	d->videoAspectActions.addAction(d->ui.video_aspect_211_100_action);

	d->ui.video_crop_off_action->setData(Backend::VideoOutput::CropOff);
	d->ui.video_crop_4_3_action->setData(4.0/3.0);
	d->ui.video_crop_16_9_action->setData(16.0/9.0);
	d->ui.video_crop_2_35_1_action->setData(2.35/1.0);
	d->videoCropActions.addAction(d->ui.video_crop_off_action);
	d->videoCropActions.addAction(d->ui.video_crop_4_3_action);
	d->videoCropActions.addAction(d->ui.video_crop_16_9_action);
	d->videoCropActions.addAction(d->ui.video_crop_2_35_1_action);

	d->ui.sub_channel_off_action->setData(-1);
	d->subChannelsActions.addAction(d->ui.sub_channel_off_action);
	d->subChannelsActions.setExclusive(true);
	d->subListActions.addAction(d->ui.subtitle_disable_action);
	d->subListActions.setExclusive(false);

// 	d->
	d->model->setPlayList(d->recent->lastPlayList());
	d->model->setCurrentRow(d->model->row(d->recent->lastSource()));

	d->cursorTimer.setSingleShot(true);
	d->cursorTimer.setInterval(5000);

	QStringList args = QApplication::arguments();
	if (args.size() > 1) {
		QUrl url(args.last());
		open(Backend::MediaSource(url.scheme().isEmpty() ? args.last() : url));
	}
	updatePreferences();
}

MainWindow::~MainWindow() {
	delete d;
}

void MainWindow::setBackendActionsEnabled(bool enabled) {
	QList<QAction*> acts = ActionCollection::get()->actions();
	for (int i=0; i<acts.size(); ++i)
		acts[i]->setEnabled(enabled);
	if (!enabled) {
		d->ui.file_exit_action->setEnabled(true);
		d->ui.play_show_playlist_action->setEnabled(true);
		d->ui.video_fullscreen_action->setEnabled(true);
// 		connect(d->ui.option_wizard_action, SIGNAL(triggered()), this, SLOT(executeWizard()));
		d->ui.option_preferences_action->setEnabled(true);
		d->ui.about_qt_action->setEnabled(true);
		d->ui.about_cmp_action->setEnabled(true);
	}
	d->ui.video_on_top_menu->setEnabled(!isFullScreen());
	
}

void MainWindow::setBackend(Backend::BackendObject *backend) {
	if (d->backend == backend)
		return;
	if (d->engine) {
		d->engine->stop();
		d->ui.center->removeWidget(d->video->widget());
		delete d->engine;
	}
	d->backend = backend;
	setBackendActionsEnabled(d->backend);
	if (!d->backend) {
		d->engine = 0;
		d->video = 0;
		d->audio = 0;
		d->repeating = 0;
		d->subtitle = 0;
		return;
	}
	
	d->engine = backend->createPlayEngine();
	connect(d->ui.play_dvd_menu_action, SIGNAL(triggered()), d->engine, SLOT(toggleDvdMenu()));
	connect(d->ui.play_stop_action, SIGNAL(triggered()), d->engine, SLOT(stop()));
	connect(d->engine, SIGNAL(stateChanged(Backend::State, Backend::State))
			, this, SLOT(slotStateChanged(Backend::State)));
	connect(d->engine, SIGNAL(started()), this, SLOT(slotStarted()));
	connect(d->engine, SIGNAL(speedChanged(double)), this, SLOT(updateSpeed(double)));
	connect(d->engine, SIGNAL(currentSourceChanged(const Backend::MediaSource&))
			, this, SLOT(updateSource(const Backend::MediaSource&)));
	connect(d->engine, SIGNAL(finished(Backend::MediaSource))
			, this, SLOT(updateFinished(const Backend::MediaSource&)));
	connect(d->engine, SIGNAL(stopped(Backend::MediaSource, int))
			, this, SLOT(updateStopped(const Backend::MediaSource&, int)));

	d->audio = d->engine->audio();
	connect(d->ui.audio_mute_action, SIGNAL(toggled(bool)), d->audio, SLOT(setMuted(bool)));
	connect(d->audio, SIGNAL(mutedChanged(bool)), d->ui.audio_mute_action, SLOT(setChecked(bool)));
	connect(d->audio, SIGNAL(availableTracksChanged(const QStringList&))
			, this, SLOT(updateAudioTracks(const QStringList&)));
	connect(d->audio, SIGNAL(currentTrackChanged(int)), this, SLOT(updateCurrentAudioTrack(int)));
		
	d->video = d->engine->video();
	connect(d->video, SIGNAL(widgetResized(const QSize&)), this, SLOT(slotResized()));
	connect(d->video, SIGNAL(widgetSizeHintChanged(const QSize&)), d->ui.video_size_100_action, SLOT(trigger()));
	
	QWidget *widget = d->video->widget();
	widget->setContextMenuPolicy(Qt::CustomContextMenu);
	d->ui.center->addWidget(widget);
	d->ui.center->setCurrentWidget(widget);
	connect(widget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));

	d->subtitle = d->engine->subtitle();
	connect(d->ui.subtitle_clear_action, SIGNAL(triggered()), d->subtitle, SLOT(clear()));
	connect(d->ui.subtitle_step_up_action, SIGNAL(triggered()), d->subtitle, SLOT(moveUp()));
	connect(d->ui.subtitle_step_down_action, SIGNAL(triggered()), d->subtitle, SLOT(moveDown()));
	connect(d->subtitle, SIGNAL(syncDelayChanged(int)), this, SLOT(updateSyncDelay(int)));
	connect(d->subtitle, SIGNAL(currentChannelChanged(int)), this, SLOT(updateCurrentSubChannel(int)));
	connect(d->subtitle, SIGNAL(availableChannelsChanged(const QStringList&))
			, this, SLOT(updateSubChannels(const QStringList&)));
	connect(d->subtitle, SIGNAL(availableSubtitlesChanged(const QStringList&))
			, this, SLOT(updateSubtitles(const QStringList&)));
	connect(d->subtitle, SIGNAL(selectedSubtitlesChanged(const QList<int>&))
			, this, SLOT(updateCurrentSubtitleIndexes(const QList<int>&)));

	d->repeater = d->engine->repeater();
	connect(d->ui.play_ab_stop_action, SIGNAL(triggered()), d->repeater, SLOT(stop()));
	
	d->pmb->setPlayEngine(d->engine);
	d->model->setPlayEngine(d->engine);
}

void MainWindow::initGui() {
	setWindowTitle(trUtf8("CMPlayer") + ' ' + Helper::version());
	setMouseTracking(true);
	setAcceptDrops(true);
	menuBar()->hide();
	addDockWidget(Qt::RightDockWidgetArea, d->dock);
	d->dock->hide();
	d->ui.play_bar->layout()->setMargin(0);
	d->ui.play_bar->layout()->setSpacing(0);
	d->ui.play_bar->addWidget(d->pmb);

	d->ui.play_dvd_menu_action->setVisible(false);
	d->ui.sub_channel_menu->menuAction()->setVisible(false);

	resize(300, 200);
}

void MainWindow::initIface() {
	d->mouseClickActions[Pref::Interface::ToggleFullScreen] = d->ui.video_fullscreen_action;
	d->mouseClickActions[Pref::Interface::TogglePlayPause] = d->ui.play_pause_action;
	
	d->wheelScrollActions[Pref::Interface::ForwardBackward]
			= qMakePair(d->ui.play_forward_action, d->ui.play_backward_action);
	d->wheelScrollActions[Pref::Interface::ForwardBackwardMore]
			= qMakePair(d->ui.play_forward_more_action, d->ui.play_backward_more_action);
	d->wheelScrollActions[Pref::Interface::ForwardBackwardMuchMore]
			= qMakePair(d->ui.play_forward_much_more_action, d->ui.play_backward_much_more_action);
	d->wheelScrollActions[Pref::Interface::VolumeUpDown]
			= qMakePair(d->ui.audio_volume_up_action, d->ui.audio_volume_down_action);
	
	d->contextMenu->addMenu(d->ui.file_open_menu);
	d->contextMenu->addSeparator();
	d->contextMenu->addMenu(d->ui.video_on_top_menu);
	d->contextMenu->addAction(d->ui.video_fullscreen_action);
	d->contextMenu->addMenu(d->ui.video_size_menu);
	d->contextMenu->addMenu(d->ui.video_aspect_menu);
	d->contextMenu->addMenu(d->ui.video_crop_menu);
	d->contextMenu->addAction(d->ui.video_equalizer_action);
	d->contextMenu->addSeparator();
	d->contextMenu->addAction(d->ui.play_dvd_menu_action);
	d->contextMenu->addAction(d->ui.play_show_playlist_action);
	d->contextMenu->addMenu(d->ui.play_speed_menu);
	d->contextMenu->addMenu(d->ui.play_ab_menu);
	d->contextMenu->addSeparator();
	d->contextMenu->addMenu(d->ui.sub_channel_menu);
	d->contextMenu->addMenu(d->ui.subtitle_select_menu);
	d->contextMenu->addMenu(d->ui.subtitle_pos_menu);
	d->contextMenu->addMenu(d->ui.subtitle_sync_menu);
	d->contextMenu->addSeparator();
	d->contextMenu->addAction(d->ui.option_preferences_action);
// 	d->contextMenu->addAction(d->ui.option_wizard_action);
	d->contextMenu->addSeparator();
	d->contextMenu->addAction(d->ui.about_qt_action);
	d->contextMenu->addAction(d->ui.about_cmp_action);
	d->contextMenu->addSeparator();
	d->contextMenu->addAction(d->ui.file_exit_action);
	updateRecentActions(d->recent->sources());
		
	ActionCollection *ac = ActionCollection::get();
	QList<QAction *> acts = menuBar()->actions();
	ac->addActions(acts);
	QMenu *wholeMenu = new QMenu(this);
	wholeMenu->addActions(acts);
	ac->setWholeMenu(wholeMenu);
	
	addActions(ac->actions());
	const Pref::Interface::KeyMap &keys = Pref::get()->interface().shortcuts();
	for (Pref::Interface::KeyMap::const_iterator it = keys.begin(); it != keys.end(); ++it) {
		QAction *act = ac->action(it.key());
		if (act)
			act->setShortcut(it.value());
	}
}

void MainWindow::updateStaysOnTop() {
	if (isFullScreen() || isHidden())
		return;
	Qt::WindowFlags f = windowFlags();
	QPoint oldPos = pos();
	bool wasOnTop =  f & Qt::WindowStaysOnTopHint;
	bool wasVisible = isVisible();
	bool wasActivated = isActiveWindow();
	bool isOnTop = (d->staysOnTop == AlwaysOnTop || (d->staysOnTop == OnlyPlaying && d->engine->isPlaying()));
	if (wasOnTop != isOnTop) {
		d->changingOnTop = true;
		if (isOnTop)
			f |= Qt::WindowStaysOnTopHint;
		else
			f &= ~Qt::WindowStaysOnTopHint;
		setWindowFlags(f);
		setVisible(wasVisible);
		if (wasActivated)
			activateWindow();
		move(oldPos);
		d->changingOnTop = false;
	}
}

void MainWindow::initSubtitles() {
	const Pref::Subtitle subtitle = d->pref->subtitle();
	QStringList files;
	const Backend::MediaSource source = d->engine->currentSource();
	if (subtitle.autoLoad() != Pref::Subtitle::NoAutoLoad && source.isLocalFile()) {
		const QStringList filter = d->backend->info()->subtitleExtensions().toNameFilter();
		QFileInfo file(source.filePath());
		QDir dir = file.dir();
		QFileInfoList all = dir.entryInfoList(filter, QDir::Files, QDir::Name);
		QString base = file.completeBaseName();
		for (int i=0; i<all.size(); ++i) {
			bool add = (subtitle.autoLoad() == Pref::Subtitle::SamePath);
			if (subtitle.autoLoad() == Pref::Subtitle::Matched)
				add = (base == QFileInfo(all[i]).completeBaseName());
			else
				add = all[i].fileName().contains(base);
			if (add)
				files.append(all[i].absoluteFilePath());
		}
		d->subtitle->load(files);
	} else
		d->subtitle->clear();
}

void MainWindow::checkClose() {
	if (!d->closed) {
		if (d->engine)
			d->engine->stop();
		d->recent->setLastPlayList(d->model->playList());
		if (d->engine)
			d->recent->setLastSource(d->engine->currentSource());
		qDebug() << d->recent->lastSource().toMrl();
		d->recent->save();
		d->closed = true;
	}
}

void MainWindow::open(const Backend::MediaSource &source) {
	d->video->expand(!d->pref->subtitle().osdStyle().high_quality && !source.isDisc());
	Backend::PlayList list;
	const Pref::General general = d->pref->general();
	if (source.isLocalFile() && general.autoAddFiles() != Pref::General::DoNotAddFiles) {
		const Backend::Info *const info = d->backend->info();
		const QFileInfo file(source.filePath());
		QString fileName = file.fileName();
		const QStringList filter = info->videoExtensions().toNameFilter() + info->audioExtensions().toNameFilter();
		QFileInfoList files = file.dir().entryInfoList(filter, QDir::Files, QDir::Name);
		bool all = (general.autoAddFiles() == Pref::General::AllFiles);
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
			list.append(Backend::MediaSource(it->absoluteFilePath()));
		}
		if (list.isEmpty())
			list.append(source);
	} else
		list.append(source);
	qDebug() << list.indexOf(source) << source.toMrl() << source.isValid();
	d->model->setPlayList(list);
	d->model->play(list.indexOf(source));
	d->recent->stackSource(source);
}

void MainWindow::showMenu(const QPoint &pos) {
	QWidget *widget = qobject_cast<QWidget*>(sender());
	if (widget)
		d->contextMenu->exec(widget->mapToGlobal(pos));
}

void MainWindow::togglePlayListVisibility() {
	d->dock->setVisible(d->dock->isHidden());
}

void MainWindow::updateSpeed(double speed) {
	static QString text = trUtf8("현재: ×%1 배속");
	d->ui.play_speed_current_action->setText(text.arg(speed));
}

void MainWindow::changeStaysOnTop(QAction *act) {
	d->staysOnTop = static_cast<StaysOnTop>(act->data().toInt());
	updateStaysOnTop();
}

void MainWindow::clearRecentFiles() {
	d->recent->clearStack();
}

void MainWindow::updateRecentSize(int size) {
	static ActionCollection *ac = ActionCollection::get();
	while (size != d->recentActions.size()) {
		if (size > d->recentActions.size()) {
			static QAction *sprt = d->ui.open_recent_menu->actions()[0];
			QAction *act = new QAction(d->ui.open_recent_menu);
			connect(act, SIGNAL(triggered()), this, SLOT(openRecent()));
			d->ui.open_recent_menu->insertAction(sprt, act);
			d->recentActions.append(act);
		} else
			delete ac->take(d->recentActions.takeLast());
	}
	for (int i=0; i<d->recentActions.size(); ++i) {
		ac->addAction(trUtf8("최근 연 파일 %1").arg(i+1), ac->take(d->recentActions[i]));
	}
}

void MainWindow::updateRecentActions(const RecentStack &stack) {
	const int count = stack.size();
	if (count != d->recentActions.size())
		updateRecentSize(count);
	for (int i=0; i<count; ++i) {
		QAction *act = d->recentActions[i];
		act->setData(stack[i].url());
		act->setText(stack[i].displayName());
		act->setVisible(stack[i].isValid());
	}
}

void MainWindow::openRecent() {
	QAction *act = qobject_cast<QAction *>(sender());
	if (act)
		open(Backend::MediaSource(act->data().toUrl()));
}

void MainWindow::slotResized() {

}

void MainWindow::updateSyncDelay(int msec) {
	d->ui.subtitle_sync_current_action->setText(trUtf8("현재값:"
			"%1").arg(static_cast<double>(msec)/1000));
}

void MainWindow::crop(QAction *act) {
	d->video->crop(act->data().toDouble());
}

void MainWindow::showPreferencesDialog() {
	Pref::PreferencesDialog dlg(this);
	if (dlg.exec()) {
		Backend::State state = d->engine->state();
		d->engine->stop();
		updatePreferences();
		if (d->backend && state != Backend::StoppedState) {
			d->model->play(d->model->currentRow());
			if (state == Backend::PausedState)
				d->engine->pause();
		}
	}
}

void MainWindow::clearSubtitleList() {
	QList<QAction *> acts = d->subListActions.actions();
	for (int i=0; i<acts.size()-1; ++i) {
		d->subListActions.removeAction(acts[i]);
		delete acts[i];
	}
	d->subListActions.removeAction(d->ui.subtitle_disable_action);
	d->ui.subtitle_disable_action->setVisible(false);
}

void MainWindow::updateCurrentSubtitleIndexes(const QList<int> &indexes) {
	d->ui.subtitle_disable_action->setChecked(!indexes.size());
	QList<QAction *> act = d->subListActions.actions();
	for (int i=0; i<act.size()-1; ++i)
		act[i]->setChecked(indexes.contains(i));
}

void MainWindow::updateSubtitles(const QStringList &names) {
	clearSubtitleList();
	const int count = names.size();
	const QList<int> &indexes = d->subtitle->selectedIndexes();
	for (int i=0; i<count; ++i) {
		QAction *act = d->ui.subtitle_select_menu->addAction(names[i]);
		act->setCheckable(true);
		act->setData(i);
		act->setChecked(indexes.contains(i));
		d->subListActions.addAction(act);
	}
	d->subListActions.addAction(d->ui.subtitle_disable_action);
	d->ui.subtitle_disable_action->setVisible(count);
	d->ui.subtitle_clear_action->setEnabled(count);
}

void MainWindow::changeCurrentSubChannel(QAction *act) {
	d->subtitle->setCurrentChannel(act->data().toInt());
}

void MainWindow::updateCurrentSubChannel(int index) {
	d->subChannelsActions.actions()[index+1]->setChecked(true);
}

void MainWindow::updateSubChannels(const QStringList &channels) {
	QList<QAction *> acts = d->subChannelsActions.actions();
	for (int i=1; i<acts.size(); ++i) {
		d->subListActions.removeAction(acts[i]);
		delete acts[i];
	}
	if (channels.empty()) {
		d->ui.sub_channel_menu->menuAction()->setVisible(false);
	} else {
		d->ui.sub_channel_menu->menuAction()->setVisible(true);
		const int count = channels.size();
		for (int i=0; i<count; ++i) {
			QAction *act = d->ui.sub_channel_menu->addAction(channels[i]);
			act->setCheckable(true);
			act->setData(i);
			act->setChecked(d->subtitle->currentChannel() == i);
			d->subChannelsActions.addAction(act);
		}
	}
}

void MainWindow::changeCurrentSubtitles(QAction* act) {
	if (act != d->ui.subtitle_disable_action) {
		if (act->isChecked()) {
			d->subtitle->select(act->data().toInt());
		} else
			d->subtitle->deselect(act->data().toInt());
	} else
		d->subtitle->deselectAll();
}

void MainWindow::addSubtitles() {
	QString dir;
	if (d->engine->currentSource().isLocalFile()) {
		QFileInfo info(d->engine->currentSource().filePath());
		dir = info.path() + '/' + info.completeBaseName();
	}
	const QString filter = trUtf8("자막 파일") + ' ' + d->backend->info()->subtitleExtensions().toFilter()
			+ ";;" + trUtf8("모든 파일") + ' ' + "(*.*)";
	EncodingFileDialog dlg(this, trUtf8("자막 열기"), dir, filter, d->subtitle->encoding());
	dlg.setFileMode(QFileDialog::ExistingFiles);
	if (dlg.exec()) {
		d->subtitle->appendSubtitles(dlg.selectedFiles(), true);
	}
}

void MainWindow::updateAudioTracks(const QStringList &tracks) {
	const bool show = tracks.size() > 1;
	d->ui.audio_track_menu->setVisible(show);
	if (show) {
		QList<QAction*> acts = d->audioTrackActions.actions();
		for (int i=0; i < acts.size(); ++i) {
			d->audioTrackActions.removeAction(acts[i]);
			delete acts[i];
		}
		for (int i=0; tracks.size(); ++i) {
			QAction *act = d->audioTrackActions.addAction(tracks[i]);
			act->setData(i);
		}
	}
}

void MainWindow::updateCurrentAudioTrack(int index) {
	d->audioTrackActions.actions()[index]->setCheckable(true);
}

void MainWindow::changeCurrentAuidoTrack(QAction *act) {
	d->audio->setCurrentTrack(act->data().toInt());
}

void MainWindow::showEqualizer() {
	static EqualizerDialog *dlg = new EqualizerDialog(d->video, this);
	if (dlg->isHidden())
		dlg->show();
}

void MainWindow::setFullScreen(bool full) {
	if (full == isFullScreen())
		return;
	d->ui.play_bar->setHidden(full);
	static QSize winSize = size();
	if (full) {
		winSize = size();
		d->dock->hide();
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		d->cursorTimer.start();
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		updateStaysOnTop();
		resize(winSize);
		if (cursor().shape() == Qt::BlankCursor)
			unsetCursor();
	}
	if (d->video)
		d->video->setFullScreenMode(full);
	d->videoSizeActions.setDisabled(full);
	d->ui.video_size_menu->setDisabled(full);

	d->onTopActions.setDisabled(full);
}

void MainWindow::changeVideoSize(QAction *act) {
	setVideoSize(act->data().toDouble());
}

void MainWindow::setVideoSize(double rate) {
	if (rate > 0.0)
		resize(size() + d->video->widgetSizeHint()*std::sqrt(rate) - d->video->widget()->size());
}

void MainWindow::slotStateChanged(Backend::State /*state*/) {
	if (d->engine->isPlaying()) {
		d->ui.play_pause_action->setIcon(QIcon(":/img/media-playback-pause.png"));
		d->ui.play_pause_action->setText(trUtf8("일시정지"));
	} else {
		d->ui.play_pause_action->setIcon(QIcon(":/img/media-playback-start.png"));
		d->ui.play_pause_action->setText(trUtf8("재생"));
	}
	updatePlayText();
	if (!d->changingOnTop)
		updateStaysOnTop();
}

void MainWindow::hideEvent(QHideEvent *event) {
	QMainWindow::hideEvent(event);
	if (d->pref->general().pauseWhenMinimized() && ((windowFlags() & Qt::WindowStaysOnTopHint) || !d->changingOnTop) && d->engine->isPlaying()) {
		d->engine->pause();
		d->pausedByHiding = true;
	}
}

void MainWindow::showEvent(QShowEvent *event) {
	QMainWindow::showEvent(event);
	if (d->pausedByHiding && d->pref->general().playWhenRestored()) {
		d->engine->play();
		d->pausedByHiding = false;
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	checkClose();
	event->accept();
}

void MainWindow::changeAspectRatio(QAction *act) {
	d->video->setAspectRatio(act->data().toDouble());
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
	return QMainWindow::eventFilter(obj, event);
}

void MainWindow::openFile() {
	Backend::Info *info = d->backend->info();
	const QString filter = trUtf8("비디오 파일") +' '+ info->videoExtensions().toFilter() +";;"
			+ trUtf8("음악 파일") + ' ' + info->audioExtensions().toFilter() + ";;"
			+ trUtf8("모든 파일") + ' ' + "(*.*)";
	const QString file = QFileDialog::getOpenFileName(this, trUtf8("파일 열기"), QString(), filter);
	if (!file.isEmpty())
		open(Backend::MediaSource(file));
}

void MainWindow::showAboutDialog() {
	QDialog dlg(this);
	Ui::Ui_AboutDialog ui;
	ui.setupUi(&dlg);
	Backend::Info *info = d->backend->info();
	ui.version_label->setText(ui.version_label->text().arg(Helper::version())
			.arg(QT_VERSION_STR).arg(info->compileVersion())
			.arg(qVersion()).arg(info->runtimeVersion()));
	dlg.exec();
}

void MainWindow::hideCursor() {
	if (cursor().shape() != Qt::BlankCursor)
		setCursor(Qt::BlankCursor);
}

#define checkInCenter() (d->ui.center->geometry().contains(event->pos()))
#define checkButton(b) (event->buttons() & (b))
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	const bool in = checkInCenter();
	const bool dragMove = in && checkButton(Qt::LeftButton) && !isFullScreen();
	bool set = false;
	if (dragMove) {
		if ((set = (cursor().shape() != Qt::SizeAllCursor)))
			setCursor(Qt::SizeAllCursor);
		move(event->globalPos() - d->dragPos);
	} else
		unsetCursor();
	if (isFullScreen()) {
		if (in)
			d->cursorTimer.start();
		else
			d->cursorTimer.stop();
		static const int h = d->pmb->height();
		QRect r = rect();
		r.setTop(r.height() - h);
		d->ui.play_bar->setVisible(r.contains(event->pos()));
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (!checkInCenter())
		return;
	if (checkButton(Qt::LeftButton) && !isFullScreen())
		d->dragPos = event->globalPos() - frameGeometry().topLeft();
	if (checkButton(Qt::MidButton))
		d->mouseClickActions[d->pref->interface().middleClickAction()]->trigger();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	QMainWindow::mouseDoubleClickEvent(event);
	if (checkButton(Qt::LeftButton) && checkInCenter())
		d->mouseClickActions[d->pref->interface().doubleClickAction()]->trigger();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
	QMainWindow::mouseReleaseEvent(event);
	if (cursor().shape() == Qt::SizeAllCursor)
		unsetCursor();
}

void MainWindow::wheelEvent(QWheelEvent *event) {
	if (!checkInCenter()) {
		QMainWindow::wheelEvent(event);
		return;
	}
	const int delta = event->delta();
	const Pref::Interface &iface = d->pref->interface();
	if (delta > 0)
		d->wheelScrollActions[iface.wheelScrollAction()].first->trigger();
	else if (delta < 0)
		d->wheelScrollActions[iface.wheelScrollAction()].second->trigger();
	else {
		event->ignore();
		return;
	}
	event->accept();
}

#undef checkInCenter
#undef checkButton

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	if (!event->mimeData()->hasUrls())
		return;
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.size() && urls[0].scheme() == "file")
		open(Backend::MediaSource(urls[0]));
}

void MainWindow::selectABSection() {
	static bool a = false, b = false;
	if (d->repeater->isRepeating())
		a = b = false;
	if (a && !b) {
		d->repeater->setBToCurrentTime();
		b = true;
		d->repeater->start();
		a = b = false;
	} else if (!a) {
		d->repeater->setAToCurrentTime();
		d->repeater->stop();
		a = true;
	}
}

void MainWindow::showABRepeatDialog() {
	static ABRepeatDialog *dlg = new ABRepeatDialog(d->repeater, this);
	dlg->show();
}

void MainWindow::adjustSizeForDock(bool visible) {
	if (!isFullScreen() && !d->dock->isFloating())
		resize(width() + (visible ? 1 : -1 ) * d->dock->width(), height());
}

void MainWindow::playPause() {
	if (d->engine->isStopped())
		d->model->play(d->model->currentRow());
	else if (d->engine->isPlaying())
		d->engine->pause();
	else
		d->engine->play();
}

void MainWindow::increaseSpeed() {
	d->engine->setSpeed(d->engine->speed() + 0.1);
}

void MainWindow::decreaseSpeed() {
	d->engine->setSpeed(d->engine->speed() - 0.1);
}

void MainWindow::restoreSpeed() {
	d->engine->setSpeed(1.0);
}

void MainWindow::doubleSpeed() {
	d->engine->setSpeed(d->engine->speed() * 2.0);
}

void MainWindow::halfSpeed() {
	d->engine->setSpeed(d->engine->speed() * 0.5);
}

void MainWindow::increaseVolume() {
	d->audio->setVolume(d->audio->volume() + d->pref->interface().volumeStep());
}

void MainWindow::decreaseVolume() {
	d->audio->setVolume(d->audio->volume() - d->pref->interface().volumeStep());
}

void MainWindow::forward() {
	if (d->engine->isSeekable())
		d->engine->seek(d->pref->interface().seekingStep(), true, isFullScreen());
}

void MainWindow::forwardMore() {
	if (d->engine->isSeekable())
		d->engine->seek(d->pref->interface().seekingMoreStep(), true, isFullScreen());
}

void MainWindow::forwardMuchMore() {
	if (d->engine->isSeekable())
		d->engine->seek(d->pref->interface().seekingMuchMoreStep(), true, isFullScreen());
}

void MainWindow::backward() {
	if (d->engine->isSeekable())
		d->engine->seek(-d->pref->interface().seekingStep(), true, isFullScreen());
}

void MainWindow::backwardMore() {
	if (d->engine->isSeekable())
		d->engine->seek(-d->pref->interface().seekingMoreStep(), true, isFullScreen());
}

void MainWindow::backwardMuchMore() {
	if (d->engine->isSeekable())
		d->engine->seek(-d->pref->interface().seekingMuchMoreStep(), true, isFullScreen());
}

void MainWindow::increaseSyncDelay() {
	d->subtitle->addSyncDelay(d->pref->interface().syncDelayStep());
}

void MainWindow::decreaseSyncDelay() {
	d->subtitle->addSyncDelay(-d->pref->interface().syncDelayStep());
}
 
void MainWindow::updatePreferences() {
	Backend::BackendObject *obj = Backend::Manager::get()->load(d->pref->backend().fileName());
	if (d->backend != obj)
		setBackend(obj);
	if (!d->backend)
		return;
	d->audio->setInitialVolume(d->pref->general().resetVolume()
			? d->pref->general().initialVolume() : -1);

	d->subtitle->setInitialPos(d->pref->subtitle().initialPos()/100.0);
	d->subtitle->setStyle(d->pref->subtitle().osdStyle());
	d->subtitle->setEncoding(d->pref->subtitle().encoding());
	d->subtitle->setAutoSelect(d->pref->subtitle().autoSelect());
	d->subtitle->setPriority(d->pref->subtitle().priority());
}

void MainWindow::updatePlayText() {
	Backend::MediaSource source = d->model->currentSource();
	QString text;
	if (source.isValid()) {
		text = source.displayName();
		const int row = d->model->currentRow();
		const int count = d->model->count();
		if (count && row < count)
			text += QString(" (%1/%2)").arg(row + 1).arg(count);
		if (!text.isEmpty())
			text += " - ";
		if (!d->engine || d->engine->isStopped())
			text += trUtf8("정지");
		else if (d->engine->isPlaying())
			text += trUtf8("재생중");
		else
			text += trUtf8("일시정지");
	}
	d->pmb->setPlayText(text);
}

void MainWindow::slotStarted() {
	initSubtitles();
	setVideoSize(1.0);
}

void MainWindow::updateFinished(const Backend::MediaSource &source) {
	if (source.isLocalFile())
		d->recent->setFinished(source);
}

void MainWindow::updateStopped(const Backend::MediaSource &source, int time) {
	if (source.isLocalFile())
		d->recent->setStopped(source, time);
}

void MainWindow::updateSource(const Backend::MediaSource &source) {
	d->ui.play_dvd_menu_action->setVisible(source.isDisc());
}

void MainWindow::executeWizard() {
	//Preferences::SettingsWizard wizard(this);
	//wizard.exec();
}

void MainWindow::slotFullScreen() {
	const bool full = isFullScreen();
	if (full) {
		d->ui.video_fullscreen_action->setIcon(QIcon(":/img/view-fullscreen.png"));
		d->ui.video_fullscreen_action->setText(trUtf8("전체화면 켜기"));
	} else {
		d->ui.video_fullscreen_action->setIcon(QIcon(":/img/view-restore.png"));
		d->ui.video_fullscreen_action->setText(trUtf8("전체화면 끄기"));
	}
	setFullScreen(!full);
}

void MainWindow::openDVD() {
	open(Backend::MediaSource(Backend::MediaSource::Disc, "/"));
}
