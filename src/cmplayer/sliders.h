#ifndef SLIDERS_H
#define SLIDERS_H

#include <QtGui/QSlider>

class VideoPlayer;

class JumpSlider : public QSlider {
Q_OBJECT
public:
	JumpSlider(QWidget *parent = 0);
protected:
	virtual void mousePressEvent(QMouseEvent *event);
};

class SeekSlider : public JumpSlider {
Q_OBJECT
	public:
	SeekSlider(VideoPlayer *player, QWidget *parent = 0);
private slots:
	void setDuration(int duration) {setRange(0, duration);}
	void slotTick(int time) {tick = true; setValue(time); tick = false;}
	void seek(int msec);
private:
	VideoPlayer *player;
	bool tick;
};

class VolumeSlider : public JumpSlider {
Q_OBJECT
public:
	VolumeSlider(VideoPlayer *player, QWidget *parent = 0);
};

#endif
