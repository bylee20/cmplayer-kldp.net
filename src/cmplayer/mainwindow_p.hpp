#ifndef MAINWINDOW_P_HPP
#define MAINWINDOW_P_HPP

#include "mainwindow.hpp"
#include "menu.hpp"
#include "pref.hpp"
#include "playengine.hpp"
#include "videoscene.hpp"
#include "audiocontroller.hpp"
#include "subtitlerenderer.hpp"
#include "subtitleview.hpp"
#include "recentinfo.hpp"
#include "timelineosdrenderer.hpp"
#include "playinfoview.hpp"
#include "abrepeater.hpp"
#include "playlistview.hpp"
#include "historyview.hpp"
#include "appstate.hpp"
#include "translator.hpp"
#include "subtitle_parser.hpp"

struct MainWindow::Data {
	bool moving, changingSub, pausedByHiding, dontShowMsg, dontPause;
	QMenu *context;
	RootMenu menu;			const Pref &pref;
	PlayEngine *engine;		VideoScene *video;
	SubtitleRenderer *subtitle;	AudioController *audio;
	TimeLineOsdRenderer *timeLine;	TextOsdRenderer *message;
	PlayInfoView *playInfo;
	QPoint prevPos;			QTimer *hider;
	RecentInfo recent;		ABRepeater *ab;
	PlaylistView *playlist;		HistoryView *history;
//	FavoritesView *favorite;
#ifndef Q_WS_MAC
	QSystemTrayIcon *tray;
#endif
// methods
	Data(): pref(Pref::get()) {}

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
		const AppState &as = AppState::get();
		menu("video")("aspect").g()->trigger(as.aspect_ratio);
		menu("video")("crop").g()->trigger(as.crop_ratio);
		menu("video")("overlay").g()->trigger(as.overlay.id());
		menu("subtitle").g("display")->trigger((int)as.sub_letterbox);
		menu("subtitle").g("align")->trigger((int)as.sub_align_top);
		menu("window")["simple-mode"]->setChecked(as.simple_mode);
		menu("window").g("sot")->trigger(as.stays_on_top.id());

		audio->setVolume(as.volume);
		audio->setMuted(as.muted);
		audio->setPreAmp(as.amp);
		audio->setVolumeNormalized(as.volume_normalized);

		engine->setSpeed(as.speed);
		subtitle->setPos(as.sub_pos);
		subtitle->setDelay(as.sub_sync_delay);
		dontShowMsg = false;
	}

	void save_state() const {
		AppState &as = AppState::get();
		as.aspect_ratio = video->aspectRatio();
		as.crop_ratio = video->cropRatio();
		as.volume = audio->volume();
		as.volume_normalized = audio->isVolumeNormalized();
		as.muted = audio->isMuted();
		as.amp = audio->preAmp();
		as.speed = engine->speed();
		as.sub_pos = subtitle->pos();
		as.sub_sync_delay = subtitle->delay();
		as.stays_on_top = stay_on_top_mode();
		as.sub_letterbox = subtitle->osd()->letterboxHint();
		as.sub_align_top = subtitle->isTopAligned();
		as.simple_mode = menu("window")["simple-mode"]->isChecked();
		QAction *act = menu("video")("overlay").g()->checkedAction();
		if (act)
			as.overlay.set(act->data().toInt());
		as.save();
	}

	Enum::StaysOnTop stay_on_top_mode() const {
		const int id = menu("window").g("sot")->checkedAction()->data().toInt();
		return Enum::StaysOnTop::from(id, Enum::StaysOnTop::Playing);
	}

	static VideoScene::SkinMode skin_mode(bool simple, bool /*fullscreen*/) {
//		if (fullscreen)
//			return VideoScene::AutoSkin;
		return simple ? VideoScene::NeverSkin : VideoScene::AlwaysSkin;
	}

	void apply_pref() {
		Subtitle::Parser::setMsPerCharactor(pref.ms_per_char);
		Translator::load(pref.locale);
		subtitle->osd()->setStyle(pref.sub_style);
		audio->setTargetGain((double)pref.normalizer_gain/100.0);
		audio->setNormalizerSmoothness(pref.normalizer_smoothness);
		menu.update();
		menu.save();
	#ifndef Q_WS_MAC
		tray->setVisible(pref.enable_system_tray);
	#endif
	}
};

#endif // MAINWINDOW_P_HPP
