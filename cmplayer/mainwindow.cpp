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
#include <xine/xineengine.h>
#include <xine/xinestream.h>
#include <xine/xineaudio.h>
#include <xine/mediasource.h>
//#include <mplayer/subtitleoutput.h>
//#include <mplayer/informations.h>
//#include <mplayer/abrepeater.h>
#include <QUrl>
#include <QFileInfo>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), d(new Data(this)) {
	d->init();
// 	QStringList args = QApplication::arguments();
// 	if (args.size() > 1) {
// 		QUrl url(args.last());
// 		if (url.scheme().isEmpty())
// 			d->open(MPlayer::MediaSource(args.last()));
// 		else
// 			d->open(MPlayer::MediaSource(url));
// 	}
	updatePreferences();
}

MainWindow::~MainWindow() {
	delete d;
}

void MainWindow::showMenu(const QPoint &pos) {
//	d->contextMenu->exec(d->video->widget()->mapToGlobal(pos));
}

void MainWindow::togglePlayListVisibility() {
//	d->dock->setVisible(d->dock->isHidden());
}

void MainWindow::updateSpeed(double speed) {
	static QString text = trUtf8("현재: ×%1 배속");
	d->ui.play_speed_current_action->setText(text.arg(speed));
}

void MainWindow::changeStaysOnTop(QAction *act) {
//	d->staysOnTop = Data::StaysOnTop(act->data().toInt());
//	d->updateStaysOnTop();
}

void MainWindow::clearRecentFiles() {
//	d->recent->clearStack();
}

void MainWindow::updateRecentSize(int size) {
//	static ActionCollection *ac = ActionCollection::get();
//	while (size != d->recentActions.size()) {
//		if (size > d->recentActions.size()) {
//			static QAction *sprt = d->ui.open_recent_menu->actions()[0];
//			QAction *act = new QAction(d->ui.open_recent_menu);
//			connect(act, SIGNAL(triggered()), this, SLOT(openRecent()));
//			d->ui.open_recent_menu->insertAction(sprt, act);
//			d->recentActions.append(act);
//		} else
//			delete ac->take(d->recentActions.takeLast());
//	}
//	for (int i=0; i<d->recentActions.size(); ++i) {
//		ac->addAction(trUtf8("최근 연 파일 %1").arg(i+1), ac->take(d->recentActions[i]));
//	}
}

void MainWindow::updateRecentActions(const RecentStack &stack) {
//	const int count = stack.size();
//	if (count != d->recentActions.size())
//		updateRecentSize(count);
//	for (int i=0; i<count; ++i) {
//		QAction *act = d->recentActions[i];
//		act->setData(stack[i].url());
//		act->setText(stack[i].displayName());
//		act->setVisible(stack[i].isValid());
//	}
}

void MainWindow::openRecent() {
//	QAction *act = qobject_cast<QAction *>(sender());
//	if (act)
//		d->open(MPlayer::MediaSource(act->data().toUrl()));
}

void MainWindow::slotResized() {
	if (!d->resizedByAct && !isFullScreen())
		d->ui.video_size_user_action->setChecked(true);
	else
		d->resizedByAct = false;
}

void MainWindow::updateSyncDelay(int msec) {
	d->ui.subtitle_sync_current_action->setText(trUtf8("현재값:"
			"%1").arg(static_cast<double>(msec)/1000));
}

void MainWindow::crop(QAction *act) {
//	d->video->crop(act->data().toDouble());
}

void MainWindow::showPreferencesDialog() {
	Pref::PreferencesDialog dlg(this);
	if (dlg.exec()) {
		updatePreferences();
//		if (d->engine->isPlaying())
//			d->engine->replay();
	}
}

void MainWindow::clearSubtitleList() {
//	QList<QAction *> acts = d->subListActions->actions();
//	for (int i=0; i<acts.size()-1; ++i) {
//		d->subListActions->removeAction(acts[i]);
//		delete acts[i];
//	}
//	d->subListActions->removeAction(d->ui.subtitle_disable_action);
//	d->ui.subtitle_disable_action->setVisible(false);
}

void MainWindow::updateCurrentSubtitleIndexes(const QList<int> &indexes) {
//	d->ui.subtitle_disable_action->setChecked(!indexes.size());
//	QList<QAction *> act = d->subListActions->actions();
//	for (int i=0; i<act.size()-1; ++i)
//		act[i]->setChecked(indexes.contains(i));
}

void MainWindow::updateSubtitles(const QStringList &names) {
//	clearSubtitleList();
//	const int count = names.size();
//	const QList<int> &indexes = d->subout->selectedIndexes();
//	for (int i=0; i<count; ++i) {
//		QAction *act = d->ui.subtitle_select_menu->addAction(names[i]);
//		act->setCheckable(true);
//		act->setData(i);
//		act->setChecked(indexes.contains(i));
//		d->subListActions->addAction(act);
//	}
//	d->subListActions->addAction(d->ui.subtitle_disable_action);
//	d->ui.subtitle_disable_action->setVisible(count);
//	d->ui.subtitle_clear_action->setEnabled(count);
}

void MainWindow::changeCurrentSubtitles(QAction* act) {
//	if (act != d->ui.subtitle_disable_action) {
//		if (act->isChecked()) {
//			d->subout->select(act->data().toInt());
//		} else
//			d->subout->deselect(act->data().toInt());
//	} else
//		d->subout->deselectAll();
}

void MainWindow::addSubtitles() {
//	static const QString Filter = trUtf8("자막 파일") + ' ' + d->info->subtitleFilter().toFilter() + ";;"
//			+ trUtf8("모든 파일") + ' ' + "(*.*)";
//	QString dir;
//	if (d->engine->currentSource().isLocalFile()) {
//		QFileInfo info(d->engine->currentSource().filePath());
//		dir = info.path() + '/' + info.completeBaseName();
//	}
//	const QStringList files = QFileDialog::getOpenFileNames(this, trUtf8("자막 열기"), dir, Filter);
//	if (files.size())
//		d->subout->appendSubtitles(files, true);
}

void MainWindow::showEqualizer() {
//	EqualizerDialog dlg(d->video, this);
//	dlg.exec();
}

void MainWindow::setFullScreen(bool full) {
//	if (full == isFullScreen())
//		return;
//	d->subout->setDisplayOnMarginEnabled(full && d->pref->subtitle().displayOnMarginWhenFullScreen);
//	d->ui.play_bar->setHidden(full);
//	static QSize winSize = size();
//	if (full) {
//		winSize = size();
//		d->dock->hide();
//		setWindowState(windowState() ^ Qt::WindowFullScreen);
//		d->engine->setOSDLevel(1);
//	} else {
//		d->engine->setOSDLevel(0);
//		setWindowState(windowState() ^ Qt::WindowFullScreen);
//		d->updateStaysOnTop();
//		resize(winSize);
//	}
//	d->videoSizeActions->setDisabled(full);
}

void MainWindow::changeVideoSize(QAction *act) {
	d->resizedByAct = act != d->ui.video_size_user_action;
	setVideoSize(act->data().toDouble());
}

void MainWindow::setVideoSize(double rate) {
//	if (rate > 0.0)
//		resize(size() + d->video->widgetSizeHint()*std::sqrt(rate) - d->video->widget()->size());
}

void MainWindow::slotStateChanged(Xine::State /*state*/) {
	if (d->stream->isPlaying()) {
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
	if (d->pref->general().pauseWhenMinimized && ((windowFlags() & Qt::WindowStaysOnTopHint) || !d->changingOnTop) && d->stream->isPlaying()) {
		d->stream->pause();
		d->pausedByHiding = true;
	}
}

void MainWindow::showEvent(QShowEvent *event) {
	QMainWindow::showEvent(event);
	if (d->pausedByHiding && d->pref->general().playWhenRestored) {
		d->stream->play();
		d->pausedByHiding = false;
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	d->checkClose();
	event->accept();
}

void MainWindow::changeAspectRatio(QAction *act) {
//	d->video->setAspectRatio(act->data().toDouble());
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
	return QMainWindow::eventFilter(obj, event);
}

void MainWindow::openFile() {
//	static const QString Filter = trUtf8("비디오 파일") +' '+ d->info->videoExtensions().toFilter() +";;"
//			+ trUtf8("음악 파일") + ' ' + d->info->audioExtensions().toFilter() + ";;"
//			+ trUtf8("모든 파일") + ' ' + "(*.*)";
//	const QString file = QFileDialog::getOpenFileName(this, trUtf8("파일 열기"), QString(), Filter);
	const QString file = QFileDialog::getOpenFileName(this, trUtf8("파일 열기"));
	if (!file.isEmpty())
		d->stream->play(file);
//	if (!file.isEmpty())
//		d->open(Xine::MediaSource(file));
}

void MainWindow::showAboutDialog() {
	QDialog dlg(this);
	Ui::Ui_AboutDialog ui;
	ui.setupUi(&dlg);
	ui.version_label->setText(ui.version_label->text().arg(Helper::version()).arg(QT_VERSION_STR));
	dlg.exec();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
	if (d->dragMove && event->buttons() & Qt::LeftButton)
		move(event->globalPos() - d->dragPos);
	if (isFullScreen()) {
		static const int h = d->pmb->height();
		QRect r = rect();
		r.setTop(r.height() - h);
		d->ui.play_bar->setVisible(r.contains(event->pos()));
	}
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	QMainWindow::mouseMoveEvent(event);
//	bool inCentral = d->video->widget()->geometry().contains(event->pos());
//	d->dragMove = event->buttons() & Qt::LeftButton && inCentral && !isFullScreen();
//	if (d->dragMove)
//		d->dragPos = event->globalPos() - frameGeometry().topLeft();
//	if (event->button() == Qt::MidButton && inCentral)
//		d->mouseClickActions[d->pref->interface().middleClickAction]->trigger();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	QMainWindow::mouseDoubleClickEvent(event);
//	if (event->button() == Qt::LeftButton
//			&& d->video->widget()->geometry().contains(event->pos()))
//		d->mouseClickActions[d->pref->interface().doubleClickAction]->trigger();
}

void MainWindow::wheelEvent(QWheelEvent *event) {
	const int delta = event->delta();
	if (delta > 0)
		d->wheelScrollActions[d->pref->interface().wheelScrollAction].first->trigger();
	else if (delta < 0)
		d->wheelScrollActions[d->pref->interface().wheelScrollAction].second->trigger();
	else
		event->ignore();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	if (!event->mimeData()->hasUrls())
		return;
	QList<QUrl> urls = event->mimeData()->urls();
	//if (urls.size() && urls[0].scheme() == "file")
	//	d->open(MPlayer::MediaSource(urls[0]));
}

void MainWindow::selectABSection() {
//	static bool a = false, b = false;
//	if (d->repeater->isRepeating())
//		a = b = false;
//	if (a && !b) {
//		d->repeater->setBToCurrentTime();
//		b = true;
//		d->repeater->start();
//		a = b = false;
//	} else if (!a) {
//		d->repeater->setAToCurrentTime();
//		d->repeater->stop();
//		a = true;
//	}
}

void MainWindow::showABRepeatDialog() {
//	static ABRepeatDialog *dlg = new ABRepeatDialog(d->repeater, this);
//	dlg->show();
}

void MainWindow::adjustSizeForDock(bool visible) {
//	if (!isFullScreen() && !d->dock->isFloating())
//		resize(width() + (visible ? 1 : -1 ) * d->dock->width(), height());
}

void MainWindow::playPause() {
	//if (d->engine->isStopped())
	//	d->model->play(d->model->currentRow());
	//else
	if (d->stream->isPlaying())
		d->stream->pause();
	else
		d->stream->play();
}

void MainWindow::increaseSpeed() {
//	d->engine->setSpeed(d->engine->speed() + 0.1);
}

void MainWindow::decreaseSpeed() {
//	d->engine->setSpeed(d->engine->speed() - 0.1);
}

void MainWindow::restoreSpeed() {
//	d->engine->setSpeed(1.0);
}

void MainWindow::doubleSpeed() {
//	d->engine->setSpeed(d->engine->speed() * 2.0);
}

void MainWindow::halfSpeed() {
//	d->engine->setSpeed(d->engine->speed() * 0.5);
}

void MainWindow::increaseVolume() {
	d->audio->setVolume(d->audio->volume() + d->pref->interface().volumeStep);
}

void MainWindow::decreaseVolume() {
	d->audio->setVolume(d->audio->volume() - d->pref->interface().volumeStep);
}

void MainWindow::forward() {
	qDebug("forward seek to %d, current %d\n", time, d->stream->currentTime());
	d->stream->seek(d->stream->currentTime() + d->pref->interface().seekingStep);
}

void MainWindow::forwardMore() {
	d->stream->seek(d->stream->currentTime() + d->pref->interface().seekingMoreStep);
}

void MainWindow::forwardMuchMore() {
	d->stream->seek(d->stream->currentTime() + d->pref->interface().seekingMuchMoreStep);
}

void MainWindow::backward() {
	d->stream->seek(d->stream->currentTime() - d->pref->interface().seekingStep);
}

void MainWindow::backwardMore() {
	d->stream->seek(d->stream->currentTime() - d->pref->interface().seekingMoreStep);
}

void MainWindow::backwardMuchMore() {
	d->stream->seek(d->stream->currentTime() - d->pref->interface().seekingMuchMoreStep);
}

void MainWindow::increaseSyncDelay() {
//	d->subout->addSyncDelay(d->pref->interface().syncDelayStep);
}

void MainWindow::decreaseSyncDelay() {
//	d->subout->addSyncDelay(-d->pref->interface().syncDelayStep);
}

void MainWindow::updatePreferences() {
//	d->info->setMPlayerPath(d->pref->general().mplayerPath);
//
//	d->engine->setOptions(d->pref->general().additionalOptions);
//
//	d->video->setDriver(d->pref->general().videoOutput);
//	d->video->setSoftwareEqualizerEnabled(d->pref->general().useSoftwareEqualizer);
//
//	d->audio->setDriver(d->pref->general().audioOutput);
//	d->audio->setEnabledSoftwareVolume(d->pref->general().useSoftwareVolume);
//	d->audio->setVolumeAmplification(d->pref->general().volumeAmplification);
//	const int initVol = d->pref->general().resetVolume ? d->pref->general().initialVolume : -1;
//	d->audio->setInitialVolume(initVol);
//
//	d->subout->setInitialPos(d->pref->subtitle().initialPos/100.0);
//	d->subout->setFont(d->pref->subtitle().font);
//	d->subout->setAutoScale(d->pref->subtitle().autoScale);
//	d->subout->setEncoding(d->pref->subtitle().encoding);
//	d->subout->setAutoSelect(d->pref->subtitle().autoSelect);
//	d->subout->setPriority(d->pref->subtitle().priority);
//	d->subout->setDisplayOnMarginEnabled(d->pref->subtitle().displayOnMarginWhenFullScreen && isFullScreen());
}

void MainWindow::updatePlayText() {
//	MPlayer::MediaSource source = d->model->currentSource();
//	QString text;
//	if (source.isValid()) {
//		text = source.displayName();
//		const int row = d->model->currentRow();
//		const int count = d->model->count();
//		if (count && row < count)
//			text += QString(" (%1/%2)").arg(row + 1).arg(count);
//		if (!text.isEmpty())
//			text += " - ";
//		if (d->engine->isPlaying())
//			text += trUtf8("재생중");
//		else if (d->engine->isPaused())
//			text += trUtf8("일시정지");
//		else
//			text += trUtf8("정지");
//	}
//	d->pmb->setPlayText(text);
}

void MainWindow::slotStarted() {
	d->initSubtitles();
	setVideoSize(1.0);
	d->resetSeekDVDMenu();
}

void MainWindow::updateFinished() {
//	MPlayer::MediaSource source = d->engine->currentSource();
//	if (source.isLocalFile())
//		d->recent->setFinished(d->engine->currentSource());
//	else if (source.isDisc())
//		d->clearSeekDVDMenu();
}

void MainWindow::updateStopped(qint64 time) {
//	MPlayer::MediaSource source = d->engine->currentSource();
//	if (source.isLocalFile())
//		d->recent->setStopped(d->engine->currentSource(), time);
//	else if (source.isDisc())
//		d->clearSeekDVDMenu();
}

void MainWindow::executeWizard() {
	//Preferences::SettingsWizard wizard(this);
	//wizard.exec();
}

void MainWindow::openDVD() {
//	d->open(MPlayer::MediaSource(MPlayer::DVD));
}

void MainWindow::seekDVDTitle(QAction */*act*/) {
//	d->engine->playTitle(act->data().toInt());
}

void MainWindow::seekDVDChapter(QAction */*act*/) {
//	d->engine->seekChapter(act->data().toInt());
}

