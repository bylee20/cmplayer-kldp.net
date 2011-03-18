#ifndef PLAYENGINE_HPP
#define PLAYENGINE_HPP

#include <QtCore/QObject>
#include "global.hpp"
#include "mrl.hpp"
#include "videoframe.hpp"
#include <vlc/vlc.h>

class NativeVideoRenderer;	class AudioController;
class VideoRenderer;

struct Track {
	int id;
	QString name;
};

typedef QList<Track> TrackList;

//typedef QMap<MediaMetaData, QVariant> StreamData;

class PlayEngine : public QObject {
	Q_OBJECT
public:
	~PlayEngine();
	int position() const;
	void setMrl(const Mrl &mrl);
	MediaState state() const;
	bool isSeekable() const;
	void setSpeed(double speed);
	double speed() const;
	bool hasVideo() const;
	MediaStatus status() const;
	int duration() const;
	bool atEnd() const;
	Mrl mrl() const;
	bool isPlaying() const {return state() == PlayingState;}
	bool isPaused() const {return state() == PausedState;}
	bool isStopped() const {return state() == StoppedState;}
	TrackList audioTracks() const;
	int currentAudioTrackId() const;
	int currentTitleId() const;
	QString audioTrackName(int id) const;
public slots:
	void setCurrentAudioTrack(int id);
	bool play();
	void stop();
	bool pause();
	bool seek(int pos);
signals:
	void aboutToFinished();
	void stopped(Mrl mrl, int pos, int duration);
	void finished(Mrl mrl);
	void tick(int pos);
	void mrlChanged(const Mrl &mrl);
	void stateChanged(MediaState state, MediaState old);
	void seekableChanged(bool seekable);
	void speedChanged(double speed);
	void positionChanged(int pos);
	void hasVideoChanged(bool has);
	void hasAudioChanged(bool has);
	void audioTracksChanged(const QList<Track> &tracks);
	void titlesChanged(const QList<Track> &titles);
	void chaptersChanged(const QList<Track> &chapters);
	void durationChanged(int duration);
	void tagsChanged();
	void statusChanged(MediaStatus status);
// internal signals
	void _updateDuration(int duration);
	void _ticking();
	void _updateSeekable(bool seekable);
	void _updateState(MediaState state);
	void _updateTitle(int id);
private slots:
	void ticking();
	void updateSeekable(bool seekable);
	void updateDuration(int duration);
	void updateState(MediaState state);
	void updateTitle(int id);
	void initialSeek();
private:
	void setStatus(MediaStatus status);
private:
	typedef libvlc_track_description_t TrackDesc;
	TrackList parseTrackDesc(TrackDesc *desc);
	friend class LibVlc;
	PlayEngine();
	void parseEvent(const libvlc_event_t *event);
	struct Data;
	Data *d;
};

#endif // PLAYENGINE_HPP
