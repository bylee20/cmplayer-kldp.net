#include "pref.hpp"
#include "record.hpp"
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QLocale>

Pref &Pref::ref() {
	static Pref self;
	return self;
}

#define SAVE(value) (r.setValue((#value), (value)))
#define LOAD(val, def, converter) (val = r.value(#val, def).converter())
#define SAVE_ENUM(val) (r.setValue(#val, val.name()))
#define LOAD_ENUM(val, def) (val = val.value(r.value(#val, #def).toString(), def))

void Pref::save() const {
	Record r;
	r.beginGroup("preference");

	SAVE(rememberStopped);
	SAVE(askWhenRecordFound);
	SAVE(pauseMinimized);
	SAVE(pauseVideoOnly);
	SAVE(hideCursor);
	SAVE(hideDelay);
	SAVE(enableSystemTray);
	SAVE(hideClosed);
	SAVE(singleApplication);
	SAVE(disableScreensaver);
	SAVE(subtitleEncoding);
	SAVE(subtitlePriority);
	SAVE(useSubtitleEncodingAutoDetection);
	SAVE(subtitleEncodingConfidence);
	SAVE(msPerChar);
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

	subtitleStyle.save(&r, "SubtitleStyle");
	saveMouse(r, "DoubleClickAction", doubleClickMap);
	saveMouse(r, "MiddleClickAction", middleClickMap);
	saveMouse(r, "WheelScrollAction", wheelScrollMap);

	r.endGroup();
}

void Pref::load() {
	Record r;
	r.beginGroup("preference");

	LOAD(rememberStopped, true, toBool);
	LOAD(askWhenRecordFound, true, toBool);
	LOAD(pauseMinimized, true, toBool);
	LOAD(pauseVideoOnly, true, toBool);
	LOAD(hideCursor, true, toBool);
	LOAD(hideDelay, 3000, toInt);
	LOAD(enableSystemTray, true, toBool);
	LOAD(hideClosed, true, toBool);
	LOAD(singleApplication, true, toBool);
	LOAD(disableScreensaver, true, toBool);
	LOAD(subtitleEncoding, "UTF-8", toString);
	LOAD(useSubtitleEncodingAutoDetection, true, toBool);
	LOAD(subtitleEncodingConfidence, 70, toInt);
	LOAD(msPerChar, 500, toInt);
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
	subtitleStyle.borderWidth = 0.045;
	subtitleStyle.textSize = 0.035;
	subtitleStyle.font.setBold(true);
	subtitleStyle.load(&r, "SubtitleStyle");
	loadMouse(r, "DoubleClickAction", doubleClickMap
			, Qt::NoModifier, ClickActionPair(true, ToggleFullScreen));
	loadMouse(r, "DoubleClickAction", doubleClickMap
			, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(r, "DoubleClickAction", doubleClickMap
			, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(r, "DoubleClickAction", doubleClickMap
			, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(r, "MiddleClickAction", middleClickMap
			, Qt::NoModifier, ClickActionPair(true, TogglePlayPause));
	loadMouse(r, "MiddleClickAction", middleClickMap
			, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(r, "MiddleClickAction", middleClickMap
			, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(r, "MiddleClickAction", middleClickMap
			, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
	loadMouse(r, "WheelScrollAction", wheelScrollMap
			, Qt::NoModifier, WheelActionPair(true, VolumeUpDown));
	loadMouse(r, "WheelScrollAction", wheelScrollMap
			, Qt::AltModifier, WheelActionPair(false, VolumeUpDown));
	loadMouse(r, "WheelScrollAction", wheelScrollMap
			, Qt::ControlModifier, WheelActionPair(true, AmpUpDown));
	loadMouse(r, "WheelScrollAction", wheelScrollMap
			, Qt::ShiftModifier, WheelActionPair(false, VolumeUpDown));
	r.endGroup();
}
