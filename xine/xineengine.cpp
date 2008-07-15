#include "xineengine.h"
#include "xinestream.h"
#include "xinevideo.h"
#include "xineaudio.h"
#include <QDir>
#include <QTimer>
#include <QTime>

namespace Xine {

XineEngine::XineEngine(QWidget *parentWidget) {
	m_xine = 0;
	m_stream = new XineStream(this);
	m_audio = new XineAudio(this);
	m_video = new XineVideo(this, parentWidget);
	m_init = false;
	m_parentWidget = parentWidget;
}

XineEngine::~XineEngine() {
	if (!m_stream->isStopped())
		m_stream->stop();
	delete m_stream;
	delete m_audio;
	delete m_video;
	xine_exit(m_xine);
}

bool XineEngine::initialize(const QString &videoDriver, const QString &audioDriver) {
	if (m_init)
		return true;

	m_xine = xine_new();
	const QString configFile = QDir::homePath() + "/.xine/config";
	xine_config_load(m_xine, QFile::encodeName(configFile));
	xine_init(m_xine);

	if (!m_audio->open(audioDriver))
		return false;
	if (!m_video->open(videoDriver))
		return false;
	if (!m_stream->open(m_audio, m_video))
		return false;
	return m_init = true;
}

QWidget *XineEngine::widget() {
	return m_video;
}

}
