#include "volumeslider.h"
#include "audiooutput.h"

namespace Backend {

VolumeSlider::VolumeSlider(QWidget *parent)
: JumpSlider(parent), audio(0) {
	setMaximum(100);
	setMinimum(0);
	setMaximumWidth(70);
	setValue(0);
}

void VolumeSlider::setAudioOutput(AudioOutput *audio) {
	if (this->audio != audio) {
		this->audio = audio;
		setValue(audio->volume());
		connect(this, SIGNAL(valueChanged(int)), audio, SLOT(setVolume(int)));
		connect(audio, SIGNAL(volumeChanged(int)), this, SLOT(setValue(int)));
	}
}

}
