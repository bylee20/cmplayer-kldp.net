#include "videomanipulator.hpp"
#include "videoeqfilter.hpp"
#include "videomanipulator_p.hpp"
#include "i420picture.hpp"
#include "imageoverlayfilter.hpp"
#include "nativevideorenderer.hpp"
#include <gst/video/video.h>
#include <QtCore/QRect>
#include <QtCore/QDebug>

struct VideoManipulator::Data {
	GstVideoMan *man;
	CropMixFilter *mixer;
	ConvertToI420Filter *conv;
	ImageOverlayFilter *overlay;
	VideoInfo info_in, info_out;
	VideoEqFilter *eq;
	I420Picture pic;
	NativeVideoRenderer *renderer;
};

VideoManipulator::VideoManipulator()
: d(new Data) {
	static bool init = false; if (!init) {avcodec_init(); init = true;}
	d->man = GST_VIDEO_MAN(g_object_new(GstVideoManClass::getType(), 0));
	d->man->d->man = this;
	gst_object_ref(GST_OBJECT(d->man));
	d->mixer = new CropMixFilter;
	d->mixer->setManipulator(this);
	d->overlay = new ImageOverlayFilter;
	d->overlay->setManipulator(this);
	d->conv = new ConvertToI420Filter;
	d->eq = new VideoEqFilter;
	d->eq->setManipulator(this);
	d->renderer = 0;
}

VideoManipulator::~VideoManipulator() {
	gst_object_unref(GST_OBJECT(d->man));
	delete d->mixer;
	delete d->conv;
	delete d->eq;
	delete d->overlay;
	delete d;
}

GstElement *VideoManipulator::element() const {
	return GST_ELEMENT(d->man);
}

VideoEqFilter *VideoManipulator::equalizer() const {
	return d->eq;
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
}

void VideoManipulator::reconfigure() {
	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(d->man));
}

void VideoManipulator::setRenderer(NativeVideoRenderer *renderer) {
	d->renderer = renderer;
}

void VideoManipulator::rerender() {
	if (!d->renderer)
		return;
	GstState state;
	if (!gst_element_get_state(GST_ELEMENT(d->man), &state, 0, 100*GST_MSECOND))
		return;
	if (state != GST_STATE_PAUSED)
		return;
	const int size = gst_video_format_get_size(GST_VIDEO_FORMAT_I420, d->info_out.width, d->info_out.height);
	if (size <= 0)
		return;
	GstBaseTransform *trans = GST_BASE_TRANSFORM(d->man);
	if (!trans->srcpad || !trans->srcpad->caps)
		return;
	GstCaps *caps = trans->srcpad->caps;
	GstBuffer *buffer = d->renderer->allocBuffer(size, caps);
	if (render(buffer))
		d->renderer->showFrame(buffer);
	gst_buffer_unref(buffer);
}

bool VideoManipulator::render(GstBuffer *buffer) {
	if (!buffer)
		return false;
	I420Picture out_pic;
	out_pic.init(buffer);
	d->mixer->transform(&out_pic, d->pic);
	d->overlay->transform(&out_pic);
	return true;
}

void VideoManipulator::crop(int h, int v) {
	d->mixer->crop(h, v);
}

void VideoManipulator::setVideoInfo(const VideoInfo &in, const VideoInfo &out) {
	d->info_in = in;
	d->info_out = out;
	emit videoInfoObtained(d->info_in);
}

GstFlowReturn VideoManipulator::transform(GstBuffer *in, GstBuffer *out) {
	if (d->man->d->in_pix == PIX_FMT_NB || d->info_in.width <= 0 || d->info_in.height <= 0)
		return GST_FLOW_NOT_NEGOTIATED;
	d->pic.free();
	d->pic.alloc(d->info_in.width, d->info_in.height);
	if (!d->conv->transform(&d->pic, in->data, d->info_in.width, d->info_in.height, d->man->d->in_pix))
		return GST_FLOW_NOT_SUPPORTED;
	d->eq->transform(&d->pic);
	render(out);
	return GST_FLOW_OK;
}

ImageOverlayFilter *VideoManipulator::overlay() const {
	return d->overlay;
}

