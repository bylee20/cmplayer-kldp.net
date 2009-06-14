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
	Core::ABRepeater *repeater() const;
	void play(int time);
	void playNext(int time);
	void play();
	void pause();
	bool isPlaying() const;
	bool isPaused() const;
	bool isStopped() const;
	bool isSeekable() const;
	bool isMuted() const;
	bool hasVideo() const;
	double frameRate() const;
	void seek(int time, bool relative, bool show);
	int duration() const;
	int currentTime() const;
	Core::State state() const;
	int speed() const;
	int subtitlePos() const;
	int syncDelay() const;
	int volume() const;
	int amplifyingRate() const;
	int colorProperty(Core::ColorProperty::Value prop) const;
	void showMessage(const QString &message);
	bool setVideoRenderer(const QString &renderer);
	bool setAudioRenderer(const QString &renderer);
	QString videoRenderer() const;
	QString audioRenderer() const;
	Core::MediaSource currentSource() const;
	double aspectRatio() const;
	double cropRatio() const;
	const Map &engines() const;
	void setCurrentSpu(const QString &spu);
	void setSubtitle(const Core::Subtitle &subtitle);
	void setAmplifyingRate(int amp);
	void setColorProperty(Core::ColorProperty::Value prop, int value);
	void setSubtitleStyle(const Core::OsdStyle &style);
	void setSpeed(int speed);
	void setSubtitlePos(int pos);
	void setSyncDelay(int delay);
	void setVolumeNormalized(bool enabled);
	void setNextSource(const Core::MediaSource &source);
	bool hasNextSource() const;
	bool useSoftwareEqualizer() const;
	static const BackendMap &load();
	static const BackendMap &backend();
	const Core::Info *info() const;
	const Core::Subtitle &subtitle() const;
public slots:
	void triggerSnapshot();
	void stop();
	void toggleDvdMenu();
	void resetSpeed() {setSpeed(100);}
	void setBackend(const QString &name);
	void setMuted(bool muted);
	void setSubtitleVisible(bool visible);
	void setSubtitleHidden(bool hidden) {setSubtitleVisible(!hidden);}
	void setVolume(int volume);
	void setCurrentSource(Core::MediaSource source);
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
	void setCurrentTrack(const QString &track);
	void setUseSoftwareEqualizer(bool use);
signals:
	void useSoftwareEqualizerChanged(bool use);
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
	void colorPropertyChanged();
	void frameRateChanged(double frameRate);
private slots:
	void slotStateChanged(Core::State state, Core::State old);
	void slotFinished(Core::MediaSource source);
	void slotStopped(Core::MediaSource source, int time);
private:
	void resize();
	void resizeEvent(QResizeEvent *event);
	class Widget;
	struct Backend;
	struct Data;
	Data *d;
};

#endif
