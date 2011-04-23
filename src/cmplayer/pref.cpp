#include "pref.hpp"
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QLocale>

Pref &Pref::ref() {
	static Pref self;
	return self;
}

#define SAVE(value) (set.setValue((#value), (value)))
#define LOAD(val, def, converter) (val = set.value(#val, def).converter())
#define SAVE_ENUM(val) (set.setValue(#val, val.name()))
#define LOAD_ENUM(val, def) (val = val.value(set.value(#val, #def).toString(), def))

void Pref::save() const {
	QSettings set;
	set.beginGroup("preference");

	SAVE(remember_stopped);
	SAVE(ask_record_found);
	SAVE(pause_minimized);
	SAVE(pause_video_only);
	SAVE(hide_cursor);
	SAVE(hide_delay);
	SAVE(enable_system_tray);
	SAVE(hide_rather_close);
	SAVE(single_app);
	SAVE(disable_screensaver);
	SAVE(sub_enc);
	SAVE(sub_priority);
	SAVE(sub_enc_autodetection);
	SAVE(sub_enc_confidence);
	SAVE(ms_per_char);
	SAVE(seek_step1);
	SAVE(seek_step2);
	SAVE(seek_step3);
	SAVE(speed_step);
	SAVE(volume_step);
	SAVE(amp_step);
	SAVE(sub_pos_step);
	SAVE(volume_step);
	SAVE(sync_delay_step);
	SAVE(brightness_step);
	SAVE(saturation_step);
	SAVE(contrast_step);
	SAVE(hue_step);
	SAVE(locale);
	SAVE(window_style);
	SAVE(sub_ext);
	SAVE(blur_kern_c);
	SAVE(blur_kern_n);
	SAVE(blur_kern_d);
	SAVE(sharpen_kern_c);
	SAVE(sharpen_kern_n);
	SAVE(sharpen_kern_d);
	SAVE(adjust_contrast_min_luma);
	SAVE(adjust_contrast_max_luma);
	SAVE(auto_contrast_threshold);

	SAVE_ENUM(auto_add_files);
	SAVE_ENUM(sub_autoload);
	SAVE_ENUM(sub_autoselect);

	sub_style.save(&set, "sub_style");
	saveMouse(set, "double_click_map", double_click_map);
	saveMouse(set, "middle_click_map", middle_click_map);
	saveMouse(set, "wheel_scroll_map", wheel_scroll_map);

	set.endGroup();
}

void Pref::load() {
	QSettings set;
	set.beginGroup("preference");

	LOAD(remember_stopped, true, toBool);
	LOAD(ask_record_found, true, toBool);
	LOAD(pause_minimized, true, toBool);
	LOAD(pause_video_only, true, toBool);
	LOAD(hide_cursor, true, toBool);
	LOAD(hide_delay, 3000, toInt);
	LOAD(blur_kern_c, 1, toInt);
	LOAD(blur_kern_n, 2, toInt);
	LOAD(blur_kern_d, 1, toInt);
	LOAD(sharpen_kern_c, 5, toInt);
	LOAD(sharpen_kern_n, -1, toInt);
	LOAD(sharpen_kern_d, 0, toInt);
	LOAD(adjust_contrast_min_luma, 16, toInt);
	LOAD(adjust_contrast_max_luma, 235, toInt);
	LOAD(auto_contrast_threshold, 0.5, toDouble);

	LOAD(enable_system_tray, true, toBool);
	LOAD(hide_rather_close, true, toBool);
	LOAD(single_app, true, toBool);
	LOAD(disable_screensaver, true, toBool);
	LOAD(locale, QLocale::system(), toLocale);
	if (this->locale.language() == QLocale::Korean)
		LOAD(sub_enc, "CP949", toString);
	else
		LOAD(sub_enc, "UTF-8", toString);
	LOAD(sub_enc_autodetection, true, toBool);
	LOAD(sub_enc_confidence, 70, toInt);
	LOAD(ms_per_char, 500, toInt);
	LOAD(sub_priority, QStringList(), toStringList);
	LOAD(seek_step1, DefaultSeekingStep1, toInt);
	LOAD(seek_step2, DefaultSeekingStep2, toInt);
	LOAD(seek_step3, DefaultSeekingStep3, toInt);
	LOAD(speed_step, DefaultSpeedStep, toInt);
	LOAD(volume_step, DefaultVolumeStep, toInt);
	LOAD(amp_step, DefaultAmpStep, toInt);
	LOAD(sub_pos_step, DefaultSubPosStep, toInt);
	LOAD(sync_delay_step, DefaultSyncDelayStep, toInt);
	LOAD(brightness_step, DefaultColorPropStep, toInt);
	LOAD(saturation_step, DefaultColorPropStep, toInt);
	LOAD(contrast_step, DefaultColorPropStep, toInt);
	LOAD(hue_step, DefaultColorPropStep, toInt);
	LOAD(window_style, QString(), toString);
	LOAD(sub_ext, QString(), toString);

	LOAD_ENUM(auto_add_files, AllFiles);
	LOAD_ENUM(sub_autoload, Contain);
	LOAD_ENUM(sub_autoselect, SameName);

//	sub_style.alignment = Qt::AlignHCenter | Qt::AlignBottom;
	sub_style.border_width = 0.045;
	sub_style.text_scale = 0.035;
	sub_style.has_shadow = true;
	sub_style.shadow_color = Qt::black;
	sub_style.shadow_offset = QPointF(0, 0);
	sub_style.shadow_blur = 3;
	sub_style.font.setBold(true);
	sub_style.load(&set, "sub_style");
	loadMouse(set, "double_click_map", double_click_map
			, Qt::NoModifier, ClickActionPair(true, ToggleFullScreen));
	loadMouse(set, "double_click_map", double_click_map
			, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "double_click_map", double_click_map
			, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "double_click_map", double_click_map
			, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "middle_click_map", middle_click_map
			, Qt::NoModifier, ClickActionPair(true, TogglePlayPause));
	loadMouse(set, "middle_click_map", middle_click_map
			, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "middle_click_map", middle_click_map
			, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "middle_click_map", middle_click_map
			, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "wheel_scroll_map", wheel_scroll_map
			, Qt::NoModifier, WheelActionPair(true, VolumeUpDown));
	loadMouse(set, "wheel_scroll_map", wheel_scroll_map
			, Qt::AltModifier, WheelActionPair(false, VolumeUpDown));
	loadMouse(set, "wheel_scroll_map", wheel_scroll_map
			, Qt::ControlModifier, WheelActionPair(true, AmpUpDown));
	loadMouse(set, "wheel_scroll_map", wheel_scroll_map
			, Qt::ShiftModifier, WheelActionPair(false, VolumeUpDown));
	set.endGroup();
}
