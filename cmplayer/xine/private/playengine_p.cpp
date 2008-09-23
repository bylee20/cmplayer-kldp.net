#include "playengine_p.h"
#include "../playengine.h"
#include "../xinestream.h"
#include <QtCore/QEvent>
#include <QtGui/QApplication>
#include <QtCore/QDebug>

namespace Backend {

namespace Xine {

SeekThread::SeekThread(PlayEngine *engine)
: QThread(engine) {
	m_time = 0;
	m_before = -1;
	m_engine = engine;
	m_pos = -1;
	m_count = 0;
}

SeekThread::~SeekThread() {
}

void SeekThread::run() {
	if (m_mode == Time) {
		if (m_time > 0) {
			const int current = m_engine->currentTime();
			bool forward = m_time >= current;
			if (forward) {
				xine_play(m_engine->stream()->stream(), 0, m_time + 5000*m_count);
				if (m_before == m_time)
					++m_count;
				else
					m_count = 0;
			} else {
				xine_play(m_engine->stream()->stream(), 0, m_time);
				m_count = 0;
			}
			m_before = m_time;
		}
	} else {
		if (m_pos > 0)
			xine_play(m_engine->stream()->stream(), m_pos, 0);
	}
	if (m_paused)
		QApplication::postEvent(m_engine, new QEvent(QEvent::Type(QEvent::User + 2)));
}

TickThread::TickThread(PlayEngine *engine)
: QThread(engine) {
	m_engine = engine;
	m_time = 0;
	m_pos = 0;
}

void TickThread::run() {
	for (;;) {
		do {
			if (!m_engine->isPlaying())
				break;
			int time, pos;
			if (!xine_get_pos_length(m_engine->stream()->stream(), &pos, &time, 0))
				break;
			m_engine->setCurrentTime(m_time = time);
			m_engine->setCurrentBytes(m_pos = pos);
		} while(false);
		usleep(80000);
	}
}

}

}
