#include "xinestream.h"
#include "xineengine.h"
#include "xinepost.h"
#include <QtCore/QDebug>
#include <core/utility.h>

namespace Xine {

XineStream::XineStream() {
	stream = 0; eventQueue = 0; videoPort = 0;
	audioPort = 0; connection = 0;
	post.append(QMap<QString, XinePost*>());
	post.append(QMap<QString, XinePost*>());
	audioDriver = "auto";
	videoDriver = "auto";
}

bool XineStream::open(int video, void *visual, XineEventFunc event, void *data) {
	if (stream)
		close();
	xine_t *xine = XineEngine::xine();
	videoPort = xine_open_video_driver(xine, videoDriver.toLocal8Bit(), video, visual);
	audioPort = xine_open_audio_driver(xine, audioDriver.toLocal8Bit(), 0);
	stream = xine_stream_new(xine, audioPort, videoPort);
	eventQueue = xine_event_new_queue(stream);
	xine_event_create_listener_thread(eventQueue, event, data);
	emit opened();
	return true;
}

void XineStream::close() {
	emit aboutToClose();
	if (stream)
		xine_close(stream);
	if (eventQueue) {
		xine_event_dispose_queue(eventQueue);
		eventQueue = 0;
	}
	if (stream) {
		xine_dispose(stream);
		stream = 0;
	}
	unwireAllPosts();
	if (audioPort) {
		xine_close_audio_driver(XineEngine::xine(), audioPort);
		audioPort = 0;
	}
	if (videoPort) {
		xine_close_video_driver(XineEngine::xine(), videoPort);
		videoPort = 0;
	}
}

XinePost *XineStream::makePost(const QString &name) {
	xine_post_t *post = 0;
	xine_t *xine = XineEngine::xine();
	if ((post = xine_post_init(xine, name.toAscii(), 0, &audioPort, &videoPort)))
		return new XinePost(post, name);
	return 0;
}

XinePost *XineStream::addPost(PostType type, const QString &name) {
	QMap<QString, XinePost*>::const_iterator it = post[type].find(name);
	if (it == post[type].end()) {
		XinePost *post = makePost(name);
		if (post) {
			unwirePosts(type);
			it = this->post[type].insert(name, post);
			wirePosts(type);
		} else
			return 0;
	}
	return *it;
}

void XineStream::removePost(PostType type, const QString &name) {
	QMap<QString, XinePost*>::iterator it = post[type].find(name);
	if (it != post[type].end()) {
		unwirePosts(type);
		post[type].erase(it);
		rewirePosts(type);
	}
}

void XineStream::wirePosts(PostType type) {
	if (!stream || post[type].isEmpty())
		return;
	QMapIterator<QString, XinePost*> it(post[type]);
	it.toBack();
	if (type == AudioPost)
		xine_post_wire_audio_port(it.peekPrevious().value()->output(), audioPort);
	else
		xine_post_wire_video_port(it.peekPrevious().value()->output(), videoPort);
	xine_post_in_t *input = it.previous().value()->input();
	while (it.hasPrevious()) {
		xine_post_wire(it.peekPrevious().value()->output(), input);
		input = it.previous().value()->input();
	}
	if (type == AudioPost)
		xine_post_wire(xine_get_audio_source(stream), input);
	else
		xine_post_wire(xine_get_video_source(stream), input);
}

void XineStream::unwirePosts(PostType type) {
	if (!stream)
		return;
	if (type == AudioPost)
		xine_post_wire_audio_port(xine_get_audio_source(stream), audioPort);
	else
		xine_post_wire_video_port(xine_get_video_source(stream), videoPort);
}

}
