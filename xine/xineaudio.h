#ifndef XINEAUDIOOUTPUT_H
#define MPLAYERAUDIOOUTPUT_H

#include <QObject>
#include <xine.h>

namespace Xine {

class XineEngine;

class XineAudio : public QObject {
	Q_OBJECT
public:
	XineAudio(XineEngine *engine);
	~XineAudio();
	int volume() const {return m_volume;}
	bool isMuted() const {return m_muted;}
	qreal volumeAmplification() const {return m_volAmp;}
	void setVolumeAmplification(qreal amp);
	int initialVolume() const {return m_initVol;}
	bool initailize(const QString &driver = QString("auto"));
	xine_audio_port_t *port() {return m_port;}
	const QString &driver() const {return m_driver;}
	bool open(const QString &driver);
	bool isOpen() const {return m_open;}
	void close();
public slots:
	void setInitialVolume(int value) {m_initVol = qBound(0, value, 100);}
	void setVolume(int volume);
	void setMuted(bool muted);
signals:
	void volumeChanged(int value);
	void mutedChanged(bool muted);
private:
	int realVolume() const {return qBound(0, int(m_volume*m_volAmp), 200);}
	XineEngine *m_engine;
	xine_audio_port_t *m_port;
	QString m_driver;
	int m_volume, m_initVol;
	qreal m_volAmp;
	bool m_muted, m_open;
};

}

#endif
