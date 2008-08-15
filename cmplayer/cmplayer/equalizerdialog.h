#ifndef EQUALIZERDIALOG_H
#define EQUALIZERDIALOG_H

#include <QDialog>

namespace Xine {class VideoOutput;}

class EqualizerDialog : public QDialog {
	Q_OBJECT
public:
	EqualizerDialog(Xine::VideoOutput *video, QWidget *parent = 0);
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
	struct Data;
	Data *d;
};


#endif
