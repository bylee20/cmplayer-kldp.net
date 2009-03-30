#ifndef MPLAYER_PLAYENGINE_H
#define MPLAYER_PLAYENGINE_H

#include <core/playengine.h>

class QStringList;

namespace MPlayer {

class PlayEngine : public Core::PlayEngine {
	Q_OBJECT
public:
	enum Command {Invalid, SubPos, Volume};
	PlayEngine(QObject *parent = 0);
	~PlayEngine();
	bool tellmp(const QString &cmd);
	bool tellmp1(const QString &cmd, const QVariant &value, bool queue = false);
	bool tellmp2(const QString &cmd, const QVariant &value
			, const QVariant &option, bool queue = false);
	virtual int currentTime() const {return m_time;}
	virtual const Core::Info &info() const;
	virtual void play(int time);
	virtual void play();
	virtual void stop();
	virtual void pause();
	virtual void seek(int time);
	virtual void seek(int time, bool relative, bool showTimeLinee, int timeout);
	virtual void showMessage(const QString &message, int timeout);
	virtual void showTimeLine(int time, int duration, int timeout);
	virtual void triggerSnapshot();
public slots:
	void replay();
// 	void seekChapter(int number);
// 	void playTitle(int number);
private slots:
	void update();
	void slotOsdRectChanged();
	void slotStateChanged(Core::State state, Core::State old);
	void slotProcFinished();
	void slotGotSnapshot(const QString &file);
private:
	static const QString &getDontMessUp();
	static const QStringList &getDefaultArgs();
	void setOsdLevel(int level);
	void setCurrentTime(int time) {if (m_time != time) emit tick(m_time = time);}
	void customEvent(QEvent *event);
	virtual bool updateCurrentTrack(const QString &track);
	virtual void updateVolume();
	virtual void updateSpeed(double speed);
	virtual void updateColorProperty(Core::ColorProperty::Value prop, double value);
	virtual void updateColorProperty();
	virtual void updateSubtitleStyle(const Core::OsdStyle &style);
	virtual void updateSubtitle(const Core::Subtitle &subtitle);
	virtual void updateSubtitleVisiblity(bool visible);
	virtual void updateSubtitlePos(double pos);
	virtual void updateSyncDelay(int delay);
	virtual void updateCurrentSource(const Core::MediaSource &source);
	virtual bool updateVideoRenderer(const QString &name);
	virtual bool updateAudioRenderer(const QString &name);
	int toRealSubPos(double pos) const;
	void exiting();
	bool start(int time = 0);
	void updateInfo();
	bool enqueueCommand(const QString &cmd, const QString &full);
	void doCommands();
	void updateMuted();
	void applySubtitle(const Core::Subtitle &sub);
	class Data;
	class Thread;
	friend class MPlayerProcess;
	int m_time;
	Data *d;
};

}

#endif
