#ifndef BACKEND_VOLUMESLIDER_H
#define BACKEND_VOLUMESLIDER_H

#include "private/jumpslider.h"

namespace Backend {

class AudioOutput;

class VolumeSlider : public JumpSlider {
	Q_OBJECT
public:
	VolumeSlider(AudioOutput *audio, QWidget *parent = 0);
};

}

#endif
