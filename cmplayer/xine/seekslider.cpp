#include "seekslider.h"
#include "xinestream.h"

namespace Xine {

SeekSlider::SeekSlider(XineStream *stream, QWidget *parent)
: CSlider(parent), m_stream(stream), m_tick(false) {
	setRange(0, 65535);
	setValue(0);
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	connect(m_stream, SIGNAL(tickPos(int)), this, SLOT(slotTick(int)));
	connect(m_stream, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	setEnabled(m_stream->isSeekable());
}

void SeekSlider::seek(int pos) {
	if (!m_tick)
		m_stream->seekPos(pos);
}

void SeekSlider::slotTick(int pos) {
	m_tick = true;
	setValue(pos);
	m_tick = false;
}

}
