#include "xineengine.h"
#include "xinestream.h"
#include "xinevideo.h"
#include "xineaudio.h"
#include "xinepost.h"
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
	unwireAudioPosts();
	unwireVideoPosts();
	for (int i=0; i<m_audioPosts.size(); ++i)
		delete m_audioPosts[i];
	for (int i=0; i<m_videoPosts.size(); ++i)
		delete m_videoPosts[i];
	delete m_stream;
	delete m_audio;
	delete m_video;
	xine_exit(m_xine);
}

bool XineEngine::initialize(const QString &videoDriver, const QString &audioDriver) {
	if (m_init)
		return true;

	m_xine = xine_new();
	const QString configFile = QDir::homePath() + "/.cmplayer/xine";
	xine_config_load(m_xine, QFile::encodeName(configFile));
	xine_config_save(m_xine, QFile::encodeName(configFile));
	xine_init(m_xine);

	if (!m_audio->open(audioDriver))
		return false;
	if (!m_video->open(videoDriver))
		return false;
	if (!m_stream->open(m_audio, m_video))
		return false;
	m_videoPosts.append(new XinePost(this, "expand"));
	wireAudioPosts();
	wireVideoPosts();
	return m_init = true;
}

QWidget *XineEngine::widget() {
	return m_video;
}

void XineEngine::wireAudioPosts() {
	if (!m_stream->isOpen() || m_audioPosts.isEmpty())
		return;
	xine_post_wire_audio_port(m_audioPosts.last()->output(), m_audio->port());
	for (int i=m_audioPosts.size() - 1; i > 0; --i)
		xine_post_wire(m_audioPosts[i-1]->output(), m_audioPosts[i]->input());
	xine_post_wire(xine_get_video_source(m_stream->stream()), m_audioPosts[0]->input());
}

void XineEngine::wireVideoPosts() {
	if (!m_stream->isOpen() || m_videoPosts.isEmpty())
		return;
	xine_post_wire_video_port(m_videoPosts.last()->output(), m_video->port());
	for (int i=m_videoPosts.size() - 1; i > 0; --i)
		xine_post_wire(m_videoPosts[i-1]->output(), m_videoPosts[i]->input());
	xine_post_wire(xine_get_video_source(m_stream->stream()), m_videoPosts[0]->input());
}

void XineEngine::unwireAudioPosts() {
	if (!m_stream->isOpen() && m_audio->port())
		xine_post_wire_audio_port(xine_get_audio_source(m_stream->stream())
				, m_audio->port());
}

void XineEngine::unwireVideoPosts() {
	if (!m_stream->isOpen() && m_video->port())
		xine_post_wire_video_port(xine_get_video_source(m_stream->stream())
				, m_video->port());
}

}
