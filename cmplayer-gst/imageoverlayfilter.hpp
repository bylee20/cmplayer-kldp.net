#ifndef IMAGEOVERLAYFILTER_HPP
#define IMAGEOVERLAYFILTER_HPP

#include <gst/gst.h>
#include <QtCore/QSize>
#include "videofilter.hpp"

class QImage;			class I420Picture;
class QPoint;

class ImageOverlayFilter : public VideoFilter {
	class Item;
public:
	ImageOverlayFilter();
	~ImageOverlayFilter();
	bool transform(I420Picture *pic);
	bool isInPlace() const {return true;}
	int newOverlay();
	void removeOverlay(int id);
	void setOverlay(int id, const QImage &image, const QPoint &pos);
	void setZIndex(int id, double zIndex);
	void setOverlay(int id, uchar *data, const QSize &size, const QPoint &pos);
private:
	static void blend(I420Picture *pic, const Item &item);
	Item *getItem(int id);
	struct Data;
	Data *d;
};

#endif // IMAGEOVERLAYFILTER_HPP
