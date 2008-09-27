#ifndef BACKEND_VIDEOOUTPUT_H
#define BACKEND_VIDEOOUTPUT_H

#include <QtCore/QSize>
#include "private/avoutput.h"

class QWidget;						class QRect;
class QRectF;

namespace Backend {

class PlayEngine;					class VideoWidget;

class VideoOutput : public AVOutput {
	Q_OBJECT
public:
	static const double AspectRatioAuto = -1.0;
	static const double CropOff = -1.0;
	VideoOutput(PlayEngine *engine);
	~VideoOutput();
	inline QWidget *widget() {return m_widget;}
	inline const QSize &videoSize() const {return m_videoSize;}
	inline double brightness() const {return m_brightness;}
	inline double contrast() const {return m_contrast;}
	inline double hue() const {return m_hue;}
	inline double saturation() const {return m_saturation;}
	QSize widgetSizeHint() const;
	void crop(double ratio);
	inline double cropRatio() const {return m_cropRatio;}
	void setAspectRatio(double ratio);
	inline double aspectRatio() const {return m_aspectRatio;}
	inline bool isExpanded() const {return m_expanded;}
	inline bool isFullScreenMode() const {return m_fullScreen;}
	QRect osdRect(bool forScaled) const;
public slots:
	bool expand(bool expanded);
	void setBrightness(double value);
	void setContrast(double value);
	void setHue(double value);
	void setSaturation(double value);
	void setFullScreenMode(bool on);
signals:
	void widgetSizeHintChanged(const QSize &size);
	void widgetResized(const QSize &size);
	void sizeUpdated();
protected:
	QRectF videoRect() const;
	inline double aspectRatioF() const {
		return m_aspectRatio > 0.0 ? m_aspectRatio : m_videoRatio;}
	virtual bool updateExpand(bool expand) = 0;
	virtual void updateHue(double value) = 0;
	virtual void updateContrast(double value) = 0;
	virtual void updateSaturation(double value) = 0;
	virtual void updateBrightness(double value) = 0;
	void updateVideoSize(const QSize &size);
	inline QWidget *internalWidget() const {return m_video;}
	inline QWidget *visualWidget() const {return m_visual;}
	inline double videoRatio() const {return m_videoRatio;}
	inline double videoOverDesktop() const {return m_videoOverDesktop;}
	inline static double ratio(const QSizeF &size) {return ratio(size.width(), size.height());}
	inline static double ratio(const QSize &size) {return ratio(size.width(), size.height());}
	inline static double ratio(double w, double h) {return w/h;}
	inline static bool isSame(double val1, double val2) {return qAbs(val1-val2) < 0.0001;}
protected slots:
	void update();
	void updateVideo();
private:
	friend class VideoWidget;
	QSize m_videoSize;
	QWidget *m_video, *m_visual, *m_widget;
	double m_hue, m_saturation, m_brightness, m_contrast, m_videoRatio;
	double m_videoOverDesktop, m_cropRatio, m_aspectRatio;
	bool m_expanded, m_fullScreen;
};

}

#endif

