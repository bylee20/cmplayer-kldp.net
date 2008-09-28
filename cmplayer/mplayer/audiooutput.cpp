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

void AudioOutput::updateTracks(const QMap<int, QString> &ids) {
	QStringList tracks;
	m_tracks.clear();
	QMap<int, QString>::const_iterator it = ids.begin();
	for (int i=0; it != ids.end(); ++it, ++i) {
		QString track = trUtf8("트랙%1").arg(i+1);
		if (!it.value().isEmpty())
			track += ':' + it.value();
		tracks.append(track);
		m_tracks.append(it.key());
	}
	setTracks(tracks);
}

void AudioOutput::updateCurrentTrack(int index) {
	if (index >=0 && index < m_tracks.size())
		m_engine->tellmp("switch_audio " + QString::number(m_tracks[index]));
}

}

}
