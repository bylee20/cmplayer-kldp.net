#include "playengine.h"
#include "xinestream.h"
#include "xineengine.h"
#include "videooutput.h"
#include "audiooutput.h"
#include "subtitleoutput.h"
#include "private/playengine_p.h"
#include <QtCore/QDebug>
#include <xine/xineutils.h>
#include "config.h"

namespace Backend {

namespace Xine {

struct PlayEngine::Data {
	XineStream *stream;
	SeekThread *seeker;
	TickThread *ticker;
	Config config;
};

PlayEngine::PlayEngine(const Backend::FactoryIface *factory, QObject *parent)
: Backend::PlayEngine(factory, parent), d(new Data) {
	d->stream = XineEngine::get()->createStream(this);
	d->seeker = new SeekThread(this);
	d->ticker = new TickThread(this);
	connect(this, SIGNAL(stateChanged(Backend::State, Backend::State))
			, this, SLOT(slotStateChanged(Backend::State, Backend::State)));
	setAudio(d->stream->audio());
	setVideo(d->stream->video());
	setSubtitle(d->stream->subtitle());
	d->stream->open(d->config.videoDriver(), d->config.audioDriver());
}

PlayEngine::~PlayEngine() {
	delete d->stream;
	delete d;
}

bool PlayEngine::isSeeking() const {
	return d->seeker->isRunning();
}

void PlayEngine::updateMediaInfo() {
	setHasVideo(xine_get_stream_info(d->stream->stream()
			, XINE_STREAM_INFO_HAS_VIDEO));
	setSeekable(xine_get_stream_info(d->stream->stream()
			, XINE_STREAM_INFO_SEEKABLE));
	int length = -1;
	for (int i=0; i<5; ++i) {
		if (xine_get_pos_length(d->stream->stream(), 0, 0, &length))
			break;
		xine_usec_sleep(100000);
	}
	if (length >= 0)
		setTotalTime(length);
}

void PlayEngine::updateInfo() {
	if (!d->stream->isOpen() || !currentSource().isValid())
		return;
	updateMediaInfo();
	if (hasVideo())
		d->stream->video()->updateSizeInfo();
}
void PlayEngine::toggleDvdMenu() {
	if (currentSource().isDisc()) {
		xine_event_t event;
		event.type = XINE_EVENT_INPUT_MENU1;
		event.data = 0;
		event.data_length = 0;
		xine_event_send(d->stream->stream(), &event);
	}
}

void PlayEngine::play(int start) {
	stop();
	d->stream->audio()->setVolumeAmplification(d->config.volumeAmplifiaction());
	if (!d->stream->isOpen() || d->stream->videoDriver() != d->config.videoDriver()
			|| d->stream->audioDriver() != d->config.audioDriver())
		d->stream->open(d->config.videoDriver(), d->config.audioDriver());
	qDebug() << "play" << currentSource().toMrl() << "from" << start;
	if (xine_open(d->stream->stream(), currentSource().toMrl().toLocal8Bit())) {
		updateInfo();
		xine_play(d->stream->stream(), 0, start);
		setState(PlayingState);
		updateSpeed(1.0);
	}
}

void PlayEngine::play() {
	if ((!isPaused() && !isStopped()) || !d->stream->isOpen())
		return; 
	if (isPaused()) {
		xine_set_param(d->stream->stream(), XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
		setState(PlayingState);
	} else
		play(0);
}

void PlayEngine::pause() {
	if (!d->stream->isOpen() || !isPlaying())
		return;
	xine_set_param(d->stream->stream(), XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
	setState(PausedState);
}

void PlayEngine::stopThreads() {
	if (d->ticker->isRunning()) {
		d->ticker->terminate();
		d->ticker->wait(2000);
	}
	if (d->seeker->isRunning()) {
		d->seeker->terminate();
		d->seeker->wait(2000);
	}
}

void PlayEngine::stop() {
	if (!d->stream->isOpen() || isStopped())
		return;
	if (d->ticker->isRunning()) {
		d->ticker->terminate();
		d->ticker->wait(1000);
	}
	xine_stop(d->stream->stream());
	emit stopped(currentSource(), d->ticker->currentTime());
	setState(StoppedState);
}

void PlayEngine::seek(int time, bool relative, bool showTime) {
	if ((!isPlaying() && !isPaused()) || !isSeekable() || d->seeker->isRunning())
		return;
	d->seeker->setTime(time + (relative ? d->ticker->currentTime() : 0), isPaused());
	d->seeker->setMode(SeekThread::Time);
	d->seeker->start();
	if (showTime)
		d->stream->showTimeBar();
}

void PlayEngine::seekByBytes(int bytes) {
	if ((!isPlaying() && !isPaused()) || !isSeekable() || d->seeker->isRunning())
		return;
	d->seeker->setBytes(qBound(0, bytes, 65535), isPaused());
	d->seeker->setMode(SeekThread::Bytes);
	d->seeker->start();
}

XineStream *PlayEngine::stream() {
	return d->stream;
}

void PlayEngine::updateSpeed(double speed) {
	if (d->stream->isOpen()) {
		const int val = (qAbs(speed-1.0) < 0.0001)
				? XINE_FINE_SPEED_NORMAL : double(XINE_FINE_SPEED_NORMAL)*speed;
		xine_set_param(d->stream->stream(), XINE_PARAM_FINE_SPEED, val);
	}
}

void PlayEngine::slotStateChanged(Backend::State state, Backend::State old) {
	if (state == PlayingState) {
		if (!d->ticker->isRunning())
			d->ticker->start();
		if (old == StoppedState) {
			updateSpeed(speed());
			emit started();
			d->stream->showOsdText(trUtf8("재생 시작: %1").arg(currentSource().displayName()));
		}
	} else if (old == PlayingState) {
		if (d->ticker->isRunning()) {
			d->ticker->terminate();
			d->ticker->wait(1000);
		}
	}
}

void PlayEngine::updateCurrentSource(const MediaSource &source) {
}

}

}

