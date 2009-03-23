#ifndef XINE_XINESTREAM_H
#define XINE_XINESTREAM_H

#include <xine.h>
#include <xcb/xcb.h>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QObject>

namespace Xine {

typedef xine_event_listener_cb_t XineEventFunc;
class XinePost;

class XineStream : public QObject {
	Q_OBJECT
public:
	enum PostType {AudioPost = 0, VideoPost = 1};
	XineStream();
	bool open(int video, void *visual, XineEventFunc event, void *data);
	void close();
	XinePost *setPost(PostType type, const QString &name, bool on);
	XinePost *addPost(PostType type, const QString &name);
	void removePost(PostType type, const QString &name);
	void wirePosts(PostType type);
	void unwirePosts(PostType type);
	void rewirePosts(PostType type) {unwirePosts(type); wirePosts(type);}
	void wireAllPosts() {wirePosts(VideoPost); wirePosts(AudioPost);}
	void unwireAllPosts() {unwirePosts(VideoPost); unwirePosts(AudioPost);}
	void rewireAllPosts() {rewirePosts(VideoPost); rewirePosts(AudioPost);}
	xine_stream_t *stream;
	xine_event_queue_t *eventQueue;
	xine_video_port_t *videoPort;
	QString videoDriver;
	xine_audio_port_t *audioPort;
	QString audioDriver;
	xcb_visual_t visual;
	xcb_connection_t *connection;
signals:
	void aboutToClose();
	void opened();
private:
	XinePost *makePost(const QString &name);
	QList<QMap<QString, XinePost*> > post;
};

}

#endif
