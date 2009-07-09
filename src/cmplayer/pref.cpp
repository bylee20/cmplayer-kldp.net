#include "pref.h"
#include "helper.h"
#include "videoplayer.h"
#include <core/osdstyle.h>
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
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	set.beginGroup("Pref");
	
	SAVE(rememberStopped);
	SAVE(askWhenRecordFound);
	SAVE(daysToKeepRecords);
	SAVE(maximumRecordCount);
	SAVE(playRestored);
	SAVE(pauseMinimized);
	SAVE(pauseVideoOnly);
	SAVE(hideCursor);
	SAVE(hideInFullScreen);
	SAVE(hideDelay);
	SAVE(normalizeVolume);
	SAVE(useSoftwareEqualizer);
	SAVE(enableSystemTray);
	SAVE(hideClosed);
	SAVE(singleApplication);
	SAVE(disableScreensaver);
	SAVE(subtitleEncoding);
	SAVE(subtitlePriority);
	SAVE(useSubtitleEncodingAutoDetection);
	SAVE(subtitleEncodingConfidence);
	SAVE(seekingStep1);
	SAVE(seekingStep2);
	SAVE(seekingStep3);
	SAVE(speedStep);
	SAVE(volumeStep);
	SAVE(ampStep);
	SAVE(subtitlePosStep);
	SAVE(volumeStep);
	SAVE(syncDelayStep);
	SAVE(brightnessStep);
	SAVE(saturationStep);
	SAVE(contrastStep);
	SAVE(hueStep);
	SAVE(locale);
	SAVE(windowStyle);
	SAVE(subtitleExtension);
	
	SAVE_ENUM(autoAddFiles);
	SAVE_ENUM(subtitleAutoLoad);
	SAVE_ENUM(subtitleAutoSelect);

	subtitleStyle.save(&set, "SubtitleStyle");
	saveMouse(set, "DoubleClickAction", doubleClickMap);
	saveMouse(set, "MiddleClickAction", middleClickMap);
	saveMouse(set, "WheelScrollAction", wheelScrollMap);
	
	set.endGroup();
}

void Pref::load() {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	set.beginGroup("Pref");
	
	LOAD(rememberStopped, true, toBool);
	LOAD(askWhenRecordFound, true, toBool);
	LOAD(daysToKeepRecords, 365, toInt);
	LOAD(maximumRecordCount, 1000, toInt);
	LOAD(playRestored, true, toBool);
	LOAD(pauseMinimized, true, toBool);
	LOAD(pauseVideoOnly, true, toBool);
	LOAD(hideCursor, true, toBool);
	LOAD(hideInFullScreen, true, toBool);
	LOAD(hideDelay, 3000, toInt);
	LOAD(normalizeVolume, true, toBool);
	LOAD(useSoftwareEqualizer, false, toBool);
	LOAD(enableSystemTray, true, toBool);
	LOAD(hideClosed, true, toBool);
	LOAD(singleApplication, true, toBool);
	LOAD(disableScreensaver, true, toBool);
	LOAD(subtitleEncoding, "UTF-8", toString);
	LOAD(useSubtitleEncodingAutoDetection, true, toBool);
	LOAD(subtitleEncodingConfidence, 70, toInt);
	LOAD(subtitlePriority, QStringList(), toStringList);
	LOAD(seekingStep1, DefaultSeekingStep1, toInt);
	LOAD(seekingStep2, DefaultSeekingStep2, toInt);
	LOAD(seekingStep3, DefaultSeekingStep3, toInt);
	LOAD(speedStep, DefaultSpeedStep, toInt);
	LOAD(volumeStep, DefaultVolumeStep, toInt);
	LOAD(ampStep, DefaultAmpStep, toInt);
	LOAD(subtitlePosStep, DefaultSubPosStep, toInt);
	LOAD(syncDelayStep, DefaultSyncDelayStep, toInt);
	LOAD(brightnessStep, DefaultColorPropStep, toInt);
	LOAD(saturationStep, DefaultColorPropStep, toInt);
	LOAD(contrastStep, DefaultColorPropStep, toInt);
	LOAD(hueStep, DefaultColorPropStep, toInt);
	LOAD(locale, QLocale::c(), toLocale);
	LOAD(windowStyle, QString(), toString);
	LOAD(subtitleExtension, QString(), toString);
	
	LOAD_ENUM(autoAddFiles, AllFiles);
	LOAD_ENUM(subtitleAutoLoad, Contain);
	LOAD_ENUM(subtitleAutoSelect, SameName);

	subtitleStyle.alignment = Qt::AlignHCenter | Qt::AlignBottom;
	subtitleStyle.borderWidth = 0.03;
	subtitleStyle.textSize = 0.04;
	subtitleStyle.font.setBold(true);
	subtitleStyle.load(&set, "SubtitleStyle");
	loadMouse(set, "DoubleClickAction", doubleClickMap
			, Qt::NoModifier, ClickActionPair(true, ToggleFullScreen));
	loadMouse(set, "DoubleClickAction", doubleClickMap
			, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "DoubleClickAction", doubleClickMap
			, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "DoubleClickAction", doubleClickMap
			, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "MiddleClickAction", middleClickMap
			, Qt::NoModifier, ClickActionPair(true, TogglePlayPause));
	loadMouse(set, "MiddleClickAction", middleClickMap
			, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "MiddleClickAction", middleClickMap
			, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "MiddleClickAction", middleClickMap
			, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(set, "WheelScrollAction", wheelScrollMap
			, Qt::NoModifier, WheelActionPair(true, VolumeUpDown));
	loadMouse(set, "WheelScrollAction", wheelScrollMap
			, Qt::AltModifier, WheelActionPair(false, VolumeUpDown));
	loadMouse(set, "WheelScrollAction", wheelScrollMap
			, Qt::ControlModifier, WheelActionPair(true, AmpUpDown));
	loadMouse(set, "WheelScrollAction", wheelScrollMap
			, Qt::ShiftModifier, WheelActionPair(false, VolumeUpDown));
	set.endGroup();
}
