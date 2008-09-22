#ifndef BACKEND_MPLAYER_AUDIOOUTPUT_H
#define BACKEND_MPLAYER_AUDIOOUTPUT_H

#include <backend/audiooutput.h>

namespace Backend {

namespace MPlayer {

class PlayEngine;

class AudioOutput : public Backend::AudioOutput {
	Q_OBJECT
public:
	AudioOutput(PlayEngine *engine);
private:
	virtual void updateVolume(int volume);
	virtual void updateMuted(bool muted);
	PlayEngine *m_engine;
};

}

}

#endif
