#ifndef AUDIOCONTROLLER_HPP
#define AUDIOCONTROLLER_HPP

#include <gst/gst.h>
#include <QtCore/QObject>

class AudioController : public QObject {
	Q_OBJECT
public:
	AudioController();
	~AudioController();
	GstElement *bin() const;
	int volume() const;
	bool isMuted() const;
	void setPreAmp(double amp);
	double preAmp() const;
public slots:
	void setVolume(int volume);
	void setMuted(bool muted);
signals:
	void volumeChanged(int volume);
	void mutedChanged(bool muted);
private:
	double gstVolume();
	struct Data;
	Data *d;
};

#endif // AUDIOCONTROLLER_HPP
