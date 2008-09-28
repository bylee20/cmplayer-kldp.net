#ifndef XINEAUDIOOUTPUT_H
#define XINEAUDIOOUTPUT_H

#include <QtCore/QObject>
#include <xine.h>
#include <QtCore/QStringList>
#include <backend/audiooutput.h>

namespace Backend {

namespace Xine {

class XineStream;						class PlayEngine;

class AudioOutput : public Backend::AudioOutput {
	Q_OBJECT
public:
	AudioOutput(PlayEngine *engine, XineStream *stream);
	~AudioOutput();
	xine_audio_port_t *&port();
	void updateTracks();
	void setVolumeAmplification(double amp);
private slots:
	void update();
	void slotVolumeChanged(int volume);
	void slotMutedChanged(bool muted);
private:
	virtual void updateVolume(int volume);
	virtual void updateMuted(bool muted);
	virtual void updateCurrentTrack(int index);
	int realVolume(int vol) const;
	struct Data {
		Data(): engine(0), stream(0), port(0), volAmp(1.0) {}
		PlayEngine *engine;
		XineStream *stream;
		xine_audio_port_t *port;
		double volAmp;
	};
	Data *d;
};

inline xine_audio_port_t *&AudioOutput::port() {
	return d->port;
}

inline int AudioOutput::realVolume(int vol) const {
	return qBound(0, int(vol*d->volAmp),200);
}

}

}

#endif
