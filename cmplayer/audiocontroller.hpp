#ifndef AUDIOCONTROLLER_HPP
#define AUDIOCONTROLLER_HPP

//#include <gst/gst.h>
#include <QtCore/QObject>

class PlayEngine;

class AudioController : public QObject {
	Q_OBJECT
public:
	AudioController(/*PlayEngine *engine*/);
	~AudioController();
	int volume() const;
	bool isMuted() const;
	void setPreAmp(double amp);
	double preAmp() const;
	bool isVolumeNormalized() const;
public slots:
	void setVolumeNormalized(bool norm);
	void setVolume(int volume);
	void setMuted(bool muted);
signals:
	void volumeChanged(int volume);
	void mutedChanged(bool muted);
	void volumeNormalizedChanged(bool norm);
private:
	void prepare(int channels);
	void apply(int samples, float *buffer);
	friend class LibVlc;

	float volumeRate() const;
	struct Data;
	Data *d;
};

#endif // AUDIOCONTROLLER_HPP
