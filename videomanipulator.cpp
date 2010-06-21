#include "videomanipulator.hpp"
#include "videomanipulator_p.hpp"
#include "i420picture.hpp"
#include "imageoverlayfilter.hpp"
#include <QtCore/QRect>
#include <QtCore/QDebug>

struct VideoManipulator::Data {
	GstVideoMan *man;
	CropMixFilter *mixer;
	ImageOverlayFilter *overlay;
	VideoInfo info_in, info_out;
};

VideoManipulator::VideoManipulator()
: d(new Data) {
	d->man = GST_VIDEO_MAN(g_object_new(GstVideoManClass::getType(), 0));
	d->man->d->man = this;
	gst_object_ref(GST_OBJECT(d->man));
	d->mixer = new CropMixFilter;
	d->mixer->setManipulator(this);
	d->overlay = new ImageOverlayFilter;
	d->overlay->setManipulator(this);
}

VideoManipulator::~VideoManipulator() {
	gst_object_unref(GST_OBJECT(d->man));
	delete d->mixer;
	delete d;
}

GstElement *VideoManipulator::element() const {
	return GST_ELEMENT(d->man);
}

QRect VideoManipulator::videoRect() const {
	const int x = (d->man->d->border_h + d->mixer->horizontalCropSize())>>1;
	const int y = (d->man->d->border_v + d->mixer->verticalCropSize())>>1;
	const int width = d->info_out.width - (x<<1);
	const int height = d->info_out.height - (y<<1);
	return QRect(x, y, width, height);
}

void VideoManipulator::setBorder(int h, int v) {
	if (d->man->d->border_h == h && d->man->d->border_v == v)
		return;
	d->man->d->border_h = h;
	d->man->d->border_v = v;
	reconfigure();
	rerender();
}

void VideoManipulator::reconfigure() {
	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(d->man));
}

void VideoManipulator::rerender() {

}

void VideoManipulator::crop(int h, int v) {
	d->mixer->crop(h, v);
}

void VideoManipulator::setVideoInfo(const VideoInfo &in, const VideoInfo &out) {
	d->info_in = in;
	d->info_out = out;
	emit videoInfoObtained(d->info_in);
}

void VideoManipulator::transform(GstBuffer *in, GstBuffer *out) {
	I420Picture in_pic, out_pic;
	in_pic.init(in);
	out_pic.init(out);
//	d->mixer->transform(&out_pic, in_pic);
//	d->overlay->transform(&out_pic);
}

ImageOverlayFilter *VideoManipulator::overlay() const {
	return d->overlay;
}

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
