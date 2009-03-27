#ifndef CORE_VIDEORENDERERIFACE_H
#define CORE_VIDEORENDERERIFACE_H

#include "namespace.h"
#include "videoframe.h"
#include <QtCore/QObject>
#include <QtCore/QSize>

class QWidget;					class QRectF;
class QImage;

namespace Core {

class AbstractOsdRenderer;			class VideoRendererIface;

class VideoRendererIface {
public:
	VideoRendererIface(QWidget *target);
	virtual ~VideoRendererIface();
	void setAspectRatio(double ratio);
	void setCropRatio(double ratio);
	double aspectRatio() const {return m_aspect;}
	double cropRatio() const {return m_crop;}
	virtual void rerender();
 	const QSize &videoSize() const {return m_videoSize;}
	QSize widgetSizeHint() const;
	QWidget *target() {return m_target;}
	virtual VideoType type() const = 0;
	double videoRatio() const {return m_videoRatio;}
	double aspectRatioF() const {return m_aspectF;}
	virtual AbstractOsdRenderer *createOsd() {return 0;}
	virtual QImage grabCurrentFrame() const;
protected:
	static QImage imageFromYv12(const QByteArray &yv12, int width, int height);
	static QImage imageFromYuy2(const QByteArray &yuy2, int width, int height);
	static QByteArray yuy2ToYv12(const QByteArray &yuy2, int width, int height);
	static QByteArray yv12ToRgb(const QByteArray &yv12, int width, int height);
	static double ratio(double w, double h) {return w/h;}
	static double ratio(const QSize &size) {return ratio(size.width(), size.height());}
	static double ratio(const QSizeF &size) {return size.width()/size.height();}
	void updateVideoSize(const QSize &size);
	void updateFrameInfo(const VideoFrame::Info &info, bool setVideo = false);
	const VideoFrame::Info &frameInfo() const {return m_frame;}
	virtual void calculate() {}
private:
	QSize m_videoSize;
	double m_videoRatio, m_aspect, m_aspectF, m_crop;
	VideoFrame::Info m_frame;
	QWidget *m_target;
};

}

#endif
