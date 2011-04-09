#ifndef MAINWINDOW_P_HPP
#define MAINWINDOW_P_HPP

#include "timelineosdrenderer.hpp"
#include "colorproperty.hpp"
#include "playlistview.hpp"
#include "historyview.hpp"
#include "playlistmodel.hpp"
#include "aboutdialog.hpp"
#include "subtitlerenderer.hpp"
#include "charsetdetector.hpp"
#include "snapshotdialog.hpp"
#include "subtitle_parser.hpp"
#include "audiocontroller.hpp"
#include "controlwidget.hpp"
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
#include "info.hpp"
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <qmath.h>

class MainWindowData {
	bool moving, changingSub, pausedByHiding, dontShowMsg, dontPause;
	Menu &menu;			const Pref &pref;
	ControlWidget *control;		QWidget *center;
	PlayEngine *engine;		VideoRenderer *video;
	SubtitleRenderer *subtitle;	AudioController *audio;
	TimeLineOsdRenderer *timeLine;	TextOsdRenderer *message;
//	Subtitle subLoaded;		QList<int> subSelected;
	QPoint prevPos;			QTimer *hider;
	RecentInfo *recent;		ABRepeater *ab;
	PlaylistView *playlist;		HistoryView *history;
//	FavoritesView *favorite;
#ifndef Q_WS_MAC
	QSystemTrayIcon *tray;
#endif
	friend class MainWindow;
// methods
	MainWindowData(Menu &menu): menu(menu), pref(Pref::get()) {}
	void sync_subtitle_file_menu() {
		if (changingSub)
			return;
		changingSub = true;
		Menu &list = menu("subtitle")("list");
		ActionGroup *g = list.g();
		const Subtitle &loaded = subtitle->loaded();
		while (g->actions().size() < loaded.size()) {
			list.addActionToGroupWithoutKey("", true);
		}
		while (g->actions().size() > loaded.size()) {
			delete g->actions().last();
		}
		const QList<QAction*> actions = g->actions();
		const QList<bool> &selection = subtitle->selection();
		Q_ASSERT(loaded.size() == actions.size());
		Q_ASSERT(loaded.size() == selection.size());
		for (int i=0; i<actions.size(); ++i) {
			actions[i]->setText(loaded[i].name());
			actions[i]->setData(i);
			actions[i]->setChecked(selection[i]);
		}
		changingSub = false;
	}
};

#endif // MAINWINDOW_P_HPP
