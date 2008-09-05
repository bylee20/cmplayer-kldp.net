#ifndef XINESEEKSLIDER_H
#define XINESEEKSLIDER_H

#include "private/cslider.h"

namespace Xine {

class XineStream;

class SeekSlider : public CSlider {
	Q_OBJECT
public:
	SeekSlider(XineStream *stream, QWidget *parent = 0);
private slots:
	void slotTick(int pos);
	void seek(int msec);
private:
	XineStream *m_stream;
	bool m_tick;
};

}

#endif
