#ifndef BACKEND_MPLAYER_AUDIOOUTPUT_H
#define BACKEND_MPLAYER_AUDIOOUTPUT_H

#include <backend/audiooutput.h>
#include <QtCore/QMap>

namespace Backend {

namespace MPlayer {

class PlayEngine;

class AudioOutput : public Backend::AudioOutput {
	Q_OBJECT
public:
	AudioOutput(PlayEngine *engine);
	void updateTracks(const QMap<int, QString> &ids);
private:
	virtual void updateVolume(int volume);
	virtual void updateMuted(bool muted);
	virtual void updateCurrentTrack(int index);
	PlayEngine *m_engine;
	QList<int> m_tracks;
};

}

}

#endif
