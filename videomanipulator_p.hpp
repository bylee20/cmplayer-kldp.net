#ifndef VIDEOMANIPULATOR_P_HPP
#define VIDEOMANIPULATOR_P_HPP

#include <QtCore/QtGlobal>
#include <QtCore/QMutex>
#include <QtCore/QReadWriteLock>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/base/gstbasetransform.h>
#include "videofilter.hpp"

class VideoManipulator;

class CropMixFilter : public VideoFilter {
public:
	CropMixFilter();
	bool transform(I420Picture *out, const I420Picture &in);
	void crop(int crop_h, int crop_v);
	bool isInPlace() const {return false;}
	int horizontalCropSize() const {return m_crop_h;}
	int verticalCropSize() const {return m_crop_v;}
private:
	int m_crop_h, m_crop_v;
};

struct GstVideoMan {
	GstBaseTransform parent;
	void renderIn(GstBuffer *buffer);
	void ctor();
	void dtor();
	friend class GstVideoManClass;
public:
	struct Data {
		VideoManipulator *man;
		QMutex mutex;
		int in_width, in_height, out_width, out_height;
		double in_fps, out_fps;
		double in_par, out_par; // pixel-aspect-ratio
		int border_h, border_v;
	};
	Data *d;
//	void reconfigure();
//	bool renegotiate();
//	void rerender();
//	void setBorder(int h, int v);
//	void crop(int h, int v);
};

struct GstVideoManClass {
	GstBaseTransformClass parent;
public:
	static GstBaseTransformClass *parentClass;
	static void classInitTrampoline(gpointer klass, gpointer data);
	static GType getType();
	static void baseInit(gpointer klass);
	static void classInit(GstVideoManClass *klass);
	static void init(GstVideoMan *video, GstVideoManClass *klass);
	static void finalize(GObject *obj);
	static GstCaps *makeAvailableCaps();

	static gboolean setCaps(GstBaseTransform *trans, GstCaps *incaps, GstCaps *outcaps);
	static gboolean getUnitSize(GstBaseTransform *trans, GstCaps *caps, guint *size);
	static GstFlowReturn transform(GstBaseTransform *btrans, GstBuffer *inbuf, GstBuffer *outbuf);
	static GstCaps *transformCaps(GstBaseTransform *trans, GstPadDirection dir, GstCaps *caps);
};

#define GST_TYPE_VIDEO_MAN 	      (GstVideoManClass::getType())
#define GST_VIDEO_MAN(obj) 	      (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_MAN,GstVideoMan))
#define GST_VIDEO_MAN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_MAN,GstVideoManClass))
#define GST_IS_VIDEO_MAN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_MAN))
#define GST_IS_VIDEO_MAN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_MAN))


#endif // VIDEOMANIPULATOR_P_HPP
