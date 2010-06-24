#include "videomanipulator_p.hpp"
#include "i420picture.hpp"
#include "videomanipulator.hpp"
#include <string.h>
#include <cmath>
#include <liboil/liboil.h>
#include <gst/controller/gstcontroller.h>
#include <gst/video/video.h>
#include <QtCore/QDebug>
#include "gst_fcs/gst_fcs_gstffmpegcodecmap.h"



bool ConvertToI420Filter::transform(I420Picture *out
		, const uchar *data, int width, int height, PixelFormat pix) {
	AVPicture pic;
	gst_ffmpegcsp_avpicture_fill(&pic, const_cast<uchar*>(data), pix, width, height, false);
	AVPicture out_pic;
	out_pic.data[0] = out->y;
	out_pic.data[1] = out->u;
	out_pic.data[2] = out->v;
	out_pic.linesize[0] = out->width_y;
	out_pic.linesize[1] = out_pic.linesize[2] = out->width_uv;
	return img_convert(&out_pic, PIX_FMT_YUV420P, &pic, pix, width, height) != -1;
}

inline static void mix(uchar *dest, int dest_w, int dest_h
		, const uchar *src, int src_w, int src_h
		, int crop_h, int crop_v) {
	const int copyLineSize = src_w - crop_h;
	const int copyLineCount = ((src_h - crop_v));
	if (copyLineSize <= 0 || copyLineCount <=0)
		return; // Nothing to draw
	const int src_x = (crop_h>>1);
	const int src_y = (crop_v>>1);
	const int dest_x = ((dest_w - src_w)>>1) + src_x;
	const int dest_y = ((dest_h - src_h)>>1) + src_y;
	dest += dest_y*dest_w + dest_x;
	src += src_y*src_w + src_x;
	for (int i=0; i<copyLineCount; ++i) {
		memcpy(dest, src, copyLineSize);
		dest += dest_w;
		src += src_w;
	}
}

CropMixFilter::CropMixFilter() {
	m_crop_h = m_crop_v = 0;
}

bool CropMixFilter::transform(I420Picture *out, const I420Picture &in) {
	const uchar black[3] = {16, 128, 128};
	oil_splat_u8_ns(out->y, &black[0], out->size_y());
	oil_splat_u8_ns(out->u, &black[1], out->size_uv());
	oil_splat_u8_ns(out->v, &black[2], out->size_uv());
	mix(out->y, out->width_y, out->height_y, in.y, in.width_y, in.height_y, m_crop_h, m_crop_v);
	mix(out->u, out->width_uv, out->height_uv, in.u, in.width_uv, in.height_uv, m_crop_h>>1, m_crop_v>>1);
	mix(out->v, out->width_uv, out->height_uv, in.v, in.width_uv, in.height_uv, m_crop_h>>1, m_crop_v>>1);
	return true;
}

void CropMixFilter::crop(int crop_h, int crop_v) {
	if (m_crop_h != crop_h || m_crop_v != crop_v) {
		m_crop_h = crop_h;
		m_crop_v = crop_v;
		rerender();
	}
}

/*****************************************************************************************/

GstBaseTransformClass *GstVideoManClass::parentClass = 0;

GType GstVideoManClass::getType() {
	static volatile gsize gonce = 0;
	if (g_once_init_enter(&gonce)) {
		GType type = gst_type_register_static_full(
			GST_TYPE_BASE_TRANSFORM,
			g_intern_static_string("GstVideoMan"),
			sizeof(GstVideoManClass),
			baseInit,
			0,	/* base_finalize */
			classInitTrampoline,
			0,	/* class_finalize */
			0,	/* class_data */
			sizeof(GstVideoMan),
			0,	/* n_preallocs */
			(GInstanceInitFunc)init,
			0,
			(GTypeFlags)0);
//		additional_initializations (type);
		g_once_init_leave(&gonce, (gsize)type);
	}
	return (GType)gonce;
}

void GstVideoManClass::classInitTrampoline(gpointer klass, gpointer /*data*/) {
	parentClass = reinterpret_cast<GstBaseTransformClass*>(g_type_class_peek_parent(klass));
	classInit(reinterpret_cast<GstVideoManClass*>(klass));
}

GstCaps *GstVideoManClass::makeSrcCaps() {
	return gst_ffmpeg_pixfmt_to_caps(PIX_FMT_YUV420P);
}

GstCaps *GstVideoManClass::makeSinkCaps() {
	GstCaps *caps = gst_caps_new_empty();
	for (int i = 0; i < PIX_FMT_NB; ++i) {
		const PixelFormat pix = static_cast<PixelFormat>(i);
		GstCaps *temp = gst_ffmpeg_pixfmt_to_caps(pix);
		if (temp)
			gst_caps_append(caps, temp);
	}
	return caps;
}

void GstVideoManClass::baseInit(gpointer klass) {
	GstElementClass *elem = GST_ELEMENT_CLASS(klass);
	static GstPadTemplate *src = gst_pad_template_new("src",
		GST_PAD_SRC, GST_PAD_ALWAYS, makeSrcCaps());
	static GstPadTemplate *sink = gst_pad_template_new("sink",
		GST_PAD_SINK, GST_PAD_ALWAYS, makeSinkCaps());
	gst_element_class_add_pad_template(elem, src);
	gst_element_class_add_pad_template(elem, sink);
}

void GstVideoManClass::classInit(GstVideoManClass *klass) {
	GObjectClass *gobj = G_OBJECT_CLASS(klass);
	gobj->finalize = finalize;

	GstBaseTransformClass *trans = GST_BASE_TRANSFORM_CLASS(klass);
	trans->transform_caps = transformCaps;
	trans->set_caps = setCaps;
	trans->get_unit_size = getUnitSize;
	trans->transform = transform;
	trans->fixate_caps = fixateCaps;
	trans->before_transform = beforeTransform;
	trans->passthrough_on_same_caps = FALSE;
}

void GstVideoManClass::init(GstVideoMan *man, GstVideoManClass */*klass*/) {
	man->ctor();
}

void GstVideoManClass::finalize(GObject *obj) {
	GST_VIDEO_MAN(obj)->dtor();
	G_OBJECT_CLASS(parentClass)->finalize(obj);
}

gboolean GstVideoManClass::setCaps(GstBaseTransform *trans, GstCaps *in, GstCaps *out) {
	GstVideoMan *man = GST_VIDEO_MAN(trans);
	GstVideoMan::Data *d = man->d;
	GstVideoFormat format;
	if (!gst_video_format_parse_caps(in, &format, &d->in_width, &d->in_height))
		return false;
	if (!gst_video_format_parse_caps(out, &format, &d->out_width, &d->out_height))
		return false;
	if ((d->in_width+d->border_h) != d->out_width || (d->in_height+d->border_v) != d->out_height)
		return false;
	int num, den;
	if (!gst_video_parse_caps_framerate(in, &num, &den))
		return false;
	d->in_fps = (double)num/den;
	if (!gst_video_parse_caps_framerate(out, &num, &den))
		return false;
	d->out_fps = (double)num/den;
	if (!qFuzzyCompare(d->in_fps, d->out_fps))
		return false;
	if (!gst_video_parse_caps_pixel_aspect_ratio(in, &num, &den))
		return false;
	d->in_par = (double)num/den;
	if (!gst_video_parse_caps_pixel_aspect_ratio(out, &num, &den))
		return false;
	d->out_par = (double)num/den;
	d->in_pix = gst_ffmpeg_caps_to_pixfmt(in);
	if (d->in_pix == PIX_FMT_NB)
		return false;
	const PixelFormat pix = gst_ffmpeg_caps_to_pixfmt(out);
	if (pix != PIX_FMT_YUV420P)
		return false;
	if (d->man) {
		VideoInfo in, out;
		in.fps = d->in_fps;
		in.height = d->in_height;
		in.width = d->in_width;
		in.pixelAspectRatio = d->in_par;
		out.fps = d->out_fps;
		out.width = d->out_width;
		out.height = d->out_height;
		out.pixelAspectRatio = d->out_par;
		d->man->setVideoInfo(in, out);
	}
	return true;
}

static inline int addDelta(int from, int delta) {
	const qint64 to = (qint64)from + (qint64)delta;
	return CLAMP(to, 1, G_MAXINT);
}

static bool addDelta(const GValue *from, int delta, GValue *to) {
	g_value_init(to, G_VALUE_TYPE(from));
	do {
		if (G_VALUE_HOLDS_INT(from)) {
			const int val = addDelta(g_value_get_int(from), delta);
			g_value_set_int(to, val);
		} else if (GST_VALUE_HOLDS_INT_RANGE(from)) {
			const int min = addDelta(gst_value_get_int_range_min(from), delta);
			const int max = addDelta(gst_value_get_int_range_max(from), delta);
			if (min > max)
				break;
			gst_value_set_int_range(to, min, max);
		} else if (GST_VALUE_HOLDS_LIST(from)) {
			for (uint i = 0; i < gst_value_list_get_size(from); ++i) {
				const GValue *oldVal = gst_value_list_get_value(from, i);
				GValue newVal = { 0, };
				if (addDelta(oldVal, delta, &newVal))
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

static bool addDelta(GstStructure *str, int delta, const char *field) {
	const GValue *from = gst_structure_get_value(str, field);
	GValue to = {0,};
	if (!addDelta(from, delta, &to))
		return false;
	gst_structure_set_value(str, field, &to);
	g_value_unset(&to);
	return true;
}

GstCaps *GstVideoManClass::transformCaps(GstBaseTransform *trans, GstPadDirection dir, GstCaps *from) {
	GstVideoMan::Data *d = GST_VIDEO_MAN(trans)->d;
	GstCaps *def = 0;
	int dh = 0, dw = 0;
	GstPad *opp = 0;
	if (dir == GST_PAD_SINK) {
		def = makeSinkCaps();
		dh = d->border_v;
		dw = d->border_h;
		opp = trans->srcpad;
	} else if (dir == GST_PAD_SRC) {
		def = makeSrcCaps();
		dh = -d->border_v;
		dw = -d->border_h;
		opp = trans->sinkpad;
	} else
		return 0;
	GstCaps *to = gst_caps_intersect(from, def);
	gst_caps_unref(def);
	if ((!d->border_v && !d->border_h) || !to)
		return to;
	const int size = gst_caps_get_size(to);
	for (int i=0; i<size; ++i) {
		GstStructure *str = gst_caps_get_structure(to, i);
		if (!addDelta(str, dh, "height") || !addDelta(str, dw, "width")) {
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

gboolean GstVideoManClass::getUnitSize(GstBaseTransform */*trans*/, GstCaps *caps, guint *size) {
	Q_ASSERT(size);
	GstVideoFormat format;
	int width, height;
	const PixelFormat pix = gst_ffmpeg_caps_to_pixfmt(caps);
	if (pix == PIX_FMT_NB)
		return false;
	if (!gst_video_format_parse_caps(caps, &format, &width, &height))
		return false;
	*size = gst_video_format_get_size(format, width, height);
	return true;
}


GstFlowReturn GstVideoManClass::transform(GstBaseTransform *trans, GstBuffer *in, GstBuffer *out) {
	VideoManipulator *man = GST_VIDEO_MAN(trans)->d->man;
	return man->transform(in, out);
}

void GstVideoManClass::fixateCaps(GstBaseTransform */*trans*/, GstPadDirection /*dir*/, GstCaps *caps, GstCaps *opp) {
	int width, height;
	if (!gst_video_format_parse_caps(caps, 0, &width, &height))
		return;
	GstStructure *str = gst_caps_get_structure(opp, 0);
	gst_structure_fixate_field_nearest_int(str, "width", width);
	gst_structure_fixate_field_nearest_int(str, "height", height);
}

void GstVideoManClass::beforeTransform(GstBaseTransform *trans, GstBuffer *in) {
	GstClockTime timestamp = GST_BUFFER_TIMESTAMP(in);
	GstClockTime streamTime = gst_segment_to_stream_time(&trans->segment, GST_FORMAT_TIME, timestamp);
	if (GST_CLOCK_TIME_IS_VALID(streamTime))
		gst_object_sync_values(G_OBJECT (trans), streamTime);
}

void GstVideoMan::ctor() {
	gst_base_transform_set_qos_enabled(GST_BASE_TRANSFORM(this), true);
	d = new Data;
	d->in_height = d->in_width = d->out_height = d->out_width = 0;
	d->border_v = d->border_h = 0;
	d->man = 0;
	d->in_pix = PIX_FMT_NB;
}

void GstVideoMan::dtor() {
	delete d;
}
