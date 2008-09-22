#ifndef EQUALIZERDIALOG_H
#define EQUALIZERDIALOG_H

#include <QDialog>

namespace Backend {class VideoOutput;}

class EqualizerDialog : public QDialog {
	Q_OBJECT
public:
	EqualizerDialog(Backend::VideoOutput *video, QWidget *parent = 0);
	~EqualizerDialog();
protected:
	void showEvent(QShowEvent *event);
private slots:
	void setBrightness(int value);
	void setContrast(int value);
	void setHue(int value);
	void setSaturation(int value);
	void resetValues();
	void setValuesDefault();
private:
	static int to200(double val) {return qRound(val*200);}
	static double to1(int val) {return qBound(-1.0, val/200.0, 1.0);}
	struct Data;
	Data *d;
};


#endif
