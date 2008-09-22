#ifndef MPLAYERVOLUMESLIDER_H
#define MPLAYERVOLUMESLIDER_H

#include "private/cslider.h"

namespace Xine {

class AudioOutput;

class VolumeSlider : public CSlider {
	Q_OBJECT
public:
	VolumeSlider(AudioOutput *audio, QWidget *parent = 0);
};

}

#endif
