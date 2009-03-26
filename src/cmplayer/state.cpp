#include "state.h"
#include "helper.h"
#include <QtCore/QSettings>
#include <QtCore/QSize>

State::Data::Data()
: keys(TypeMax), values(TypeMax) {
#define INIT(type, init) {keys[type] = #type;values[type] = QVariant(init);}
	INIT(ScreenAspectRatio, -1.0);
	INIT(ScreenCrop, -1.0);
	INIT(ScreenOnTop, QString("Disable"));
	INIT(PlaySpeed, 100);
	INIT(AudioRenderer, Map());
	INIT(AudioVolume, 100);
	INIT(AudioMuted, false);
	INIT(AudioAmp, 100);
	INIT(VideoRenderer, Map());
	INIT(SubtitlePos, 100);
	INIT(SubtitleSync, 0);
	INIT(LastOpenFile, QString());
	INIT(DockWidth, 100);
#undef INIT
}

State::Data State::d;

void State::save() const {
	QSettings set(Helper::stateFile(), QSettings::IniFormat);
	set.beginGroup("PlayerState");
	for (int i=0; i<d.values.size(); ++i)
		set.setValue(d.keys[i], d.values[i]);
	set.endGroup();
}

void State::load() {
	QSettings set(Helper::stateFile(), QSettings::IniFormat);
	set.beginGroup("PlayerState");
	for (int i=0; i<d.values.size(); ++i)
		d.values[i] = set.value(d.keys[i], d.values[i]);
	set.endGroup();
}

