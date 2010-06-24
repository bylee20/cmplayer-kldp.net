#include "playengine.hpp"
#include <QtGui/QMessageBox>
#include <QtCore/QUrl>
#include <gst/gst.h>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QVariant>
#include "bushelper.hpp"
#include "nativevideorenderer.hpp"
#include "audiocontroller.hpp"
#include "mrl.hpp"
#include "recentinfo.hpp"
#include "pref.hpp"

struct PlayEngine::Data {
	MediaState state;
	MediaStatus status;
	QMap<QByteArray, QVariant> tags;
	int lastPos, duration;
	Mrl mrl;
	BusHelper *bus;
	GstElement *playbin, *source;
	QTimer busTimer, ticker;
	bool seekable, hasAudio, hasVideo, finishing;
	double speed;
	QList<QMap<MediaMetaData, QVariant> > audioStream;
	NativeVideoRenderer *native;
	AudioController *audio;
	int prevTick, seek;
};

PlayEngine::PlayEngine(): d(new Data) {
	d->prevTick = 0;
	d->seek = -1;
	d->finishing = false;
	d->state = StoppedState;
	d->status = NoMediaStatus;
	d->hasAudio = d->hasVideo = d->seekable = false;
	d->speed = 1.0;
	d->native = new NativeVideoRenderer(this);
	d->audio = new AudioController(this);
	d->lastPos = d->duration = 0;

	d->playbin = gst_element_factory_make("playbin2", 0);
	gst_object_ref(GST_OBJECT(d->playbin));
	gst_object_sink(GST_OBJECT(d->playbin));

	g_object_set(G_OBJECT(d->playbin), "audio-sink", d->audio->bin(), NULL);
	g_object_set(G_OBJECT(d->playbin), "video-sink", d->native->bin(), NULL);
	g_signal_connect(G_OBJECT(d->playbin), "about-to-finish", G_CALLBACK(getAboutToFinish), this);
	d->bus = new BusHelper(gst_pipeline_get_bus(GST_PIPELINE(d->playbin)), this);
	d->bus->start();
	d->ticker.setInterval(50);
	connect(&d->ticker, SIGNAL(timeout()), this, SLOT(ticking()));
	d->ticker.start();
}

PlayEngine::~PlayEngine() {
	stop();
	d->bus->stop();
	d->ticker.stop();
	if (d->native->parentWidget()) {
		d->native->hide();
		d->native->setParent(0);
	}
	gst_object_unref(GST_OBJECT(d->playbin));
	delete d->native;
	delete d;
}

void PlayEngine::setMrl(const Mrl &mrl) {
	if (mrl != d->mrl) {
		if (isPlaying())
			stop();
		d->mrl = mrl;
		emit mrlChanged(d->mrl);
		if (!d->tags.isEmpty()) {
			d->tags.clear();
			emit tagsChanged();
		}
		if (!d->audioStream.isEmpty()) {
			d->audioStream.clear();
			emit streamsChanged();
		}

		if (d->playbin) {
			const QByteArray uri = d->mrl.url().toEncoded();
			g_object_set(G_OBJECT(d->playbin), "uri", uri.data(), NULL);
		}
	}
}

static void addTagToMap(const GstTagList *list, const gchar *tag, gpointer user_data) {
	QMap<QByteArray, QVariant> *map = reinterpret_cast<QMap<QByteArray, QVariant>*>(user_data);

	GValue val;
	val.g_type = 0;
	gst_tag_list_copy_value(&val, list, tag);

	switch(G_VALUE_TYPE(&val)) {
	case G_TYPE_STRING: {
		const gchar *str_value = g_value_get_string(&val);
		map->insert(QByteArray(tag), QString::fromUtf8(str_value));
		break;
	} case G_TYPE_INT:
		map->insert(QByteArray(tag), g_value_get_int(&val));
		break;
	case G_TYPE_UINT:
		map->insert(QByteArray(tag), g_value_get_uint(&val));
		break;
	case G_TYPE_LONG:
		map->insert(QByteArray(tag), qint64(g_value_get_long(&val)));
		break;
	case G_TYPE_BOOLEAN:
		map->insert(QByteArray(tag), g_value_get_boolean(&val));
		break;
	case G_TYPE_CHAR:
		map->insert(QByteArray(tag), g_value_get_char(&val));
		break;
	case G_TYPE_DOUBLE:
		map->insert(QByteArray(tag), g_value_get_double(&val));
		break;
	default:
		// GST_TYPE_DATE is a function, not a constant, so pull it out of the switch
		if (G_VALUE_TYPE(&val) == GST_TYPE_DATE) {
			const GDate *date = gst_value_get_date(&val);
			if (g_date_valid(date)) {
				int year = g_date_get_year(date);
				int month = g_date_get_month(date);
				int day = g_date_get_day(date);
				map->insert(QByteArray(tag), QDate(year,month,day));
				if (!map->contains("year"))
					map->insert("year", year);
			}
		} else if (G_VALUE_TYPE(&val) == GST_TYPE_FRACTION) {
			int nom = gst_value_get_fraction_numerator(&val);
			int denom = gst_value_get_fraction_denominator(&val);
			if (denom > 0)
				map->insert(QByteArray(tag), double(nom)/denom);
		}
		break;
	}
	g_value_unset(&val);
}

void PlayEngine::handleGstMessage(void *ptr) {
	GstMessage *msg = (GstMessage*)ptr;
	//tag message comes from elements inside playbin, not from playbin itself
	if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_TAG) {
		GstTagList *list = 0;
		gst_message_parse_tag(msg, &list);
		if (list) {
			gst_tag_list_foreach(list, addTagToMap, &d->tags);
			gst_tag_list_free(list);
			emit tagsChanged();
		}
	}
	if (GST_MESSAGE_SRC(msg) != GST_OBJECT_CAST(d->playbin)) {
		gst_message_unref(msg);
		return;
	}
	switch (GST_MESSAGE_TYPE(msg))  {
	case GST_MESSAGE_STATE_CHANGED: {
		GstState oldState, newState, pending;
		gst_message_parse_state_changed(msg, &oldState, &newState, &pending);
		switch (newState) {
		case GST_STATE_VOID_PENDING:
		case GST_STATE_NULL:
			setSeekable(false);
			if (d->finishing) {
				setState(FinishedState);
				d->finishing = false;
			} else
				setState(StoppedState);
			break;
		case GST_STATE_READY:
			setSeekable(false);
			if (d->finishing) {
				setState(FinishedState);
				d->finishing = false;
			} else
				setState(StoppedState);
			break;
		case GST_STATE_PAUSED:
			setState(PausedState);
			if (oldState == GST_STATE_READY) {
				setSeekable(true);
				if (d->seek > 500) {
					seek(d->seek);
					d->seek = -1;
				}
			}
			break;
		case GST_STATE_PLAYING:
			if (oldState != GST_STATE_PLAYING)
				getStreamInfo();
			setState(PlayingState);
			break;
		default:
			break;
		}
		break;
	} case GST_MESSAGE_EOS:
		eos();
		break;
	case GST_MESSAGE_TAG:
	case GST_MESSAGE_STREAM_STATUS:
	case GST_MESSAGE_UNKNOWN:
		break;
	case GST_MESSAGE_ERROR: {
		GError *err;
		gchar *debug;
		gst_message_parse_error (msg, &err, &debug);
//		emit error(int(QMediaPlayer::ResourceError), QString::fromUtf8(err->message));
		qWarning() << "error from bus:" << QString::fromUtf8(err->message);
		g_error_free (err);
		g_free (debug);
		break;
	} case GST_MESSAGE_WARNING:
	case GST_MESSAGE_INFO:
		break;
	case GST_MESSAGE_BUFFERING: {
		int progress = 0;
		gst_message_parse_buffering(msg, &progress);
//		emit bufferingProgressChanged(progress);
		break;
	} case GST_MESSAGE_STATE_DIRTY:
	case GST_MESSAGE_STEP_DONE:
	case GST_MESSAGE_CLOCK_PROVIDE:
	case GST_MESSAGE_CLOCK_LOST:
	case GST_MESSAGE_NEW_CLOCK:
	case GST_MESSAGE_STRUCTURE_CHANGE:
	case GST_MESSAGE_APPLICATION:
	case GST_MESSAGE_ELEMENT:
		break;
	case GST_MESSAGE_SEGMENT_START: {
		const GstStructure *structure = gst_message_get_structure(msg);
		d->lastPos = g_value_get_int64(gst_structure_get_value(structure, "position"))/GST_MSECOND;
		emit positionChanged(d->lastPos);
		break;
	} case GST_MESSAGE_SEGMENT_DONE:
		break;
	case GST_MESSAGE_DURATION:
		queryDuration();
		break;
	case GST_MESSAGE_LATENCY:
#if (GST_VERSION_MAJOR >= 0) && (GST_VERSION_MINOR >= 10) && (GST_VERSION_MICRO >= 13)
	case GST_MESSAGE_ASYNC_START:
	case GST_MESSAGE_ASYNC_DONE:
#if GST_VERSION_MICRO >= 23
	case GST_MESSAGE_REQUEST_STATE:
#endif
#endif
	case GST_MESSAGE_ANY:
		break;
	default:
		break;
	}
	gst_message_unref(msg);
}

bool PlayEngine::atEnd() const {
	return d->status == EosStatus;
}

MediaState PlayEngine::state() const {
	return d->state;
}

bool PlayEngine::isSeekable() const {
	return d->seekable;
}

void PlayEngine::setSeekable(bool seekable) {
	if (d->seekable != seekable)
		emit seekableChanged(d->seekable = seekable);
}

void PlayEngine::setState(MediaState state) {
	if (d->state != state) {
		const MediaState old = d->state;
		d->state = state;
		emit stateChanged(d->state, old);
	}
}

double PlayEngine::speed() const {
	return d->speed;
}

void PlayEngine::setSpeed(double speed) {
	if (qFuzzyCompare(speed, 1.0))
		speed = 1.0;
	if (!qFuzzyCompare(d->speed, speed)) {
		d->speed = speed;
		seek(position());
		emit speedChanged(d->speed);
	}
}

void PlayEngine::getStreamInfo() {
	queryDuration();

	int audioCount = 0;
	int videoCount = 0;
	g_object_get(G_OBJECT(d->playbin), "n-audio", &audioCount, NULL);
	g_object_get(G_OBJECT(d->playbin), "n-video", &videoCount, NULL);
//	g_object_get(G_OBJECT(m_playbin), "n-text", &textStreamsCount, NULL);

	const bool hasVideo = videoCount > 0;
	const bool hasAudio = audioCount > 0;

	d->audioStream.clear();
	for (int i=0; i<audioCount; ++i) {
		StreamData data;
		GstTagList *tags = 0;
		g_signal_emit_by_name(G_OBJECT(d->playbin), "get-audio-tags", i, &tags);
		if (tags && gst_is_tag_list(tags)) {
			char *lang = 0;
			if (gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &lang))
				data[LanguageCode] = QString::fromUtf8(lang);
			g_free(lang);
		}
		d->audioStream << data;
	}
	if (d->hasAudio != hasAudio)
		emit hasAudioChanged(d->hasAudio = hasAudio);
	if (d->hasVideo != hasVideo)
		emit hasVideoChanged(d->hasVideo = hasVideo);
	emit streamsChanged();
}

void PlayEngine::finish() {
	d->finishing = true;
	gst_element_set_state(d->playbin, GST_STATE_NULL);
	emit finished(d->mrl);
}

void PlayEngine::eos() {
	qDebug() << "EOS!";
	finish();
}

void PlayEngine::setStatus(MediaStatus status) {
	if (d->status != status) {
		emit statusChanged(d->status = status);
	}
}

void PlayEngine::queryDuration() {
	GstFormat format = GST_FORMAT_TIME;
	gint64 duration = 0;
	if (gst_element_query_duration(d->playbin, &format, &duration)) {
		const int newDuration = duration / GST_MSECOND;
		if (d->duration != newDuration)
			emit durationChanged(d->duration = newDuration);
	}
}

bool PlayEngine::play() {
	d->seek = -1;
	if (!d->playbin || d->mrl.url().isEmpty()) {
		setState(StoppedState);
		return false;
	} else if ((d->state != StoppedState && d->state != FinishedState) || !Pref::get().rememberStopped)
		return gst_element_set_state(d->playbin, GST_STATE_PLAYING);
	const RecentInfo &recent = RecentInfo::get();
	const int record = recent.stoppedTime(d->mrl);
	if (record > 0) {
		if (Pref::get().askWhenRecordFound) {
			const QDateTime date = recent.stoppedDate(d->mrl);
			const QString title = tr("Stopped Record Found");
			const QString text = tr("This file was stopped during its playing before.\n"
				"Played Date: %1\nStopped Time: %2\n"
				"Do you want to start from where it's stopped?\n"
				"(You can configure not to ask anymore in the preferecences.)")
				.arg(date.toString(Qt::ISODate)).arg(msecsToString(record, "h:mm:ss"));
			const QMessageBox::StandardButtons b = QMessageBox::Yes | QMessageBox::No;
			if (QMessageBox::question(d->native, title, text, b) == QMessageBox::Yes)
				d->seek = record;
		} else
			d->seek = record;
	}
	qDebug() << "let's play!" << d->mrl.toString();
	int ret = gst_element_set_state(d->playbin, GST_STATE_PLAYING);
	qDebug() << ret;
	return ret;
}

bool PlayEngine::pause() {
	if (d->playbin && (gst_element_set_state(d->playbin, GST_STATE_PAUSED) != GST_STATE_CHANGE_FAILURE))
		return true;
	setState(StoppedState);
	return false;
}

void PlayEngine::stop() {
	if (d->playbin) {
		int pos = -1;
		if (!isStopped())
			pos = position();
		gst_element_set_state(d->playbin, GST_STATE_NULL);
		if (pos != -1)
			emit stopped(d->mrl, pos);
	}
	setState(StoppedState);
}

void PlayEngine::flush() {
	if (!d->playbin || d->state == StoppedState)
		return;
	static int hack = 1;
	const int flags = GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH;
	const gint64 pos = static_cast<gint64>(position())*GST_MSECOND + hack;
	hack = -hack;
	gst_element_seek(d->playbin, d->speed
		, GST_FORMAT_TIME, GstSeekFlags(flags)
		, GST_SEEK_TYPE_SET, pos, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

bool PlayEngine::seek(int ms) {
	if (!d->playbin || d->state == StoppedState || d->state == FinishedState || d->finishing)
		return false;
	if (d->duration > 500 && ms > d->duration - 500) {
		finish();
		return true;
	}
	const gint64 pos = static_cast<gint64>(ms)*GST_MSECOND;
	const int flags = GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_SEGMENT | GST_SEEK_FLAG_FLUSH;
	return gst_element_seek(d->playbin, d->speed
		, GST_FORMAT_TIME, GstSeekFlags(flags)
		, GST_SEEK_TYPE_SET, pos, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

int PlayEngine::duration() const {
	return d->duration;
}

int PlayEngine::position() const {
	GstFormat format = GST_FORMAT_TIME;
	gint64 pos = 0;
	if (d->playbin && gst_element_query_position(d->playbin, &format, &pos))
		return pos / GST_MSECOND;
	return 0;
}

void PlayEngine::ticking() {
	if (d->state == PlayingState) {
		int tick = position();
		if (tick != d->prevTick && !d->finishing) {
			if (duration() - tick < 100 && !d->mrl.isDVD())
				finish();
			else {
				emit this->tick(d->prevTick = tick);
			}
		}
	}
}

NativeVideoRenderer *PlayEngine::renderer() const {
	return d->native;
}

AudioController *PlayEngine::audio() const {
	return d->audio;
}

Mrl PlayEngine::mrl() const {
	return d->mrl;
}

void PlayEngine::navigateDVDMenu(int cmd) {
	if (!d->mrl.isDVD())
		return;
	switch (cmd) {
	case NavAngleMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_ANGLE_MENU;
		break;
	case NavAudioMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_AUDIO_MENU;
		break;
	case NavChapterMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_CHAPTER_MENU;
		break;
	case NavToggleMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_MENU;
		break;
	case NavRootMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_ROOT_MENU;
		break;
	case NavSubPicMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_SUBPICTURE_MENU;
		break;
	case NavTitleMenu:
		cmd = GST_NAVIGATION_COMMAND_DVD_TITLE_MENU;
		break;
	default:
		return;
	}
	GstNavigation *nav = d->native->nav();
	gst_navigation_send_command(nav, GstNavigationCommand(cmd));
}

bool PlayEngine::hasAudio() const {
	return d->hasAudio;
}

bool PlayEngine::hasVideo() const {
	return d->hasVideo;
}

const QList<StreamData> &PlayEngine::audioStreams() const {
	return d->audioStream;
}

void PlayEngine::setCurrentAudioStream(int idx) {
	if (0 <= idx && idx < d->audioStream.size())
		g_object_set(G_OBJECT(d->playbin), "current-audio", idx, NULL);
}

int PlayEngine::currentAudioStream() const {
	int idx = 0;
	g_object_get(G_OBJECT(d->playbin), "current-audio", &idx, NULL);
	return idx < 0 ? 0 : idx;
}

void PlayEngine::getAboutToFinish(GstElement */*object*/, gpointer user_data) {
	PlayEngine *engine = reinterpret_cast<PlayEngine*>(user_data);
	PlayEngine::Data *d = engine->d;
	qDebug() << "about to finish" << d->duration - engine->position();
}
