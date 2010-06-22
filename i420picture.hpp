#ifndef I420PICTURE_HPP
#define I420PICTURE_HPP

#include <gst/gst.h>
#include <QtCore/QtGlobal>

struct I420Picture {
public:
	I420Picture() {
		buffer = y = u = v = 0;
		width_uv = width_y = height_uv = height_y = 0;
	}
	~I420Picture() {
		free();
	}
	void alloc(int width, int height);
	void free();
	bool init(GstBuffer *buffer);
	void init(uchar *data, int width, int height);
	int size_y() const {return width_y*height_y;}
	int size_uv() const {return width_uv*height_uv;}
	bool isEmpty() const {
		return width_y <= 0 || height_y <= 0 || width_uv <= 0 || height_uv <= 0;
	}
	uchar *y, *u, *v;
	int width_y, width_uv;
	int height_y, height_uv;
private:
	uchar *buffer;
};

#endif // I420PICTURE_HPP
