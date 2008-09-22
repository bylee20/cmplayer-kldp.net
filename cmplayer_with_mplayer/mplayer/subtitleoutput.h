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
// 	void setDisplayOnMarginEnabled(bool enabled);
// 	bool isDisplayOnMarginEnabled() const;
	virtual void setVisible(bool visible) {visible ? show() : hide();}
public slots:
	void show();
	void hide();
	void remove();
protected slots:
// 	void update();
private:
	virtual void updateClear();
	virtual void updateSyncDelay(int msec);
	virtual void updatePos(double pos);
	virtual void updateCurrentChannel(int index);
	virtual void updateStyle(const OsdStyle &style);
	
	void setPosScale(qreal scale);
	struct Data;
	friend struct Data;
	friend class VideoOutput;
	Data *d;
};

}

}

#endif
