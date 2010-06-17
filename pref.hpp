#ifndef PREF_HPP
#define PREF_HPP

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QLocale>
#include <QtCore/QPair>
#include <QtCore/QStringList>
//#include <core/osdstyle.h>
#include <QtCore/QSettings>
#include "global.hpp"

class QLocale;

namespace Core {class OsdStyle;}

class Pref {
public:
	static const Pref &get() {return ref();}
	typedef QPair<bool, ClickActionEnum> ClickActionPair;
	typedef QPair<bool, WheelActionEnum> WheelActionPair;
	typedef QMap<ModifierEnum, ClickActionPair> ClickActionMap;
	typedef QMap<ModifierEnum, WheelActionPair> WheelActionMap;
	static const int DefaultSeekingStep1 = 10000;
	static const int DefaultSeekingStep2 = 30000;
	static const int DefaultSeekingStep3 = 60000;
	static const int DefaultVolumeStep = 2;
	static const int DefaultSyncDelayStep = 500;
	static const int DefaultAmpStep = 10;
	static const int DefaultSubPosStep = 1;
	static const int DefaultSpeedStep = 10;
	static const int DefaultColorPropStep = 1;

	bool rememberStopped, playRestored, pauseMinimized, pauseVideoOnly;
	bool askWhenRecordFound;
	bool hideCursor, hideInFullScreen, normalizeVolume, useSoftwareEqualizer;
	bool enableSystemTray, hideClosed, singleApplication;
	bool disableScreensaver, useSubtitleEncodingAutoDetection;
	AutoAddFilesEnum autoAddFiles;
//	Core::OsdStyle subtitleStyle;
	QString subtitleEncoding, windowStyle, subtitleExtension;
	QStringList subtitlePriority;
	SubtitleAutoLoadEnum subtitleAutoLoad;
	SubtitleAutoSelectEnum subtitleAutoSelect;
	int msPerChar;
	int daysToKeepRecords, maximumRecordCount;
	int seekingStep1, seekingStep2, seekingStep3, speedStep;
	int volumeStep, syncDelayStep, ampStep, subtitlePosStep;
	int brightnessStep, saturationStep, contrastStep, hueStep;
	int hideDelay, subtitleEncodingConfidence;
	ClickActionMap doubleClickMap, middleClickMap;
	WheelActionMap wheelScrollMap;
	QLocale locale;

	void save() const;
	void load();
	class Dialog;
private:
	Pref() {load();}
	static Pref &ref();
	template<typename T>
	void saveMouse(QSettings &set, const QString &group, const T &t) const {
		set.beginGroup(group);
		for (typename T::const_iterator it = t.begin(); it != t.end(); ++it) {
			set.beginGroup(it.key().name());
			set.setValue("Enabled", it.value().first);
			set.setValue("Action", it.value().second.name());
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
		value.first = set.value("Enabled", def.first).toBool();
		value.second = T::mapped_type::second_type::value(set.value("Action"
				, def.second.name()).toString(), def.second);
		t[key] = value;
		set.endGroup();
		set.endGroup();
	}
};


#endif // PREF_HPP
