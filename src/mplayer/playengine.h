#ifndef MPLAYER_PLAYENGINE_H
#define MPLAYER_PLAYENGINE_H

#include <core/playengine.h>

class QStringList;

namespace MPlayer {

class PlayEngine : public Core::PlayEngine {
	Q_OBJECT
public:
	PlayEngine(QObject *parent = 0);
	~PlayEngine();
	bool tellmp(const QString &command);
	template<typename T>
	bool tellmp(const QString &command, T value, int option) {
		return tellmp(command, QString::number(value), QString::number(option));
	}
	template<typename T>
	bool tellmp(const QString &command, T value) {
		return tellmp(command, QString::number(value));
	}
	bool tellmp(const QString &command, const QString &value, const QString &option = QString::null);
	virtual int currentTime() const {return m_time;}
	virtual const Core::Info &info() const;
	virtual void play(int time);
	virtual void play();
	virtual void stop();
	virtual void pause();
	virtual void seek(int time);
	virtual void seek(int time, bool relative, bool showTimeLinee, int duration);
	virtual void showMessage(const QString &message, int duration);
	virtual void showTimeLine(int time, int duration);
public slots:
	void replay();
// 	void seekChapter(int number);
// 	void playTitle(int number);
protected slots:
	void update();
private slots:
	void slotOsdRectChanged();
	void slotStateChanged(Core::State state, Core::State old);
	void slotProcFinished();
private:
	static const QString &getDontMessUp();
	static const QStringList &getDefaultArgs();
	void setOsdLevel(int level);
	void setCurrentTime(int time) {if (m_time != time) emit tick(m_time = time);}
	virtual bool updateCurrentTrack(const QString &track);
	virtual void updateVolume();
	virtual void updateSpeed(double speed);
	virtual void updateVideoProperty(Core::VideoProperty prop, double value);
	virtual void updateVideoProperties(double b, double c, double s, double h);
	virtual void updateSubtitleStyle(const Core::OsdStyle &style);
	virtual void updateSubtitle(const Core::Subtitle &subtitle);
	virtual void updateSubtitleVisiblity(bool visible);
	virtual void updateSubtitlePos(double pos);
	virtual void updateSyncDelay(int delay);
	virtual void updateCurrentSource(const Core::MediaSource &source);
	virtual bool updateVideoRenderer(const QString &name);
	virtual bool updateAudioRenderer(const QString &name);
// 	virtual void updateVideo();
// 	virtual void updateAudio();
	int toRealSubPos(double pos) const;
	void exiting();
	bool start(int time = 0);
	void updateInfo();
	class Data;
	friend class MPlayerProcess;
	int m_time;
	Data *d;
};

}

#endif
