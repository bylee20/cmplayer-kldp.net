#include "volumeslider.h"
#include "audiooutput.h"

namespace Backend {

VolumeSlider::VolumeSlider(AudioOutput *audio, QWidget *parent)
: JumpSlider(parent) {
	setMaximum(100);
	setMinimum(0);
	setMaximumWidth(70);
	setValue(audio->volume());
	connect(this, SIGNAL(valueChanged(int)), audio, SLOT(setVolume(int)));
	connect(audio, SIGNAL(volumeChanged(int)), this, SLOT(setValue(int)));
}

}
