#include "gstimageoverlay.hpp"
#include <gst/controller/gstcontroller.h>
#include <gst/video/video.h>
#include <QtCore/QLinkedList>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include "playengine.hpp"
#include "osdrenderer.hpp"

static GType gst_image_overlay_get_type();

GST_BOILERPLATE(GstImageOverlay, gst_image_overlay, GstBaseTransform, GST_TYPE_BASE_TRANSFORM);

GType GstImageOverlay::gtype() {return gst_image_overlay_get_type();}

//		"video/x-raw-rgb",
//		"width", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"height", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 2147483647, 1,
//		"bpp", G_TYPE_INT, 32,
//		"depth", G_TYPE_INT, 32,
//		"red_mask", G_TYPE_INT, int(0xff00),
//		"green_mask", G_TYPE_INT, int(0xff0000),
//		"blue_mask", G_TYPE_INT, int(0xff000000),
//		"alpha_mask", G_TYPE_INT, int(0xff),
//		"endianness", G_TYPE_INT, 4321,

GstCaps *GstImageOverlay::makeDefualtCaps() {
	GstCaps *caps = gst_caps_new_empty();
	gst_caps_append_structure(caps, gst_structure_new(
		"video/x-raw-yuv",
		"width", GST_TYPE_INT_RANGE, 1, 2147483647,
		"height", GST_TYPE_INT_RANGE, 1, 2147483647,
		"framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 2147483647, 1,
		"format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('Y', 'V', '1', '2'),
	NULL));
	gst_caps_append_structure(caps, gst_structure_new(
		"video/x-raw-yuv",
		"width", GST_TYPE_INT_RANGE, 1, 2147483647,
		"height", GST_TYPE_INT_RANGE, 1, 2147483647,
		"framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 2147483647, 1,
		"format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('I', '4', '2', '0'),
	NULL));
	return caps;
}

//	gst_caps_new_simple ("video/x-raw-yuv",
//		"width", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"height", GST_TYPE_INT_RANGE, 1, 2147483647,
//		"framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 2147483647, 1,
//		"format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('A', 'Y', 'U', 'V'), NULL)

void gst_image_overlay_base_init(gpointer klass) {
	GstElementClass *elem = GST_ELEMENT_CLASS(klass);

	GstCaps *caps = GstImageOverlay::makeDefualtCaps();
	GstPadTemplate *pad = gst_pad_template_new("sink", GST_PAD_SINK, GST_PAD_ALWAYS, caps);
	gst_element_class_add_pad_template(elem, pad);

	caps = GstImageOverlay::makeDefualtCaps();
	pad = gst_pad_template_new("src", GST_PAD_SRC, GST_PAD_ALWAYS, caps);
	gst_element_class_add_pad_template(elem, pad);
}

static void gst_image_overlay_finalize(GObject *obj) {
	delete GST_IMAGE_OVERLAY(obj)->d;
	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static GstFlowReturn gst_image_overlay_transform_ip(GstBaseTransform *base, GstBuffer *outbuf) {
	return GST_IMAGE_OVERLAY(base)->transform_ip(outbuf);
}

static gboolean gst_image_overlay_set_caps(GstBaseTransform *trans, GstCaps *in, GstCaps *out) {
	return GST_IMAGE_OVERLAY(trans)->set_caps(in, out);
}

static void gst_image_overlay_class_init(GstImageOverlayClass *klass) {
	GObjectClass *obj = G_OBJECT_CLASS(klass);
	obj->finalize = gst_image_overlay_finalize;

	GstBaseTransformClass *trans = GST_BASE_TRANSFORM_CLASS(klass);
	trans->transform_ip = gst_image_overlay_transform_ip;
	trans->set_caps = gst_image_overlay_set_caps;
}

static void gst_image_overlay_init(GstImageOverlay *self, GstImageOverlayClass */*klass*/) {
	self->d = new GstImageOverlay::Data;
}

inline static void blend (guint8 *dest, const guint8 *src
		, gint src_stride, gint dest_stride
		, gint src_width, gint src_height
		, gint dest_width, gint b_alpha) {
	gint i, j;
	gint src_add = src_stride - src_width;
	gint dest_add = dest_stride - dest_width;

	for (i = 0; i < src_height; i++) {
		for (j = 0; j < src_width; j++) {
#define BLEND(D,S,alpha) (((D) * (256 - (alpha)) + (S) * (alpha)) >> 8)
			*dest = BLEND (*dest, *src, b_alpha);
#undef BLEND
			dest++;
			src++;
		}
		src += src_add;
		dest += dest_add;
	}
}

#define Y_FROM_RGB(R, G, B) ((( 66*(int)(R)+129*(int)(G) +25*(int)(B))>>8)+16)
#define U_FROM_RGB(R, G, B) (((112*(int)(R) -94*(int)(G) -18*(int)(B))>>8)+128)
#define V_FROM_RGB(R, G, B) (((-38*(int)(R) -74*(int)(G)+112*(int)(B))>>8)+128)

#define Y_FROM_UCHAR_R(pR) (Y_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))
#define U_FROM_UCHAR_R(pR) (U_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))
#define V_FROM_UCHAR_R(pR) (V_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))

#define BLEND_ALPHA(D,S,A) (((int)(D)*(255 - (A)) + (int)(S) * (A))/255)
#define BLEND_Y_UCHAR_R(Y, pR) ()

void GstImageOverlay::blend(GstBuffer *buffer, const Image &image) {
	const int image_w = image.image.width();	const int image_h = image.image.height();
	const int frame_w = d->frameSize.width();	const int frame_h = d->frameSize.height();
	const int image_w_h = (image_w >> 1);		const int image_h_h = (image_h >> 1);
	const int left = image.pos.x();			const int top = image.pos.y();

	uchar *const frame = GST_BUFFER_DATA(buffer);
	int acc_u, acc_v, acc_a, alpha, i, j;
	uchar *y1 = frame + left + top*frame_w;		uchar *y2 = y1 + 1;
	uchar *y3 = y1 + frame_w;			uchar *y4 = y3 + 1;

	uchar *v = frame + frame_w*frame_h + (left >> 1) + (top >> 1)*(frame_w >> 1);
	uchar *u = v + ((frame_w*frame_h) >> 2);
	if (d->frameFormat == GST_VIDEO_FORMAT_I420)
		qSwap(v, u);

	const uchar *r1 = image.image.bits();		const uchar *r2 = r1 + 4;
	const uchar *r3 = r1 + (image_w << 2);		const uchar *r4 = r3 + 4;

	const int delta_y = (frame_w << 1) - image_w;
	const int delta_r = (image_w) << 2;
	const int delta_uv = (frame_w >> 1) - image_w_h;

	for (j = 0; j<image_h_h; ++j) {
		if ((j << 1) + top >= frame_h)
			break;
		for (i = 0; i<image_w_h; ++i) {
			if ((i << 1) + left < frame_w) {
				acc_u = acc_v = acc_a = 0;
#define DO_QUATER(n) {\
	alpha = r##n[3];\
	acc_u += U_FROM_UCHAR_R(r##n);\
	acc_v += V_FROM_UCHAR_R(r##n);\
	if (alpha) {\
		acc_a += alpha;\
		if (alpha == 0xff) *y##n = Y_FROM_UCHAR_R(r##n);\
		else *y##n = BLEND_ALPHA(*y##n, Y_FROM_UCHAR_R(r##n), alpha);\
}}
				DO_QUATER(1);		DO_QUATER(2);
				DO_QUATER(3);		DO_QUATER(4);
#undef DO_QUATER
				if (acc_a) {
					alpha = (acc_a >> 2);
					*u = BLEND_ALPHA(*u, (acc_u >> 2), alpha);
					*v = BLEND_ALPHA(*v, (acc_v >> 2), alpha);
				}
			}
			y1 += 2;	y2 += 2;
			y3 += 2;	y4 += 2;
			++u;		++v;
			r1 += 8;	r2 += 8;
			r3 += 8;	r4 += 8;
		}
		y1 += delta_y;		y2 += delta_y;
		y3 += delta_y;		y4 += delta_y;
		u += delta_uv;		v += delta_uv;
		r1 += delta_r;		r2 += delta_r;
		r3 += delta_r;		r4 += delta_r;
	}
}

GstFlowReturn GstImageOverlay::transform_ip(GstBuffer *outbuf) {
	if (GST_CLOCK_TIME_IS_VALID(GST_BUFFER_TIMESTAMP(outbuf)))
		gst_object_sync_values(G_OBJECT(this), GST_BUFFER_TIMESTAMP(outbuf));
	if (!gst_buffer_is_writable(outbuf))
		return GST_FLOW_OK;
	for (QMultiMap<double, Image>::const_iterator it = d->image.begin(); it != d->image.end(); ++it) {
		if (it->image.isNull())
			continue;
		it->mutex->lock();
		blend(outbuf, *it);
		it->mutex->unlock();
	}
	return GST_FLOW_OK;
}

bool GstImageOverlay::set_caps(GstCaps *in, GstCaps */*out*/) {
	QSize s;	GstVideoFormat f;
	gst_video_format_parse_caps(in, &f, &s.rwidth(), &s.rheight());
	d->frameSize = s;
	d->frameFormat = f;
	return true;
}

void GstImageOverlay::setImage(int id, const QImage &image, const QPoint &pos) {
	Image *img = getImage(id);
	if (img) {
		img->mutex->lock();
		QRect rect = image.rect();
		if (pos.x() < 0) {
			img->pos.setX(0);
			rect.setLeft(-pos.x());
		} else
			img->pos.setX(pos.x());
		if (pos.y() < 0) {
			img->pos.setY(0);
			rect.setTop(-pos.y());
		} else
			img->pos.setY(pos.y());
		img->image = image.copy(rect);
		img->mutex->unlock();
		if (d->engine && d->engine->state() == PausedState) {
			d->engine->flush();//seek(d->engine->position());
		}
	}
}

GstImageOverlay::Image *GstImageOverlay::getImage(int id) {
	for (QMultiMap<double, Image>::iterator it = d->image.begin(); it != d->image.end(); ++it) {
		if (it.value().id == id)
			return &it.value();
	}
	return 0;
}

void GstImageOverlay::setZIndex(int id, double zIndex) {
	Image *image = getImage(id);
	if (image) {
		image->mutex->lock();
		image->zIndex = zIndex;
		image->mutex->unlock();
	}
}

int GstImageOverlay::register_() const {
	static int key = 0;
	d->image.insert(0, Image(++key, 0))->mutex = new QMutex;
	return key;
}
