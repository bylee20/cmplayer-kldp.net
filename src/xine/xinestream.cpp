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
	do {
		qDebug() << xine << videoDriver << audioDriver << video << visual;
		videoPort = xine_open_video_driver(xine, videoDriver.toLocal8Bit(), video, visual);
		if (!videoPort)
			break;
		audioPort = xine_open_audio_driver(xine, audioDriver.toLocal8Bit(), 0);
		if (!audioPort)
			break;
		stream = xine_stream_new(xine, audioPort, videoPort);
		eventQueue = xine_event_new_queue(stream);
		xine_event_create_listener_thread(eventQueue, event, data);

		for (int i=0; i<post.size(); ++i) {
			QMap<QString, XinePost*>::iterator it;
			for (it = post[i].begin(); it != post[i].end(); ++it) {
				it.value() = makePost(it.key());
			}
		}
		wireAllPosts();
	
		emit opened();
		return true;
	} while (false);
	if (audioPort) {
		xine_close_audio_driver(XineEngine::xine(), audioPort);
		audioPort = 0;
	}
	if (videoPort) {
		xine_close_video_driver(XineEngine::xine(), videoPort);
		videoPort = 0;
	}
	return false;
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
	for (int i=0; i<post.size(); ++i) {
		QMap<QString, XinePost*>::iterator it;
		for (it = post[i].begin(); it != post[i].end(); ++it) {
			delete it.value();
			it.value() = 0;
		}
	}
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

XinePost *XineStream::setPost(PostType type, const QString &name, bool on) {
	if (on)
		return addPost(type, name);
	else
		removePost(type, name);
	return 0;
}

XinePost *XineStream::addPost(PostType type, const QString &name) {
	QMap<QString, XinePost*>::const_iterator it = post[type].find(name);
	if (it == post[type].end()) {
		unwirePosts(type);
		it = this->post[type].insert(name, makePost(name));
		wirePosts(type);
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
	QMap<QString, XinePost*>::iterator it = --post[type].end();
	if (type == AudioPost)
		xine_post_wire_audio_port(it.value()->output(), audioPort);
	else
		xine_post_wire_video_port(it.value()->output(), videoPort);
	xine_post_in_t *input = it.value()->input();
	while (it != post[type].begin()) {
		if (!(--it).value())
			continue;
		xine_post_out_t *output = it.value()->output();
		xine_post_wire(output, input);
		input = it.value()->input();
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
