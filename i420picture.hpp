#ifndef I420PICTURE_HPP
#define I420PICTURE_HPP

#include <gst/gst.h>
#include <QtCore/QtGlobal>

struct I420Picture {
public:
	bool init(GstBuffer *buffer);
	void init(uchar *data, int width, int height);
	int size_y() const {return width_y*height_y;}
	int size_uv() const {return width_uv*height_uv;}
	uchar *y, *u, *v;
	int width_y, width_uv;
	int height_y, height_uv;
};

#endif // I420PICTURE_HPP
