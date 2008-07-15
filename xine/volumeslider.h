#ifndef MPLAYERVOLUMESLIDER_H
#define MPLAYERVOLUMESLIDER_H

#include "cslider.h"

namespace Xine {

class XineAudio;

class VolumeSlider : public CSlider {
	Q_OBJECT
public:
	VolumeSlider(XineAudio *audio, QWidget *parent = 0);
};

}

#endif
