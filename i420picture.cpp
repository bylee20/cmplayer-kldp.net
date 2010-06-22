#include "i420picture.hpp"
#include <gst/video/video.h>

void I420Picture::init(uchar *data, int width, int height) {
	const GstVideoFormat format = GST_VIDEO_FORMAT_I420;
	y = data + gst_video_format_get_component_offset(format, 0, width, height);
	u = data + gst_video_format_get_component_offset(format, 1, width, height);
	v = data + gst_video_format_get_component_offset(format, 2, width, height);
	width_y = gst_video_format_get_row_stride(format, 0, width);
	width_uv = gst_video_format_get_row_stride(format, 1, width);
	height_y = gst_video_format_get_component_height(format, 0, height);
	height_uv = gst_video_format_get_component_height(format, 1, height);
}

bool I420Picture::init(GstBuffer *buffer) {
	GstVideoFormat format;
	int width, height;
	if (!gst_video_format_parse_caps(buffer->caps, &format, &width, &height))
		return false;
	if (format != GST_VIDEO_FORMAT_I420)
		return false;
	init(buffer->data, width, height);
	return true;
}

void I420Picture::alloc(int width, int height) {
	const int size = gst_video_format_get_size(GST_VIDEO_FORMAT_I420, width, height);
	buffer = new uchar[size];
	init(buffer, width, height);
}

void I420Picture::free() {
	delete[] buffer;
	buffer = 0;
}
