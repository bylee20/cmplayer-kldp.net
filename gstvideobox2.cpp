#include "gstvideobox2.hpp"
#include <QtCore/QDebug>
// from GStreamer gstvideobox
#include <math.h>
#include <liboil/liboil.h>
#include <string.h>

#include <gst/controller/gstcontroller.h>

/* From videotestsrc.c */
static const guint8 yuv_colors_Y = 16;
static const guint8 yuv_colors_U = 128;
static const guint8 yuv_colors_V = 128;

static const gint cog_ycbcr_sdtv_to_ycbcr_hdtv_matrix_8bit[] = {
  256, -30, -53, 10600,
  0, 261, 29, -4367,
  0, 19, 262, -3289,
};

static const gint cog_ycbcr_hdtv_to_ycbcr_sdtv_matrix_8bit[] = {
  256, 25, 49, -9536,
  0, 253, -28, 3958,
  0, -19, 252, 2918,
};

static const gint cog_identity_matrix_8bit[] = {
  256, 0, 0, 0,
  0, 256, 0, 0,
  0, 0, 256, 0,
};

#define APPLY_MATRIX(m,o,v1,v2,v3) ((m[o*4] * v1 + m[o*4+1] * v2 + m[o*4+2] * v3 + m[o*4+3]) >> 8)

static void fill_planar_yuv(GstVideoFormat format, guint8 * dest, gboolean sdtv, gint width, gint height) {
	guint8 empty_pixel[3] = {yuv_colors_Y, yuv_colors_U, yuv_colors_V};
  guint8 *destY, *destU, *destV;
  gint strideY, strideUV;
  gint heightY, heightUV;

  strideY = gst_video_format_get_row_stride (format, 0, width);
  strideUV = gst_video_format_get_row_stride (format, 1, width);

  destY =
      dest + gst_video_format_get_component_offset (format, 0, width, height);
  destU =
      dest + gst_video_format_get_component_offset (format, 1, width, height);
  destV =
      dest + gst_video_format_get_component_offset (format, 2, width, height);

  heightY = gst_video_format_get_component_height (format, 0, height);
  heightUV = gst_video_format_get_component_height (format, 1, height);

  oil_splat_u8_ns (destY, &empty_pixel[0], strideY * heightY);
  oil_splat_u8_ns (destU, &empty_pixel[1], strideUV * heightUV);
  oil_splat_u8_ns (destV, &empty_pixel[2], strideUV * heightUV);
}

static void
copy_i420_i420 (GstVideoFormat dest_format, guint8 * dest,
    gboolean dest_sdtv, gint dest_width, gint dest_height, gint dest_x,
    gint dest_y, GstVideoFormat src_format, const guint8 * src,
    gboolean src_sdtv, gint src_width, gint src_height, gint src_x, gint src_y,
    gint w, gint h)
{
  gint i, j;
  guint8 *destY, *destU, *destV;
  const guint8 *srcY, *srcU, *srcV;
  guint8 *destY2;
  const guint8 *srcY2, *srcU2, *srcV2;
  gint dest_strideY, dest_strideUV;
  gint src_strideY, src_strideUV;
  gint src_y_idx, src_uv_idx;
  gint dest_y_idx, dest_uv_idx;
  gint matrix[12];
  gint y1, y2, y3, y4;
  gint u1, u2, u3, u4;
  gint v1, v2, v3, v4;

  dest_strideY =
      gst_video_format_get_row_stride (GST_VIDEO_FORMAT_I420, 0, dest_width);
  dest_strideUV =
      gst_video_format_get_row_stride (GST_VIDEO_FORMAT_I420, 1, dest_width);
  src_strideY =
      gst_video_format_get_row_stride (GST_VIDEO_FORMAT_I420, 0, src_width);
  src_strideUV =
      gst_video_format_get_row_stride (GST_VIDEO_FORMAT_I420, 1, src_width);

  destY =
      dest + gst_video_format_get_component_offset (GST_VIDEO_FORMAT_I420, 0,
      dest_width, dest_height);
  destU =
      dest + gst_video_format_get_component_offset (dest_format, 1,
      dest_width, dest_height);
  destV =
      dest + gst_video_format_get_component_offset (dest_format, 2,
      dest_width, dest_height);

  srcY =
      src + gst_video_format_get_component_offset (GST_VIDEO_FORMAT_I420, 0,
      src_width, src_height);
  srcU =
      src + gst_video_format_get_component_offset (src_format, 1,
      src_width, src_height);
  srcV =
      src + gst_video_format_get_component_offset (src_format, 2,
      src_width, src_height);


  destY = destY + dest_y * dest_strideY + dest_x;
  destU = destU + (dest_y / 2) * dest_strideUV + dest_x / 2;
  destV = destV + (dest_y / 2) * dest_strideUV + dest_x / 2;

  srcY = srcY + src_y * src_strideY + src_x;
  srcU = srcU + (src_y / 2) * src_strideUV + src_x / 2;
  srcV = srcV + (src_y / 2) * src_strideUV + src_x / 2;

  destY2 = destY + dest_strideY;
  srcY2 = srcY + src_strideY;

  h = dest_y + h;
  w = dest_x + w;

  if (src_sdtv != dest_sdtv)
    memcpy (matrix,
	dest_sdtv ? cog_ycbcr_hdtv_to_ycbcr_sdtv_matrix_8bit :
	cog_ycbcr_sdtv_to_ycbcr_hdtv_matrix_8bit, 12 * sizeof (gint));
  else
    memcpy (matrix, cog_identity_matrix_8bit, 12 * sizeof (gint));

  /* 1. Handle the first destination scanline specially if it
   *    doesn't start at the macro pixel boundary, i.e. blend
   *    with the background! */
  if (dest_y % 2 == 1) {
    /* 1.1. Handle the first destination pixel if it doesn't
     *      start at the macro pixel boundary, i.e. blend with
     *      the background! */
    if (dest_x % 2 == 1) {
      y1 = srcY[0];
      u1 = srcU[0];
      v1 = srcV[0];

      destY[0] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[0] =
	  CLAMP ((3 * destU[0] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 4, 0,
	  255);
      destV[0] =
	  CLAMP ((3 * destV[0] + APPLY_MATRIX (matrix, 2, y1, u1, v1)) / 4, 0,
	  255);

      j = dest_x + 1;
      src_y_idx = dest_y_idx = dest_uv_idx = 1;
      src_uv_idx = (src_x % 2) + 1;
    } else {
      j = dest_x;
      src_y_idx = dest_y_idx = dest_uv_idx = 0;
      src_uv_idx = (src_x % 2);
    }

    /* 1.2. Copy all macro pixels from the source to the destination
     *      but blend with the background because we're only filling
     *      the lower part of the macro pixels. */
    for (; j < w - 1; j += 2) {
      y1 = srcY[src_y_idx];
      y2 = srcY[src_y_idx + 1];

      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];
      src_uv_idx++;
      u2 = srcU[src_uv_idx / 2];
      v2 = srcV[src_uv_idx / 2];
      src_uv_idx++;

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY[dest_y_idx + 1] =
	  CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);
      destU[dest_uv_idx] =
	  CLAMP ((2 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 1, y2, u2, v2)) / 4, 0, 255);
      destV[dest_uv_idx] =
	  CLAMP ((2 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 2, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 2, y2, u2, v2)) / 4, 0, 255);

      dest_y_idx += 2;
      src_y_idx += 2;
      dest_uv_idx++;
    }

    /* 1.3. Now copy the last pixel if one exists and blend it
     *      with the background because we only fill part of
     *      the macro pixel. In case this is the last pixel of
     *      the destination we will a larger part. */
    if (j == w - 1 && j == dest_width - 1) {
      y1 = srcY[src_y_idx];
      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 2, 0,
	  255);
      destV[dest_uv_idx] =
	  CLAMP ((destV[dest_uv_idx] + APPLY_MATRIX (matrix, 2, y1, u1,
		  v1)) / 2, 0, 255);
    } else if (j == w - 1) {
      y1 = srcY[src_y_idx];
      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (3 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 4,
	  0, 255);
      destV[dest_uv_idx] =
	  CLAMP ((3 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 2, y1, u1,
		  v1)) / 4, 0, 255);
    }

    destY += dest_strideY;
    destY2 += dest_strideY;
    destU += dest_strideUV;
    destV += dest_strideUV;
    srcY += src_strideY;
    srcY2 += src_strideY;
    src_y++;
    if (src_y % 2 == 0) {
      srcU += src_strideUV;
      srcV += src_strideUV;
    }
    i = dest_y + 1;
  } else {
    i = dest_y;
  }

  /* 2. Copy all macro pixel scanlines, the destination scanline
   *    now starts at macro pixel boundary. */
  for (; i < h - 1; i += 2) {
    /* 2.1. Handle the first destination pixel if it doesn't
     *      start at the macro pixel boundary, i.e. blend with
     *      the background! */

    srcU2 = srcU;
    srcV2 = srcV;
    if (src_y % 2 == 1) {
      srcU2 += src_strideUV;
      srcV2 += src_strideUV;
    }

    if (dest_x % 2 == 1) {
      y1 = srcY[0];
      y2 = srcY2[0];
      u1 = srcU[0];
      v1 = srcV[0];
      u2 = srcU2[0];
      v2 = srcV2[0];

      destY[0] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY2[0] = CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);
      destU[0] = CLAMP (
	  (2 * destU[0] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 1, y2, u2, v2)) / 4, 0, 255);
      destV[0] = CLAMP (
	  (2 * destV[0] + APPLY_MATRIX (matrix, 2, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 2, y2, u2, v2)) / 4, 0, 255);
      j = dest_x + 1;
      src_y_idx = dest_y_idx = dest_uv_idx = 1;
      src_uv_idx = (src_x % 2) + 1;
    } else {
      j = dest_x;
      src_y_idx = dest_y_idx = dest_uv_idx = 0;
      src_uv_idx = (src_x % 2);
    }

    /* 2.2. Copy all macro pixels from the source to the destination.
     *      All pixels now start at macro pixel boundary, i.e. no
     *      blending with the background is necessary. */
    for (; j < w - 1; j += 2) {
      y1 = srcY[src_y_idx];
      y2 = srcY[src_y_idx + 1];
      y3 = srcY2[src_y_idx];
      y4 = srcY2[src_y_idx + 1];

      u1 = srcU[src_uv_idx / 2];
      u3 = srcU2[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];
      v3 = srcV2[src_uv_idx / 2];
      src_uv_idx++;
      u2 = srcU[src_uv_idx / 2];
      u4 = srcU2[src_uv_idx / 2];
      v2 = srcV[src_uv_idx / 2];
      v4 = srcV2[src_uv_idx / 2];
      src_uv_idx++;

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY[dest_y_idx + 1] =
	  CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);
      destY2[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y3, u3, v3), 0, 255);
      destY2[dest_y_idx + 1] =
	  CLAMP (APPLY_MATRIX (matrix, 0, y4, u4, v4), 0, 255);

      destU[dest_uv_idx] = CLAMP (
	  (APPLY_MATRIX (matrix, 1, y1, u1, v1) + APPLY_MATRIX (matrix, 1, y2,
		  u2, v2) + APPLY_MATRIX (matrix, 1, y3, u3,
		  v3) + APPLY_MATRIX (matrix, 1, y4, u4, v4)) / 4, 0, 255);
      destV[dest_uv_idx] = CLAMP (
	  (APPLY_MATRIX (matrix, 2, y1, u1, v1) + APPLY_MATRIX (matrix, 2, y2,
		  u2, v2) + APPLY_MATRIX (matrix, 2, y3, u3,
		  v3) + APPLY_MATRIX (matrix, 2, y4, u4, v4)) / 4, 0, 255);

      dest_y_idx += 2;
      src_y_idx += 2;
      dest_uv_idx++;
    }

    /* 2.3. Now copy the last pixel if one exists and blend it
     *      with the background because we only fill part of
     *      the macro pixel. In case this is the last pixel of
     *      the destination we will a larger part. */
    if (j == w - 1 && j == dest_width - 1) {
      y1 = srcY[src_y_idx];
      y2 = srcY2[src_y_idx];

      u1 = srcU[src_uv_idx / 2];
      u2 = srcU2[src_uv_idx / 2];

      v1 = srcV[src_uv_idx / 2];
      v2 = srcV2[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY2[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (APPLY_MATRIX (matrix, 1, y1, u1, v1) + APPLY_MATRIX (matrix, 2, y2,
		  u2, v2)) / 2, 0, 255);
      destV[dest_uv_idx] = CLAMP (
	  (APPLY_MATRIX (matrix, 1, y1, u1, v1) + APPLY_MATRIX (matrix, 2, y2,
		  u2, v2)) / 2, 0, 255);
    } else if (j == w - 1) {
      y1 = srcY[src_y_idx];
      y2 = srcY2[src_y_idx];

      u1 = srcU[src_uv_idx / 2];
      u2 = srcU2[src_uv_idx / 2];

      v1 = srcV[src_uv_idx / 2];
      v2 = srcV2[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY2[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (2 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 2, y2, u2, v2)) / 4, 0, 255);
      destV[dest_uv_idx] = CLAMP (
	  (2 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 2, y2, u2, v2)) / 4, 0, 255);
    }

    destY += 2 * dest_strideY;
    destY2 += 2 * dest_strideY;
    destU += dest_strideUV;
    destV += dest_strideUV;
    srcY += 2 * src_strideY;
    srcY2 += 2 * src_strideY;

    src_y += 2;
    srcU += src_strideUV;
    srcV += src_strideUV;
  }

  /* 3. Handle the last scanline if one exists. This again
   *    doesn't start at macro pixel boundary but should
   *    only fill the upper part of the macro pixels. */
  if (i == h - 1 && i == dest_height - 1) {
    /* 3.1. Handle the first destination pixel if it doesn't
     *      start at the macro pixel boundary, i.e. blend with
     *      the background! */
    if (dest_x % 2 == 1) {
      y1 = srcY[0];
      u1 = srcU[0];
      v1 = srcV[0];

      destY[0] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[0] =
	  CLAMP ((destU[0] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 2, 0, 255);
      destV[0] =
	  CLAMP ((destV[0] + APPLY_MATRIX (matrix, 2, y1, u1, v1)) / 2, 0, 255);

      j = dest_x + 1;
      src_y_idx = dest_y_idx = dest_uv_idx = 1;
      src_uv_idx = (src_x % 2) + 1;
    } else {
      j = dest_x;
      src_y_idx = dest_y_idx = dest_uv_idx = 0;
      src_uv_idx = (src_x % 2);
    }

    /* 3.2. Copy all macro pixels from the source to the destination
     *      but blend with the background because we're only filling
     *      the upper part of the macro pixels. */
    for (; j < w - 1; j += 2) {
      y1 = srcY[src_y_idx];
      y2 = srcY[src_y_idx + 1];

      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];
      src_uv_idx++;
      u2 = srcU[src_uv_idx / 2];
      v2 = srcV[src_uv_idx / 2];
      src_uv_idx++;

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY[dest_y_idx + 1] =
	  CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);

      destU[dest_uv_idx] = CLAMP (
	  (2 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 1, y2, u2, v2)) / 4, 0, 255);
      destV[dest_uv_idx] = CLAMP (
	  (2 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 2, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 2, y2, u2, v2)) / 4, 0, 255);

      dest_y_idx += 2;
      src_y_idx += 2;
      dest_uv_idx++;
    }

    /* 3.3. Now copy the last pixel if one exists and blend it
     *      with the background because we only fill part of
     *      the macro pixel. In case this is the last pixel of
     *      the destination we will a larger part. */
    if (j == w - 1 && j == dest_width - 1) {
      y1 = srcY[src_y_idx];
      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 2, 0,
	  255);
      destV[dest_uv_idx] =
	  CLAMP ((destV[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1)) / 2, 0, 255);
    } else if (j == w - 1) {
      y1 = srcY[src_y_idx];
      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (3 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 4,
	  0, 255);
      destV[dest_uv_idx] =
	  CLAMP ((3 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1)) / 4, 0, 255);
    }
  } else if (i == h - 1) {
    /* 3.1. Handle the first destination pixel if it doesn't
     *      start at the macro pixel boundary, i.e. blend with
     *      the background! */
    if (dest_x % 2 == 1) {
      y1 = srcY[0];
      u1 = srcU[0];
      v1 = srcV[0];

      destY[0] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[0] =
	  CLAMP ((3 * destU[0] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 4, 0,
	  255);
      destV[0] =
	  CLAMP ((3 * destV[0] + APPLY_MATRIX (matrix, 2, y1, u1, v1)) / 4, 0,
	  255);

      j = dest_x + 1;
      src_y_idx = dest_y_idx = dest_uv_idx = 1;
      src_uv_idx = (src_x % 2) + 1;
    } else {
      j = dest_x;
      src_y_idx = dest_y_idx = dest_uv_idx = 0;
      src_uv_idx = (src_x % 2);
    }

    /* 3.2. Copy all macro pixels from the source to the destination
     *      but blend with the background because we're only filling
     *      the upper part of the macro pixels. */
    for (; j < w - 1; j += 2) {
      y1 = srcY[src_y_idx];
      y2 = srcY[src_y_idx + 1];

      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];
      src_uv_idx++;
      u2 = srcU[src_uv_idx / 2];
      v2 = srcV[src_uv_idx / 2];
      src_uv_idx++;

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destY[dest_y_idx + 1] =
	  CLAMP (APPLY_MATRIX (matrix, 0, y2, u2, v2), 0, 255);

      destU[dest_uv_idx] = CLAMP (
	  (2 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 1, y2, u2, v2)) / 4, 0, 255);
      destV[dest_uv_idx] = CLAMP (
	  (2 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 2, y1, u1,
		  v1) + APPLY_MATRIX (matrix, 2, y2, u2, v2)) / 4, 0, 255);

      dest_y_idx += 2;
      src_y_idx += 2;
      dest_uv_idx++;
    }

    /* 3.3. Now copy the last pixel if one exists and blend it
     *      with the background because we only fill part of
     *      the macro pixel. In case this is the last pixel of
     *      the destination we will a larger part. */
    if (j == w - 1 && j == dest_width - 1) {
      y1 = srcY[src_y_idx];
      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 2, 0,
	  255);
      destV[dest_uv_idx] =
	  CLAMP ((destV[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1)) / 2, 0, 255);
    } else if (j == w - 1) {
      y1 = srcY[src_y_idx];
      u1 = srcU[src_uv_idx / 2];
      v1 = srcV[src_uv_idx / 2];

      destY[dest_y_idx] = CLAMP (APPLY_MATRIX (matrix, 0, y1, u1, v1), 0, 255);
      destU[dest_uv_idx] = CLAMP (
	  (3 * destU[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1, v1)) / 4,
	  0, 255);
      destV[dest_uv_idx] =
	  CLAMP ((3 * destV[dest_uv_idx] + APPLY_MATRIX (matrix, 1, y1, u1,
		  v1)) / 4, 0, 255);
    }
  }
}

enum
{
  PROP_0,
  PROP_LEFT,
  PROP_RIGHT,
  PROP_TOP,
  PROP_BOTTOM,
      /* FILL ME */
};

static GstStaticPadTemplate gst_video_box2_src_template =
    GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (
	GST_VIDEO_CAPS_YUV ("I420") ";"
	GST_VIDEO_CAPS_YUV ("YV12"))
    );

static GstStaticPadTemplate gst_video_box2_sink_template =
    GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (
	GST_VIDEO_CAPS_YUV ("I420") ";"
	GST_VIDEO_CAPS_YUV ("YV12"))
    );

GST_BOILERPLATE (GstVideoBox2, gst_video_box2, GstBaseTransform, GST_TYPE_BASE_TRANSFORM);

static void gst_video_box2_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_video_box2_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_video_box2_recalc_transform (GstVideoBox2 * video_box);
static GstCaps *gst_video_box2_transform_caps (GstBaseTransform * trans,
    GstPadDirection direction, GstCaps * from);
static gboolean gst_video_box2_set_caps (GstBaseTransform * trans,
    GstCaps * in, GstCaps * out);
static gboolean gst_video_box2_get_unit_size (GstBaseTransform * trans,
    GstCaps * caps, guint * size);
static GstFlowReturn gst_video_box2_transform (GstBaseTransform * trans,
    GstBuffer * in, GstBuffer * out);
static void gst_video_box2_before_transform (GstBaseTransform * trans,
    GstBuffer * in);

static void gst_video_box2_base_init(gpointer g_class) {
  GstElementClass *element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class, "Video box filter",
      "Filter/Effect/Video",
      "Resizes a video by adding borders or cropping",
      "Wim Taymans <wim@fluendo.com>");

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_video_box2_sink_template));
  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_video_box2_src_template));
}

static void gst_video_box2_finalize (GObject * object) {
  GstVideoBox2 *video_box = GST_VIDEO_BOX2 (object);

  if (video_box->mutex) {
    g_mutex_free (video_box->mutex);
    video_box->mutex = NULL;
  }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void gst_video_box2_class_init (GstVideoBox2Class * klass) {
	GObjectClass *gobject_class = (GObjectClass *) klass;
	GstBaseTransformClass *trans_class = (GstBaseTransformClass *) klass;

	gobject_class->set_property = gst_video_box2_set_property;
	gobject_class->get_property = gst_video_box2_get_property;
	gobject_class->finalize = gst_video_box2_finalize;

	g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_LEFT
		, g_param_spec_int ("left", "Left", "Pixels to box at left (<0  = add a border)"
		, G_MININT, G_MAXINT, 0
		, GParamFlags(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_CONTROLLABLE)));
	g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_RIGHT
		, g_param_spec_int ("right", "Right", "Pixels to box at right (<0 = add a border)"
		, G_MININT, G_MAXINT, 0
		, GParamFlags(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_CONTROLLABLE)));
	g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TOP
		, g_param_spec_int ("top", "Top", "Pixels to box at top (<0 = add a border)"
		, G_MININT, G_MAXINT, 0
		, GParamFlags(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_CONTROLLABLE)));
	g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_BOTTOM
		, g_param_spec_int ("bottom", "Bottom", "Pixels to box at bottom (<0 = add a border)"
		, G_MININT, G_MAXINT, 0
		, GParamFlags(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_CONTROLLABLE)));

	trans_class->transform = GST_DEBUG_FUNCPTR(gst_video_box2_transform);
	trans_class->before_transform = GST_DEBUG_FUNCPTR(gst_video_box2_before_transform);
	trans_class->transform_caps = GST_DEBUG_FUNCPTR (gst_video_box2_transform_caps);
	trans_class->set_caps = GST_DEBUG_FUNCPTR (gst_video_box2_set_caps);
	trans_class->get_unit_size = GST_DEBUG_FUNCPTR(gst_video_box2_get_unit_size);
//	trans_class->fixate_caps = GST_DEBUG_FUNCPTR (gst_video_box2_fixate_caps);
}

static void gst_video_box2_init(GstVideoBox2 *video_box, GstVideoBox2Class */*g_class*/) {
	video_box->box_right = 0;
	video_box->box_left = 0;
	video_box->box_top = 0;
	video_box->box_bottom = 0;
	video_box->crop_right = 0;
	video_box->crop_left = 0;
	video_box->crop_top = 0;
	video_box->crop_bottom = 0;
	video_box->mutex = g_mutex_new ();
}

static void
gst_video_box2_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstVideoBox2 *video_box = GST_VIDEO_BOX2 (object);

  g_mutex_lock (video_box->mutex);
  switch (prop_id) {
    case PROP_LEFT:
      video_box->box_left = g_value_get_int (value);
      if (video_box->box_left < 0) {
	video_box->border_left = -video_box->box_left;
	video_box->crop_left = 0;
      } else {
	video_box->border_left = 0;
	video_box->crop_left = video_box->box_left;
      }
      break;
    case PROP_RIGHT:
      video_box->box_right = g_value_get_int (value);
      if (video_box->box_right < 0) {
	video_box->border_right = -video_box->box_right;
	video_box->crop_right = 0;
      } else {
	video_box->border_right = 0;
	video_box->crop_right = video_box->box_right;
      }
      break;
    case PROP_TOP:
      video_box->box_top = g_value_get_int (value);
      if (video_box->box_top < 0) {
	video_box->border_top = -video_box->box_top;
	video_box->crop_top = 0;
      } else {
	video_box->border_top = 0;
	video_box->crop_top = video_box->box_top;
      }
      break;
    case PROP_BOTTOM:
      video_box->box_bottom = g_value_get_int (value);
      if (video_box->box_bottom < 0) {
	video_box->border_bottom = -video_box->box_bottom;
	video_box->crop_bottom = 0;
      } else {
	video_box->border_bottom = 0;
	video_box->crop_bottom = video_box->box_bottom;
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
  gst_video_box2_recalc_transform (video_box);

  GST_DEBUG_OBJECT (video_box, "Calling reconfigure");
  gst_base_transform_reconfigure (GST_BASE_TRANSFORM_CAST (video_box));

  g_mutex_unlock (video_box->mutex);
}

static void
gst_video_box2_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstVideoBox2 *video_box = GST_VIDEO_BOX2 (object);

  switch (prop_id) {
    case PROP_LEFT:
      g_value_set_int (value, video_box->box_left);
      break;
    case PROP_RIGHT:
      g_value_set_int (value, video_box->box_right);
      break;
    case PROP_TOP:
      g_value_set_int (value, video_box->box_top);
      break;
    case PROP_BOTTOM:
      g_value_set_int (value, video_box->box_bottom);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static inline int gst_video_box2_transform_dimension(int val, int delta) {
	qint64 new_val = (qint64) val + (qint64) delta;
	new_val = CLAMP(new_val, 1, G_MAXINT);
	return new_val;
}

static bool gst_video_box2_transform_dimension_value(const GValue *src_val, int delta, GValue *dest_val) {
	bool ret = true;
	g_value_init(dest_val, G_VALUE_TYPE(src_val));
	if (G_VALUE_HOLDS_INT(src_val)) {
		int ival = g_value_get_int(src_val);
		ival = gst_video_box2_transform_dimension(ival, delta);
		g_value_set_int(dest_val, ival);
	} else if (GST_VALUE_HOLDS_INT_RANGE(src_val)) {
		int min = gst_value_get_int_range_min(src_val);
		int max = gst_value_get_int_range_max(src_val);
		min = gst_video_box2_transform_dimension(min, delta);
		max = gst_video_box2_transform_dimension(max, delta);
		if (min > max) {
			ret = false;
			g_value_unset(dest_val);
		} else {
			gst_value_set_int_range(dest_val, min, max);
		}
	} else if (GST_VALUE_HOLDS_LIST(src_val)) {
		for (int i = 0; i < gst_value_list_get_size (src_val); ++i) {
			const GValue *list_val;
			GValue newval = { 0, };
			list_val = gst_value_list_get_value(src_val, i);
			if (gst_video_box2_transform_dimension_value(list_val, delta, &newval))
				gst_value_list_append_value(dest_val, &newval);
			g_value_unset(&newval);
		}
		if (gst_value_list_get_size(dest_val) == 0) {
			g_value_unset(dest_val);
			ret = false;
		}
	} else {
		g_value_unset(dest_val);
		ret = false;
	}
	return ret;
}

static GstCaps *gst_video_box2_transform_caps(GstBaseTransform *trans, GstPadDirection direction, GstCaps *from) {
  GstVideoBox2 *video_box = GST_VIDEO_BOX2 (trans);
  GstCaps *to, *ret;
  const GstCaps *templ;
  const gchar *name;
  GstStructure *structure;
  GstPad *other;

  to = gst_caps_copy (from);
  /* Just to be sure... */
  gst_caps_truncate (to);
  structure = gst_caps_get_structure (to, 0);

  /* Transform width/height */
    gint dw = 0, dh = 0;
    const GValue *v;
    GValue w_val = { 0, };
    GValue h_val = { 0, };

    /* calculate width and height */
    if (direction == GST_PAD_SINK) {
      dw -= video_box->box_left;
      dw -= video_box->box_right;
    } else {
      dw += video_box->box_left;
      dw += video_box->box_right;
    }

    if (direction == GST_PAD_SINK) {
      dh -= video_box->box_top;
      dh -= video_box->box_bottom;
    } else {
      dh += video_box->box_top;
      dh += video_box->box_bottom;
    }

    v = gst_structure_get_value (structure, "width");
    if (!gst_video_box2_transform_dimension_value (v, dw, &w_val)) {
      GST_WARNING_OBJECT (video_box, "could not tranform width value with dw=%d"
	  ", caps structure=%" GST_PTR_FORMAT, dw, structure);
      gst_caps_unref (to);
      to = gst_caps_new_empty ();
      return to;
    }
    gst_structure_set_value (structure, "width", &w_val);

    v = gst_structure_get_value (structure, "height");
    if (!gst_video_box2_transform_dimension_value (v, dh, &h_val)) {
      g_value_unset (&w_val);
      gst_caps_unref (to);
      to = gst_caps_new_empty ();
      return to;
    }
    gst_structure_set_value (structure, "height", &h_val);
    g_value_unset (&w_val);
    g_value_unset (&h_val);

   name = gst_structure_get_name (structure);
  if (g_str_equal (name, "video/x-raw-yuv")) {
    guint32 fourcc;

    if (gst_structure_get_fourcc (structure, "format", &fourcc) &&
	    (fourcc == GST_STR_FOURCC ("I420") || fourcc == GST_STR_FOURCC ("YV12"))) {
      GValue list = { 0, };
      GValue val = { 0, };
      GstStructure *s2;

      /* get rid of format */
      gst_structure_remove_field (structure, "format");
      gst_structure_remove_field (structure, "color-matrix");
      gst_structure_remove_field (structure, "chroma-site");

      s2 = gst_structure_copy (structure);

      g_value_init (&list, GST_TYPE_LIST);
      g_value_init (&val, GST_TYPE_FOURCC);
      gst_value_set_fourcc (&val, GST_STR_FOURCC ("I420"));
      gst_value_list_append_value (&list, &val);
      g_value_reset (&val);
      gst_value_set_fourcc (&val, GST_STR_FOURCC ("YV12"));
      gst_value_list_append_value (&list, &val);
      g_value_unset (&val);
      gst_structure_set_value (structure, "format", &list);
      g_value_unset (&list);

      gst_caps_append_structure (to, s2);
    }
  }

  /* filter against set allowed caps on the pad */
  other = (direction == GST_PAD_SINK) ? trans->srcpad : trans->sinkpad;

  templ = gst_pad_get_pad_template_caps (other);
  ret = gst_caps_intersect (to, templ);
  gst_caps_unref (to);

  GST_DEBUG_OBJECT (video_box, "direction %d, transformed %" GST_PTR_FORMAT
      " to %" GST_PTR_FORMAT, direction, from, ret);

  return ret;
}

static gboolean
gst_video_box2_recalc_transform (GstVideoBox2 * video_box)
{
//  if (video_box->in_format == video_box->out_format &&
//      video_box->box_left == 0 && video_box->box_right == 0 &&
//      video_box->box_top == 0 && video_box->box_bottom == 0 &&
//      video_box->in_sdtv == video_box->out_sdtv) {
//
//    gst_base_transform_set_passthrough (GST_BASE_TRANSFORM_CAST (video_box),
//	TRUE);
//  } else {
    gst_base_transform_set_passthrough (GST_BASE_TRANSFORM_CAST (video_box),
	FALSE);
//  }
  return true;
}

static gboolean gst_video_box2_set_caps (GstBaseTransform *trans, GstCaps *in, GstCaps *out) {
	GstVideoBox2 *video_box = GST_VIDEO_BOX2(trans);

	g_mutex_lock (video_box->mutex);

	bool ret = gst_video_format_parse_caps(in, &video_box->in_format
		, &video_box->in_width, &video_box->in_height);
	ret &= gst_video_format_parse_caps(out, &video_box->out_format
		, &video_box->out_width, &video_box->out_height);

	const char *matrix = gst_video_parse_caps_color_matrix(in);
	video_box->in_sdtv = matrix ? g_str_equal(matrix, "sdtv") : TRUE;
	matrix = gst_video_parse_caps_color_matrix(out);
	video_box->out_sdtv = matrix ? g_str_equal(matrix, "sdtv") : TRUE;

	if (!ret || video_box->in_format != video_box->out_format) {
		g_mutex_unlock (video_box->mutex);
		return false;
	}
	ret = gst_video_box2_recalc_transform (video_box);
	g_mutex_unlock (video_box->mutex);
	return ret;
}

static gboolean gst_video_box2_get_unit_size(GstBaseTransform *trans, GstCaps *caps, guint *size) {
	qDebug() << "size!!";
	GstVideoBox2 *video_box = GST_VIDEO_BOX2(trans);
	GstVideoFormat format;
	gint width, height;
	gboolean ret;

	g_assert (size);

	ret = gst_video_format_parse_caps (caps, &format, &width, &height);
	if (!ret) {
		GST_ERROR_OBJECT (video_box, "Invalid caps: %" GST_PTR_FORMAT, caps);
		return FALSE;
	}

	*size = gst_video_format_get_size (format, width, height);

	GST_LOG_OBJECT (video_box, "Returning from _unit_size %d", *size);

	return TRUE;
}

//static void
//gst_video_box2_fixate_caps (GstBaseTransform * trans,
//    GstPadDirection direction, GstCaps * caps, GstCaps * othercaps)
//{
//  gint width, height;
//  GstStructure *s;
//  gboolean ret;
//
//  ret = gst_video_format_parse_caps (caps, NULL, &width, &height);
//  if (!ret)
//    return;
//
//  s = gst_caps_get_structure (othercaps, 0);
//  gst_structure_fixate_field_nearest_int (s, "width", width);
//  gst_structure_fixate_field_nearest_int (s, "height", height);
//}

static void gst_video_box2_process(GstVideoBox2 *video_box, const guint8 *src, guint8 *dest) {
  gint br, bl, bt, bb, crop_w, crop_h;

  crop_h = 0;
  crop_w = 0;

  br = video_box->box_right;
  bl = video_box->box_left;
  bt = video_box->box_top;
  bb = video_box->box_bottom;

  if (br >= 0 && bl >= 0) {
    crop_w = video_box->in_width - (br + bl);
  } else if (br >= 0 && bl < 0) {
    crop_w = video_box->in_width - (br);
  } else if (br < 0 && bl >= 0) {
    crop_w = video_box->in_width - (bl);
  } else if (br < 0 && bl < 0) {
    crop_w = video_box->in_width;
  }

  if (bb >= 0 && bt >= 0) {
    crop_h = video_box->in_height - (bb + bt);
  } else if (bb >= 0 && bt < 0) {
    crop_h = video_box->in_height - (bb);
  } else if (bb < 0 && bt >= 0) {
    crop_h = video_box->in_height - (bt);
  } else if (bb < 0 && bt < 0) {
    crop_h = video_box->in_height;
  }

  if (crop_h < 0 || crop_w < 0) {
//	  qDebug() << "fill1";
    fill_planar_yuv(video_box->out_format, dest,
	video_box->out_sdtv, video_box->out_width, video_box->out_height);
  } else if (bb == 0 && bt == 0 && br == 0 && bl == 0) {
//	  qDebug() << "copy1";
    copy_i420_i420(video_box->out_format, dest, video_box->out_sdtv,
	video_box->out_width, video_box->out_height, 0, 0, video_box->in_format,
	src, video_box->in_sdtv, video_box->in_width, video_box->in_height, 0,
	0, crop_w, crop_h);
  } else {
    gint src_x = 0, src_y = 0;
    gint dest_x = 0, dest_y = 0;

    /* Fill everything if a border should be added somewhere */
    if (bt < 0 || bb < 0 || br < 0 || bl < 0) {
//	    qDebug() << "fill2";
      fill_planar_yuv(video_box->out_format, dest,
	  video_box->out_sdtv, video_box->out_width, video_box->out_height);
}

    /* Top border */
    if (bt < 0) {
      dest_y += -bt;
    } else {
      src_y += bt;
    }

    /* Left border */
    if (bl < 0) {
      dest_x += -bl;
    } else {
      src_x += bl;
    }

    /* Frame */
//    qDebug() << "copy2";
    copy_i420_i420(video_box->out_format, dest, video_box->out_sdtv,
	video_box->out_width, video_box->out_height, dest_x, dest_y,
	video_box->in_format, src, video_box->in_sdtv, video_box->in_width,
	video_box->in_height, src_x, src_y, crop_w, crop_h);
  }

  GST_LOG_OBJECT (video_box, "image created");
}

static void gst_video_box2_before_transform(GstBaseTransform *trans, GstBuffer *in) {
	GstVideoBox2 *video_box = GST_VIDEO_BOX2(trans);
	GstClockTime timestamp = GST_BUFFER_TIMESTAMP(in);
	GstClockTime stream_time = gst_segment_to_stream_time(&trans->segment, GST_FORMAT_TIME, timestamp);
	if (GST_CLOCK_TIME_IS_VALID(stream_time))
		gst_object_sync_values(G_OBJECT(video_box), stream_time);
}

static GstFlowReturn gst_video_box2_transform(GstBaseTransform *trans, GstBuffer *in, GstBuffer *out) {
	GstVideoBox2 *video_box = GST_VIDEO_BOX2 (trans);
	const uchar *indata = GST_BUFFER_DATA(in);
	uchar *outdata = GST_BUFFER_DATA (out);

	g_mutex_lock (video_box->mutex);

//	copy_i420_i420

	gst_video_box2_process (video_box, indata, outdata);
	g_mutex_unlock (video_box->mutex);
	return GST_FLOW_OK;
}
