#ifndef EQUALIZERDIALOG_H
#define EQUALIZERDIALOG_H

#include <QDialog>

namespace MPlayer {class VideoOutput;}

class EqualizerDialog : public QDialog {
	Q_OBJECT
public:
	EqualizerDialog(MPlayer::VideoOutput *video, QWidget *parent = 0);
	~EqualizerDialog();
protected:
	void showEvent(QShowEvent *event);
private slots:
	void setBrightness(int value);
	void setContrast(int value);
	void setGamma(int value);
	void setHue(int value);
	void setSaturation(int value);
	void resetValues();
	void setValuesDefault();
private:
	struct Data;
	Data *d;
};


#endif
