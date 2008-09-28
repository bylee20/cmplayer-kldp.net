#include "audiooutput.h"
#include "xineengine.h"
#include "xinestream.h"
#include "playengine.h"

namespace Backend {

namespace Xine {

AudioOutput::AudioOutput(PlayEngine *engine, XineStream *stream)
: Backend::AudioOutput(engine), d(new Data) {
	d->engine = engine;
	d->stream = stream;
	connect(this, SIGNAL(volumeChanged(int)), this, SLOT(slotVolumeChanged(int)));
	connect(this, SIGNAL(mutedChanged(bool)), this, SLOT(slotMutedChanged(bool)));
}

AudioOutput::~AudioOutput() {
	delete d;
}

void AudioOutput::slotVolumeChanged(int volume) {
	d->stream->showOsdText(trUtf8("소리 크기: %1%").arg(volume));
}

void AudioOutput::updateVolume(int volume) {
	if (d->stream->isOpen())
		xine_set_param(d->stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL
				, realVolume(volume));
}

void AudioOutput::setVolumeAmplification(double amp) {
	d->volAmp = qBound(0.0, amp, 2.0);
	if (d->stream->isOpen())
		xine_set_param(d->stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL
				, realVolume(volume()));
}

void AudioOutput::slotMutedChanged(bool muted) {
	d->stream->showOsdText(muted ? trUtf8("소리 끄기") : trUtf8("소리 켜기"));
}
void AudioOutput::updateMuted(bool muted) {
	if (d->stream->isOpen())
		xine_set_param(d->stream->stream(), XINE_PARAM_AUDIO_AMP_MUTE, muted?1:0);
}

void AudioOutput::updateTracks() {
	if (!d->stream->isOpen())
		return;
	const int count = xine_get_stream_info(d->stream->stream(), XINE_STREAM_INFO_MAX_AUDIO_CHANNEL);
	QStringList tracks;
	char buffer[256];
	for(int i=0; i<count; ++i) {
		QString track = trUtf8("트랙%1").arg(i+1);
		if (xine_get_audio_lang(d->stream->stream(), i, buffer))
			track += ':' + QString::fromLocal8Bit(buffer);
		tracks.append(track);
	}
	setTracks(tracks);
	setCurrentTrack(xine_get_param(d->stream->stream(), XINE_PARAM_AUDIO_CHANNEL_LOGICAL));
}

void AudioOutput::updateCurrentTrack(int index) {
	if (d->stream->isOpen())
		xine_set_param(d->stream->stream(), XINE_PARAM_AUDIO_CHANNEL_LOGICAL, index);
}

}

}
