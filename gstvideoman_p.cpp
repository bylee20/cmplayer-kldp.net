#include "gstvideoman_p.hpp"
#include <QtCore/QDebug>
#include <string.h>
#include <gst/gst.h>
#include <gst/controller/gstcontroller.h>
#include <libavutil/pixfmt.h>
#include <liboil/liboil.h>
extern "C" {
#include <libswscale/swscale.h>
}

void YUV420Frame::init(uchar *data, int width, int height) {
	width_y = width;
	height_y = height;
	width_uv = (width>>1);
	height_uv = (height>>1);
	y = data;
	u = y + size_y();
	v = u + size_uv();
}

void YUV420Frame::init(uchar *data, GstVideoFormat format, int width, int height) {
	y = data + gst_video_format_get_component_offset(format, 0, width, height);
	u = data + gst_video_format_get_component_offset(format, 1, width, height);
	v = data + gst_video_format_get_component_offset(format, 2, width, height);
	width_y = gst_video_format_get_row_stride(format, 0, width);
	width_uv = gst_video_format_get_row_stride(format, 1, width);
	height_y = gst_video_format_get_component_height(format, 0, height);
	height_uv = gst_video_format_get_component_height(format, 1, height);
}

void YUV420Frame::fill() {
	const uchar black[3] = {16, 128, 128};
	oil_splat_u8_ns(y, &black[0], size_y());
	oil_splat_u8_ns(u, &black[1], size_uv());
	oil_splat_u8_ns(v, &black[2], size_uv());
}

bool YUV420Frame::fromPicture(const AVPicture &pic, PixelFormat format, int width, int height) {
	width_y = width;
	width_uv = (width>>1);
	height_y = height;
	height_uv = (height>>1);
	uchar *dest[3] = {y, u, v};
	int line[3] = {width_y, width_uv, width_uv};
	SwsContext *ctx = sws_getContext(width, height, format
		, width, height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, 0, 0, 0);
	return sws_scale(ctx, pic.data, pic.linesize, 0, height, dest, line) != -1;
}

inline static void lay_y(uchar *dest_y1, int dest_w, int dest_h
		, const uchar *src_y1, int src_w, int src_h
		, int crop_h, int crop_v) {
	const int copyLineSize = src_w - crop_h;
	const int copyLineCount = ((src_h - crop_v)>>1);
	if (copyLineSize <= 0 || copyLineCount <=0)
		return; // Nothing to draw
	const int src_x = (crop_h>>1);
	const int src_y = (crop_v>>1);
	const int dest_x = ((dest_w - src_w)>>1) + src_x;
	const int dest_y = ((dest_h - src_h)>>1) + src_y;
	const int dest_w_d = (dest_w<<1);
	const int src_w_d = (src_w<<1);
	dest_y1 += dest_y*dest_w + dest_x;
	uchar *dest_y2 = dest_y1 + dest_w;
	src_y1 += src_y*src_w + src_x;
	const uchar *src_y2 = src_y1 + src_w;
	for (int i=0; i<copyLineCount; ++i) {
		memcpy(dest_y1, src_y1, copyLineSize);
		memcpy(dest_y2, src_y2, copyLineSize);
		dest_y1 += dest_w_d;
		dest_y2 += dest_w_d;
		src_y1 += src_w_d;
		src_y2 += src_w_d;
	}
}

inline static void lay_uv(uchar *dest_u, uchar *dest_v, int dest_w, int dest_h
		, const uchar *src_u, const uchar *src_v, int src_w, int src_h
		, int crop_h, int crop_v) {
	const int copyLineSize = src_w - crop_h;
	const int copyLineCount = src_h - crop_v;
	if (copyLineSize <= 0 || copyLineCount <=0)
		return; // Nothing to draw
	const int src_x = (crop_h>>1);
	const int src_y = (crop_v>>1);
	const int dest_x = ((dest_w - src_w)>>1) + src_x;
	const int dest_y = ((dest_h - src_h)>>1) + src_y;
	const int dest_offset = dest_y*dest_w + dest_x;
	const int src_offset = src_y*src_w + src_x;
	dest_u += dest_offset;
	dest_v += dest_offset;
	src_u += src_offset;
	src_v += src_offset;
	for (int i=0; i<copyLineCount; ++i) {
		memcpy(dest_u, src_u, copyLineSize);
		memcpy(dest_v, src_v, copyLineSize);
		dest_u += dest_w;
		dest_v += dest_w;
		src_u += src_w;
		src_v += src_w;
	}
}

void YUV420Frame::lay(const YUV420Frame &image, int crop_h, int crop_v) {
	lay_y(y, width_y, height_y, image.y, image.width_y, image.height_y, crop_h, crop_v);
	lay_uv(u, v, width_uv, height_uv, image.u, image.v, image.width_uv, image.height_uv
		, crop_h>>1, crop_v>>1);
}

GstCaps *makeVideoCaps(const char *mimetype, const char *fieldname, ...) {
	GstCaps *caps = gst_caps_new_simple(mimetype
		, "width", GST_TYPE_INT_RANGE, 1, G_MAXINT
		, "height", GST_TYPE_INT_RANGE, 1, G_MAXINT
		, "framerate", GST_TYPE_FRACTION_RANGE, 0, 1, G_MAXINT, 1, NULL);
	GstStructure *str = gst_caps_get_structure (caps, 0);
	if (str) {
		va_list var_args;
		va_start(var_args, fieldname);
		gst_structure_set_valist(str, fieldname, var_args);
		va_end(var_args);
	}
	return caps;
}

GstCaps *capsFromPixFmt(enum PixelFormat pix_fmt) {
//  GstCaps *caps = NULL;

	int bpp = 0, depth = 0, endianness = 0;
	gulong g_mask = 0, r_mask = 0, b_mask = 0, a_mask = 0;
	guint32 fmt = 0;

	switch (pix_fmt) {
	case PIX_FMT_YUV420P:
		fmt = GST_MAKE_FOURCC ('I', '4', '2', '0');
		break;
	case PIX_FMT_NV12:
		fmt = GST_MAKE_FOURCC ('N', 'V', '1', '2');
		break;
	case PIX_FMT_NV21:
		fmt = GST_MAKE_FOURCC ('N', 'V', '2', '1');
		break;
//    case PIX_FMT_YVU420P:
//      fmt = GST_MAKE_FOURCC ('Y', 'V', '1', '2');
//      break;
//#define PIX_FMT_YUV422 PIX_FMT_YUYV422
	case PIX_FMT_YUYV422:
		fmt = GST_MAKE_FOURCC ('Y', 'U', 'Y', '2');
		break;
	case PIX_FMT_UYVY422:
		fmt = GST_MAKE_FOURCC ('U', 'Y', 'V', 'Y');
		break;
//    case PIX_FMT_YVYU422:
//      fmt = GST_MAKE_FOURCC ('Y', 'V', 'Y', 'U');
//      break;
	case PIX_FMT_UYYVYY411:
		fmt = GST_MAKE_FOURCC ('I', 'Y', 'U', '1');
		break;
	case PIX_FMT_RGB24:
		bpp = depth = 24;
		endianness = G_BIG_ENDIAN;
		r_mask = 0xff0000;
		g_mask = 0x00ff00;
		b_mask = 0x0000ff;
		break;
	case PIX_FMT_BGR24:
		bpp = depth = 24;
		endianness = G_BIG_ENDIAN;
		r_mask = 0x0000ff;
		g_mask = 0x00ff00;
		b_mask = 0xff0000;
		break;
	case PIX_FMT_YUV422P:
		fmt = GST_MAKE_FOURCC ('Y', '4', '2', 'B');
		break;
	case PIX_FMT_YUV444P:
		fmt = GST_MAKE_FOURCC ('Y', '4', '4', '4');
		break;
//	case PIX_FMT_xRGB32:
//		bpp = 32;
//		depth = 24;
//		endianness = G_BIG_ENDIAN;
//#if (G_BYTE_ORDER == G_BIG_ENDIAN)
//		r_mask = 0xff000000;
//		g_mask = 0x00ff0000;
//		b_mask = 0x0000ff00;
//#else
//		r_mask = 0x000000ff;
//		g_mask = 0x0000ff00;
//		b_mask = 0x00ff0000;
//#endif
//		break;
//	case PIX_FMT_BGRx32:
//		bpp = 32;
//		depth = 24;
//		endianness = G_BIG_ENDIAN;
//#if (G_BYTE_ORDER == G_BIG_ENDIAN)
//		r_mask = 0x000000ff;
//		g_mask = 0x0000ff00;
//		b_mask = 0x00ff0000;
//#else
//		r_mask = 0xff000000;
//		g_mask = 0x00ff0000;
//		b_mask = 0x0000ff00;
//#endif
//		break;
	case PIX_FMT_RGBA:
		bpp = 32;
		depth = 32;
		endianness = G_BIG_ENDIAN;
#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
		r_mask = 0x00ff0000;
		g_mask = 0x0000ff00;
		b_mask = 0x000000ff;
		a_mask = 0xff000000;
#else
		r_mask = 0x0000ff00;
		g_mask = 0x00ff0000;
		b_mask = 0xff000000;
		a_mask = 0x000000ff;
#endif
		break;
	case PIX_FMT_BGRA:
		bpp = 32;
		depth = 32;
		endianness = G_BIG_ENDIAN;
#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
		r_mask = 0x0000ff00;
		g_mask = 0x00ff0000;
		b_mask = 0xff000000;
		a_mask = 0x000000ff;
#else
		r_mask = 0x00ff0000;
		g_mask = 0x0000ff00;
		b_mask = 0x000000ff;
		a_mask = 0xff000000;
#endif
		break;
	case PIX_FMT_ARGB:
		bpp = 32;
		depth = 32;
		endianness = G_BIG_ENDIAN;
#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
		r_mask = 0xff000000;
		g_mask = 0x00ff0000;
		b_mask = 0x0000ff00;
		a_mask = 0x000000ff;
#else
		r_mask = 0x000000ff;
		g_mask = 0x0000ff00;
		b_mask = 0x00ff0000;
		a_mask = 0xff000000;
#endif
		break;
	case PIX_FMT_ABGR:
		bpp = 32;
		depth = 32;
		endianness = G_BIG_ENDIAN;
#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
		r_mask = 0x000000ff;
		g_mask = 0x0000ff00;
		b_mask = 0x00ff0000;
		a_mask = 0xff000000;
#else
		r_mask = 0xff000000;
		g_mask = 0x00ff0000;
		b_mask = 0x0000ff00;
		a_mask = 0x000000ff;
#endif
		break;
	case PIX_FMT_YUV410P:
		fmt = GST_MAKE_FOURCC ('Y', 'U', 'V', '9');
		break;
//	case PIX_FMT_YVU410P:
//		fmt = GST_MAKE_FOURCC ('Y', 'V', 'U', '9');
//		break;
	case PIX_FMT_YUV411P:
		fmt = GST_MAKE_FOURCC ('Y', '4', '1', 'B');
		break;
	case PIX_FMT_RGB565:
		bpp = depth = 16;
		endianness = G_BYTE_ORDER;
		r_mask = 0xf800;
		g_mask = 0x07e0;
		b_mask = 0x001f;
		break;
	case PIX_FMT_RGB555:
		bpp = 16;
		depth = 15;
		endianness = G_BYTE_ORDER;
		r_mask = 0x7c00;
		g_mask = 0x03e0;
		b_mask = 0x001f;
		break;
	case PIX_FMT_PAL8:
		bpp = depth = 8;
		endianness = G_BYTE_ORDER;
		break;
//	case PIX_FMT_V308:
//		fmt = GST_MAKE_FOURCC ('v', '3', '0', '8');
//		break;
//	case PIX_FMT_AYUV4444:
//		fmt = GST_MAKE_FOURCC ('A', 'Y', 'U', 'V');
//		break;
	case PIX_FMT_GRAY8:{
//		bpp = depth = 8;
		GstCaps *caps = makeVideoCaps("video/x-raw-gray"
			, "bpp", G_TYPE_INT, bpp, "depth", G_TYPE_INT, depth, NULL);
		GstCaps *temp = makeVideoCaps("video/x-raw-yuv"
			, "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('Y', '8', '0', '0'), NULL);
		gst_caps_append(caps, temp);
		return caps;
	} case PIX_FMT_GRAY16LE:
//		bpp = depth = 16;
		return makeVideoCaps("video/x-raw-gray"
			, "bpp", G_TYPE_INT, 16, "depth", G_TYPE_INT, 16
			, "endianness", G_TYPE_INT, G_LITTLE_ENDIAN, NULL);
	case PIX_FMT_GRAY16BE:
//		bpp = depth = 16;
		return makeVideoCaps("video/x-raw-gray"
			, "bpp", G_TYPE_INT, 16, "depth", G_TYPE_INT, 16
			, "endianness", G_TYPE_INT, G_BIG_ENDIAN, NULL);
	default:
		break;
	}
	if (!bpp) {
		if (!fmt)
			return 0;
		return makeVideoCaps("video/x-raw-yuv"
			, "format", GST_TYPE_FOURCC, fmt, NULL);
	}
	if (a_mask != 0) {
		return makeVideoCaps("video/x-raw-rgb"
			, "bpp", G_TYPE_INT, bpp
			, "depth", G_TYPE_INT, depth
			, "red_mask", G_TYPE_INT, r_mask
			, "green_mask", G_TYPE_INT, g_mask
			, "blue_mask", G_TYPE_INT, b_mask
			, "alpha_mask", G_TYPE_INT, a_mask
			, "endianness", G_TYPE_INT, endianness
			, NULL);
	} else if (r_mask != 0) {
		return makeVideoCaps("video/x-raw-rgb"
			, "bpp", G_TYPE_INT, bpp
			, "depth", G_TYPE_INT, depth
			, "red_mask", G_TYPE_INT, r_mask
			, "green_mask", G_TYPE_INT, g_mask
			, "blue_mask", G_TYPE_INT, b_mask
			, "endianness", G_TYPE_INT, endianness
			, NULL);
	} else {
		return makeVideoCaps("video/x-raw-rgb"
			, "bpp", G_TYPE_INT, bpp
			, "depth", G_TYPE_INT, depth
			, "endianness", G_TYPE_INT, endianness
			, NULL);
		//	if (context && context->pix_fmt == PIX_FMT_PAL8) {
		//	  gst_ffmpeg_set_palette (caps, context);
		//	}
	}
}

bool capsWithCodec(GstCaps *caps, AVCodecContext *ctx) {
	if (!ctx || gst_caps_get_size(caps) != 1)
		return false;

	GstVideoFormat format;
	if (!gst_video_format_parse_caps(caps, &format, &ctx->width, &ctx->height))
		return false;

	int num, den;
	if (!gst_video_parse_caps_framerate(caps, &num, &den))
		return false;
	ctx->time_base.den = num;
	ctx->time_base.num = den;

	switch (format) {
	case GST_VIDEO_FORMAT_I420:
	case GST_VIDEO_FORMAT_YV12:
		ctx->pix_fmt = PIX_FMT_YUV420P;
		break;
	case GST_VIDEO_FORMAT_YUY2:
		ctx->pix_fmt = PIX_FMT_YUYV422;
		break;
	case GST_VIDEO_FORMAT_UYVY:
		ctx->pix_fmt = PIX_FMT_UYVY422;
		break;
//	case GST_VIDEO_FORMAT_AYUV:
	case GST_VIDEO_FORMAT_RGBx:
	case GST_VIDEO_FORMAT_RGBA:
		ctx->pix_fmt = PIX_FMT_RGBA;
		break;
	case GST_VIDEO_FORMAT_RGB:
		ctx->pix_fmt = PIX_FMT_RGB24;
		break;
	case GST_VIDEO_FORMAT_BGRx:
	case GST_VIDEO_FORMAT_BGRA:
		ctx->pix_fmt = PIX_FMT_BGRA;
		break;
	case GST_VIDEO_FORMAT_xRGB:
	case GST_VIDEO_FORMAT_ARGB:
		ctx->pix_fmt = PIX_FMT_ARGB;
		break;
	case GST_VIDEO_FORMAT_xBGR:
	case GST_VIDEO_FORMAT_ABGR:
		ctx->pix_fmt = PIX_FMT_ABGR;
		break;
	case GST_VIDEO_FORMAT_BGR:
		ctx->pix_fmt = PIX_FMT_BGR24;
		break;
	case GST_VIDEO_FORMAT_Y41B:
		ctx->pix_fmt = PIX_FMT_YUV411P;
		break;
	case GST_VIDEO_FORMAT_Y42B:
		ctx->pix_fmt = PIX_FMT_YUV422P;
		break;
//		  GST_VIDEO_FORMAT_YVYU,
	case GST_VIDEO_FORMAT_Y444:
		ctx->pix_fmt = PIX_FMT_YUV444P;
		break;
//	case GST_VIDEO_FORMAT_v210:
//		  GST_VIDEO_FORMAT_v216,
	case GST_VIDEO_FORMAT_NV12:
		ctx->pix_fmt = PIX_FMT_NV12;
		break;
	case GST_VIDEO_FORMAT_NV21:
		ctx->pix_fmt = PIX_FMT_NV21;
		break;
	case GST_VIDEO_FORMAT_GRAY8:
		ctx->pix_fmt = PIX_FMT_GRAY8;
		break;
	case GST_VIDEO_FORMAT_GRAY16_BE:
		ctx->pix_fmt = PIX_FMT_GRAY16BE;
		break;
	case GST_VIDEO_FORMAT_GRAY16_LE:
		ctx->pix_fmt = PIX_FMT_GRAY16LE;
		break;
	default:
		ctx->pix_fmt = PIX_FMT_NONE;
		return false;
	}
	return true;
}

#define Y_FROM_RGB(R, G, B) ((( 66*(int)(R)+129*(int)(G) +25*(int)(B))>>8)+16)
#define U_FROM_RGB(R, G, B) (((112*(int)(R) -94*(int)(G) -18*(int)(B))>>8)+128)
#define V_FROM_RGB(R, G, B) (((-38*(int)(R) -74*(int)(G)+112*(int)(B))>>8)+128)

#define Y_FROM_UCHAR_R(pR) (Y_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))
#define U_FROM_UCHAR_R(pR) (U_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))
#define V_FROM_UCHAR_R(pR) (V_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))

#define BLEND_ALPHA(D,S,A) (((int)(D)*(255 - (A)) + (int)(S) * (A))/255)
#define BLEND_Y_UCHAR_R(Y, pR) ()

void YUV420Frame::blend(const ImageOverlay &overlay) {
	const int image_w = overlay.image.width();
	const int image_h = overlay.image.height();
	const int frame_w = qMin(width_y, (width_uv<<1));
	const int frame_h = qMin(height_y, (height_uv<<1));
	const int image_w_h = (image_w >> 1);
	const int image_h_h = (image_h >> 1);
	const int left = overlay.pos.x();
	const int top = overlay.pos.y();
	const int y_offset = left + top*frame_w;
	const int uv_offset = (left >> 1) + (top >> 1)*(frame_w >> 1);
	int acc_u, acc_v, acc_a, alpha, i, j;
	uchar *y1 = y + y_offset;
	uchar *y2 = y1 + 1;
	uchar *y3 = y1 + frame_w;
	uchar *y4 = y3 + 1;
	uchar *v = this->v + uv_offset;
	uchar *u = this->u + uv_offset;

	const uchar *r1 = overlay.image.bits();		const uchar *r2 = r1 + 4;
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

