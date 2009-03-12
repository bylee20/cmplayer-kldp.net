#include "helper.h"
#include "state.h"
#include <QtCore/QSettings>
#include "videoplayer.h"

State::Data::Data()
: keys(TypeMax), values(TypeMax) {
#define INIT(type, init) {keys[type] = #type;values[type] = QVariant(init);}
	INIT(ScreenAspectRatio, -1.0);
	INIT(ScreenCrop, -1.0);
	INIT(ScreenOnTop, QString("Disable"));
	INIT(PlaySpeed, 100);
// 	INIT(PlayEngine, Map());
	INIT(AudioRenderer, Map());
	INIT(AudioVolume, 100);
	INIT(AudioMuted, false);
	INIT(AudioAmp, 100);
	INIT(VideoRenderer, Map());
	INIT(SubtitlePos, 100);
	INIT(SubtitleSync, 0);
#undef INIT
}

State::Data State::d;

void State::save() const {
// 	d.values[State::ScreenAspectRatio] = player->aspectRatio();
// 	d.values[State::ScreenCrop] = player->cropRatio();
// 	d.values[State::AudioVolume] = player->volume();
// 	d.values[State::AudioMuted] = player->isMuted();
// 	d.values[State::ScreenOnTop] = d->acts.screen.onTop.group->data();
	// 	d->state[State::CurrentEngine] = info.name();
// 	State::Map engine = d->state[State::PlayEngine].toMap();
// 	State::Map::iterator it = engine.find(info.name());
// 	if (it == engine.end())
// 		it = engine.insert(info.name(), State::Map());
// 	State::Map state = it.value().toMap();
// 	if (!state.contains("VideoRenderer"))
// 		state["VideoRenderer"] = info.defaultVideoRenderer();
// 	if (!state.contains("AudioRenderer"))
// 		state["AudioRenderer"] = info.defaultAuidoRenderer();
// 	d->acts.video.renderer.group->trigger(state["VideoRenderer"]);
// 	d->acts.audio.renderer.group->trigger(state["AudioRenderer"]);
// 	it.value() = state;
// 	d->state[State::PlayEngine] = engine;
// 	if (time != -1)
// 		d->player->play(time);


// 	updateStatePlayEngine();
// 	d->state.save();

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

