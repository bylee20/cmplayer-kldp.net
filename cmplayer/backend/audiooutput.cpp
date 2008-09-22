#include "audiooutput.h"
#include "playengine.h"
#include "factoryiface.h"
#include "info.h"

namespace Backend {

AudioOutput::AudioOutput(PlayEngine *engine) 
: AVOutput(engine), m_volume(100), m_initVol(100), m_muted(false) {
	connect(engine, SIGNAL(started()), this, SLOT(update()));
}

void AudioOutput::update() {
	updateMuted(m_muted);
	updateVolume(m_initVol > 0 ? m_initVol : m_volume);
}

void AudioOutput::setVolume(int volume) {
	volume = qBound(0, volume, 100);
	if (m_volume != volume) {
		updateVolume(volume);
		emit volumeChanged(m_volume = volume);
	}
}

void AudioOutput::setMuted(bool muted) {
	if (m_muted != muted) {
		updateMuted(muted);
		m_muted = muted;
	}
}

}
