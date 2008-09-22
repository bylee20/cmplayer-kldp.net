#ifndef MPLAYERVIDEOOUTPUT_H
#define MPLAYERVIDEOOUTPUT_H

#include <backend/videooutput.h>

class QSize;

namespace Backend {

namespace MPlayer {

class PlayEngine;

class VideoOutput : public Backend::VideoOutput {
	Q_OBJECT
public:
	VideoOutput(PlayEngine *engine);
	~VideoOutput();
	PlayEngine *playEngine() const;
	int internalWidgetId() const;
	virtual bool supportsExpansion() const {return true;}
private:
	virtual bool updateExpand(bool expand);
	virtual void updateHue(double value);
	virtual void updateContrast(double value);
	virtual void updateSaturation(double value);
	virtual void updateBrightness(double value);
	void setExpanded(bool expanded);
	qreal monitorRatio() const;
	struct Data;
	friend struct Data;
	friend class PlayEngine;
	Data *d;
};

}

}

#endif
