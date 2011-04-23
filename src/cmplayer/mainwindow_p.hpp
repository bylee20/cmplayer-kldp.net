#ifndef MAINWINDOW_P_HPP
#define MAINWINDOW_P_HPP

#include "avmisc.hpp"
#include "timelineosdrenderer.hpp"
#include "playinfoview.hpp"
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
	PlayInfoView *playInfo;
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
		const QList<SubtitleRenderer::Loaded> loaded = subtitle->loaded();
		while (g->actions().size() < loaded.size()) {
			list.addActionToGroupWithoutKey("", true);
		}
		while (g->actions().size() > loaded.size()) {
			delete g->actions().last();
		}
		const QList<QAction*> actions = g->actions();
		Q_ASSERT(loaded.size() == actions.size());
		for (int i=0; i<actions.size(); ++i) {
			actions[i]->setText(loaded[i].name());
			actions[i]->setData(i);
			actions[i]->setChecked(loaded[i].isSelected());
		}
		changingSub = false;
	}

	void load_state() {
		dontShowMsg = true;
		AppState as;
		as.load();

		menu("video")("aspect").g()->trigger(as[AppState::AspectRatio]);
		menu("video")("crop").g()->trigger(as[AppState::Crop]);
		menu("window").g("sot")->trigger(StaysOnTopEnum::value(as[AppState::StaysOnTop].toString()));
		menu("subtitle").g("display")->trigger((int)as[AppState::SubLetterbox].toBool());
		menu("subtitle").g("align")->trigger((int)as[AppState::SubAlignTop].toBool());

		audio->setVolume(as[AppState::Volume].toInt());
		audio->setMuted(as[AppState::Muted].toBool());
		audio->setPreAmp(as[AppState::Amp].toDouble());
		audio->setVolumeNormalized(as[AppState::VolNorm].toBool());

		engine->setSpeed(as[AppState::PlaySpeed].toDouble());
		subtitle->setPos(as[AppState::SubPos].toDouble());
		subtitle->setDelay(as[AppState::SubSync].toInt());
		dontShowMsg = false;
	}

	void save_state() const {
		AppState as;
		as[AppState::AspectRatio] = video->aspectRatio();
		as[AppState::Crop] = video->cropRatio();
		as[AppState::Volume] = audio->volume();
		as[AppState::VolNorm] = audio->isVolumeNormalized();
		as[AppState::Muted] = audio->isMuted();
		as[AppState::Amp] = audio->preAmp();
		as[AppState::PlaySpeed] = engine->speed();
		as[AppState::SubPos] = subtitle->pos();
		as[AppState::SubSync] = subtitle->delay();
		as[AppState::StaysOnTop] = StaysOnTopEnum::name(stay_on_top_mode());
		as[AppState::SubLetterbox] = subtitle->osd()->letterboxHint();
		as[AppState::SubAlignTop] = subtitle->isTopAligned();
		as.save();
	}

	StaysOnTop stay_on_top_mode() const {
		const int data = menu("window").g("sot")->checkedAction()->data().toInt();
		switch (data) {
		case OnTopPlaying:
			return OnTopPlaying;
		case AlwaysOnTop:
			return AlwaysOnTop;
		case DontStayOnTop:
			return DontStayOnTop;
		default:
			return OnTopPlaying;
		}
	}

	void apply_pref() {
		Subtitle::Parser::setMsPerCharactor(pref.ms_per_char);
		Translator::load(pref.locale);
		app()->setStyle(pref.window_style);
		subtitle->osd()->setStyle(pref.sub_style);
		menu.updatePref();
	#ifndef Q_WS_MAC
		tray->setVisible(pref.enableSystemTray);
	#endif
		control->setState(engine->state());
	}
};

#endif // MAINWINDOW_P_HPP
