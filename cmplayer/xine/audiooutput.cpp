#include "audiooutput.h"
#include "xineengine.h"
#include "xinestream.h"
#include <QDebug>

namespace Xine {

AudioOutput::AudioOutput(XineStream *stream) {
	m_stream = stream;
	m_port = 0;
	m_volume = 100;
	m_initVol = 100;
	m_volAmp = 1.0;
	m_muted = false;
	m_driver = "auto";
	m_curTrack = -1;
	connect(m_stream, SIGNAL(started()), this, SLOT(update()));
}

void AudioOutput::setDriver(const QString &driver) {
	if (m_driver == driver)
		return;
	m_driver = driver;
	if (m_stream->isOpen()) {
		m_stream->close();
		m_stream->open();
	}
}

void AudioOutput::setVolume(int volume) {
	if (m_volume == volume)
		return;
	m_volume = qBound(0, volume, 100);
	if (m_stream->isOpen()) {
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL, realVolume(m_volume));
		m_stream->showOsdText(trUtf8("소리 크기: %1%").arg(m_volume));
	}
	emit volumeChanged(m_volume);
}

void AudioOutput::setVolumeAmplification(qreal amp) {
	if (qAbs(m_volAmp - amp) < 0.009)
		return;
	m_volAmp = qBound(0.0, amp, 2.0);
	if (m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL, realVolume(m_volume));
}

void AudioOutput::setMuted(bool muted) {
	if (m_muted == muted)
		return;
	if (m_stream->isOpen()) {
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_MUTE, muted ? 1 : 0);
		m_stream->showOsdText(muted ? trUtf8("소리 끄기") : trUtf8("소리 켜기"));
	}
	emit mutedChanged(m_muted = muted);
}

void AudioOutput::update() {
	if (m_muted)
		xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_MUTE, m_muted ? 1 : 0);
	xine_set_param(m_stream->stream(), XINE_PARAM_AUDIO_AMP_LEVEL,
			realVolume(m_initVol > 0 ? m_initVol : m_volume));
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
