#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <backend/backend.h>

class RecentStack;

namespace Backend {class MediaSource;}

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void setFullScreen(bool full);
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void showEvent(QShowEvent *event);
private slots:
	void openFile();
	void openDVD();
	void openRecent();
	void changeAspectRatio(QAction *act);
	void changeStaysOnTop(QAction *act);
	void slotStateChanged(Backend::State state);
	void slotResized();
	void setVideoSize(double rate);
	void changeVideoSize(QAction *act);
	void changeCurrentSubtitles(QAction *act);
	void updateSubtitles(const QStringList &names);
	void changeCurrentSubChannel(QAction *act);
	void updateCurrentSubChannel(int index);
	void updateSubChannels(const QStringList &channels);
	void updateCurrentSubtitleIndexes(const QList<int> &indexes);
	void showPreferencesDialog();
	void clearSubtitleList();
	void addSubtitles();
	void showEqualizer();
	void crop(QAction *act);
	void updateSyncDelay(int msec);
	void updateRecentActions(const RecentStack &files);
	void updateRecentSize(int size);
	void clearRecentFiles();
	void showMenu(const QPoint &pos);
	void showAboutDialog();
	void selectABSection();
	void showABRepeatDialog();
	void togglePlayListVisibility();
	void adjustSizeForDock(bool visible);
	void updateSpeed(double speed);

	void playPause();
	void increaseVolume();
	void decreaseVolume();
	void forward();
	void forwardMore();
	void forwardMuchMore();
	void backward();
	void backwardMore();
	void backwardMuchMore();
	void increaseSpeed();
	void decreaseSpeed();
	void restoreSpeed();
	void doubleSpeed();
	void halfSpeed();
	void increaseSyncDelay();
	void decreaseSyncDelay();

	void updatePlayText();
	void updatePreferences();
	void executeWizard();
	void slotStarted();
	void slotFullScreen();
	void updateSource(const Backend::MediaSource &source);
	void updateFinished(const Backend::MediaSource &source);
	void updateStopped(const Backend::MediaSource &source, int time);
	void hideCursor();
private:
	Q_DISABLE_COPY(MainWindow)
	class Data;
	friend class Data;
	Data *d;
};

#endif
