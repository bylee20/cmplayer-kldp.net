#ifndef XINEAUDIOOUTPUT_H
#define XINEAUDIOOUTPUT_H

#include <QObject>
#include <xine.h>
#include <QStringList>

namespace Xine {

class XineStream;

class AudioOutput : public QObject {
	Q_OBJECT
public:
	AudioOutput(XineStream *stream);
	int volume() const {return m_volume;}
	bool isMuted() const {return m_muted;}
	qreal volumeAmplification() const {return m_volAmp;}
	void setVolumeAmplification(qreal amp);
	int initialVolume() const {return m_initVol;}
	const QString &driver() const {return m_driver;}
	void setDriver(const QString &driver);
	xine_audio_port_t *&port() {return m_port;}
	void updateTracks();
public slots:
	void setInitialVolume(int value) {m_initVol = qBound(0, value, 100);}
	void setVolume(int volume);
	void setMuted(bool muted);
signals:
	void volumeChanged(int value);
	void mutedChanged(bool muted);
	void availableTracksChanged(const QStringList &tracks);
	void currentTrackChanged(int index);
private slots:
	void update();
private:
	int realVolume(int vol) const {return qBound(0, int(vol*m_volAmp), 200);}
	XineStream *m_stream;
	xine_audio_port_t *m_port;
	QString m_driver;
	int m_volume, m_initVol;
	double m_volAmp;
	QStringList m_tracks;
	int m_curTrack;
	bool m_muted;
};

}

#endif
