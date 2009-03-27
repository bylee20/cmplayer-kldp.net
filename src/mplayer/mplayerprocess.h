#ifndef MPLAYER_MPLAYERPROCESS_H
#define MPLAYER_MPLAYERPROCESS_H

#include <QtCore/QProcess>

namespace MPlayer {

class PlayEngine;				class MediaInfo;

class MPlayerProcess : public QProcess {
	Q_OBJECT
public:
	MPlayerProcess(PlayEngine *engine = 0);
	virtual ~MPlayerProcess();
	void setMediaInfo(MediaInfo *info);
	bool isRunning() {return state() != QProcess::NotRunning;}
public slots:
	void interpretMessages();
signals:
	void gotSnapshot(const QString &file);
private:
	struct Data;
	Data *d;
};

}

#endif /* MPLAYERPROCESS_H_ */
