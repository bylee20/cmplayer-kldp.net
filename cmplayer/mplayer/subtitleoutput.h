#ifndef BACKEND_MPLAYER_SUBTITLEMANAGER_H
#define BACKEND_MPLAYER_SUBTITLEMANAGER_H

#include <backend/subtitleoutput.h>

namespace Backend {

namespace MPlayer {

class PlayEngine;

class SubtitleOutput : public Backend::SubtitleOutput {
	Q_OBJECT
public:
	SubtitleOutput(PlayEngine *engine);
	~SubtitleOutput();
	virtual void setVisible(bool visible) {visible ? show() : hide();}
		virtual void updatePos(double pos);
public slots:
	void show();
	void hide();
	void remove();
private slots:
	void slotSizeUpdated();
private:
	virtual void updateClear();
	virtual void updateSyncDelay(int msec);

	virtual void updateCurrentChannel(int index);
	virtual void updateStyle(const OsdStyle &style);

	struct Data;
	friend struct Data;
	friend class VideoOutput;
	Data *d;
};

}

}

#endif
