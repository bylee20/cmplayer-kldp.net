#ifndef VIDEOMANIPULATOR_HPP
#define VIDEOMANIPULATOR_HPP

#include <gst/gst.h>
#include <QtCore/QObject>

class NativeVideoRenderer;		class QRect;
class ImageOverlayFilter;

struct VideoInfo {
	VideoInfo() {width = height = 0; fps = 0.0;}
	int width, height;
	double fps;
};

class VideoManipulator : public QObject {
	Q_OBJECT
public:
	VideoManipulator();
	~VideoManipulator();
	GstElement *element() const;
	ImageOverlayFilter *overlay() const;
	void setRenderer(NativeVideoRenderer *renderer);
	QRect videoRect() const;
	void setBorder(int h, int v);
	void crop(int h, int v);
	void reconfigure();
	void rerender();
signals:
	void videoInfoObtained(const VideoInfo &info);
private:
	void transform(GstBuffer *in, GstBuffer *out);
	friend class GstVideoMan;
	friend class GstVideoManClass;
	void setVideoInfo(const VideoInfo &in, const VideoInfo &out);
	void manipulate();
	struct Data;
	Data *d;
};

#endif // VIDEOMANIPULATOR_HPP
