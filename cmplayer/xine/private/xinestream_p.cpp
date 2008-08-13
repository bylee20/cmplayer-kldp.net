#include "xinestream_p.h"
#include "../xinestream.h"
#include <QDebug>
#include <QMutexLocker>

namespace Xine {

SeekThread::SeekThread(XineStream *stream)
: QThread(stream) {
	m_time = 0;
	m_before = -1;
	m_stream = stream;
	m_seekTime = false;
	m_pos = -1;
	m_count = 0;
}

SeekThread::~SeekThread() {
}

void SeekThread::run() {
	if (m_seekTime) {
		if (m_time > 0) {
			bool forward = m_time >= m_stream->currentTime();
			int time = qBound(0, m_time + m_count*(forward? 1000 : -1000), m_stream->totalTime());
			xine_play(m_stream->stream(), 0, time);
			if (m_time != m_before)
				m_count = 0;
			else
				++m_count;
			m_before = m_time;
		}
	} else {
		if (m_pos > 0)
			xine_play(m_stream->stream(), m_pos, 0);
	}
}

TickThread::TickThread(XineStream *stream)
: QThread(stream) {
	m_stream = stream;
	m_time = 0;
	m_pos = 0;
}

void TickThread::run() {
	for (;;) {
		do {
			if (!m_stream->isPlaying())
				break;
			int time, pos;
			if (!xine_get_pos_length(m_stream->stream(), &pos, &time, 0))
				break;
			if (m_time != time)
				emit tick(m_time = time);
			if (m_pos != pos)
				emit tickPos(m_pos = pos);
		} while(false);
		usleep(80000);
	}
}

}

