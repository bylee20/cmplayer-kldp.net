#include "audiocontroller.hpp"
#include "avmisc.hpp"
#include <math.h>
#include <QtCore/QDebug>

class AudioController::Volume {
public:
	Volume() {
		m_size = 20;
		m_maxLevel = 2.0f;
		m_sum = m_gain = m_last = 0;
		m_channels = 0;
		m_volume = 100;
		m_amp = 1.0;
		m_muted = false;
		m_normalized = false;
	}
	~Volume() {delete [] m_last;}
	void init(const AudioFormat &format) {
		if (m_last)
			delete [] m_last;
		/* We need to store (nb_buffers+1)*nb_channels floats */
		m_channels = format.channels;
		m_last = new float[m_channels*(m_size + 2)];
		m_sum = new float[m_channels];
		m_gain = new float[m_channels];
	}
	void setMaxLevel(float lv) {m_maxLevel = qMax(0.01f, lv);}
	AudioBuffer *process(AudioBuffer *in) {
		float *buffer = (float*)in->data;
		const int samples = in->samples;
		const float coef = coefficient();
		if (m_muted || !m_normalized) {
			for(int i=0; i<samples; ++i) {
				for(int ch=0; ch<m_channels; ++ch)
					buffer[ch] *= coef;
				buffer += m_channels;
			}
		} else {
			// copied from vlc volnorm
			float avg = 0;
			/* Calculate the average power level on this buffer */
			for(int i=0; i<samples; ++i) {
				for(int ch=0; ch<m_channels; ++ch) {
					float f_sample = buffer[ch];
					float f_square = pow(f_sample, 2 );
					m_sum[ch] += f_square;
				}
				buffer += m_channels;
			}
			buffer = (float*)in->data;

			/* sum now contains for each channel the sigma(value²) */
			for(int ch=0; ch<m_channels; ++ch) {
				/* Shift our lastbuff */
				memmove(&m_last[ch*m_size]
					, &m_last[ch*m_size + 1], (m_size-1)*sizeof(float));
				/* Insert the new average : sqrt(sigma(value²)) */
				m_last[ch*m_size + m_size - 1] = sqrt(m_sum[ch]);
				m_sum[ch] = 0;
				/* Get the average power on the lastbuff */
				avg = 0;
				for(int i=0; i<m_size ; ++i)
					avg += m_last[ch*m_size + i];
				avg = avg/m_size;
				//fprintf(stderr,"Average %f, max %f\n", f_average, d->f_max );
				m_gain[ch] = (avg>m_maxLevel) ? avg/m_maxLevel : 1.0f;
			}

			/* Apply gain */
			for(int i=0; i<samples; ++i) {
				for(int ch=0; ch<m_channels; ++ch) {
					buffer[ch] /= m_gain[ch];
					buffer[ch] *= coef;
				}
				buffer += m_channels;
			}
		}
		return 0;
	}
	void setMuted(bool muted) {m_muted = muted;}
	void setVolume(int volume) {m_volume = volume;}
	void setAmp(double amp) {m_amp = amp;}
	int volume() const {return m_volume;}
	bool isMuted() const {return m_muted;}
	double amp() const {return m_amp;}
	void setNormalized(bool norm) {m_normalized = norm;}
	bool isNormalized() const {return m_normalized;}
private:
	float coefficient() const {return m_muted ? .0 : (m_amp*m_volume)/100.;}
	int m_volume;
	double m_amp;
	bool m_muted;
	float *m_last, *m_sum, *m_gain;
	float m_maxLevel;
	int m_size, m_channels;
	bool m_normalized;
};

struct AudioController::Data {
	AudioUtil *util;
	AudioFormat format;
	Volume volume;
};

void AudioController::setUtil(AudioUtil *util) {
	d->util = util;
}

void AudioController::prepare(const AudioFormat *format) {
	d->format = *format;
	d->volume.init(d->format);
}

AudioBuffer *AudioController::process(AudioBuffer *in) {
	return d->volume.process(in);
}

AudioController::AudioController(/*PlayEngine *engine*/): d(new Data) {
}

AudioController::~AudioController() {
	delete d;
}

int AudioController::volume() const {
	return d->volume.volume();
}

bool AudioController::isMuted() const {
	return d->volume.isMuted();
}

void AudioController::setVolume(int volume) {
	volume = qBound(0, volume, 100);
	if (d->volume.volume() != volume) {
		d->volume.setVolume(volume);
		emit volumeChanged(volume);
	}
}

void AudioController::setMuted(bool muted) {
	if (d->volume.isMuted() != muted) {
		d->volume.setMuted(muted);
		emit mutedChanged(muted);
	}
}

void AudioController::setPreAmp(double amp) {
	amp = qFuzzyCompare(amp, 1.0) ? 1.0 : qBound(0.0, amp, 10.0);
	if (!qFuzzyCompare(d->volume.amp(), amp))
		d->volume.setAmp(amp);
}

double AudioController::preAmp() const {
	return d->volume.amp();
}

void AudioController::setVolumeNormalized(bool norm) {
	if (d->volume.isNormalized() != norm) {
		d->volume.setNormalized(norm);
		emit volumeNormalizedChanged(norm);
	}
}

bool AudioController::isVolumeNormalized() const {
	return d->volume.isNormalized();
}

void AudioController::setTempoScaled(bool scaled) {
	if (d->util->scaletempo_enabled != scaled)
		emit tempoScaledChanged((d->util->scaletempo_enabled = scaled));
}

bool AudioController::isTempoScaled() const {
	return d->util->scaletempo_enabled != 0;
}
