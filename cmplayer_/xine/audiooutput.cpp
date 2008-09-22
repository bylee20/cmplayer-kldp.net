#include "audiooutput.h"
#include "xineengine.h"
#include "xinestream.h"
#include "playengine.h"

namespace Backend {

namespace Xine {

AudioOutput::AudioOutput(PlayEngine *engine, XineStream *stream)
: Backend::AudioOutput(engine) {
	m_engine = engine;
	m_stream = stream;
	m_port = 0;
	m_curTrack = -1;
	connect(this, SIGNAL(volumeChanged(int)), this, SLOT(slotVolumeChanged(int)));
	connect(this, SIGNAL(mutedChanged(bool)), this, SLOT(slotMutedChanged(bool)));
}

void AudioOutput::slotVolumeChanged(int volume) {
	m_stream->showOsdText(trUtf8("소리 크기: %1%").arg(volume));
}

void AudioOutput::updateVolume(int volume) {
	if (m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL
				, realVolume(volume));
}

void AudioOutput::setVolumeAmplification(double amp) {
	m_volAmp = qBound(0.0, amp, 2.0);
	if (m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL
				, realVolume(volume()));
}

void AudioOutput::slotMutedChanged(bool muted) {
	m_stream->showOsdText(muted ? trUtf8("소리 끄기") : trUtf8("소리 켜기"));
}
void AudioOutput::updateMuted(bool muted) {
	if (m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_MUTE, muted?1:0);
}

void AudioOutput::updateTracks() {
	const int count = xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_MAX_AUDIO_CHANNEL);
	m_tracks.clear();
	char buffer[256];
	for(int i=0; i<count; ++i) {
		QString track = trUtf8("트랙%1").arg(i+1);
		if (xine_get_audio_lang(m_stream->stream(), i, buffer))
			track += ':' + QString::fromLocal8Bit(buffer);
		m_tracks.append(track);
	}
	emit availableTracksChanged(m_tracks);
	int cur = xine_get_param(m_stream->stream(), XINE_PARAM_AUDIO_CHANNEL_LOGICAL);
	if (cur != m_curTrack)
		emit currentTrackChanged(m_curTrack = cur);
}

}

}
