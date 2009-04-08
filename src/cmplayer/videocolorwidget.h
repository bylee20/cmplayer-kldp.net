#ifndef VIDEOCOLORWIDGET_H
#define VIDEOCOLORWIDGET_H

#include <QtGui/QWidget>

class VideoPlayer;			class QHBoxLayout;

class EqSlider : public QWidget {
	Q_OBJECT
public:
	EqSlider(int id, const QString &name, QWidget *parent = 0);
	~EqSlider();
	void setRange(int min, int max);
	void setValue(int value);
	int id() const;
	int value() const;
signals:
	void valueChanged(int value);
private slots:
	void slotValueChanged(int value);
private:
	struct Data;
	Data *d;
};

class VideoColorWidget : public QWidget {
Q_OBJECT
public:
	VideoColorWidget(VideoPlayer *player, QWidget *parent = 0);
	~VideoColorWidget();
private slots:
	void resetValue();
	void useSoftEq(bool use);
	void updateValues();
	void applyValue(int value);
private:
	void addSet(int prop, const QString &name, QHBoxLayout *hbox);
	struct Data;
	Data *d;
};

#endif
