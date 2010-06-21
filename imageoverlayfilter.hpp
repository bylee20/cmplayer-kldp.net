#ifndef IMAGEOVERLAYFILTER_HPP
#define IMAGEOVERLAYFILTER_HPP

#include <gst/gst.h>
#include "videofilter.hpp"

class QImage;			class QPoint;
class I420Picture;

class ImageOverlayFilter : public VideoFilter {
	class Item;
public:
	ImageOverlayFilter();
	~ImageOverlayFilter();
	bool transform(I420Picture *pic);
	bool isInPlace() const {return true;}
	int newOverlay();
	void setOverlay(int id, const QImage &image, const QPoint &pos);
	void setZIndex(int id, double zIndex);
private:
	static void blend(I420Picture *pic, const Item &item);
	Item *getItem(int id);
	struct Data;
	Data *d;
};

#endif // IMAGEOVERLAYFILTER_HPP
