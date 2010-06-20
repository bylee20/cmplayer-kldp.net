#include "gstvideoman.hpp"
#include "nativevideorenderer.hpp"
#include "global.hpp"
#include <QtCore/QDebug>
#include <gst/video/video.h>
#include "gstvideoman_p.hpp"

#include <QTime>

GST_BOILERPLATE(GstVideoMan, gst_video_man, GstBaseTransform, GST_TYPE_BASE_TRANSFORM);

static gboolean gst_video_man_set_caps(GstBaseTransform *trans, GstCaps *incaps, GstCaps *outcaps);
static gboolean gst_video_man_get_unit_size(GstBaseTransform *trans, GstCaps *caps, guint *size);
static GstFlowReturn gst_video_man_transform(GstBaseTransform *btrans, GstBuffer *inbuf, GstBuffer *outbuf);
static void gst_video_man_fixate_caps(GstBaseTransform *trans
		, GstPadDirection direction, GstCaps *caps, GstCaps *opp);

static GstCaps *gst_video_man_transform_caps(GstBaseTransform *trans, GstPadDirection dir, GstCaps *caps) {
	return GST_VIDEO_MAN(trans)->transformCaps(dir, caps);
}

static gboolean gst_video_man_set_caps(GstBaseTransform *trans, GstCaps *in, GstCaps *out) {
	return GST_VIDEO_MAN(trans)->setCaps(in, out);
}

static GstCaps *makeAcceptableCaps() {
	GstCaps *temp;
	GstCaps *caps = gst_caps_new_empty();
	for (int i = 0; i < PIX_FMT_NB; ++i) {
		temp = capsFromPixFmt((PixelFormat)i);
		if (temp != NULL)
			gst_caps_append(caps, temp);
	}
	return caps;
}

static GstCaps *makeProducibleCaps() {
	GstCaps *caps = gst_caps_new_empty();
	GstCaps *i420 = makeVideoCaps("video/x-raw-yuv"
		, "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('I', '4', '2', '0'), NULL);
	GstCaps *yv12 = makeVideoCaps("video/x-raw-yuv"
		, "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('Y', 'V', '1', '2'), NULL);
	gst_caps_append(caps, i420);
	gst_caps_append(caps, yv12);
	return caps;
}

static void gst_video_man_base_init (gpointer klass) {
	static bool init = false; if (!init) {avcodec_init();init = true;}

	GstElementClass *elem = GST_ELEMENT_CLASS (klass);
	static GstPadTemplate *src = gst_pad_template_new("src"
		, GST_PAD_SRC, GST_PAD_ALWAYS, makeProducibleCaps());
	static GstPadTemplate *sink = gst_pad_template_new("sink"
		, GST_PAD_SINK, GST_PAD_ALWAYS, makeAcceptableCaps());
	gst_element_class_add_pad_template(elem, src);
	gst_element_class_add_pad_template(elem, sink);
}

static void gst_video_man_finalize(GObject *obj) {
	GST_VIDEO_MAN(obj)->dtor();
	G_OBJECT_CLASS (parent_class)->finalize(obj);
}

static void gst_video_man_class_init(GstVideoManClass *klass) {
	GObjectClass *gobj = G_OBJECT_CLASS(klass);
	gobj->finalize = gst_video_man_finalize;

	GstBaseTransformClass *trans = GST_BASE_TRANSFORM_CLASS(klass);
	trans->transform_caps = GST_DEBUG_FUNCPTR (gst_video_man_transform_caps);
	trans->set_caps = GST_DEBUG_FUNCPTR (gst_video_man_set_caps);
	trans->get_unit_size = GST_DEBUG_FUNCPTR (gst_video_man_get_unit_size);
	trans->transform = GST_DEBUG_FUNCPTR (gst_video_man_transform);
	trans->fixate_caps = gst_video_man_fixate_caps;
	trans->passthrough_on_same_caps = FALSE;
}

static void gst_video_man_init(GstVideoMan *man, GstVideoManClass */*klass*/) {
	man->ctor();
}

static gboolean gst_video_man_get_unit_size(GstBaseTransform *trans, GstCaps *caps, guint *size) {
	return GST_VIDEO_MAN(trans)->getUnitSize(caps, size);
}

static GstFlowReturn gst_video_man_transform(GstBaseTransform *trans, GstBuffer *inbuf, GstBuffer *outbuf) {
	return GST_VIDEO_MAN(trans)->transform(inbuf, outbuf);
}

static void gst_video_man_fixate_caps(GstBaseTransform *trans
		, GstPadDirection direction, GstCaps *caps, GstCaps *opp) {
	GST_VIDEO_MAN(trans)->fixateCaps(direction, caps, opp);
}

/*********************************************************************/

void GstVideoMan::ctor() {
	gst_base_transform_set_qos_enabled(GST_BASE_TRANSFORM(this), TRUE);
	d = new Data;
	d->in_height = d->in_width = d->out_height = d->out_width = 0;
	d->from_pixfmt = d->to_pixfmt = PIX_FMT_NB;
	d->border_v = d->border_h = 0;
	d->crop_h = d->crop_v = 0;
	d->tempBuffer = 0;
	d->tempBufferSize = 0;
	d->in_format = d->out_format = GST_VIDEO_FORMAT_UNKNOWN;
	d->renderer = 0;
}

void GstVideoMan::dtor() {
	delete[] d->tempBuffer;
	for (OverlayMap::iterator it = d->overlay.begin(); it != d->overlay.end(); ++it)
		delete *it;
	delete d;
}

void GstVideoMan::updateTempBuffer() {
	const int size = (d->in_width*d->in_height*3>>1);
	if (size == d->tempBufferSize)
		return;
	d->mutex.lock();
	delete[] d->tempBuffer;
	d->tempBuffer = new uchar[d->tempBufferSize = size];
	d->mutex.unlock();
}

GstFlowReturn GstVideoMan::transform(GstBuffer *in, GstBuffer *out) {
	if (G_UNLIKELY(d->from_pixfmt == PIX_FMT_NB || d->to_pixfmt == PIX_FMT_NB))
		return GST_FLOW_NOT_NEGOTIATED;
	AVPicture from;
	avpicture_fill(&from, GST_BUFFER_DATA(in), d->from_pixfmt, d->in_width, d->in_height);
	QMutexLocker locker(&d->mutex);
	YUV420Frame temp;
	temp.init(d->tempBuffer, d->in_width, d->in_height);
	if (!temp.fromPicture(from, d->from_pixfmt, d->in_width, d->in_height))
		return GST_FLOW_NOT_SUPPORTED;
	YUV420Frame frame;
	frame.init(GST_BUFFER_DATA(out), d->out_format, d->out_width, d->out_height);
	frame.fill();
	frame.lay(temp, d->crop_h, d->crop_v);
	locker.unlock();
	for (OverlayMap::const_iterator it = d->overlay.begin(); it != d->overlay.end(); ++it) {
		const ImageOverlay &overlay = *(*it);
		if (overlay.image.isNull())
			continue;
		overlay.mutex.lock();
		frame.blend(overlay);
		overlay.mutex.unlock();
	}
	return GST_FLOW_OK;
}

bool GstVideoMan::getUnitSize(GstCaps *caps, guint *size) {
	g_assert(size);

	AVCodecContext *ctx = avcodec_alloc_context();
	g_assert(ctx != 0);
	ctx->pix_fmt = PIX_FMT_NB;

	capsWithCodec(caps, ctx);
	bool ret = G_LIKELY(ctx->pix_fmt != PIX_FMT_NB);
	av_free(ctx);
	if (ret) {
		GstVideoFormat format;
		int width, height;
		if ((ret = gst_video_format_parse_caps(caps, &format, &width, &height)))
			*size = gst_video_format_get_size(format, width, height);
	}
	return ret;
}

static inline int transform(int from, int delta) {
	const qint64 to = (qint64)from + (qint64)delta;
	return CLAMP(to, 1, G_MAXINT);
}

static bool transform(const GValue *from, int delta, GValue *to) {
	g_value_init(to, G_VALUE_TYPE(from));
	do {
		if (G_VALUE_HOLDS_INT(from)) {
			const int val = transform(g_value_get_int(from), delta);
			g_value_set_int(to, val);
		} else if (GST_VALUE_HOLDS_INT_RANGE(from)) {
			const int min = transform(gst_value_get_int_range_min(from), delta);
			const int max = transform(gst_value_get_int_range_max(from), delta);
			if (min > max)
				break;
			gst_value_set_int_range(to, min, max);
		} else if (GST_VALUE_HOLDS_LIST(from)) {
			for (uint i = 0; i < gst_value_list_get_size(from); ++i) {
				const GValue *oldVal = gst_value_list_get_value(from, i);
				GValue newVal = { 0, };
				if (transform(oldVal, delta, &newVal))
					gst_value_list_append_value(to, &newVal);
				g_value_unset(&newVal);
			}
			if (!gst_value_list_get_size(to))
				break;
		} else
			break;
		return true;
	} while (0);
	g_value_unset(to);
	return false;
}

static bool transform(GstStructure *str, int delta, const char *field) {
	const GValue *from = gst_structure_get_value(str, field);
	GValue to = {0,};
	if (!transform(from, delta, &to))
		return false;
	gst_structure_set_value(str, field, &to);
	g_value_unset(&to);
	return true;
}

void GstVideoMan::setBorder(int h, int v) {
	if (d->border_h == h && d->border_v == v)
		return;
	d->border_h = h;
	d->border_v = v;
	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(this));
}

void GstVideoMan::crop(int h, int v) {
	if (d->crop_h == h && d->crop_v == v)
		return;
	d->crop_h = h;
	d->crop_v = v;
	gst_base_transform_reconfigure(GST_BASE_TRANSFORM(this));
}

GstCaps *GstVideoMan::transformCaps(GstPadDirection dir, GstCaps *from) {
	GstCaps *def = 0;
	int dh = 0, dw = 0;
	GstPad *opp = 0;
	if (dir == GST_PAD_SINK) {
		def = makeAcceptableCaps();
		dh = d->border_v;
		dw = d->border_h;
		opp = GST_BASE_TRANSFORM(this)->srcpad;
	} else if (dir == GST_PAD_SRC) {
		def = makeProducibleCaps();
		dh = -d->border_v;
		dw = -d->border_h;
		opp = GST_BASE_TRANSFORM(this)->sinkpad;
	} else
		return 0;
	GstCaps *to = gst_caps_intersect(from, def);
	gst_caps_unref(def);
	if ((!d->border_v && !d->border_h) || !to)
		return to;
	const int size = gst_caps_get_size(to);
	for (int i=0; i<size; ++i) {
		GstStructure *str = gst_caps_get_structure(to, i);
		if (!::transform(str, dh, "height") || !::transform(str, dw, "width")) {
			qDebug() << "failed";
			gst_caps_unref(to);
			return gst_caps_new_empty();
		}
	}

	const GstCaps *templ = gst_pad_get_pad_template_caps(opp);
	GstCaps *ret = gst_caps_intersect(to, templ);
	gst_caps_unref(to);
	return ret;
}

void GstVideoMan::fixateCaps(GstPadDirection /*dir*/, GstCaps *caps, GstCaps *opp) {
	int width, height;
	if (!gst_video_format_parse_caps(caps, NULL, &width, &height))
		return;
	GstStructure *str = gst_caps_get_structure(opp, 0);
	gst_structure_fixate_field_nearest_int(str, "width", width);
	gst_structure_fixate_field_nearest_int(str, "height", height);
	qDebug() << "fix:" << width << height;
}

bool GstVideoMan::setCaps(GstCaps *in, GstCaps *out) {
	d->from_pixfmt = PIX_FMT_NONE;
	d->to_pixfmt = PIX_FMT_NONE;

	if (!gst_video_format_parse_caps(in, &d->in_format, &d->in_width, &d->in_height))
		return false;
	if (!gst_video_format_parse_caps(out, &d->out_format, &d->out_width, &d->out_height))
		return false;
	if ((d->in_width+d->border_h) != d->out_width || (d->in_height+d->border_v) != d->out_height)
		return false;
	updateTempBuffer();
	int num, den;
	if (!gst_video_parse_caps_framerate(in, &num, &den))
		return false;
	d->in_fps = (double)num/den;
	if (!gst_video_parse_caps_framerate(out, &num, &den))
		return false;
	d->out_fps = (double)num/den;
	if (!gst_video_parse_caps_pixel_aspect_ratio(in, &num, &den))
		return false;
	if (!qFuzzyCompare(d->in_fps, d->out_fps))
		return false;
	d->in_par = (double)num/den;
	if (!gst_video_parse_caps_pixel_aspect_ratio(out, &num, &den))
		return false;
	d->out_par = (double)num/den;

	AVCodecContext *ctx = avcodec_alloc_context();
	ctx->width = d->in_width;
	ctx->height = d->in_height;
	ctx->pix_fmt = PIX_FMT_NONE;
	if (!capsWithCodec(in, ctx)) {
		av_free(ctx);
		return false;
	}
	d->from_pixfmt = ctx->pix_fmt;
	ctx->pix_fmt = PIX_FMT_NONE;
	if (!capsWithCodec(out, ctx)) {
		av_free(ctx);
		return false;
	}
	d->to_pixfmt = ctx->pix_fmt;
	av_free(ctx);

	if (d->renderer) {
		VideoInfo info;
		info.fps = d->in_fps;
		info.height = d->in_height;
		info.width = d->in_width;
		d->renderer->setInfo(info);
	}

	return true;
}

ImageOverlay *GstVideoMan::getOverlay(int id) {
	for (OverlayMap::iterator it = d->overlay.begin(); it != d->overlay.end(); ++it) {
		if ((*it)->id == id)
			return *it;
	}
	return 0;
}

int GstVideoMan::newOverlay() {
	static int key = 0;
	ImageOverlay *overlay = new ImageOverlay(++key, 0);
	d->overlay.insert(overlay->zIndex, overlay);
	return overlay->id;
}

void GstVideoMan::setOverlay(int id, const QImage &image, const QPoint &pos) {
	ImageOverlay *overlay = getOverlay(id);
	if (!overlay)
		return;
	overlay->mutex.lock();
	QRect rect = image.rect();
	if (pos.x() < 0) {
		overlay->pos.setX(0);
		rect.setLeft(-pos.x());
	} else
		overlay->pos.setX(pos.x());
	if (pos.y() < 0) {
		overlay->pos.setY(0);
		rect.setTop(-pos.y());
	} else
		overlay->pos.setY(pos.y());
	overlay->image = image.copy(rect);
	overlay->mutex.unlock();
//	if (d->engine && d->engine->state() == PausedState) {
//		d->engine->flush();//seek(d->engine->position());
//	}
}

void GstVideoMan::setZIndex(int id, double zIndex) {
	ImageOverlay *overlay = getOverlay(id);
	if (overlay) {
		overlay->mutex.lock();
		overlay->zIndex = zIndex;
		overlay->mutex.unlock();
	}
}
