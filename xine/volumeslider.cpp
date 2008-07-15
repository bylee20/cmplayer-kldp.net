#include "volumeslider.h"
#include "xineaudio.h"

namespace Xine {

VolumeSlider::VolumeSlider(XineAudio *audio, QWidget *parent)
: CSlider(parent) {
	setMaximum(100);
	setMinimum(0);
	setMaximumWidth(70);
	setValue(audio->volume());
	connect(this, SIGNAL(valueChanged(int)), audio, SLOT(setVolume(int)));
	connect(audio, SIGNAL(volumeChanged(int)), this, SLOT(setValue(int)));
}

}
