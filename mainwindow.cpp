#include "mainwindow.hpp"
#include <QtGui/QMouseEvent>
#include "playlistview.hpp"
#include "screensavermanager.hpp"
#include "charsetdetector.hpp"
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
#include <QtGui/QPainter>

#include "textosdrenderer.hpp"
#include "audiocontroller.hpp"
#include "pref.hpp"
#include "global.hpp"
#include "info.hpp"
#include <qmath.h>
#include "abrepeater.hpp"
#include "logoview.hpp"
#include "toolbox.hpp"

class MainWindow::VideoScreen : public QWidget {
public:
	VideoScreen(QWidget *video, QWidget *parent)
	: QWidget(parent) {
		m_video = video;
		m_video->setParent(this);
		m_video->move(0, 0);
		m_sync = true;
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		sync();
	}
	void setSyncSize(bool sync) {
		m_sync = sync;
		this->sync();
	}
	void sync() {
		if (m_sync)
			m_video->resize(size());
		else
			m_video->resize(desktopSize());
	}
	QSize sizeHint() const {
		return m_video->sizeHint();
	}
private:
	void resizeEvent(QResizeEvent *event) {
		sync();
		QWidget::resizeEvent(event);
	}
	QWidget *m_video;
	bool m_sync;
};

struct MainWindow::Data {
	Data(Menu &menu): menu(menu), pref(Pref::get()) {}
	Menu &menu;
	LogoView *logo;
	VideoScreen *screen;
	const Pref &pref;
	QStackedWidget *stack;
	ControlWidget *control;
	PlayEngine *engine;
	SubtitleRenderer *subtitle;
	TimeLineOsdRenderer *timeLine;
	TextOsdRenderer *message;
	Subtitle subLoaded;
	QList<int> subSelected;
	ABRepeater *ab;
	ToolBox *tool;
	bool moving, changingSub;
	QPoint prevPos;
};

MainWindow::MainWindow(): d(new Data(Menu::create(this))) {
	d->changingSub = d->moving = false;
	d->engine = new PlayEngine;
	d->subtitle = new SubtitleRenderer;
	d->timeLine = new TimeLineOsdRenderer;
	d->message = new TextOsdRenderer(Qt::AlignTop | Qt::AlignLeft);
	setupUi();
	d->menu.updatePref();
	d->ab = new ABRepeater(d->engine, d->subtitle);

	Menu &menu = d->menu;
	Menu &open = d->menu("open");
	connect(open["file"], SIGNAL(triggered()), this, SLOT(openFile()));
//	connect(open["url"], SIGNAL(triggered()), this, SLOT(open()));
	connect(open["dvd"], SIGNAL(triggered()), this, SLOT(openDvd()));
//	connect(open("recent").g(), SIGNAL(triggered(Core::Mrl)), this, SLOT(openMrl(Core::Mrl)));
//	connect(open("recent")["clear"], SIGNAL(triggered()), d->recent, SLOT(clearStack()));
	Menu &dvdMenu = d->menu("dvd-menu");
	connect(dvdMenu.g(), SIGNAL(triggered(int)), d->engine, SLOT(navigateDVDMenu(int)));

	Menu &screen = menu("screen");
	connect(screen("size").g(), SIGNAL(triggered(double)), this, SLOT(setVideoSize(double)));
	connect(screen("aspect").g(), SIGNAL(triggered(double))
		, d->engine->renderer(), SLOT(setAspectRatio(double)));
	connect(screen("crop").g(), SIGNAL(triggered(double))
		, d->engine->renderer(), SLOT(setCropRatio(double)));
//	connect(screen("on top").g(), SIGNAL(triggered(QAction*)), this, SLOT(updateOnTop()));
//	connect(screen["snapshot"], SIGNAL(triggered()), this, SLOT(takeSnapshot()));
	Menu &play = menu("play");
	connect(play["stop"], SIGNAL(triggered()), d->engine, SLOT(stop()));
	connect(play("speed").g(), SIGNAL(triggered(int)), this, SLOT(setSpeed(int)));
	connect(play["prev"], SIGNAL(triggered()), d->tool->playlist(), SLOT(playPrevious()));
	connect(play["next"], SIGNAL(triggered()), d->tool->playlist(), SLOT(playNext()));
	connect(play["pause"], SIGNAL(triggered()), this, SLOT(togglePlayPause()));
	connect(play["list"], SIGNAL(triggered()), this, SLOT(togglePlaylist()));
//	connect(play("engine").g(), SIGNAL(triggered(const QString &))
//			, d->engine, SLOT(setBackend(const QString &)));
	connect(play("repeat").g(), SIGNAL(triggered(int)), this, SLOT(doRepeat(int)));
	connect(play("seek").g(), SIGNAL(triggered(int)), this, SLOT(seek(int)));
	Menu &video = menu("video");
//	connect(video.g("color"), SIGNAL(triggered(QAction*))
//			, this, SLOT(setColorProperty(QAction*)));
//	connect(video("renderer").g(), SIGNAL(triggered(QAction*))
//			, this, SLOT(setRenderer(QAction*)));
	Menu &audio = menu("audio");
	connect(audio.g("volume"), SIGNAL(triggered(int)), this, SLOT(setVolume(int)));
	connect(audio["mute"], SIGNAL(toggled(bool)), this, SLOT(setMuted(bool)));
	connect(audio.g("amp"), SIGNAL(triggered(int)), this, SLOT(setAmp(int)));
//	connect(audio("renderer").g(), SIGNAL(triggered(QAction*))
//			, this, SLOT(setRenderer(QAction*)));
//	connect(audio("track").g(), SIGNAL(triggered(const QString &))
//			, d->engine, SLOT(setCurrentTrack(const QString &)));
	Menu &sub = menu("subtitle");
//	connect(sub("list")["hide"], SIGNAL(toggled(bool))
//			, d->engine, SLOT(setSubtitleHidden(bool)));
//	connect(sub("list")["open"], SIGNAL(triggered()), this, SLOT(openSubFile()));
//	connect(sub("list")["clear"], SIGNAL(triggered()), this, SLOT(clearSubs()));
	connect(sub("list").g(), SIGNAL(triggered(QAction*)), this, SLOT(updateSubtitle(QAction*)));
//	connect(sub["viewer"], SIGNAL(triggered()), this, SLOT(openSubViewer()));
	connect(sub.g("pos"), SIGNAL(triggered(int)), this, SLOT(moveSubtitle(int)));
	connect(sub.g("sync"), SIGNAL(triggered(int)), this, SLOT(setSyncDelay(int)));
//	connect(menu["pref"], SIGNAL(triggered()), this, SLOT(showPrefDialog()));
//	connect(menu["about"], SIGNAL(triggered()), this, SLOT(showAbout()));
//// 	connect(menu["help"], SIGNAL(triggered()), this, SLOT(slotHelp()));
//	connect(menu["exit"], SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(updateMrl(Mrl)));
	connect(d->engine, SIGNAL(stateChanged(MediaState,MediaState))
		, this, SLOT(updateState(MediaState,MediaState)));
	connect(d->tool->playlist(), SIGNAL(finished()), this, SLOT(handleFinished()));
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
	connect(d->logo, SIGNAL(customContextMenuRequested(QPoint))
		, this, SLOT(showContextMenu(QPoint)));
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

	d->stack = new QStackedWidget;

	d->logo = new LogoView(d->stack);
	d->stack->addWidget(d->logo);
	d->stack->addWidget(d->engine->renderer());

	d->screen = new VideoScreen(d->stack, center);

//	qDebug() << d->stack->sizePolicy();
//	qDebug() << d->screen->sizePolicy();

	d->control = new ControlWidget(d->engine, center);
	Menu &play = d->menu("play");
	d->control->connectMute(d->menu("audio")["mute"]);
	d->control->connectPlay(play["pause"]);
	d->control->connectStop(play["stop"]);
	d->control->connectPrevious(play["prev"]);
	d->control->connectNext(play["next"]);
	d->control->connectForward(play("seek")["forward1"]);
	d->control->connectBackward(play("seek")["backward1"]);
	d->control->connectOpen(d->menu("open")["file"]);
	d->control->connectFullScreen(d->menu("screen")("size")["full"]);
	d->control->connectPlaylist(d->menu("play")["list"]);

	QVBoxLayout *vbox = new QVBoxLayout(center);
	vbox->addWidget(d->screen);
	vbox->addWidget(d->control);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->setSpacing(0);

	menuBar()->hide();
	setMouseTracking(true);
	setCentralWidget(center);
	d->stack->setMouseTracking(true);
	d->screen->setMouseTracking(true);
	d->logo->setMouseTracking(true);
	d->engine->renderer()->setMouseTracking(true);
	center->setMouseTracking(true);
	d->logo->setContextMenuPolicy(Qt::CustomContextMenu);
	d->engine->renderer()->setContextMenuPolicy(Qt::CustomContextMenu);

	d->tool = new ToolBox(this);
}

void MainWindow::openFile() {
	const QString filter = Info::mediaExtensionFilter();
	const QString file = QFileDialog::getOpenFileName(this
		, tr("Open File"), QString(), filter);
	if (file.isEmpty())
		return;
	d->tool->playlist()->load(Mrl::fromLocalFile(file));
	d->tool->playlist()->play(Mrl::fromLocalFile(file));
}

void MainWindow::openDvd() {
	Mrl mrl(QUrl("dvd://"));
	d->engine->setMrl(mrl);
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
	doSubtitleAutoLoad();
	doSubtitleAutoSelection();

	Menu &list = d->menu("subtitle")("list");
	d->changingSub = true;
	for (int i=0; i<d->subLoaded.size(); ++i) {
		QAction *action = list.addActionToGroupWithoutKey(d->subLoaded[i].name(), true);
		action->setData(i);
		action->setChecked(d->subSelected.contains(i));
	}
	d->changingSub = false;
	updateSubtitle();
}

void MainWindow::clearSubtitles() {
	d->subLoaded.clear();
	d->subSelected.clear();
	QList<QAction*> acts = d->menu("subtitle")("list").g()->actions();
	for (int i=0; i<acts.size(); ++i)
		delete acts[i];
	d->subtitle->setSubtitle(d->subLoaded);
//	d->subViewer->showCurrentSubtitle();
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
	const QFileInfo fileInfo(mrl.url().toLocalFile());
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
		const QString filePath = all[i].absoluteFilePath();
		QString encoding;
		const double conf = d->pref.subtitleEncodingConfidence*0.01;
		if (d->pref.useSubtitleEncodingAutoDetection)
			encoding = CharsetDetector::detect(filePath, conf);
		if (encoding.isEmpty())
			encoding = d->pref.subtitleEncoding;
		Subtitle sub;
		if (sub.load(all[i].absoluteFilePath(), encoding))
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
	const QString base = QFileInfo(mrl.url().toLocalFile()).completeBaseName();
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
//	if (d->subViewer->isVisible())
//		d->subViewer->showCurrentSubtitle();
//		d->subViewer->setSubtitle(d->sub);
}

void MainWindow::seek(int diff) {
	if (!diff || d->engine->state() == StoppedState)
		return;
	const int target = qBound(0, d->engine->position() + diff, d->engine->duration());
	if (d->engine->seek(target)) {
		if (d->stack->currentWidget() == d->logo)
			d->logo->showTimeLine(target, d->engine->duration());
		else
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
	if (d->stack->currentWidget() == d->logo)
		d->logo->showText(message, last);
	else
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
	if (muted)
		d->menu("audio")["mute"]->setIcon(QIcon(":/img/irc-unvoice.png"));
	else
		d->menu("audio")["mute"]->setIcon(QIcon(":/img/irc-voice.png"));
	d->engine->audio()->setMuted(muted);
	showMessage(tr("Mute"), muted ? tr("On") : tr("Off"));
}

void MainWindow::setFullScreen(bool full) {
	if (full == isFullScreen())
		return;
	d->control->setHidden(full);
	d->screen->setSyncSize(!full);
// 	titleBar()->setHidden(full);
// 	const int m = full ? 0 : 3;
// 	d->center->layout()->setContentsMargins(m, m, m, m);
	if (full) {

//		d->prevWinSize = size();
		setWindowState(windowState() ^ Qt::WindowFullScreen);

//		if (d->pref.hideCursor && (!d->pref.hideInFullScreen || full))
//			d->hider.start(d->pref.hideDelay);
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
//		resize(d->prevWinSize);
//		d->hider.stop();
//		if (cursor().shape() == Qt::BlankCursor)
//			unsetCursor();
	}
}

void MainWindow::setVideoSize(double rate) {
	if (rate < 0) {
		const bool wasFull = isFullScreen();
		setFullScreen(!wasFull);
		d->menu("screen")("size")["full"]->setChecked(!wasFull);
	} else {
		if (isFullScreen())
			setFullScreen(false);
		resize(size() - d->screen->size() + d->screen->sizeHint()*qSqrt(rate));
	}
}

void MainWindow::updateState(MediaState state, MediaState old) {
	if (old == state)
		return;
	if (state == PlayingState) {
		ScreensaverManager::setDisabled(d->pref.disableScreensaver);
		d->menu("play")["pause"]->setIcon(QIcon(":/img/media-playback-pause.png"));
		d->menu("play")["pause"]->setText(tr("Pause"));

	} else {
		ScreensaverManager::setDisabled(false);
		d->menu("play")["pause"]->setIcon(QIcon(":/img/media-playback-start.png"));
		d->menu("play")["pause"]->setText(tr("Play"));
	}
	if (old == StoppedState)
		d->stack->setCurrentWidget(d->engine->renderer());
	else if (state == StoppedState)
		d->stack->setCurrentWidget(d->logo);
//	if (state == PausedState && d->pausedByHiding)
//		return;
//	updateOnTop();
}

void MainWindow::setSpeed(int diff) {
	int newSpeed = 100;
	if (diff)
		newSpeed = d->engine->speed()*100.0 + diff + 0.5;
	d->engine->setSpeed(qBound(0.1, newSpeed*0.01, 10.0));
	showMessage(tr("Speed"), QString::fromUtf8("\303\227%1").arg(d->engine->speed()));
}

void MainWindow::setAmp(int amp) {
	const int newAmp = qBound(0, qRound(d->engine->audio()->preAmp()*100 + amp), 1000);
	d->engine->audio()->setPreAmp(newAmp*0.01);
	showMessage(tr("PreAmp"), newAmp, "%");
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

void MainWindow::togglePlaylist() {
	d->tool->setVisible(!d->tool->isVisible());
}

PlayEngine *MainWindow::engine() const {
	return d->engine;
}

#define IS_IN_CENTER (d->stack->geometry().contains(d->stack->mapFrom(this, event->pos())))
#define IS_BUTTON(b) (event->buttons() & (b))

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (!IS_IN_CENTER)
		return;
	if (IS_BUTTON(Qt::LeftButton)) {
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

//	d->hider.stop();
//	if (cursor().shape() == Qt::BlankCursor)
//		unsetCursor();
	QMainWindow::mouseMoveEvent(event);

	const bool full = isFullScreen();
	if (full) {
		static const int h = d->control->height();
		QRect rect = this->rect();
		rect.setTop(rect.height() - h);
		d->control->setVisible(rect.contains(event->pos()));
	} else {
		if (d->moving) {
			const QPoint pos = event->globalPos();
			move(this->pos() + pos - d->prevPos);
			d->prevPos = pos;
		}
	}
//	if (full) {
//		static const int h = d->control->height();
//		QRect r = rect();
//		r.setTop(r.height() - h);
//		d->control->setVisible(r.contains(event->pos()));
//	}
//	if (IS_IN_CENTER && d->pref.hideCursor && (!d->pref.hideInFullScreen || full))
//		d->hider.start(d->pref.hideDelay);
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

void MainWindow::handleFinished() {
	d->stack->setCurrentWidget(d->logo);
}

void MainWindow::setSyncDelay(int diff) {
	int delay = d->subtitle->delay() + diff;
	d->subtitle->setDelay(delay);
	showMessage("Subtitle Sync", delay*0.001, "sec", true);

}

//#undef IS_IN_CENTER
//#undef IS_BUTTON

