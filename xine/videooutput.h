//#ifndef MPLAYERVIDEOOUTPUT_H
//#define MPLAYERVIDEOOUTPUT_H
//
//#include "controller.h"
//
//class QSize;
//
//namespace MPlayer {
//
//class PlayEngine;
//
//class VideoOutput : public Controller {
//	Q_OBJECT
//public:
//	static const qreal CropOff = -1.0;
//	static const qreal AspectRatioAuto = -1.0;
//	static const qreal AspectRatioWidget = 0.0;
//	VideoOutput(QWidget *parent = 0);
//	~VideoOutput();
//	PlayEngine *playEngine() const;
//	const QSize &videoSize() const;
//	qreal aspectRatio() const;
//	qreal cropRatio() const;
//	bool cropOn() const;
//	int videoWID() const;
//	int brightness() const;
//	int contrast() const;
//	int gamma() const;
//	int hue() const;
//	int saturation() const;
//	const QString &driver() const;
//	void setDriver(const QString &driver);
//	bool isSoftwareEqualizerEnabled() const;
//	void setSoftwareEqualizerEnabled(bool enabled);
//	QWidget *widget() const;
//	QSize widgetSizeHint() const;
//public slots:
//	void setBrightness(int value);
//	void setContrast(int value);
//	void setGamma(int value);
//	void setHue(int value);
//	void setSaturation(int value);
//	void crop(qreal ratio);
//	void setAspectRatio(qreal ratio);
//signals:
//	void widgetSizeHintChanged(const QSize &size);
//	void widgetResized(const QSize &size);
//protected:
//	bool eventFilter(QObject *obj, QEvent *event);
//	void link(Controller *controller);
//	void unlink(Controller *controller);
//private slots:
//	void update();
//	void updateVisual();
//private:
//	void setVideoSize(const QSize &size);
//	void setExpanded(bool expanded);
//	qreal monitorRatio() const;
//	struct Data;
//	friend struct Data;
//	friend class PlayEngine;
//	Data *d;
//};
//
//}
//
//#endif
