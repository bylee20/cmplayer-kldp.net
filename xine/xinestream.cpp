#include "xinestream.h"
#include "xineengine.h"
#include "audiooutput.h"
#include "videooutput.h"
#include "subtitleoutput.h"
#include "xinepost.h"
#include "xineosd.h"
#include "abrepeater.h"
#include "private/xinestream_p.h"
#include <QEvent>
#include <QTimer>
#include <QUrl>
#include <QApplication>
#include <xine/xineutils.h>
#include <QDebug>

#define USE_XCB 1

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
	SeekThread *seeker;
	TickThread *ticker;
	XineOsd *timeBarOsd, *textOsd;
	QTimer *timeBarTimer, *textTimer;
};

XineStream::XineStream(QObject *parent)
: QObject(parent) {
	d = new Data(this);
	m_open = m_hasVideo = m_seekable = false;
	m_video = new VideoOutput(this);
	m_audio = new AudioOutput(this);
	m_stream = 0;
	m_totalTime = 0;
	m_subout = new SubtitleOutput(this);
	m_state = StoppedState;
	m_speed = 1.0;
	m_repeater = new ABRepeater(this);
	d->eventQueue = 0;
	d->seeker = new SeekThread(this);
	d->ticker = new TickThread(this);

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
	connect(d->ticker, SIGNAL(tick(int)), this, SIGNAL(tick(int)));
	connect(d->ticker, SIGNAL(tickPos(int)), this, SIGNAL(tickPos(int)));
	connect(d->timeBarTimer, SIGNAL(timeout()), d->timeBarOsd, SLOT(hide()));
	connect(d->textTimer, SIGNAL(timeout()), d->textOsd, SLOT(hide()));
}

XineStream::~XineStream() {
	delete m_repeater;
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

bool XineStream::open() {
	close();
	xine_t *xine = XineEngine::get()->xine();
	m_video->port() = xine_open_video_driver(xine, m_video->driver().toLatin1()
#if (USE_XCB)
			, XINE_VISUAL_TYPE_XCB
#else
			, XINE_VISUAL_TYPE_X11
#endif
			, m_video->visual());
	m_audio->port() = xine_open_audio_driver(xine, m_audio->driver().toLatin1(), 0);
	if (!m_video->port() || !m_audio->port())
		return false;
	m_stream = xine_stream_new(xine, m_audio->port(), m_video->port());
	if (!m_stream)
		return false;
	d->eventQueue = xine_event_new_queue(m_stream);
	xine_event_create_listener_thread(d->eventQueue, eventListener, this);
	wireAudioPosts();
	wireVideoPosts();
	m_open = true;
	emit openStateChanged(true);
	return true;
}

bool XineStream::open(const QString &videoDriver, const QString &audioDriver) {
	close();
	m_video->setDriver(videoDriver);
	m_audio->setDriver(audioDriver);
	return open();
}

void XineStream::close() {
	if (d->ticker->isRunning()) {
		d->ticker->terminate();
		d->ticker->wait(2000);
	}
	if (d->seeker->isRunning()) {
		d->seeker->terminate();
		d->seeker->wait(2000);
	}
	if (!isOpen())
		return;
	if (!isStopped())
		stop();
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
		QApplication::postEvent(s, new QEvent(static_cast<QEvent::Type>(Data::Finshed)));
		break;
	case XINE_EVENT_FRAME_FORMAT_CHANGE: {
		xine_format_change_data_t *data = static_cast<xine_format_change_data_t*>(event->data);
		s->m_video->updateSizeInfo(QSize(data->width, data->height));
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
		s->updateMediaInfo();
		break;
	} case XINE_EVENT_UI_CHANNELS_CHANGED: {
		qDebug() << "XINE_EVENT_UI_CHANNELS_CHANGED";
		s->m_audio->updateTracks();
		s->m_subout->updateChannels();
		s->updateMediaInfo();
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

void XineStream::play(int start) {
	if (isOpen())
		stop();
	else
		open();
	qDebug() << "play" << m_source.toMrl() << "from" << start;
	if (xine_open(m_stream, m_source.toMrl().toLocal8Bit())) {
		updateInfo();
		xine_play(m_stream, 0, start);
		setState(PlayingState);
		setSpeed(1.0);
	}
}

void XineStream::play() {
	if ((m_state != PausedState && m_state != StoppedState) || !isOpen())
		return;
	if (m_state == PausedState) {
		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
		setState(PlayingState);
	} else
		play(0);
}

void XineStream::pause() {
	if (!isOpen() || m_state != PlayingState)
		return;
	xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
	setState(PausedState);
}

void XineStream::stop() {
	if (!isOpen() || m_state == StoppedState)
		return;
	if (d->ticker->isRunning()) {
		d->ticker->terminate();
		d->ticker->wait(1000);
	}
	xine_stop(m_stream);
	emit stopped(m_source, d->ticker->currentTime());
	setState(StoppedState);
}

int XineStream::currentTime() const {
	if (!isOpen())
		return -1;
	return d->ticker->currentTime();
}

int XineStream::currentPos() const {
	if (!isOpen())
		return -1;
	return d->ticker->currentPos();
}

void XineStream::setState(State state) {
	if (m_state == state)
		return;
	State old = m_state;
	m_state = state;
	if (m_state == PlayingState) {
		if (!d->ticker->isRunning())
			d->ticker->start();
		if (old == StoppedState) {
			const int val = (qAbs(m_speed-1.0) < 0.01) ? XINE_FINE_SPEED_NORMAL
					: double(XINE_FINE_SPEED_NORMAL)*m_speed;
			xine_set_param(m_stream, XINE_PARAM_FINE_SPEED, val);
			emit started();
		}
	} else if (old == PlayingState) {
		d->ticker->terminate();
		d->ticker->wait(1000);
	}
	emit stateChanged(m_state, old);
}

void XineStream::showTimeBar() {
	static const int MaxCount = 40;
	int count = qRound(double(currentTime())/double(totalTime())*double(MaxCount));
	QString bar("[");
	for (int i=0; i<MaxCount; ++i)
		bar += (i>count) ? "-" : "|";
	bar += "]";
	d->timeBarOsd->drawText(bar);
	d->timeBarOsd->update();
	d->timeBarTimer->start();
}

void XineStream::seek(int time, bool relative, bool showTimeBar) {
	if ((!isPlaying() && !isPaused()) || !m_seekable || d->seeker->isRunning())
		return;
	d->seeker->setTime(time + (relative ? d->ticker->currentTime() : 0), isPaused());
	d->seeker->setSeekTime(true);
	d->seeker->start();
	if (showTimeBar)
		this->showTimeBar();
}

void XineStream::seekPos(int pos) {
	if ((!isPlaying() && !isPaused()) || !m_seekable || d->seeker->isRunning())
		return;
	d->seeker->setPos(qBound(0, pos, 65535), isPaused());
	d->seeker->setSeekTime(false);
	d->seeker->start();
}

void XineStream::updateMediaInfo() {
	const bool hasVideo = xine_get_stream_info(m_stream, XINE_STREAM_INFO_HAS_VIDEO);
	if (m_hasVideo != hasVideo)
		emit hasVideoChanged(m_hasVideo = hasVideo);
	const bool seekable = xine_get_stream_info(m_stream, XINE_STREAM_INFO_SEEKABLE);
	if (m_seekable != seekable)
		emit seekableChanged(m_seekable = seekable);
	int length = -1;
	for (int i=0; i<5; ++i) {
		if (xine_get_pos_length(m_stream, 0, 0, &length))
			break;
		xine_usec_sleep(100000);
	}
	if (length >= 0 && length != m_totalTime)
		emit totalTimeChanged(m_totalTime = length);
}

void XineStream::updateInfo() {
	if (!isOpen() || !m_source.isValid())
		return;
	updateMediaInfo();
	if (hasVideo())
		m_video->updateSizeInfo();
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

void XineStream::setCurrentSource(const MediaSource &source) {
	if (m_source != source) {
		stop();
		emit currentSourceChanged(m_source = source);
	}
}

void XineStream::setSpeed(qreal speed) {
	if ((m_state == PlayingState || m_state == PausedState) && speed != m_speed) {
		const int val = (qAbs(speed-1.0) < 0.01) ? XINE_FINE_SPEED_NORMAL
				: double(XINE_FINE_SPEED_NORMAL)*speed;
		xine_set_param(m_stream, XINE_PARAM_FINE_SPEED, val);
		emit speedChanged(m_speed = speed);
	}
}

bool XineStream::isSeeking() const {
	return d->seeker->isRunning();
}

void XineStream::toggleDvdMenu() {
	if (m_source.discType() != Xine::DVD)
		return;
	xine_event_t event;
	event.type = XINE_EVENT_INPUT_MENU1;
	event.data = 0;
	event.data_length = 0;
	xine_event_send(m_stream, &event);
}

void XineStream::showOsdText(const QString &text, int time) {
	d->textOsd->drawText(text);
	d->textOsd->update();
	d->textTimer->start(time);
}

bool XineStream::event(QEvent *event) {
	int type = event->type();
	if (type == Data::Finshed) {
		setState(StoppedState);
		emit finished(m_source);
		event->accept();
		return true;
	} else if (type == QEvent::User+2) {
		setState(PlayingState);
		event->accept();
	}
	return QObject::event(event);
}

}
