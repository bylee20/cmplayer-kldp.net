#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "ui/ui_mainwindow.h"
#include "mainwindow.h"
#include <xine/xineengine.h>
class MainWindow;				//class PlayListModel;
class PlayMenuBar;				//class PlayListDock;
class RecentInfo;				class ActionCollection;

//namespace MPlayer {
//class MediaSource;				class VideoOutput;
//class AudioOutput;				class PlayEngine;
//class SubtitleOutput;			class Informations;
//class ABRepeater;
//}

namespace Pref {class Preferences;}

struct MainWindow::Data {
	static const int SubPosStep = 1;
	enum StaysOnTop {AlwaysOnTop, OnlyPlaying, NotStayOnTop};
	Data(MainWindow *p);
	~Data();
	void init();
	void open(Xine::MediaSource source);
	QMenu *findMenuIn(QMenu *menu, const QString &title);
	void initSubtitles();
	void updateStaysOnTop();
	void checkClose();
	void resetSeekDVDMenu();
	void clearSeekDVDMenu();
private:
	void createConnections();
	void registerActions();
	void setupGUI();
public:
	MainWindow *p;
	Ui::Ui_MainWindow ui;
	bool dragMove, repeating, pausedByHiding, resizedByAct, changingOnTop;
	StaysOnTop staysOnTop;
	QPoint dragPos;
	Pref::Preferences *pref;
	Xine::XineEngine *engine;
//	MPlayer::PlayEngine *engine;
//	MPlayer::VideoOutput *video;
//	MPlayer::AudioOutput *audio;
//	MPlayer::SubtitleOutput *subout;
//	PlayListModel *model;
//	MPlayer::Informations *info;
	RecentInfo *recent;
	PlayMenuBar *pmb;
	QActionGroup *onTopActions, *videoSizeActions, *videoAspectActions;
	QActionGroup *videoCropActions, *subListActions;
	QActionGroup *seekChapterActions, *seekTitleActions;
	QList<QAction *> recentActions;
//	PlayListDock *dock;
//	MPlayer::ABRepeater *repeater;
	QMenu *contextMenu;
	QMap<int, QAction*> mouseClickActions;
	QMap<int, QPair<QAction*, QAction*> > wheelScrollActions;
	QAction *seekDVDAction;
};



#endif
