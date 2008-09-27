#ifndef BACKEND_XINE_XINESTREAM_H
#define BACKEND_XINE_XINESTREAM_H

#include <QtCore/QObject>
#include <xine.h>
#include <backend/mediasource.h>
#include <backend/backend.h>

namespace Backend {

namespace Xine {

class AudioOutput;						class VideoOutput;
class XinePost;							class SubtitleOutput;
class PlayEngine;

class XineStream : public QObject {
	Q_OBJECT
public:
	XineStream(PlayEngine *engine);
	~XineStream();
	bool open(const QString &vd, const QString &ad);
	bool isOpen() const {return m_open;}
	void close();
	const QString &videoDriver() const {return m_vo;}
	const QString &audioDriver() const {return m_ao;}
	AudioOutput *audio() {return m_audio;}
	VideoOutput *video() {return m_video;}
	xine_stream_t *stream() {return m_stream;}
	SubtitleOutput *subtitle() {return m_subout;}
	PlayEngine *engine() {return m_engine;}
	void unwireAudioPosts();
	void unwireVideoPosts();
	void wireAudioPosts();
	void wireVideoPosts();
	void rewireAudioPosts() {unwireAudioPosts(); wireAudioPosts();}
	void rewireVideoPosts() {unwireVideoPosts(); wireVideoPosts();}
	XinePost *addVideoPost(const QString &name);
	XinePost *addAudioPost(const QString &name);
	void removeVideoPost(const QString &name);
	void removeAudioPost(const QString &name);
public slots:
	void showOsdText(const QString &text, int time = 3000);
	void showTimeBar();
signals:
	void openStateChanged(bool open);
protected:
	bool event(QEvent *event);
private:
	static void eventListener(void *user_data, const xine_event_t *event);
	XinePost *makePost(const QString &name);
	void updateInfo();
// 	void updateMediaInfo();
	friend class Data;
	class Data;
	Data *d;
	bool m_open;
	VideoOutput *m_video;
	AudioOutput *m_audio;
	SubtitleOutput *m_subout;
	xine_stream_t *m_stream;
	PlayEngine *m_engine;
	QString m_vo, m_ao;
};

}

}

#endif
