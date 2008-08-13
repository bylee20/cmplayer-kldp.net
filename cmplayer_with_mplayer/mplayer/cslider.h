#ifndef CSLIDER_H
#define CSLIDER_H

#include <QSlider>

class MPlayerEngine;

class CSlider : public QSlider {
	Q_OBJECT
public:
	CSlider(QWidget *parent = 0);
protected:
	virtual void mousePressEvent(QMouseEvent *event);
};

#endif
