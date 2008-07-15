#ifndef MPLAYERSEEKSLIDER_H
#define MPLAYERSEEKSLIDER_H

#include "cslider.h"

namespace Xine {

class XineStream;

class SeekSlider : public CSlider {
	Q_OBJECT
public:
	SeekSlider(XineStream *stream, QWidget *parent = 0);
public slots:
	void setTotalTime(qint64 ms);
private slots:
	void slotTick(qint64 ms);
	void seek(int msec);
private:
	XineStream *m_stream;
	bool m_tick;
};

}

#endif
