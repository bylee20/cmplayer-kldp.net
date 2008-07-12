#include "audiooutput.h"
#include "playengine.h"

namespace MPlayer {

struct AudioOutput::Data {
	Data(): engine(0), muted(false), softVol(true), volume(100), initVol(100), volAmp(150.0) {}
public:
	PlayEngine *engine;
	bool muted, softVol;
	int volume, initVol;
	QString driver;
	qreal volAmp;
};

AudioOutput::AudioOutput(QObject *parent)
: Controller(parent), d(new Data) {}

AudioOutput::~AudioOutput() {
	delete d;
}

void AudioOutput::link(Controller *controller) {
	PlayEngine *engine = qobject_cast<PlayEngine*>(controller);
	if (engine) {
		if (d->engine)
			unlink(d->engine);
		d->engine = engine;
		connect(d->engine, SIGNAL(started()), this, SLOT(update()));
	}
}

void AudioOutput::unlink(Controller *controller) {
	if (d->engine && d->engine == controller) {
		disconnect(d->engine, 0, this, 0);
		d->engine = 0;
	}
}

void AudioOutput::setVolume(int volume, bool relative) {
	emit volumeChanged(d->volume = qBound(0, relative ? d->volume + volume : volume, 100));
	if (d->engine)
		 d->engine->tellmp("volume " + QString::number(d->volume) + " 1");
}

void AudioOutput::setMuted(bool muted) {
	emit mutedChanged(d->muted = muted);
	if (d->engine)
		d->engine->tellmp("mute " + QString::number(d->muted ? 1 : 0));
}

void AudioOutput::update() {
	if (!d->engine || !d->engine->isRunning())
		return;
	setVolume(d->initVol < 0 ? d->volume : d->initVol);
	setMuted(d->muted);
}

int AudioOutput::volume() const {
	return d->volume;
}

bool AudioOutput::isMuted() const {
	return d->muted;
}

const QString &AudioOutput::driver() const {
	return d->driver;
}

qreal AudioOutput::volumeAmplification() const {
	return d->volAmp;
}

bool AudioOutput::isEnabledSoftwareVolume() const {
	return d->softVol;
}

void AudioOutput::setDriver(const QString &driver) {
	d->driver = driver;
}

void AudioOutput::setEnabledSoftwareVolume(bool enabled) {
	d->softVol = enabled;
}

void AudioOutput::setVolumeAmplification(qreal amp) {
	d->volAmp = amp;
}

int AudioOutput::initialVolume() const {
	return d->initVol;
}

void AudioOutput::setInitialVolume(int value) {
	d->initVol = (value < 0 || value > 100) ? -1 : value;
}

}
