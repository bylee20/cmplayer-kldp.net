#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <backend/playengine.h>
#include <backend/factoryiface.h>

class MainWindow;				class PlayListModel;
class PlayMenuBar;				class PlayListDock;
class RecentInfo;				class ActionCollection;
class QTimer;

namespace Backend {class FactoryIface;}
namespace Pref {class Preferences;}

struct MainWindow::Data {
	static const int SubPosStep = 1;
	enum StaysOnTop {AlwaysOnTop, OnlyPlaying, NotStayOnTop};
	Data(MainWindow *p);
	~Data();
	void init();
	void open(Backend::MediaSource source);
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
	bool repeating, pausedByHiding, resizedByAct, changingOnTop;
	StaysOnTop staysOnTop;
	QPoint dragPos;
	Pref::Preferences *pref;
	Backend::FactoryIface *f;
	Backend::PlayEngine *engine;
	Backend::AudioOutput *audio;
	Backend::VideoOutput *video;
	Backend::SubtitleOutput *subtitle;
	PlayListModel *model;
	RecentInfo *recent;
	PlayMenuBar *pmb;
	QActionGroup *onTopActions, *videoSizeActions, *videoAspectActions;
	QActionGroup *videoCropActions, *subListActions, *subChannelsActions;
	QList<QAction *> recentActions;
	PlayListDock *dock;
	Backend::ABRepeater *repeater;
	QMenu *contextMenu;
	QMap<int, QAction*> mouseClickActions;
	QMap<int, QPair<QAction*, QAction*> > wheelScrollActions;
	QAction *seekDVDAction;
	QTimer *cursorTimer;
};



#endif
