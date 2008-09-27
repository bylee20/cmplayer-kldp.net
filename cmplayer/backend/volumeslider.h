#ifndef BACKEND_VOLUMESLIDER_H
#define BACKEND_VOLUMESLIDER_H

#include "private/jumpslider.h"

namespace Backend {

class AudioOutput;

class VolumeSlider : public JumpSlider {
	Q_OBJECT
public:
	VolumeSlider(QWidget *parent = 0);
	void setAudioOutput(AudioOutput *audio);
private:
	AudioOutput *audio;
};

}

#endif
