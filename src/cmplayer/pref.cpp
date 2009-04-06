#include "pref.h"
#include "helper.h"
#include "videoplayer.h"
#include <core/osdstyle.h>
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QLocale>

#define SAVE(value) (set->setValue((#value), (value)))
#define LOAD(val, def, converter) (val = set->value(#val, def).converter())
#define SAVE_ENUM(val) (set->setValue(#val, val.name()))
#define LOAD_ENUM(val, def) (val = val.value(set->value(#val, #def).toString(), def))

Pref *Pref::get() {
	static Pref pref;
	return &pref;
}

struct Pref::General {
	General() {}
	void save(QSettings *set) const {
		set->beginGroup("General");
		SAVE(rememberStopped);
		SAVE(playRestored);
		SAVE(pauseMinimized);
		SAVE(pauseVideoOnly);
		SAVE(hideCursor);
		SAVE(hideInFullScreen);
		SAVE(hideDelay);
		SAVE(volumeNormalized);
		SAVE(useSoftwareEqualizer);
		SAVE(systemTrayEnabled);
		SAVE(hideWhenClosed);
		SAVE(singleApplication);
		SAVE_ENUM(autoAddFiles);
		set->endGroup();
	}
	void load(QSettings *set) {
		set->beginGroup("General");
		LOAD(rememberStopped, true, toBool);
		LOAD(playRestored, true, toBool);
		LOAD(pauseMinimized, true, toBool);
		LOAD(pauseVideoOnly, true, toBool);
		LOAD(hideCursor, true, toBool);
		LOAD(hideInFullScreen, true, toBool);
		LOAD(hideDelay, 3000, toInt);
		LOAD(volumeNormalized, true, toBool);
		LOAD(useSoftwareEqualizer, false, toBool);
		LOAD(systemTrayEnabled, true, toBool);
		LOAD(hideWhenClosed, true, toBool);
		LOAD(singleApplication, true, toBool);
		LOAD_ENUM(autoAddFiles, AllFiles);
		set->endGroup();
	}
	bool rememberStopped, playRestored, pauseMinimized, pauseVideoOnly;
	bool hideCursor, hideInFullScreen, volumeNormalized, useSoftwareEqualizer;
	bool systemTrayEnabled, hideWhenClosed, singleApplication;
	int hideDelay;
	AutoAddFilesEnum autoAddFiles;
};

struct Pref::Subtitle {
	Subtitle() {}
	void save(QSettings *set) const {
		set->beginGroup("Subtitle");
		style.save(set, "OsdStyle");
		SAVE(encoding);
		SAVE(priority);
		SAVE_ENUM(autoLoad);
		SAVE_ENUM(autoSelect);
		set->endGroup();
	}
	void load(QSettings *set) {
		set->beginGroup("Subtitle");
		style.alignment = Qt::AlignHCenter | Qt::AlignBottom;
		style.borderWidth = 0.03;
		style.textSize = 0.04;
		style.font.setBold(true);
		style.load(set, "OsdStyle");
		LOAD(encoding, "UTF-8", toString);
		LOAD(priority, QStringList(), toStringList);
		LOAD_ENUM(autoLoad, Contain);
		LOAD_ENUM(autoSelect, SameName);
		set->endGroup();
	}
	::Core::OsdStyle style;
	QString encoding;
	QStringList priority;
	SubtitleAutoLoadEnum autoLoad;
	SubtitleAutoSelectEnum autoSelect;
};

struct Pref::Interface {
	static const int DefaultSeekingStep1 = 10000;
	static const int DefaultSeekingStep2 = 30000;
	static const int DefaultSeekingStep3 = 60000;
	static const int DefaultVolumeStep = 2;
	static const int DefaultSyncDelayStep = 500;
	static const int DefaultAmpStep = 10;
	static const int DefaultSubPosStep = 1;
	static const int DefaultSpeedStep = 10;
	static const int DefaultVideoPropStep = 1;
	Interface() {}
	void save(QSettings *set) const {
		set->beginGroup("Interface");
		SAVE(seekingStep1);
		SAVE(seekingStep2);
		SAVE(seekingStep3);
		SAVE(speedStep);
		SAVE(volumeStep);
		SAVE(ampStep);
		SAVE(subPosStep);
		SAVE(volumeStep);
		SAVE(syncDelayStep);
		SAVE(brightnessStep);
		SAVE(saturationStep);
		SAVE(contrastStep);
		SAVE(hueStep);
		SAVE(locale);
		saveMouse(set, "DoubleClickAction", dbl);
		saveMouse(set, "MiddleClickAction", mdl);
		saveMouse(set, "WheelScrollAction", whl);
		set->endGroup();
	}
	void load(QSettings *set) {
		set->beginGroup("Interface");
		LOAD(seekingStep1, DefaultSeekingStep1, toInt);
		LOAD(seekingStep2, DefaultSeekingStep2, toInt);
		LOAD(seekingStep3, DefaultSeekingStep3, toInt);
		LOAD(speedStep, DefaultSpeedStep, toInt);
		LOAD(volumeStep, DefaultVolumeStep, toInt);
		LOAD(ampStep, DefaultAmpStep, toInt);
		LOAD(subPosStep, DefaultSubPosStep, toInt);
		LOAD(syncDelayStep, DefaultSyncDelayStep, toInt);
		LOAD(brightnessStep, DefaultVideoPropStep, toInt);
		LOAD(saturationStep, DefaultVideoPropStep, toInt);
		LOAD(contrastStep, DefaultVideoPropStep, toInt);
		LOAD(hueStep, DefaultVideoPropStep, toInt);
		LOAD(locale, QLocale::c(), toLocale);
		loadMouse(set, "DoubleClickAction", dbl
				, Qt::NoModifier, ClickActionPair(true, ToggleFullScreen));
		loadMouse(set, "DoubleClickAction", dbl
				, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
		loadMouse(set, "DoubleClickAction", dbl
				, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
		loadMouse(set, "DoubleClickAction", dbl
				, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
		loadMouse(set, "MiddleClickAction", mdl
				, Qt::NoModifier, ClickActionPair(true, TogglePlayPause));
		loadMouse(set, "MiddleClickAction", mdl
				, Qt::AltModifier, ClickActionPair(false, ToggleFullScreen));
		loadMouse(set, "MiddleClickAction", mdl
				, Qt::ControlModifier, ClickActionPair(false, ToggleFullScreen));
		loadMouse(set, "MiddleClickAction", mdl
				, Qt::ShiftModifier, ClickActionPair(false, ToggleFullScreen));
		loadMouse(set, "WheelScrollAction", whl
				, Qt::NoModifier, WheelActionPair(true, VolumeUpDown));
		loadMouse(set, "WheelScrollAction", whl
				, Qt::AltModifier, WheelActionPair(false, VolumeUpDown));
		loadMouse(set, "WheelScrollAction", whl
				, Qt::ControlModifier, WheelActionPair(true, AmpUpDown));
		loadMouse(set, "WheelScrollAction", whl
				, Qt::ShiftModifier, WheelActionPair(false, VolumeUpDown));
		set->endGroup();
	}
	int seekingStep1, seekingStep2, seekingStep3, speedStep;
	int volumeStep, syncDelayStep, ampStep, subPosStep;
	int brightnessStep, saturationStep, contrastStep, hueStep;
	ClickActionMap dbl, mdl;
	WheelActionMap whl;
	QLocale locale;
private:
	template<typename T>
	void saveMouse(QSettings *set, const QString &group, const T &t) const {
		set->beginGroup(group);
		for (typename T::const_iterator it = t.begin(); it != t.end(); ++it) {
			set->beginGroup(it.key().name());
			set->setValue("Enabled", it.value().first);
			set->setValue("Action", it.value().second.name());
			set->endGroup();
		}
		set->endGroup();
	}
	template<typename T>
	void loadMouse(QSettings *set, const QString &group, T &t
			, const typename T::key_type &key, const typename T::mapped_type &def) {
		set->beginGroup(group);
		set->beginGroup(key.name());
		typename T::mapped_type value;
		value.first = set->value("Enabled", def.first).toBool();
		value.second = T::mapped_type::second_type::value(set->value("Action"
				, def.second.name()).toString(), def.second);
		t[key] = value;
		set->endGroup();
		set->endGroup();
	}
};

// struct Pref::Backend {
// 	Backend() {}
// 	void save(QSettings *set) const {
// 		set->beginGroup("Backend");
// 		set->beginGroup("Priority");
// 		
// 		set->setValue("File", names[Core::LocalFile]);
// 		set->setValue("URL", names[Core::Url]);
// 		set->setValue("DVD", names[Core::Dvd]);
// 		
// 		set->endGroup();
// 		set->endGroup();
// 	}
// 	void load(QSettings *set) {
// 		QString defName;
// 		const BackendMap &map = VideoPlayer::load();
// 		if (!map.isEmpty())
// 			defName = map.begin().key();
// 		
// 		set->beginGroup("Backend");
// 		set->beginGroup("Priority");
// 		
// 		names[Core::LocalFile] = set->value("File", defName).toString();
// 		names[Core::Url] = set->value("URL", defName).toString();
// 		names[Core::Dvd] = set->value("DVD", defName).toString();
// 		
// 		set->endGroup();
// 		set->endGroup();
// 	}
// 	QMap< ::Core::MediaType, QString> names;
// };

Pref::Pref() {
	sub = new Subtitle;
	iface = new Interface;
	gen = new General;
// 	back = new Backend;
	load();
}

Pref::~Pref() {
	delete sub;
	delete iface;
	delete gen;
// 	delete back;
}

SubtitleAutoLoad Pref::subtitleAutoLoad() const {
	return sub->autoLoad;
}

void Pref::setSubtitleAutoLoad(SubtitleAutoLoad mode) {
	sub->autoLoad = mode;
}

SubtitleAutoSelect Pref::subtitleAutoSelect() const {
	return sub->autoSelect;
}

void Pref::setSubtitleAutoSelect(SubtitleAutoSelect mode) {
	sub->autoSelect = mode;
}

const Core::OsdStyle &Pref::subtitleStyle() const {
	return sub->style;
}

void Pref::setSubtitleStyle(const ::Core::OsdStyle &style) {
	sub->style = style;
}

const QString &Pref::subtitleEncoding() const {
	return sub->encoding;
}

void Pref::setSubtitleEncoding(const QString &encoding) {
	sub->encoding = encoding;
}

int Pref::seekingStep(SeekingStep step) const {
	switch (step) {
	case SeekingStep1:
		return iface->seekingStep1;
	case SeekingStep2:
		return iface->seekingStep2;
	default:
		return iface->seekingStep3;
	}
}

int Pref::seekingStep1() const {
	return iface->seekingStep1;
}

void Pref::setSeekingStep1(int step) {
	iface->seekingStep1 = step;
}

int Pref::seekingStep2() const {
	return iface->seekingStep2;
}

void Pref::setSeekingStep2(int step) {
	iface->seekingStep2 = step;
}

int Pref::seekingStep3() const {
	return iface->seekingStep3;
}

void Pref::setSeekingStep3(int step) {
	iface->seekingStep3 = step;
}

int Pref::volumeStep() const {
	return iface->volumeStep;
}

void Pref::setVolumeStep(int step) {
	iface->volumeStep = step;
}

int Pref::syncDelayStep() const {
	return iface->syncDelayStep;
}

void Pref::setSyncDelayStep(int step) {
	iface->syncDelayStep = step;
}

void Pref::setRememberStopped(bool remember) {
	gen->rememberStopped = remember;
}

bool Pref::rememberStopped() const {
	return gen->rememberStopped;
}

void Pref::setAutoAddFiles(AutoAddFiles mode) {
	gen->autoAddFiles = mode;
}

AutoAddFiles Pref::autoAddFiles() const {
	return gen->autoAddFiles;
}

void Pref::setPauseWhenMinimized(bool enabled) {
	gen->pauseMinimized = enabled;
}

bool Pref::pauseWhenMinimized() const {
	return gen->pauseMinimized;
}

void Pref::setPlayWhenRestored(bool enabled) {
	gen->playRestored = enabled;
}

bool Pref::playWhenRestored() const {
	return gen->playRestored;
}

void Pref::setSubtitlePriority(const QStringList &priority) {
	sub->priority = priority;
}

const QStringList &Pref::subtitlePriority() const {
	return sub->priority;
}

// const QString &Pref::backendName(::Core::MediaType media) const {
// 	return back->names[media];
// }
// 
// void Pref::setBackendName(::Core::MediaType media, const QString &name) {
// 	back->names[media] = name;
// }

void Pref::save() const {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	gen->save(&set);
	sub->save(&set);
	iface->save(&set);
// 	back->save(&set);
}

void Pref::load() {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	gen->load(&set);
	sub->load(&set);
	iface->load(&set);
// 	back->load(&set);
}

const Pref::ClickActionMap &Pref::doubleClickMap() const {
	return iface->dbl;
}

const Pref::ClickActionMap &Pref::middleClickMap() const {
	return iface->mdl;
}

const Pref::WheelActionMap &Pref::wheelScrollMap() const {
	return iface->whl;
}

void Pref::setDoubleClickMap(const ClickActionMap &map) {
	iface->dbl = map;
}

void Pref::setMiddleClickMap(const ClickActionMap &map) {
	iface->mdl = map;
}

void Pref::setWheelScrollMap(const WheelActionMap &map) {
	iface->whl = map;
}

int Pref::ampStep() const {
	return iface->ampStep;
}

int Pref::subtitlePosStep() const {
	return iface->subPosStep;
}

void Pref::setAmpStep(int step) {
	iface->ampStep = step;
}

void Pref::setSubtitlePosStep(int step) {
	iface->subPosStep = step;
}

int Pref::speedStep() const {
	return iface->speedStep;
}

void Pref::setSpeedStep(int step) {
	iface->speedStep = step;
}

int Pref::brightnessStep() const {
	return iface->brightnessStep;
}

int Pref::saturationStep() const {
	return iface->saturationStep;
}

int Pref::contrastStep() const {
	return iface->contrastStep;
}

int Pref::hueStep() const {
	return iface->hueStep;
}

void Pref::setBrightnessStep(int step) {
	iface->brightnessStep = step;
}

void Pref::setSaturationStep(int step) {
	iface->saturationStep = step;
}

void Pref::setContrastStep(int step) {
	iface->contrastStep = step;
}

void Pref::setHueStep(int step) {
	iface->hueStep = step;
}

const QLocale &Pref::locale() const {
	return iface->locale;
}

void Pref::setLocale(const QLocale &locale) {
	iface->locale = locale;
}

void Pref::setPauseVideoOnly(bool enabled) {
	gen->pauseVideoOnly = enabled;
}

bool Pref::pauseVideoOnly() const {
	return gen->pauseVideoOnly;
}

void Pref::setHideCursor(bool enabled) {
	gen->hideCursor = enabled;
}

void Pref::setHideDelay(int delay) {
	gen->hideDelay = delay;
}

void Pref::setHideInFullScreen(bool enabled) {
	gen->hideInFullScreen = enabled;
}

bool Pref::hideCursor() const {
	return gen->hideCursor;
}

int Pref::hideDelay() const {
	return gen->hideDelay;
}

bool Pref::hideInFullScreen() const {
	return gen->hideInFullScreen;
}

bool Pref::isVolumeNormalized() const {
	return gen->volumeNormalized;
}

void Pref::setVolumeNormalized(bool on) {
	gen->volumeNormalized = on;
}

bool Pref::useSoftwareEqualizer() const {
	return gen->useSoftwareEqualizer;
}

void Pref::setUseSoftwareEqualizer(bool use) {
	gen->useSoftwareEqualizer = use;
}

bool Pref::isSystemTrayEnabled() const {
	return gen->systemTrayEnabled;
}

bool Pref::hideWhenClosed() const {
	return gen->hideWhenClosed;
}

bool Pref::singleApplication() const {
	return gen->singleApplication;
}

void Pref::setSystemTrayEnabled(bool enabled) {
	gen->systemTrayEnabled = enabled;
}

void Pref::setHideWhenClosed(bool enabled) {
	gen->hideWhenClosed = enabled;
}

void Pref::setSingleApplication(bool enabled) {
	gen->singleApplication = enabled;
}

