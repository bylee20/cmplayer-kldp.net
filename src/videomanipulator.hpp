#ifndef VIDEOMANIPULATOR_HPP
#define VIDEOMANIPULATOR_HPP

#include <gst/gst.h>
#include <QtCore/QObject>

class NativeVideoRenderer;		class QRect;
class ImageOverlayFilter;		class VideoEqFilter;

struct VideoInfo {
	VideoInfo() {width = height = 0; fps = 0.0; pixelAspectRatio = 1.0;}
	int width, height;
	double fps, pixelAspectRatio;
};

class VideoManipulator : public QObject {
	Q_OBJECT
public:
	VideoManipulator();
	~VideoManipulator();
	GstElement *element() const;
	ImageOverlayFilter *overlay() const;
	VideoEqFilter *equalizer() const;
	void setRenderer(NativeVideoRenderer *renderer);
	QRect videoRect() const;
	void setBorder(int h, int v);
	void crop(int h, int v);
	void reconfigure();
	void rerender();
signals:
	void videoInfoObtained(const VideoInfo &info);
private:
	bool render(GstBuffer *buffer);
	GstFlowReturn transform(GstBuffer *in, GstBuffer *out);
	friend class GstVideoMan;
	friend class GstVideoManClass;
	void setVideoInfo(const VideoInfo &in, const VideoInfo &out);
	void manipulate();
	struct Data;
	Data *d;
};

#endif // VIDEOMANIPULATOR_HPP
