#include "audiooutput.h"
#include "playengine.h"

namespace Backend {

namespace MPlayer {

AudioOutput::AudioOutput(PlayEngine *engine)
: Backend::AudioOutput(engine) {
	m_engine = engine;
}

void AudioOutput::updateVolume(int volume) {
	m_engine->tellmp("volume " + QString::number(volume) + " 1");
}

void AudioOutput::updateMuted(bool muted) {
	m_engine->tellmp("mute " + QString::number(muted ? 1 : 0));
}

}

}
