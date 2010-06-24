/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_FFMPEG_CODECMAP_H__
#define __GST_FFMPEG_CODECMAP_H__

#include "gst_fcs/gst_fcs_avcodec.h"
#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * _codectype_to_caps () gets the GstCaps that belongs to
 * a certain CodecType for a pad with uncompressed data.
 */

GstCaps *gst_ffmpeg_pixfmt_to_caps (enum PixelFormat pix_fmt);

/*
 * caps_with_codectype () transforms a GstCaps that belongs to
 * a pad for uncompressed data to a filled-in context.
 */

enum PixelFormat gst_ffmpeg_caps_to_pixfmt (const GstCaps * caps);

//void
//gst_ffmpegcsp_caps_with_codectype (const GstCaps  *caps,
//                                   AVCodecContext *context);

/*
 * Fill in pointers in an AVPicture, aligned by 4 (required by X).
 */

int
gst_ffmpegcsp_avpicture_fill (AVPicture * picture,
                              uint8_t *   ptr,
                              enum PixelFormat pix_fmt,
                              int         width,
                              int         height,
			      int         interlaced);

#ifdef __cplusplus
}
#endif

#endif /* __GST_FFMPEG_CODECMAP_H__ */

