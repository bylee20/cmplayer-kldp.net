#include "gstvolnorm.hpp"
#include <QtCore/QDebug>
#include <cmath>
#include <limits>
//#include <gst/controller/gstcontroller.h>

// smooth must be in ]0.0, 1.0[
#define SMOOTH_MUL 0.06
#define SMOOTH_LASTAVG 0.06
//#define DEFAULT_TARGET 0.25
#define MIN_SAMPLE_SIZE 32000

template<class T>
inline static T clamp(T min, T value, T max) {return (value < min) ? min : ((value > max) ? max : value);}

static GType gst_vol_norm_get_type();

GST_BOILERPLATE(GstVolNorm, gst_vol_norm, GstAudioFilter, GST_TYPE_AUDIO_FILTER);

GType GstVolNorm::gtype() {return gst_vol_norm_get_type();}

static void gst_vol_norm_finalize(GObject *obj) {
	delete GST_VOL_NORM(obj)->d;
	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

#define ALLOWED_CAPS \
	"audio/x-raw-float, " \
	"rate = (int) [ 1, MAX ], " \
	"channels = (int) [ 1, MAX ], " \
	"endianness = (int) BYTE_ORDER, " \
	"width = (int) 32; " \
	"audio/x-raw-int, " \
	"channels = (int) [ 1, MAX ], " \
	"rate = (int) [ 1,  MAX ], " \
	"endianness = (int) BYTE_ORDER, " \
	"width = (int) 16, " \
	"depth = (int) 16, " \
	"signed = (bool) TRUE; "

void gst_vol_norm_base_init(gpointer klass) {
	GstAudioFilterClass *filter = GST_AUDIO_FILTER_CLASS(klass);
	GstCaps *caps = gst_caps_from_string(ALLOWED_CAPS);
	gst_audio_filter_class_add_pad_templates(filter, caps);
	gst_caps_unref(caps);
}

static GstFlowReturn gst_vol_norm_transform_ip(GstBaseTransform *base, GstBuffer *outbuf) {
	return GST_VOL_NORM(base)->transform_ip(outbuf);
}

static gboolean gst_vol_norm_setup(GstAudioFilter *filter, GstRingBufferSpec *format) {
	return GST_VOL_NORM(filter)->setup(format);
}

static void gst_vol_norm_class_init(GstVolNormClass *klass) {
	GObjectClass *gobj = G_OBJECT_CLASS(klass);
	gobj->finalize = gst_vol_norm_finalize;

	GstBaseTransformClass *trans = GST_BASE_TRANSFORM_CLASS(klass);
	trans->transform_ip = gst_vol_norm_transform_ip;

	GstAudioFilterClass *filter = GST_AUDIO_FILTER_CLASS(klass);
	filter->setup = gst_vol_norm_setup;
}

// mul is the value by which the samples are scaled
// and has to be in [MUL_MIN, MUL_MAX]
#define MUL_INIT 1.0
#define MUL_MIN 0.1
#define MUL_MAX 5.0

template<typename T> struct NormalizeLevel {};

template<> struct NormalizeLevel<int16_t> {
	static const float Silence = (SHRT_MAX * 0.01);
	static const float Ideal = (SHRT_MAX * 0.25);
	static const int16_t Min = SHRT_MIN;
	static const int16_t Max = SHRT_MAX;
	inline static int16_t clamp(int16_t value) {return ::clamp<int16_t>(Min, value, Max);}
};

template<> struct NormalizeLevel<float> {
	static const float Silence = (INT_MAX * 0.01);
	static const float Ideal = (INT_MAX * 0.25);
	inline static float clamp(float value) {return value;}
//	static const float Min = std::numeric_limits<float>::min();
//	static const float Max = std::numeric_limits<float>::max();
};


static void gst_vol_norm_init(GstVolNorm *self, GstVolNormClass *klass) {
	Q_UNUSED(klass);
	self->d = new GstVolNorm::Data;
	self->d->mul = MUL_INIT;
	self->d->lastAvg = NormalizeLevel<int16_t>::Ideal;
	self->d->on = true;
	self->d->idx = 0;
}

static bool isInt16(const GstRingBufferSpec &format) {
	return format.type == GST_BUFTYPE_LINEAR && format.width == 16;
}

static bool isFloat32(const GstRingBufferSpec &format) {
	return format.type == GST_BUFTYPE_FLOAT && format.width == 32;
}

bool GstVolNorm::setup(GstRingBufferSpec *format) {
	return format->caps && (isInt16(*format) || isFloat32(*format));
}


template<typename T, typename Temp>
static void normalize1(GstBuffer *const buffer, GstVolNorm *const self) {
	GstVolNorm::Data *const d = self->d;
	T *data = reinterpret_cast<T*>(GST_BUFFER_DATA(buffer));
	const int size = GST_BUFFER_SIZE(buffer)/sizeof(T);
	Temp temp = 0;
	float curAvg = 0.0;
	for (int i=0; i<size; ++i) {
		temp = data[i];
		curAvg += temp*temp;
	}
	curAvg = std::sqrt(curAvg/(float)size);
	if (curAvg > NormalizeLevel<T>::Silence) {
		const float needed = NormalizeLevel<T>::Ideal / (curAvg*d->mul);
		d->mul = (1.0 - SMOOTH_MUL) * d->mul + SMOOTH_MUL * needed;
		d->mul = clamp<float>(MUL_MIN, d->mul, MUL_MAX);
	}
	for (int i=0; i<size; ++i) {
		temp = d->mul*data[i];
		data[i] = NormalizeLevel<T>::clamp(temp);
	}
	const float newAvg = d->mul*curAvg;
	d->lastAvg = (1.0 - SMOOTH_LASTAVG) * d->lastAvg + SMOOTH_LASTAVG * newAvg;
}

template<typename T, typename Temp>
static void normalize2(GstBuffer *const buffer, GstVolNorm *const self) {
	GstVolNorm::Data *const d = self->d;
	T *data = reinterpret_cast<T*>(GST_BUFFER_DATA(buffer));
	const int size = GST_BUFFER_SIZE(buffer)/sizeof(T);
	float curAvg = 0.0;
	Temp temp = 0;
	int totalLen = 0;
	for (int i=0; i<size; ++i) {
		temp = data[i];
		curAvg += temp*temp;
	}
	curAvg = std::sqrt(curAvg/(float)size);
	float avg = 0.0;
	for (int i=0; i<GstVolNorm::NSAMPLES; ++i) {
		avg += d->mem[i].avg*(float)d->mem[i].len;
		totalLen += d->mem[i].len;
	}
	if (totalLen > MIN_SAMPLE_SIZE) {
		avg /= (float)totalLen;
		if (avg >= NormalizeLevel<T>::Silence) {
			d->mul = NormalizeLevel<T>::Ideal/avg;
			d->mul = clamp<float>(MUL_MIN, d->mul, MUL_MAX);
		}
	}
	for (int i=0; i<size; ++i) {
		temp = d->mul*data[i];
		data[i] = NormalizeLevel<T>::clamp(temp);
	}
	const float newAvg = d->mul * curAvg;
	d->mem[d->idx].len = size;
	d->mem[d->idx].avg = newAvg;
	d->idx = (d->idx + 1) % GstVolNorm::NSAMPLES;
}

GstFlowReturn GstVolNorm::transform_ip(GstBuffer *outbuf) {
	if (!d->on)
		return GST_FLOW_OK;
	const GstRingBufferSpec &format = GST_AUDIO_FILTER(this)->format;
	if (isInt16(format))
		normalize1<int16_t, int>(outbuf, this);
	else if (isFloat32(format))
		normalize1<float, float>(outbuf, this);
	else
		return GST_FLOW_ERROR;
	return GST_FLOW_OK;
}
