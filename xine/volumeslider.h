#ifndef MPLAYERVOLUMESLIDER_H
#define MPLAYERVOLUMESLIDER_H

#include "cslider.h"

namespace Xine {

class XineEngine;

class VolumeSlider : public CSlider {
	Q_OBJECT
public:
	VolumeSlider(XineEngine *audio, QWidget *parent = 0);
};

}

#endif
