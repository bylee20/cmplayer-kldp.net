#ifndef XINEAUDIOOUTPUT_H
#define XINEAUDIOOUTPUT_H

#include <QObject>
#include <xine.h>
#include <QStringList>
#include <backend/audiooutput.h>

namespace Backend {

namespace Xine {

class XineStream;						class PlayEngine;

class AudioOutput : public Backend::AudioOutput {
	Q_OBJECT
public:
	AudioOutput(PlayEngine *engine, XineStream *stream);
	xine_audio_port_t *&port() {return m_port;}
	void updateTracks();
	void setVolumeAmplification(double amp);
signals:
	void availableTracksChanged(const QStringList &tracks);
	void currentTrackChanged(int index);
private slots:
	void update();
	void slotVolumeChanged(int volume);
	void slotMutedChanged(bool muted);
private:
	virtual void updateVolume(int volume);
	virtual void updateMuted(bool muted);
	int realVolume(int vol) const {return qBound(0, int(vol*m_volAmp),200);}
	PlayEngine *m_engine;
	XineStream *m_stream;
	xine_audio_port_t *m_port;
	QStringList m_tracks;
	int m_curTrack;
	double m_volAmp;
};

}

}

#endif
