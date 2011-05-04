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
#include "enums.hpp"

class QLocale;

class Pref {
public:
	static void init();
	static void fin();
	static const Pref &get() {Q_ASSERT(obj != 0); return *obj;}
	template <typename Enum>
	struct ActionEnumInfo {
		typedef Enum EnumType;
		ActionEnumInfo(): enabled(false) {}
		ActionEnumInfo(bool e, const Enum &a): enabled(e), action(a) {}
		bool enabled; Enum action;
	};
	typedef ActionEnumInfo<Enum::ClickAction> ClickActionInfo;
	typedef ActionEnumInfo<Enum::WheelAction> WheelActionInfo;
	template <typename Enum>
	struct KeyModifierMap {
		typedef ::Enum::KeyModifier Modifier;
		typedef ActionEnumInfo<Enum> InfoType;
		typedef QMap<int, InfoType> Map;
		KeyModifierMap(bool enabled = false, const Enum &e = Enum()) {
			const InfoType def(enabled, e);
			const Modifier::List &list = Modifier::list();
			for (int i=0; i<list.size(); ++i) m_map[list[i].id()] = def;
		}
		InfoType &operator[](const Modifier &m) {return m_map[m.id()];}
		const InfoType operator[](const Modifier &m) const {return m_map[m.id()];}
		const InfoType operator[](int id) const {return m_map.value(id);}
		void save(QSettings &set, const QString &group) const {
			set.beginGroup(group);
			const Modifier::List &list = Modifier::list();
			for (int i=0; i<list.size(); ++i) {
				const InfoType &info = m_map[list[i].id()];
				set.beginGroup(list[i].name());
				set.setValue("enabled", info.enabled);
				set.setValue("action", info.action.name());
				set.endGroup();
			}
			set.endGroup();
		}
		void load(QSettings &set, const QString &group, const KeyModifierMap &def) {
			set.beginGroup(group);
			const Modifier::List &list = Modifier::list();
			for (int i=0; i<list.size(); ++i) {
				InfoType &info = m_map[list[i].id()];
				const InfoType &d = def[list[i]];
				set.beginGroup(list[i].name());
				info.enabled = set.value("enabled", d.enabled).toBool();
				info.action.set(set.value("action", d.action.name()).toString());
				set.endGroup();
			}
			set.endGroup();
		}
	private:
		Map m_map;
	};
	typedef KeyModifierMap<Enum::ClickAction> ClickActionMap;
	typedef KeyModifierMap<Enum::WheelAction> WheelActionMap;
	static const int DefaultSeekingStep1 = 5000;
	static const int DefaultSeekingStep2 = 30000;
	static const int DefaultSeekingStep3 = 60000;
	static const int DefaultVolumeStep = 2;
	static const int DefaultSyncDelayStep = 500;
	static const int DefaultAmpStep = 10;
	static const int DefaultSubPosStep = 1;
	static const int DefaultSpeedStep = 10;
	static const int DefaultColorPropStep = 1;

	Enum::GeneratePlaylist generate_playlist;
	bool remember_stopped, pause_minimized, pause_video_only;
	bool ask_record_found, hide_cursor, disable_screensaver;
	bool enable_system_tray, hide_rather_close, single_app;;
	bool sub_enc_autodetection;

	int blur_kern_c, blur_kern_n, blur_kern_d;
	int sharpen_kern_c, sharpen_kern_n, sharpen_kern_d;
	int adjust_contrast_min_luma, adjust_contrast_max_luma;
	int normalizer_gain, normalizer_smoothness;
	double auto_contrast_threshold;

	Enum::SubtitleAutoload sub_autoload;
	Enum::SubtitleAutoselect sub_autoselect;
	OsdStyle sub_style;
	QString sub_enc, sub_ext;
	QStringList sub_priority;
	int ms_per_char;
	int seek_step1, seek_step2, seek_step3, speed_step;
	int volume_step, sync_delay_step, amp_step, sub_pos_step;
	int brightness_step, saturation_step, contrast_step, hue_step;
	int hide_delay, sub_enc_accuracy;
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
	static Pref *obj;
	Pref() {load();}
};


#endif // PREF_HPP
