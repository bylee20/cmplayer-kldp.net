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
	~AudioOutput();
	int volume() const;
	bool isMuted() const;
	int initialVolume() const;
	const QStringList &tracks() const;
	int currentTrack() const;
public slots:
	void setInitialVolume(int value);
	void setVolume(int volume);
	void setMuted(bool muted);
	void setCurrentTrack(int index);
protected slots:
	void update();
signals:
	void volumeChanged(int value);
	void mutedChanged(bool muted);
	void availableTracksChanged(const QStringList &tracks);
	void currentTrackChanged(int index);
protected:
	virtual void updateVolume(int volume) = 0;
	virtual void updateMuted(bool muted) = 0;
	virtual void updateCurrentTrack(int index) = 0;
	void setTracks(const QStringList &tracks);
private:
	struct Data {
		Data(): volume(100), initVol(100), muted(false), curTrack(0) {}
		int volume, initVol, curTrack;
		bool muted;
		QStringList tracks;
	};
	Data *d;
};

inline int AudioOutput::volume() const {
	return d->volume;
}

inline bool AudioOutput::isMuted() const {
	return d->muted;
}

inline int AudioOutput::initialVolume() const {
	return d->initVol;
}

inline const QStringList &AudioOutput::tracks() const {
	return d->tracks;
}

inline int AudioOutput::currentTrack() const {
	return d->curTrack;
}

inline void AudioOutput::setInitialVolume(int value) {
	d->initVol = qBound(0, value, 100);
}

inline void AudioOutput::setTracks(const QStringList &tracks) {
	if (d->tracks != tracks)
		emit availableTracksChanged(d->tracks = tracks);
}

inline void AudioOutput::setCurrentTrack(int index) {
	if (d->curTrack != index && index >= 0 && index < d->tracks.size()) {
		updateCurrentTrack(index);
		emit currentTrackChanged(d->curTrack = index);
	}
}

}

#endif
