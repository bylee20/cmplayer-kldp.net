#include "mainwindow_p.h"
#include "actioncollection.h"
#include "playlistdock.h"
#include "helper.h"
#include "recentinfo.h"
#include "equalizerdialog.h"
#include "playmenubar.h"
#include "playlistmodel.h"
#include "equalizerdialog.h"
#include "abrepeatdialog.h"
#include "ui/ui_aboutdialog.h"
#include "pref/preferences.h"
#include "pref/general.h"
#include "pref/interface.h"
#include "pref/subtitle.h"
#include "pref/preferencesdialog.h"
#include <backend/playengine.h>
#include <backend/audiooutput.h>
#include <backend/mediasource.h>
#include <backend/videooutput.h>
#include <backend/info.h>
#include <backend/abrepeater.h>
#include <QUrl>
#include <QFileInfo>
#include <QFileDialog>
#include <QMouseEvent>
#include <QTimer>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent) {
	d = new Data(this);
	d->init();
 	QStringList args = QApplication::arguments();
 	if (args.size() > 1) {
 		QUrl url(args.last());
		d->open(Backend::MediaSource(url.scheme().isEmpty() ? args.last() : url));
 	}
	updatePreferences();
}

MainWindow::~MainWindow() {
	delete d;
}

void MainWindow::showMenu(const QPoint &pos) {
	d->contextMenu->exec(d->video->widget()->mapToGlobal(pos));
}

void MainWindow::togglePlayListVisibility() {
	d->dock->setVisible(d->dock->isHidden());
}

void MainWindow::updateSpeed(double speed) {
	static QString text = trUtf8("현재: ×%1 배속");
	d->ui.play_speed_current_action->setText(text.arg(speed));
}

void MainWindow::changeStaysOnTop(QAction *act) {
	d->staysOnTop = Data::StaysOnTop(act->data().toInt());
	d->updateStaysOnTop();
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
		d->open(Backend::MediaSource(act->data().toUrl()));
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
		updatePreferences();
		if (!d->engine->isStopped())
			d->engine->replay();
	}
}

void MainWindow::clearSubtitleList() {
	QList<QAction *> acts = d->subListActions->actions();
	for (int i=0; i<acts.size()-1; ++i) {
		d->subListActions->removeAction(acts[i]);
		delete acts[i];
	}
	d->subListActions->removeAction(d->ui.subtitle_disable_action);
	d->ui.subtitle_disable_action->setVisible(false);
}

void MainWindow::updateCurrentSubtitleIndexes(const QList<int> &indexes) {
	d->ui.subtitle_disable_action->setChecked(!indexes.size());
	QList<QAction *> act = d->subListActions->actions();
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
		d->subListActions->addAction(act);
	}
	d->subListActions->addAction(d->ui.subtitle_disable_action);
	d->ui.subtitle_disable_action->setVisible(count);
	d->ui.subtitle_clear_action->setEnabled(count);
}

void MainWindow::changeCurrentSubChannel(QAction *act) {
	d->subtitle->setCurrentChannel(act->data().toInt());
}

void MainWindow::updateCurrentSubChannel(int index) {
	d->subChannelsActions->actions()[index+1]->setChecked(true);
}

void MainWindow::updateSubChannels(const QStringList &channels) {
	QList<QAction *> acts = d->subChannelsActions->actions();
	for (int i=1; i<acts.size(); ++i) {
		d->subListActions->removeAction(acts[i]);
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
			d->subChannelsActions->addAction(act);
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
	const QString Filter = trUtf8("자막 파일") + ' ' + d->f->info()->subtitleExtensions().toFilter() + ";;"
			+ trUtf8("모든 파일") + ' ' + "(*.*)";
	QString dir;
	if (d->engine->currentSource().isLocalFile()) {
		QFileInfo info(d->engine->currentSource().filePath());
		dir = info.path() + '/' + info.completeBaseName();
	}
	const QStringList files = QFileDialog::getOpenFileNames(this, trUtf8("자막 열기"), dir, Filter);
	if (files.size())
		d->subtitle->appendSubtitles(files, true);
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
		d->cursorTimer->start();
	} else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		d->updateStaysOnTop();
		resize(winSize);
		if (cursor().shape() == Qt::BlankCursor)
			unsetCursor();
	}
	d->video->setFullScreen(full);
	d->videoSizeActions->setDisabled(full);
	d->ui.video_size_menu->setDisabled(full);
	d->ui.video_on_top_menu->setDisabled(full);
	d->onTopActions->setDisabled(full);
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
		d->updateStaysOnTop();
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
	d->checkClose();
	event->accept();
}

void MainWindow::changeAspectRatio(QAction *act) {
	d->video->setAspectRatio(act->data().toDouble());
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
	return QMainWindow::eventFilter(obj, event);
}

void MainWindow::openFile() {
	Backend::Info *info = d->f->info();
	const QString Filter = trUtf8("비디오 파일") +' '+ info->videoExtensions().toFilter() +";;"
			+ trUtf8("음악 파일") + ' ' + info->audioExtensions().toFilter() + ";;"
			+ trUtf8("모든 파일") + ' ' + "(*.*)";
	const QString file = QFileDialog::getOpenFileName(this, trUtf8("파일 열기"), QString(), Filter);
	if (!file.isEmpty())
		d->open(Backend::MediaSource(file));
}

void MainWindow::showAboutDialog() {
	QDialog dlg(this);
	Ui::Ui_AboutDialog ui;
	ui.setupUi(&dlg);
	Backend::Info *info = d->f->info();
	ui.version_label->setText(ui.version_label->text().arg(Helper::version())
			.arg(QT_VERSION_STR).arg(info->compileVersion())
			.arg(qVersion()).arg(info->runtimeVersion()));
	dlg.exec();
}

void MainWindow::hideCursor() {
	if (cursor().shape() != Qt::BlankCursor)
		setCursor(Qt::BlankCursor);
}

#define checkInCenter() (d->video->widget()->geometry().contains(event->pos()))
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
			d->cursorTimer->start();
		else
			d->cursorTimer->stop();
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
		d->open(Backend::MediaSource(urls[0]));
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
		if (d->engine->isPlaying())
			text += trUtf8("재생중");
		else if (d->engine->isPaused())
			text += trUtf8("일시정지");
		else
			text += trUtf8("정지");
	}
	d->pmb->setPlayText(text);
}

void MainWindow::slotStarted() {
	d->initSubtitles();
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
	d->open(Backend::MediaSource(Backend::MediaSource::Disc, "/"));
}
