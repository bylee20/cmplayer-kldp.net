#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <QtGui/QSlider>
#include <QtGui/QToolButton>

class PlayEngine;

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
	SeekSlider(PlayEngine *engine, QWidget *parent = 0);
private slots:
	void setDuration(int duration);
	void slotTick(int time);
	void seek(int msec);
private:
	PlayEngine *engine;
	bool tick;
};

class VolumeSlider : public JumpSlider {
	Q_OBJECT
public:
	VolumeSlider(PlayEngine *engine, QWidget *parent = 0);
private slots:
//	void
};

class Button : public QToolButton {
	Q_OBJECT
public:
	Button(QWidget *parent);
	Button(const QIcon &icon, QWidget *parent);
	Button(const QString &text, QWidget *parent);
	~Button();
	void setIconSize(int extent);
	void setAction(QAction *action, bool update);
	void setBlock(bool block);
private slots:
	void toggleAction(bool checked);
private:
	void init();
	struct Data;
	Data *d;
};

#endif // CONTROLS_HPP
