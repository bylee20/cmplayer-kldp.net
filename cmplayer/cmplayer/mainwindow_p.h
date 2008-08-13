#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "ui/ui_mainwindow.h"
#include "mainwindow.h"
#include <xine/xinestream.h>

class MainWindow;				class PlayListModel;
class PlayMenuBar;				class PlayListDock;
class RecentInfo;				class ActionCollection;

namespace Xine {class Informations;}
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
	Xine::XineStream *stream;
	Xine::AudioOutput *audio;
	Xine::VideoOutput *video;
	Xine::SubtitleOutput *subout;
	PlayListModel *model;
	Xine::Informations *info;
	RecentInfo *recent;
	PlayMenuBar *pmb;
	QActionGroup *onTopActions, *videoSizeActions, *videoAspectActions;
	QActionGroup *videoCropActions, *subListActions, *subChannelsActions;
	QList<QAction *> recentActions;
	PlayListDock *dock;
	Xine::ABRepeater *repeater;
	QMenu *contextMenu;
	QMap<int, QAction*> mouseClickActions;
	QMap<int, QPair<QAction*, QAction*> > wheelScrollActions;
	QAction *seekDVDAction;
};



#endif
