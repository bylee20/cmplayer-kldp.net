#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QtGui/QWidget>
#include <core/playengine.h>

namespace Core {class BackendIface;	class ABRepeater;}
typedef QMap<QString, Core::BackendIface*> BackendMap;

class VideoPlayer : public QWidget {
	Q_OBJECT
public:
	typedef QMap<QString, Core::PlayEngine*> Map;
	VideoPlayer(QWidget *main, QWidget *parent = 0);
	~VideoPlayer();
	void keepSize(bool keep);
	QSize sizeHint() const;
	const Core::PlayEngine *engine() const;
	Core::ABRepeater *repeater() const;
	void play(int time);
// 	void playNext(int time);
	void play();
	void pause();
	bool isPlaying() const;
	bool isPaused() const;
	bool isStopped() const;
	bool isSeekable() const;
	bool isMuted() const;
	bool hasVideo() const;
// 	bool hasNextSource() const;
	double frameRate() const;
	void seek(int time, bool relative, bool show);
	int duration() const;
	int currentTime() const;
	Core::State state() const;
	void setSubtitle(const Core::Subtitle &subtitle);
	int speed() const;
	int subtitlePos() const;
	int syncDelay() const;
	int volume() const;
	void setAmplifyingRate(int amp);
	int amplifyingRate() const;
	void setColorProperty(Core::ColorProperty::Value prop, int value);
	int colorProperty(Core::ColorProperty::Value prop) const;
	void showMessage(const QString &message);
	bool setVideoRenderer(const QString &renderer);
	bool setAudioRenderer(const QString &renderer);
	const QString &videoRenderer() const;
	const QString &audioRenderer() const;
	void setCurrentSpu(const QString &spu);
	Core::MediaSource currentSource() const;
// 	Core::MediaSource nextSource() const;
	double aspectRatio() const;
	double cropRatio() const;
	const Map &engines() const;
	void setSubtitleStyle(const Core::OsdStyle &style);
	void setSpeed(int speed);
	void setSubtitlePos(int pos);
	void setSyncDelay(int delay);
	void setVolumeNormalized(bool enabled);
	void setUseSoftwareEqualizer(bool use);
// 	void setNextSource(const Core::MediaSource &source);
	static const BackendMap &load(const QString &path = QString());
	static const BackendMap &backend();
// 	bool changingSource() const;
public slots:
	void triggerSnapshot();
	void stop();
	void setMuted(bool muted);
	void setSubtitleVisible(bool visible);
	void setSubtitleHidden(bool hidden) {setSubtitleVisible(!hidden);}
	void toggleDvdMenu();
	void setVolume(int volume);
	void setCurrentSource(Core::MediaSource source);
	void resetSpeed() {setSpeed(100);}
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
	void setCurrentTrack(const QString &track);
	void setBackend(const QString &name);
signals:
	void backendChanged(const QString &name);
	void stateChanged(Core::State state, Core::State old);
	void finished(Core::MediaSource source);
	void stopped(Core::MediaSource source, int time);
	void started();
	void durationChanged(int duration);
	void seekableChanged(bool seekable);
	void currentSourceChanged(const Core::MediaSource &source);
	void tick(int time);
	void volumeChanged(int volume);
	void mutedChanged(bool muted);
	void tracksChanged(const QStringList &tracks);
	void currentTrackChanged(const QString &track);
	void currentSpuChanged(const QString &spu);
	void spusChanged(const QStringList &spus);
	void snapshotTaken(const QImage &image);
private slots:
	void slotStateChanged(Core::State state, Core::State old);
	void slotFinished(Core::MediaSource source);
	void slotStopped(Core::MediaSource source, int time);
private:
	Core::PlayEngine *osdEngine();
	void setBackend(Core::PlayEngine *engine);
	void resize();
	void resizeEvent(QResizeEvent *event);
	struct Backend;
	struct Data;
	Data *d;
};

#endif
