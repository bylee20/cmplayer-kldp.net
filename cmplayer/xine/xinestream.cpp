#include "playengine.h"
#include "xinestream.h"
#include "xineengine.h"
#include "audiooutput.h"
#include "videooutput.h"
#include "subtitleoutput.h"
#include "xinepost.h"
#include "xineosd.h"
#include <QtCore/QEvent>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <xine/xineutils.h>
#include <QtCore/QDebug>

#define USE_XCB 1

namespace Backend {

namespace Xine {

struct XineStream::Data {
	enum Event {Finshed = QEvent::User + 1};
	Data(XineStream *parent) {
		p = parent;
	}
	XineStream *p;
	xine_event_queue_t *eventQueue;
	QMap<QString, XinePost*> videoPosts;
	QMap<QString, XinePost*> audioPosts;
	XineOsd *timeBarOsd, *textOsd;
	QTimer *timeBarTimer, *textTimer;
};

XineStream::XineStream(PlayEngine *engine)
: QObject(engine) {
	d = new Data(this);
	m_open = false;
	m_video = new VideoOutput(engine, this);
	m_audio = new AudioOutput(engine, this);
	m_subout = new SubtitleOutput(engine, this);
	m_stream = 0;

	d->eventQueue = 0;
	
	m_engine = engine;

	d->timeBarOsd = new XineOsd(m_video);
	d->timeBarOsd->setAlignment(Qt::AlignCenter);

	d->textOsd = new XineOsd(m_video);
	d->textOsd->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	d->timeBarTimer = new QTimer(this);
	d->timeBarTimer->setSingleShot(true);
	d->timeBarTimer->setInterval(3000);
	d->textTimer = new QTimer(this);
	d->textTimer->setSingleShot(true);
	d->textTimer->setInterval(5000);
	connect(d->timeBarTimer, SIGNAL(timeout()), d->timeBarOsd, SLOT(hide()));
	connect(d->textTimer, SIGNAL(timeout()), d->textOsd, SLOT(hide()));
}

XineStream::~XineStream() {
	delete m_subout;
	close();
	QMap<QString, XinePost*>::iterator it;
	for (it = d->audioPosts.begin(); it != d->audioPosts.end(); ++it)
		delete *it;
	for (it = d->videoPosts.begin(); it != d->videoPosts.end(); ++it)
		delete *it;
	delete m_video;
	delete m_audio;
	XineEngine::get()->unregister(this);
	delete d->timeBarOsd;
	delete d->textOsd;
	delete d;
}

bool XineStream::open(const QString &vd, const QString &ad) {
	close();
	xine_t *xine = XineEngine::get()->xine();
	m_video->port() = xine_open_video_driver(xine, vd.toLatin1()
			, XINE_VISUAL_TYPE_XCB, m_video->visual());
	m_audio->port() = xine_open_audio_driver(xine, ad.toLatin1(), 0);
	if (!m_video->port() || !m_audio->port())
		return false;
	m_stream = xine_stream_new(xine, m_audio->port(), m_video->port());
	if (!m_stream)
		return false;
	m_vo = vd;
	m_ao = ad;
	d->eventQueue = xine_event_new_queue(m_stream);
	xine_event_create_listener_thread(d->eventQueue, eventListener, this);
	wireAudioPosts();
	wireVideoPosts();
	m_open = true;
	emit openStateChanged(true);
	m_video->expand(true);
	return true;
}

void XineStream::close() {
	m_engine->stopThreads();
	if (!isOpen())
		return;
	if (!m_engine->isStopped())
		m_engine->stop();
	xine_close(m_stream);
	emit openStateChanged(false);
	unwireAudioPosts();
	unwireVideoPosts();
	xine_event_dispose_queue(d->eventQueue);
	xine_dispose(m_stream);
	m_stream = 0;
	d->eventQueue = 0;
	if (m_audio->port()) {
		xine_close_audio_driver(XineEngine::get()->xine(), m_audio->port());
		m_audio->port() = 0;
	}
	if (m_video->port()) {
		xine_close_video_driver(XineEngine::get()->xine(), m_video->port());
		m_video->port() = 0;
	}
	m_open = false;
}

void XineStream::eventListener(void *user_data, const xine_event_t *event) {
	XineStream *s = static_cast<XineStream*>(user_data);
	switch(event->type) {
	case XINE_EVENT_UI_PLAYBACK_FINISHED:
		qDebug() << "XINE_EVENT_UI_PLAYBACK_FINISHED";
		QApplication::postEvent(s, new QEvent(static_cast<QEvent::Type>(Data::Finshed)));
		break;
	case XINE_EVENT_FRAME_FORMAT_CHANGE: {
		qDebug() << "XINE_EVENT_FRAME_FORMAT_CHANGE";
		xine_format_change_data_t *data = static_cast<xine_format_change_data_t*>(event->data);
		s->m_video->updateVideoSize(QSize(data->width, data->height));
		break;
	} case XINE_EVENT_MRL_REFERENCE_EXT: {
		xine_mrl_reference_data_ext_t *ref = static_cast<xine_mrl_reference_data_ext_t *>(event->data);
		qDebug() << "XINE_EVENT_MRL_REFERENCE_EXT: " << ref->alternative
		<< ", " << ref->start_time
		<< ", " << ref->duration
		<< ", " << ref->mrl
		<< ", " << (ref->mrl + strlen(ref->mrl) + 1);
		break;
	} case XINE_EVENT_UI_SET_TITLE: {
		qDebug() << "XINE_EVENT_UI_SET_TITLE";
		xine_ui_data_t* data = static_cast<xine_ui_data_t*>(event->data);
		qDebug() << "Title:" << QString::fromLocal8Bit(data->str);
		s->m_engine->updateMediaInfo();
		break;
	} case XINE_EVENT_UI_CHANNELS_CHANGED: {
		qDebug() << "XINE_EVENT_UI_CHANNELS_CHANGED";
		s->m_audio->updateTracks();
		s->m_subout->updateChannels();
		s->m_engine->updateMediaInfo();
		break;
	} case XINE_EVENT_SPU_BUTTON: {
		xine_spu_button_t* button = static_cast<xine_spu_button_t*>(event->data);
		if (button->direction == 1)
			s->m_video->widget()->setCursor(Qt::PointingHandCursor);
		else
			s->m_video->widget()->unsetCursor();
		break;
	} default:
		break;
	}
}

void XineStream::showTimeBar() {
	static const int MaxCount = 40;
	int count = qRound(double(m_engine->currentTime())/double(m_engine->totalTime())*double(MaxCount));
	QString bar("[");
	for (int i=0; i<MaxCount; ++i)
		bar += (i>count) ? "-" : "|";
	bar += "]";
	d->timeBarOsd->drawText(bar);
	d->timeBarOsd->update();
	d->timeBarTimer->start();
}

XinePost *XineStream::makePost(const QString &name) {
	xine_post_t *post = xine_post_init(XineEngine::get()->xine()
			, name.toAscii(), 0, &m_audio->port(), &m_video->port());
	if (post)
		return new XinePost(post, name);
	return 0;
}

XinePost *XineStream::addVideoPost(const QString &name) {
	if (!d->videoPosts.contains(name)) {
		XinePost *post = makePost(name);
		if (post) {
			unwireVideoPosts();
			d->videoPosts.insert(name, post);
			wireVideoPosts();
		}
		return post;
	}
	return d->videoPosts[name];
}

XinePost *XineStream::addAudioPost(const QString &name) {
	if (!d->audioPosts.contains(name)) {
		XinePost *post = makePost(name);
		if (post) {
			unwireAudioPosts();
			d->audioPosts.insert(name, post);
			wireAudioPosts();
		}
		return post;
	}
	return d->audioPosts[name];
}

void XineStream::removeVideoPost(const QString &name) {
	if (d->videoPosts.contains(name)) {
		unwireVideoPosts();
		d->videoPosts.remove(name);
		rewireVideoPosts();
	}
}
void XineStream::removeAudioPost(const QString &name) {
	if (d->audioPosts.contains(name)) {
		unwireAudioPosts();
		d->audioPosts.remove(name);
		rewireAudioPosts();
	}
}

void XineStream::wireAudioPosts() {
	if (!isOpen() || d->audioPosts.isEmpty())
		return;
	QMutableMapIterator<QString, XinePost*> it(d->audioPosts);
	it.toBack();
	xine_post_wire_audio_port(it.peekPrevious().value()->output(), m_audio->port());
	xine_post_in_t *input = it.previous().value()->input();
	while (it.hasPrevious()) {
		xine_post_wire(it.peekPrevious().value()->output(), input);
		input = it.previous().value()->input();
	}
	xine_post_wire(xine_get_audio_source(m_stream), input);
}

void XineStream::wireVideoPosts() {
	if (!isOpen() || d->videoPosts.isEmpty())
		return;
	QMutableMapIterator<QString, XinePost*> it(d->videoPosts);
	it.toBack();
	xine_post_wire_video_port(it.peekPrevious().value()->output(), m_video->port());
	xine_post_in_t *input = it.previous().value()->input();
	while (it.hasPrevious()) {
		xine_post_wire(it.peekPrevious().value()->output(), input);
		input = it.previous().value()->input();
	}
	xine_post_wire(xine_get_video_source(m_stream), input);
}

void XineStream::unwireAudioPosts() {
	if (isOpen())
		xine_post_wire_audio_port(xine_get_audio_source(m_stream), m_audio->port());
}

void XineStream::unwireVideoPosts() {
	if (isOpen())
		xine_post_wire_video_port(xine_get_video_source(m_stream), m_video->port());
}

void XineStream::showOsdText(const QString &text, int time) {
	d->textOsd->drawText(text);
	d->textOsd->update();
	d->textTimer->start(time);
}

bool XineStream::event(QEvent *event) {
	int type = event->type();
	if (type == Data::Finshed) {
		m_engine->setState(StoppedState);
		emit m_engine->finished(m_engine->currentSource());
		event->accept();
		return true;
	} else if (type == QEvent::User+2) {
		m_engine->setState(PlayingState);
		event->accept();
	}
	return QObject::event(event);
}

}

}
