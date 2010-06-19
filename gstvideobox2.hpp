#ifndef GSTVIDEOBOX2_HPP
#define GSTVIDEOBOX2_HPP

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

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/video/video.h>

struct GstVideoBox2 {
	GstBaseTransform parent;
public:
	/* <private> */

	/* Guarding everything below */
	GMutex *mutex;
	/* caps */
	GstVideoFormat in_format, out_format;
	gint in_width, in_height;
	gboolean in_sdtv;
	gint out_width, out_height;
	gboolean out_sdtv;

	gint box_left, box_right, box_top, box_bottom;

	gint border_left, border_right, border_top, border_bottom;
	gint crop_left, crop_right, crop_top, crop_bottom;
};

struct GstVideoBox2Class {
	GstBaseTransformClass parent;
};

GType gst_video_box2_get_type (void);

#define GST_TYPE_VIDEO_BOX2 \
  (gst_video_box2_get_type())
#define GST_VIDEO_BOX2(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_BOX2,GstVideoBox2))
#define GST_VIDEO_BOX2_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_BOX2,GstVideoBox2Class))
#define GST_IS_VIDEO_BOX2(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_BOX2))
#define GST_IS_VIDEO_BOX2_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_BOX2))


#endif // GSTVIDEOBOX2_HPP
