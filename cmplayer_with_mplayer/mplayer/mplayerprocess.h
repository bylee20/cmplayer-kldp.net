#ifndef MPLAYERPROCESS_H_
#define MPLAYERPROCESS_H_

#include <QProcess>

namespace Backend {

namespace MPlayer {

class PlayEngine;				class MediaInfo;

class MPlayerProcess : public QProcess {
	Q_OBJECT
public:
	MPlayerProcess(PlayEngine *engine = 0);
	virtual ~MPlayerProcess();
	void setMediaInfo(MediaInfo *info);
public slots:
	void interpretMessages();
private:
	struct Data;
	Data *d;
};

}

}

#endif /* MPLAYERPROCESS_H_ */
