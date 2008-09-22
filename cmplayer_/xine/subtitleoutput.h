#ifndef XINE_SUBTITLEOUTPUT_H
#define XINE_SUBTITLEOUTPUT_H

#include <QObject>
#include <QStringList>
#include <backend/subtitleoutput.h>

namespace Backend {

class Subtitle;

namespace Xine {

class XineStream;							class XineOsd;
class PlayEngine;

class SubtitleOutput : public Backend::SubtitleOutput {
	Q_OBJECT
public:
	enum AutoSelect {FirstFile = 0, SameName = 1, AllLoaded = 2, EachLanguage = 3};
	SubtitleOutput(PlayEngine *engine, XineStream *stream);
	virtual ~SubtitleOutput();
	XineStream *stream() {return m_stream;}
	void setEnableHighQuality(bool enabled);
	XineOsd *osd() {return m_osd;}
	virtual void setVisible(bool visible);
private slots:
	void showSubtitle(int time);
	void update();
	void slotSyncDelayChanged(int delay);
private:
	virtual void updateStyle(const OsdStyle &style);
	virtual void updateClear();
	virtual void updateSyncDelay(int msec);
	virtual void updatePos(double pos);
	virtual void updateCurrentChannel(int index);
	friend class XineStream;
	void updateChannels();
	void clear(bool emits);
	void renderSubtitle(const QString &text);
	class Data;
	Data *d;
	PlayEngine *m_engine;
	XineStream *m_stream;
	XineOsd *m_osd;
};

}

}

#endif
