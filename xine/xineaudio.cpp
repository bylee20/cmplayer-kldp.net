#include "xineaudio.h"
#include "xineengine.h"
#include "xinestream.h"

#define isStreamValid() (isOpen() && m_engine->stream()->isOpen())
#define getStream() (m_engine->stream()->stream())

namespace Xine {

XineAudio::XineAudio(XineEngine *engine) {
	m_engine = engine;
	m_port = 0;
	m_volume = 100;
	m_initVol = 100;
	m_volAmp = 1.0;
	m_muted = m_open = false;
}

XineAudio::~XineAudio() {
	close();
}

bool XineAudio::open(const QString &driver) {
	if (isOpen())
		close();
	m_driver = driver;
	xine_t *xine = m_engine->xine();
	if (!xine)
		return false;
	return (m_open = (m_port = xine_open_audio_driver(xine , m_driver.toLatin1(), 0)));
}

void XineAudio::close() {
	if (!isOpen())
		return;
	XineStream *stream = m_engine->stream();
	if (stream->isOpen())
		stream->close();
	xine_close_audio_driver(m_engine->xine(), m_port);
	m_open = false;
}

void XineAudio::setVolume(int volume) {
	if (m_volume == volume)
		return;
	m_volume = qBound(0, volume, 100);
	if (isStreamValid())
		xine_set_param(getStream(), XINE_PARAM_AUDIO_AMP_LEVEL, realVolume());
	emit volumeChanged(m_volume);
}

void XineAudio::setVolumeAmplification(qreal amp) {
	if (qAbs(m_volAmp - amp) < 0.009)
		return;
	m_volAmp = qBound(0.0, amp, 2.0);
	if (isStreamValid())
		xine_set_param(getStream(), XINE_PARAM_AUDIO_AMP_LEVEL, realVolume());
}

void XineAudio::setMuted(bool muted) {
	if (m_muted == muted)
		return;
	if (isStreamValid())
		xine_set_param(getStream(), XINE_PARAM_AUDIO_AMP_MUTE, muted ? 1 : 0);
	emit mutedChanged(m_muted = muted);
}

}
