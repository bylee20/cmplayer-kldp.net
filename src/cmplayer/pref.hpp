#ifndef PREF_HPP
#define PREF_HPP

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QLocale>
#include <QtCore/QPair>
#include <QtCore/QStringList>
#include "osdstyle.hpp"
#include <QtCore/QSettings>
#include "global.hpp"

class QLocale;

class Pref {
public:
	static const Pref &get() {return ref();}
	typedef QPair<bool, ClickActionEnum> ClickActionPair;
	typedef QPair<bool, WheelActionEnum> WheelActionPair;
	typedef QMap<ModifierEnum, ClickActionPair> ClickActionMap;
	typedef QMap<ModifierEnum, WheelActionPair> WheelActionMap;
	static const int DefaultSeekingStep1 = 5000;
	static const int DefaultSeekingStep2 = 30000;
	static const int DefaultSeekingStep3 = 60000;
	static const int DefaultVolumeStep = 2;
	static const int DefaultSyncDelayStep = 500;
	static const int DefaultAmpStep = 10;
	static const int DefaultSubPosStep = 1;
	static const int DefaultSpeedStep = 10;
	static const int DefaultColorPropStep = 1;

	AutoAddFilesEnum auto_add_files;
	bool remember_stopped, pause_minimized, pause_video_only;
	bool ask_record_found, hide_cursor, disable_screensaver;
	bool enable_system_tray, hide_rather_close, single_app;;
	bool sub_enc_autodetection;

	int blur_kern_c, blur_kern_n, blur_kern_d;
	int sharpen_kern_c, sharpen_kern_n, sharpen_kern_d;
	int adjust_contrast_min_luma, adjust_contrast_max_luma;
	int normalizer_gain, normalizer_smoothness;
	double auto_contrast_threshold;

	SubtitleAutoLoadEnum sub_autoload;
	SubtitleAutoSelectEnum sub_autoselect;
	OsdStyle sub_style;
	QString sub_enc, sub_ext;
	QStringList sub_priority;
	int ms_per_char;
	int seek_step1, seek_step2, seek_step3, speed_step;
	int volume_step, sync_delay_step, amp_step, sub_pos_step;
	int brightness_step, saturation_step, contrast_step, hue_step;
	int hide_delay, sub_enc_confidence;
	ClickActionMap double_click_map, middle_click_map;
	WheelActionMap wheel_scroll_map;

	QString window_style;
	QLocale locale;

	void save() const;
	void load();
	class Dialog;
	class MacDialog;
	class Widget;
private:
	Pref() {load();}
	static Pref &ref();
	template<typename T>
	void saveMouse(QSettings &set, const QString &group, const T &t) const {
		set.beginGroup(group);
		for (typename T::const_iterator it = t.begin(); it != t.end(); ++it) {
			set.beginGroup(it.key().name());
			set.setValue("enabled", it.value().first);
			set.setValue("action", it.value().second.name());
			set.endGroup();
		}
		set.endGroup();
	}
	template<typename T>
	void loadMouse(QSettings &set, const QString &group, T &t
			, const typename T::key_type &key, const typename T::mapped_type &def) {
		set.beginGroup(group);
		set.beginGroup(key.name());
		typename T::mapped_type value;
		value.first = set.value("enabled", def.first).toBool();
		value.second = T::mapped_type::second_type::value(set.value("action"
				, def.second.name()).toString(), def.second);
		t[key] = value;
		set.endGroup();
		set.endGroup();
	}
};


#endif // PREF_HPP
