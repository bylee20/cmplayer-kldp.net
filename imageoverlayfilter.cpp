#include "imageoverlayfilter.hpp"
#include "i420picture.hpp"
#include <QtCore/QReadWriteLock>
#include <QtCore/QDebug>
#include <QtCore/QMultiMap>
#include <QtCore/QWaitCondition>
#include <QtGui/QImage>
#include <QtCore/QTime>

struct ImageOverlayFilter::Item {
	Item(int id = 0, double zIndex = 0.0) {
		this->zIndex = zIndex;
		this->id = id;
		x = y = width = height = 0;
		data = 0;
	}
	~Item() {delete[] data;}
	uchar *data;
	int bufLen;
	int width, height;
	double zIndex; int id;
	int x, y;
	mutable QReadWriteLock lock;
	mutable QWaitCondition cond;
};

typedef QMultiMap<double, void*> ItemMap;

struct ImageOverlayFilter::Data {
	ItemMap item;
};

ImageOverlayFilter::ImageOverlayFilter()
: d(new Data) {

}

ImageOverlayFilter::~ImageOverlayFilter() {
	for (ItemMap::iterator it = d->item.begin(); it != d->item.end(); ++it)
		delete (Item*)(*it);
	delete d;
}

bool ImageOverlayFilter::transform(I420Picture *pic) {
	for (ItemMap::const_iterator it = d->item.begin(); it != d->item.end(); ++it) {
		const Item &item = *(const Item*)(*it);
		if (!item.lock.tryLockForRead())
			continue;
		blend(pic, item);
		item.lock.unlock();
//		item.lock.unlock();
//		item.cond.wakeAll();
	}
//	const int t = time.elapsed();
//	qDebug() << t;
//	qDebug() << d->item.size();
	return true;
}

ImageOverlayFilter::Item *ImageOverlayFilter::getItem(int id) {
	for (ItemMap::iterator it = d->item.begin(); it != d->item.end(); ++it) {
		Item *item = (Item*)(*it);
		if (item->id == id)
			return item;
	}
	return 0;
}

int ImageOverlayFilter::newOverlay() {
	static int key = 0;
	Item *item = new Item(++key, 0);
	d->item.insert(item->zIndex, item);
	return item->id;
}

void ImageOverlayFilter::removeOverlay(int id) {
	ItemMap::iterator it = d->item.begin();
	for (; it != d->item.end(); ++it) {
		Item *item = (Item*)(*it);
		if (item->id == id)
			break;
	}
	if (it != d->item.end()) {
		delete (Item*)(*it);
		d->item.erase(it);
	}
}

void ImageOverlayFilter::setOverlay(int id, uchar *data, const QSize &size, const QPoint &pos) {
	Item *item = getItem(id);
	if (item) {
		uchar *old = item->data;
		item->lock.lockForWrite();
		item->data = data;
		item->width = size.width();
		item->height = size.height();
		item->x = pos.x();
		item->y = pos.y();
		item->lock.unlock();
		delete[] old;
	} else
		delete data;
}

void ImageOverlayFilter::setZIndex(int id, double zIndex) {
	Item *item = getItem(id);
	if (item) {
		item->lock.lockForWrite();
		item->zIndex = zIndex;
		item->lock.unlock();
		rerender();
	}
}



#define Y_FROM_RGB(R, G, B) ((( 66*(int)(R)+129*(int)(G) +25*(int)(B))>>8)+16)
#define U_FROM_RGB(R, G, B) (((112*(int)(R) -94*(int)(G) -18*(int)(B))>>8)+128)
#define V_FROM_RGB(R, G, B) (((-38*(int)(R) -74*(int)(G)+112*(int)(B))>>8)+128)

#define Y_FROM_UCHAR_R(pR) (Y_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))
#define U_FROM_UCHAR_R(pR) (U_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))
#define V_FROM_UCHAR_R(pR) (V_FROM_RGB(*(pR), *((pR)+1), *((pR)+2)))

#define BLEND_ALPHA(D,S,A) (((int)(D)*(255 - (A)) + (int)(S) * (A))/255)

void ImageOverlayFilter::blend(I420Picture *pic, const Item &item) {
	const int image_w = item.width;
	const int image_h = item.height;

	if (image_w <= 0 || image_h <= 0)
		return; // nothing to blend

	const int frame_w = qMin(pic->width_y, (pic->width_uv<<1));
	const int frame_h = qMin(pic->height_y, (pic->height_uv<<1));
	const int image_w_h = (image_w >> 1);
	const int image_h_h = (image_h >> 1);
	const int left = item.x;
	const int top = item.y;
	const int y_offset = left + top*frame_w;
	const int uv_offset = (left >> 1) + (top >> 1)*(frame_w >> 1);
	int acc_u, acc_v, acc_a, alpha, i, j;
	uchar *y1 = pic->y + y_offset;
	uchar *y2 = y1 + 1;
	uchar *y3 = y1 + frame_w;
	uchar *y4 = y3 + 1;
	uchar *v = pic->v + uv_offset;
	uchar *u = pic->u + uv_offset;

	const uchar *r1 = item.data;
	const uchar *r2 = r1 + 4;
	const uchar *r3 = r1 + (image_w << 2);
	const uchar *r4 = r3 + 4;

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

