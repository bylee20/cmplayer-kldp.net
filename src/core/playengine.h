#ifndef BACKEND_PLAYENGINE_H
#define BACKEND_PLAYENGINE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "mediasource.h"
#include "namespace.h"

class QSize;

namespace Core {

class Subtitle;				class OsdStyle;
class AbstractOsdRenderer;		class VideoRendererIface;
class Info;				class SubtitleRenderer;
class DummyWidget;

class PlayEngine : public QObject {
	Q_OBJECT
public:
	static const double AspectRatioAuto = -1.0;
	static const double CropRatioOff = -1.0;
	PlayEngine(QObject *parent = 0);
	~PlayEngine();
	void setCurrentSource(const MediaSource &source);
	MediaSource currentSource() const {return *d->source;}
	State state() const {return d->state;}
	bool isPlaying() const {return d->state == Playing;}
	bool isStopped() const {return d->state == Stopped;}
	bool isPaused() const {return d->state == Paused;}
	int duration() const {return d->duration;}
	bool isSeekable() const {return d->seekable;}
	int volume() const {return d->volume;}
	double amplifyingRate() const;
	bool isMuted() const;
	double aspectRatio() const;
	double cropRatio() const;
	bool isSubtitleVisible() const;
	const OsdStyle &subtitleStyle() const {return *d->subStyle;}
	const OsdStyle &messageStyle() const {return *d->msgStyle;}
	const OsdStyle &timeLineStyle() const {return *d->timeStyle;}
	void setSubtitleStyle(const OsdStyle &style);
	void setMessageStyle(const OsdStyle &style);
	void setTimeLineStyle(const OsdStyle &style);
	int syncDelay() const {return d->syncDelay;}
	double subtitlePos() const;
	double speed() const;
	double videoProperty(VideoProperty prop) const {return d->videoProps[prop];}
	const QStringList &tracks() const {return d->tracks;}
	const QString &currentTrack() const {return d->track;}
	const QStringList &spus() const {return d->spus;}
	const QString &currentSpu() const {return d->spu;}
	void setVideoRenderer(const QString &name);
	void setAudioRenderer(const QString &name);
	const QString &videoRenderer() const {return d->videoRenderer;}
	const QString &audioRenderer() const {return d->audioRenderer;}
	virtual QWidget *widget();
	virtual int currentTime() const = 0;
	virtual const Info &info() const = 0;
public slots:
	virtual void play(int time);
	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
	virtual void seek(int time) = 0;
	virtual void showMessage(const QString &message, int duration = 3000);
	virtual void showTimeLine(int time, int duration);
	virtual void toggleDvdMenu() {}
	virtual void seek(int time, bool relative, bool showTime = false, int d = 2000);
	void setCurrentTrack(const QString &track);
	void setCurrentSpu(const QString &spu);
	void setVolume(int volume);
	void setAmplifyingRate(double rate);
	void setMuted(bool muted);
	void setSubtitle(const Subtitle &subtitle);
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
	void setSubtitleVisible(bool visible);
	void setSubtitleHidden(bool hidden) {setSubtitleVisible(!hidden);}
	void setSyncDelay(int delay);
	void showTimeLine(int duration = 2000);
	void setSubtitlePos(double pos);
	void setSpeed(double speed);
	const Subtitle &subtitle() const {return *d->sub;}
	void setVideoProperty(VideoProperty prop, double value);
signals:
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
	void syncDelayChanged(int delay);
	void speedChanged(double speed);
	void tracksChanged(const QStringList &tracks);
	void currentTrackChanged(const QString &track);
	void spusChanged(const QStringList &spus);
	void currentSpuChanged(const QString &spu);
protected:
	static bool isSame(double v1, double v2) {return qAbs(v1-v2) < 1.0e-5;}
	double realVolume() const;
	void setVideoRenderer(VideoRendererIface *renderer);
	void setSubtitleOsd(AbstractOsdRenderer *osd);
	void setTimeLineOsd(AbstractOsdRenderer *osd);
	void setMessageOsd(AbstractOsdRenderer *osd);
	void setState(State state);
	void setDuration(int duration);
	void setSeekable(bool seekable);
	void setTracks(const QStringList &tracks, const QString &track = QString::null);
	void setSpus(const QStringList &spus, const QString &spu = QString::null);
	virtual bool updateCurrentSpu(const QString &/*spu*/) {return false;}
	virtual bool updateCurrentTrack(const QString &/*track*/) {return false;}
	virtual void updateVolume() = 0;
	virtual void updateSpeed(double speed) = 0;
	virtual void updateVideoProperty(VideoProperty prop, double value);
	virtual void updateVideoProperties(double b, double c, double s, double h);
	virtual void updateAspectRatio(double ratio);
	virtual void updateCropRatio(double ratio);
	virtual void updateSubtitle(const Subtitle &subtitle);
	virtual void updateTimeLineStyle(const OsdStyle &style);
	virtual void updateMessageStyle(const OsdStyle &style);
	virtual void updateSubtitleStyle(const OsdStyle &style);
	virtual void updateSubtitleVisiblity(bool visible);
	virtual void updateSubtitlePos(double pos);
	virtual void updateSyncDelay(int delay);
	virtual void updateCurrentSource(const MediaSource &/*source*/) {}
	virtual bool updateVideoRenderer(const QString &name) = 0;
	virtual bool updateAudioRenderer(const QString &name) = 0;
	virtual void updateVideo();
	virtual void updateAudio();
private slots:
	void slotTick(int time);
private:
	class Screen;
	struct Data {
		Data();
		~Data();
		bool gotInfo, muted, subVisible, seekable;
		int prevTick, prevSubTime, duration, volume, syncDelay;
		double ampRate, aspect, crop, pos, speed;
		QList<double> videoProps;
		AbstractOsdRenderer *msgOsd, *timeOsd;
		VideoRendererIface *renderer;
		SubtitleRenderer *subRenderer;
		State state;
		OsdStyle *msgStyle, *subStyle, *timeStyle;
		MediaSource *source;
		Subtitle *sub;
		QString videoRenderer, audioRenderer, track, spu;
		QStringList tracks, spus;
		Screen *screen;
	};
	Data *d;
};

}

#endif