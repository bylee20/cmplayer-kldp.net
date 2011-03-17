#include "audiocontroller.hpp"
#include <math.h>
#include <QtCore/QDebug>

struct AudioController::Data {
	int i_nb;
	float *p_last;
	float f_max;
	int channels;
	bool normalized;
	int volume;
	double amp;
	bool muted;
};

void AudioController::prepare(int channels) {
	d->channels = channels;
	if (d->p_last)
		delete [] d->p_last;
	/* We need to store (nb_buffers+1)*nb_channels floats */
	d->p_last = new float[channels*(d->i_nb + 2)];
}

void AudioController::apply(int samples, float *buffer) {
	const float rate = volumeRate();
	if (d->normalized) {
		float *pf_sum = new float[d->channels];
		float *pf_gain = new float[d->channels];
		float f_average = 0;

		float *const begin = buffer;
		/* Calculate the average power level on this buffer */

		for(int i=0; i<samples; ++i) {
			for(int chan = 0; chan < d->channels; ++chan ) {
				float f_sample = buffer[chan];
				float f_square = pow(f_sample, 2 );
				pf_sum[chan] += f_square;
			}
			buffer += d->channels;
		}
		buffer = begin;

		/* sum now contains for each channel the sigma(value²) */
		for(int chan=0; chan < d->channels; ++chan) {
			/* Shift our lastbuff */
			memmove(&d->p_last[chan*d->i_nb]
				, &d->p_last[chan*d->i_nb + 1], (d->i_nb-1)*sizeof(float));
			/* Insert the new average : sqrt(sigma(value²)) */
			d->p_last[chan * d->i_nb + d->i_nb - 1] = sqrt( pf_sum[chan] );
			pf_sum[chan] = 0;
			/* Get the average power on the lastbuff */
			f_average = 0;
			for(int i = 0; i < d->i_nb ; ++i) {
				f_average += d->p_last[chan * d->i_nb + i];
			}
			f_average = f_average / d->i_nb;
			//fprintf(stderr,"Average %f, max %f\n", f_average, p_sys->f_max );
			if( f_average > d->f_max ) {
				pf_gain[chan] = f_average / d->f_max;
			} else {
				pf_gain[chan] = 1;
			}
		}
		/* Apply gain */

		for(int i = 0; i < samples; ++i) {
			for(int chan = 0; chan < d->channels; ++chan ) {
				buffer[chan] /= pf_gain[chan];
				buffer[chan] *= rate;
			}
			buffer += d->channels;
		}
		delete [] pf_sum;
		delete [] pf_gain;
	} else {
		for(int i = 0; i < samples; ++i) {
			for(int chan = 0; chan < d->channels; ++chan ) {
				buffer[chan] *= rate;
			}
			buffer += d->channels;
		}
	}
}

AudioController::AudioController(/*PlayEngine *engine*/): d(new Data) {
	d->volume = 100;
//	d->engine = engine;
	d->normalized = false;

	d->i_nb = 20;
	d->f_max = 2.0f;
	if(d->f_max <= 0.f ) d->f_max = 0.01f;

	d->p_last = 0;

	d->amp = 1.0;
	d->muted = false;
}

AudioController::~AudioController() {
	delete[] d->p_last;
	d->p_last = 0;
	delete d;
}

float AudioController::volumeRate() const {
	if (d->muted)
		return 0.0f;
	return d->amp*(double)d->volume/100.0;
}

int AudioController::volume() const {
	return d->volume;
}

bool AudioController::isMuted() const {
	return d->muted;
}

void AudioController::setVolume(int volume) {
	volume = qBound(0, volume, 100);
	if (d->volume != volume) {
		d->volume = volume;
		emit volumeChanged(d->volume);
	}
}

void AudioController::setMuted(bool muted) {
	if (d->muted != muted) {
		d->muted = muted;
		emit mutedChanged(d->muted);
	}
}

void AudioController::setPreAmp(double amp) {
	if (qFuzzyCompare(amp, 1.0))
		amp = 1.0;
	else
		amp = qBound(0.0, amp, 10.0);
	if (!qFuzzyCompare(d->amp, amp)) {
		d->amp = amp;
	}
}

double AudioController::preAmp() const {
	return d->amp;
}

void AudioController::setVolumeNormalized(bool norm) {
	if (d->normalized != norm)
		emit volumeNormalizedChanged(d->normalized = norm);
}

bool AudioController::isVolumeNormalized() const {
	return d->normalized;
}
