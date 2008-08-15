#ifndef XINEVIDEOOUTPUT_H
#define XINEVIDEOOUTPUT_H

#include <QObject>
#include <QSize>
#include <xine.h>

class QWidget;						class QRect;

namespace Xine {

class XineStream;					class VideoWidget;

class VideoOutput : public QObject {
	Q_OBJECT
public:
	enum AspectRatio {AR_Auto, AR_4_3, AR_16_9, AR_211_100};
	enum CropRatio {CR_Off, CR_4_3, CR_16_9, CR_211_100};
	VideoOutput(XineStream *stream);
	void setDriver(const QString &driver);
	const QString &driver() const {return m_driver;}
	QWidget *widget() {return m_widget;}
	const QSize &videoSize() const {return m_videoSize;}
	int brightness() const;
	int contrast() const;
	int hue() const;
	int saturation() const;
	QSize widgetSizeHint() const;
	void *visual();
	void crop(CropRatio cr);
	void setAspectRatio(AspectRatio ar);
	AspectRatio aspectRatio() const {return m_ar;}
	XineStream *stream() {return m_stream;}
	bool isExpanded() const {return m_expanded;}
	xine_video_port_t *&port() {return m_port;}
	QRect osdRect(bool forScaled) const;
	void updateSizeInfo();
	void updateSizeInfo(const QSize &size);
public slots:
	void expand(bool expanded);
	void setBrightness(int value);
	void setContrast(int value);
	void setHue(int value);
	void setSaturation(int value);
	void setFullScreen(bool fs);
signals:
	void widgetSizeHintChanged(const QSize &size);
	void widgetResized(const QSize &size);
	void sizeUpdated();
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
private:
	static double ratio(const QSizeF &size) {return ratio(size.width(), size.height());}
	static double ratio(const QSize &size) {return ratio(size.width(), size.height());}
	static double ratio(double w, double h) {return w/h;}
	static bool isSame(double val1, double val2) {return qAbs(val1-val2) < 0.01;}
	void updateVisual();
	QSize visualSize(bool scaled) const;
	int visualBottom() const;
	friend class VideoWidget;
	XineStream *m_stream;
	xine_video_port_t *m_port;
	QString m_driver;
	QSize m_videoSize;
	QWidget *m_widget, *m_visual;
	VideoWidget *m_video;
	AspectRatio m_ar;
	CropRatio m_cr;
	bool m_expanded, m_fullScreen;
};

}

#endif

