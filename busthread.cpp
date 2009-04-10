#include "busthread.h"
#include "playengine.h"
#include <QtGui/QApplication>
#include <core/baseevent.h>
#include "events.h"

namespace Gst {
	
BusThread::BusThread(PlayEngine *engine) {
	this->engine = engine;
	this->bus = 0;
	this->pipeline = 0;
}

BusThread::~BusThread() {
	stop();
	gst_object_unref(GST_OBJECT(bus));
	bus = 0;
}

void BusThread::setPipeline(GstElement *pipeline) {
	this->pipeline = pipeline;
	bus = gst_pipeline_get_bus(GST_PIPELINE(this->pipeline));
}

void BusThread::stop() {
	if (isRunning()) {
		GstBus *temp = bus;
		bus = 0;
		if (!wait(1000)) {
			terminate();
			wait(1000);
		}
		bus = temp;
	}
}

void BusThread::run() {
	while (bus) {
		GstMessage *msg = gst_bus_pop(bus);
		if (!msg) {
			msleep(1);
			continue;
		}
		const GstMessageType type = GST_MESSAGE_TYPE(msg);
		switch (type) {
		case GST_MESSAGE_EOS:
			qDebug("GST_MESSAGE_EOS");
			QApplication::sendEvent(engine, new Core::EndOfStreamEvent(engine->currentSource()));
			break;
		case GST_MESSAGE_TAG:
			qDebug("GST_MESSAGE_TAG");
// 				GstTagList* list = 0;
// 				gst_message_parse_tag(msg, &list);
// 				if (list) {
// 					QMap<QString, QString> tags;
// 					gst_tag_list_foreach(list, foreachTag, &tags);
// 					gst_tag_list_free(list);
// 				}
			break;
		case GST_MESSAGE_STATE_CHANGED: {
			qDebug("GST_MESSAGE_STATE_CHANGED");
			if (msg->src != GST_OBJECT(pipeline))
				break;
			GstState old, state;
			gst_message_parse_state_changed(msg, &old, &state, 0);
			QApplication::sendEvent(engine, new StateChangeEvent(state, old));
			break;
		} case GST_MESSAGE_ERROR: {
			gchar *buffer = 0;
			GError *error = 0;
			gst_message_parse_error(msg, &error, &buffer);
			QString debug = QString::fromUtf8(buffer);
			g_free(buffer);
			buffer = gst_error_get_message(error->domain, error->code);
			QString errorMsg = QString::fromUtf8(buffer);
			g_free(buffer);
			qDebug("Error: %s Message:%s (%s) Code:%d", qPrintable(debug)
					, error->message, qPrintable(errorMsg), error->code);
			g_error_free(error);
			break;
		} case GST_MESSAGE_WARNING: {
			gchar *debug;
			GError *err;
			gst_message_parse_warning(msg, &err, &debug);
			qDebug("Warning: %s\nMessage:%s", debug, err->message);
			g_free(debug);
			g_error_free (err);
			break;
		} case GST_MESSAGE_DURATION: {
			qDebug("GST_MESSAGE_DURATION");
			GstFormat format = GST_FORMAT_TIME;
			gint64 duration = 0;
			gst_message_parse_duration(msg, &format, &duration);
			QApplication::sendEvent(engine, new Core::DurationEvent(duration));
			break;
		}
		case GST_MESSAGE_BUFFERING:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_ELEMENT:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_INFO:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_STREAM_STATUS:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_CLOCK_PROVIDE:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_NEW_CLOCK:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_STEP_DONE:
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_LATENCY: //only from 0.10.12
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		case GST_MESSAGE_ASYNC_DONE: //only from 0.10.13
			qDebug("GST_MESSAGE_BUFFERING");
			break;
		default:
			qDebug("%d: Invalid message type!", type);
			break;	
		}
		msleep(1);
	}
}

}
