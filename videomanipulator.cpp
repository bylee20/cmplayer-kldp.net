#include "videomanipulator.hpp"
#include "videomanipulator_p.hpp"
#include <QtCore/QRect>

struct VideoManipulator::Data {
	GstVideoMan *man;
	int border_h, border_v;
	CropMixFilter *mixer;
};

VideoManipulator::VideoManipulator()
: d(new Data) {
}

VideoManipulator::~VideoManipulator() {
	delete d;
}

GstElement *VideoManipulator::element() const {
	return GST_ELEMENT(d->man);
}

QRect VideoManipulator::videoRect() const {
	const int x = (d->border_h + d->mixer->horizontalCropSize())>>1;
	const int y = (d->border_v + d->mixer->verticalCropSize())>>1;
//	const int width = d->out_width - (x<<1);
//	const int height = d->out_height - (y<<1);
//	return QRect(x, y, width, height);
}
//
//void GstVideoMan::setBorder(int h, int v) {
//	if (d->border_h == h && d->border_v == v)
//		return;
//	d->border_h = h;
//	d->border_v = v;
//	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(this));
//	rerender();
//}
//
//void GstVideoMan::crop(int h, int v) {
//	if (d->crop_h == h && d->crop_v == v)
//		return;
//	d->crop_h = h;
//	d->crop_v = v;
//	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(this));
//	rerender();
//}
//
//
//void GstVideoMan::reconfigure() {
//	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(this));
//}
//
//void GstVideoMan::renderIn(GstBuffer *buffer) {
////	d->filter[1]->transform(d->buffer, buffer);
////	const int size = gst_video_format_get_size(d->out_format, d->out_width, d->out_height);
////	if (size != GST_BUFFER_SIZE(buffer)) {
////		qDebug() << "buffer size mismatch!";
////		return;
////	}
////	YUV420Frame temp, frame;
////	temp.init(d->buffer->data, d->in_width, d->in_height);
////	frame.init(GST_BUFFER_DATA(buffer), d->out_format, d->out_width, d->out_height);
////	frame.fill();
////	frame.lay(temp, d->crop_h, d->crop_v);
////	for (OverlayMap::const_iterator it = d->overlay.begin(); it != d->overlay.end(); ++it) {
////		const ImageOverlay &overlay = *(*it);
////		if (overlay.image.isNull())
////			continue;
////		if (overlay.lock.tryLockForRead()) {
////			frame.blend(overlay);
////			overlay.lock.unlock();
////		} else
////			qDebug() << "cannot lock. pass it.";
////	}
//}
//
//void GstVideoMan::rerender() {
//	if (!d->renderer)
//		return;
//	GstState state;
//	gst_element_get_state(GST_ELEMENT(this), &state, 0, 100*GST_MSECOND);
//	if (state != GST_STATE_PAUSED)
//		return;
//	const int size = gst_video_format_get_size(GST_VIDEO_FORMAT_I420, d->out_width, d->out_height);
//	if (size <= 0)
//		return;
//	GstCaps *caps = GST_PAD_CAPS(GST_BASE_TRANSFORM(this)->srcpad);
//	if (!caps)
//		return;
//	GstBuffer *buffer = d->renderer->allocBuffer(size, caps);
//	renderIn(buffer);
//	d->renderer->showFrame(buffer);
//}
