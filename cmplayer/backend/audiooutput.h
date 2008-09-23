#ifndef BACKEND_AUDIOOUTPUT_H
#define BACKEND_AUDIOOUTPUT_H

#include <QtCore/QStringList>
#include "private/avoutput.h"

namespace Backend {

class PlayEngine;

class AudioOutput : public AVOutput {
	Q_OBJECT
public:
	AudioOutput(PlayEngine *stream);
	~AudioOutput() {}
	int volume() const {return m_volume;}
	bool isMuted() const {return m_muted;}
	int initialVolume() const {return m_initVol;}
public slots:
	void setInitialVolume(int value) {m_initVol = qBound(0, value, 100);}
	void setVolume(int volume);
	void setMuted(bool muted);
protected slots:
	void update();
signals:
	void volumeChanged(int value);
	void mutedChanged(bool muted);
protected:
	virtual void updateVolume(int volume) = 0;
	virtual void updateMuted(bool muted) = 0;
private:
	int m_volume, m_initVol;
	bool m_muted;
};

}

#endif
