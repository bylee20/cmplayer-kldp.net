#include "audiooutput.h"
#include "playengine.h"
#include "info.h"

namespace Backend {

AudioOutput::AudioOutput(PlayEngine *engine) 
: AVOutput(engine), d(new Data) {
	connect(engine, SIGNAL(started()), this, SLOT(update()));
}

AudioOutput::~AudioOutput() {
	delete d;
}

void AudioOutput::update() {
	updateMuted(d->muted);
	updateVolume(d->initVol > 0 ? d->initVol : d->volume);
}

void AudioOutput::setVolume(int volume) {
	volume = qBound(0, volume, 100);
	if (d->volume != volume) {
		updateVolume(volume);
		emit volumeChanged(d->volume = volume);
	}
}

void AudioOutput::setMuted(bool muted) {
	if (d->muted != muted) {
		updateMuted(muted);
		d->muted = muted;
	}
}

}
