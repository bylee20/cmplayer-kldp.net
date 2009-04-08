#include "pref.h"
#include "helper.h"
#include "videoplayer.h"
#include <QtGui/QApplication>
#include <core/osdstyle.h>
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QLocale>
#include <QtGui/QStyle>

Pref::Pref(): defWinStyle(qApp->style()->objectName()) {
	qDebug() << defWinStyle;
	load();
}

Pref &Pref::ref() {
	static Pref self;
	return self;
}

#define SAVE2(value) (set.setValue((#value), (value)))
#define LOAD2(val, def, converter) (val = set.value(#val, def).converter())
#define SAVE2_ENUM(val) (set.setValue(#val, val.name()))
#define LOAD2_ENUM(val, def) (val = val.value(set.value(#val, #def).toString(), def))

void Pref::save() const {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	set.beginGroup("Pref");
	SAVE2(rememberStopped);
	SAVE2(playRestored);
	SAVE2(pauseMinimized);
	SAVE2(pauseVideoOnly);
	SAVE2(hideCursor);
	SAVE2(hideInFullScreen);
	SAVE2(hideDelay);
	SAVE2(normalizeVolume);
	SAVE2(useSoftwareEqualizer);
	SAVE2(enableSystemTray);
	SAVE2(hideClosed);
	SAVE2(singleApplication);
	SAVE2(disableScreensaver);
	SAVE2_ENUM(autoAddFiles);
	
	subtitleStyle.save(&set, "SubtitleStyle");
	SAVE2(subtitleEncoding);
	SAVE2(subtitlePriority);
	SAVE2_ENUM(subtitleAutoLoad);
	SAVE2_ENUM(subtitleAutoSelect);
	
	SAVE2(seekingStep1);
	SAVE2(seekingStep2);
	SAVE2(seekingStep3);
	SAVE2(speedStep);
	SAVE2(volumeStep);
	SAVE2(ampStep);
	SAVE2(subtitlePosStep);
	SAVE2(volumeStep);
	SAVE2(syncDelayStep);
	SAVE2(brightnessStep);
	SAVE2(saturationStep);
	SAVE2(contrastStep);
	SAVE2(hueStep);
	SAVE2(locale);
	SAVE2(windowStyle);
	saveMouse(set, "DoubleClickAction", doubleClickMap);
	saveMouse(set, "MiddleClickAction", middleClickMap);
	saveMouse(set, "WheelScrollAction", wheelScrollMap);
	set.endGroup();
}

void Pref::load() {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	set.beginGroup("Pref");
	LOAD2(rememberStopped, true, toBool);
	LOAD2(playRestored, true, toBool);
	LOAD2(pauseMinimized, true, toBool);
	LOAD2(pauseVideoOnly, true, toBool);
	LOAD2(hideCursor, true, toBool);
	LOAD2(hideInFullScreen, true, toBool);
	LOAD2(hideDelay, 3000, toInt);
	LOAD2(normalizeVolume, true, toBool);
	LOAD2(useSoftwareEqualizer, false, toBool);
	LOAD2(enableSystemTray, true, toBool);
	LOAD2(hideClosed, true, toBool);
	LOAD2(singleApplication, true, toBool);
	LOAD2(disableScreensaver, true, toBool);
	LOAD2_ENUM(autoAddFiles, AllFiles);
	
	subtitleStyle.alignment = Qt::AlignHCenter | Qt::AlignBottom;
	subtitleStyle.borderWidth = 0.03;
	subtitleStyle.textSize = 0.04;
	subtitleStyle.font.setBold(true);
	subtitleStyle.load(&set, "SubtitleStyle");
	LOAD2(subtitleEncoding, "UTF-8", toString);
	LOAD2(subtitlePriority, QStringList(), toStringList);
	LOAD2_ENUM(subtitleAutoLoad, Contain);
	LOAD2_ENUM(subtitleAutoSelect, SameName);

	LOAD2(seekingStep1, DefaultSeekingStep1, toInt);
	LOAD2(seekingStep2, DefaultSeekingStep2, toInt);
	LOAD2(seekingStep3, DefaultSeekingStep3, toInt);
	LOAD2(speedStep, DefaultSpeedStep, toInt);
	LOAD2(volumeStep, DefaultVolumeStep, toInt);
	LOAD2(ampStep, DefaultAmpStep, toInt);
	LOAD2(subtitlePosStep, DefaultSubPosStep, toInt);
	LOAD2(syncDelayStep, DefaultSyncDelayStep, toInt);
	LOAD2(brightnessStep, DefaultColorPropStep, toInt);
	LOAD2(saturationStep, DefaultColorPropStep, toInt);
	LOAD2(contrastStep, DefaultColorPropStep, toInt);
	LOAD2(hueStep, DefaultColorPropStep, toInt);
	LOAD2(locale, QLocale::c(), toLocale);
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