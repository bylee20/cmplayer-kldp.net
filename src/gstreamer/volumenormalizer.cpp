#include "volumenormalizer.h"
#include <gst/controller/gstcontroller.h>
#include <cmath>

// mul is the value by which the samples are scaled
// and has to be in [MUL_MIN, MUL_MAX]
#define MUL_INIT 1.0
#define MUL_MIN 0.1
#define MUL_MAX 5.0

#define MID_S16 (SHRT_MAX * 0.25)
#define MID_FLOAT (INT_MAX * 0.25)

// Silence level
// FIXME: should be relative to the level of the samples
#define SIL_S16 (SHRT_MAX * 0.01)
#define SIL_FLOAT (INT_MAX * 0.01) // FIXME

// smooth must be in ]0.0, 1.0[
#define SMOOTH_MUL 0.06
#define SMOOTH_LASTAVG 0.06

#define clamp(a,min,max) (((a)>(max))?(max):(((a)<(min))?(min):(a)))

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

namespace Gst {

static GType volume_normalizer_get_type();

GstAudioFilterClass *VolumeNormalizer::parentClass = 0;

GType VolumeNormalizer::gtype() {return volume_normalizer_get_type();}

// GST_BOILERPLATE_FULL(VolumeNormalizer, volume_normalizer, GstAudioFilter
// 		, GST_TYPE_AUDIO_FILTER, VolumeNormalizer::additionalInitialize);

GST_BOILERPLATE(VolumeNormalizer, volume_normalizer, GstAudioFilter, GST_TYPE_AUDIO_FILTER);

// void VolumeNormalizer::additionalInitialize(GType type) {
// 	static const GInterfaceInfo ifaceInfo = {(GInterfaceInitFunc)initializeIface, 0, 0};
// 	static const GInterfaceInfo navInfo = {(GInterfaceInitFunc)initializeNavigation, 0, 0};
// 	g_type_add_interface_static(type, GST_TYPE_IMPLEMENTS_INTERFACE, &ifaceInfo);
// 	g_type_add_interface_static(type, GST_TYPE_NAVIGATION, &navInfo);
// }

VolumeNormalizer::VolumeNormalizer()
{
}

void VolumeNormalizerClass::initialize() {
	GstAudioFilterClass *filter = GST_AUDIO_FILTER_CLASS(this);
	GstCaps *caps = gst_caps_from_string(ALLOWED_CAPS);
	gst_audio_filter_class_add_pad_templates(filter, caps);
	gst_caps_unref(caps);
}

static void volume_normalizer_base_init(gpointer klass) {
	VolumeNormalizerClass *self = GST_VOLUME_NORMALIZER_CLASS(klass);
	if (self)
		self->initialize();
}

void VolumeNormalizer::initialize(VolumeNormalizerClass *klass) {
	GstBaseTransformClass *trans = GST_BASE_TRANSFORM_CLASS(klass);
	GstAudioFilterClass *filter = GST_AUDIO_FILTER_CLASS(klass);
	trans->transform_ip = transform;
	filter->setup = setup;
}

static void volume_normalizer_class_init(VolumeNormalizerClass *klass) {
	VolumeNormalizer::initialize(klass);
}


static void volume_normalizer_init(VolumeNormalizer *self, VolumeNormalizerClass */*klass*/) {
	gst_base_transform_set_gap_aware(GST_BASE_TRANSFORM(self), true);
	self->mul = MUL_INIT;
	self->lastavg = MID_S16;
// 	self->sink = 0;
// 	self->width = 0;
// 	self->height = 0;
// 	self->bpp = 0;
// 	self->depth = 0;
}

template<> void VolumeNormalizer::normalize<int16_t>(gpointer buffer, int size) {
	register int i = 0;
	int16_t *data = reinterpret_cast<int16_t*>(buffer);
	const int len = size / 2;
	float curavg = 0.0, newavg, neededmul;
	int tmp;
	for (i = 0; i < len; i++) {
		tmp = data[i];
		curavg += tmp * tmp;
	}
	curavg = std::sqrt(curavg / (float) len);
	if (curavg > SIL_S16) {
		neededmul = MID_S16 / (curavg * mul);
		mul = (1.0 - SMOOTH_MUL) * mul + SMOOTH_MUL * neededmul;
		mul = clamp(mul, MUL_MIN, MUL_MAX);
	}
	// Scale & clamp the samples
	for (i = 0; i < len; i++) {
		tmp = mul * data[i];
		tmp = clamp(tmp, SHRT_MIN, SHRT_MAX);
		data[i] = tmp;
	}
	// Evaulation of newavg (not 100% accurate because of values clamping)
	newavg = mul * curavg;
	// Stores computed values for future smoothing
	lastavg = (1.0 - SMOOTH_LASTAVG) * lastavg + SMOOTH_LASTAVG * newavg;
}

template<> void VolumeNormalizer::normalize<float>(gpointer buffer, int size) {
	register int i = 0;
	float *data = reinterpret_cast<float*>(buffer);
	const int len = size / 4;
	float curavg = 0.0, newavg, neededmul;
	float tmp;
	for (i = 0; i < len; i++) {
		tmp = data[i];
		curavg += tmp * tmp;
	}
	curavg = std::sqrt(curavg / (float) len);
	if (curavg > SIL_FLOAT) {
		neededmul = MID_FLOAT / (curavg * mul);
		mul = (1.0 - SMOOTH_MUL) * mul + SMOOTH_MUL * neededmul;
		mul = clamp(mul, MUL_MIN, MUL_MAX);
	}
	// Scale & clamp the samples
	for (i = 0; i < len; i++)
		data[i] *= mul;
	// Evaulation of newavg (not 100% accurate because of values clamping)
	newavg = mul * curavg;
	// Stores computed values for future smoothing
	lastavg = (1.0 - SMOOTH_LASTAVG) * lastavg + SMOOTH_LASTAVG * newavg;
}


GstFlowReturn VolumeNormalizer::transform(GstBaseTransform *base, GstBuffer *buffer) {
	VolumeNormalizer *self = GST_VOLUME_NORMALIZER(base);
	const GstClockTime ts = gst_segment_to_stream_time(&base->segment
			, GST_FORMAT_TIME, GST_BUFFER_TIMESTAMP(buffer));
	if (GST_CLOCK_TIME_IS_VALID(ts))
		gst_object_sync_values(G_OBJECT(self), ts);
	if (gst_base_transform_is_passthrough(base)
			|| GST_BUFFER_FLAG_IS_SET(buffer, GST_BUFFER_FLAG_GAP))
		return GST_FLOW_OK;
	GstAudioFilter *filter = GST_AUDIO_FILTER(self);
	if (!filter->format.caps)
		return GST_FLOW_OK;
	if (filter->format.type == GST_BUFTYPE_LINEAR && filter->format.width == 16) {
		self->normalize<int16_t>(GST_BUFFER_DATA(buffer), GST_BUFFER_SIZE (buffer));
	} else if (filter->format.type == GST_BUFTYPE_FLOAT && filter->format.width == 32) {
		self->normalize<float>(GST_BUFFER_DATA(buffer), GST_BUFFER_SIZE (buffer));
	}
	return GST_FLOW_OK;
}

gboolean VolumeNormalizer::setup(GstAudioFilter *filter, GstRingBufferSpec */*format*/) {
	if (filter->format.type == GST_BUFTYPE_LINEAR && filter->format.width == 16) {
// 		self->normalize<int16_t>(GST_BUFFER_DATA(buffer), GST_BUFFER_SIZE (buffer));
	} else if (filter->format.type == GST_BUFTYPE_FLOAT && filter->format.width == 32) {
// 		self->normalize<float>(GST_BUFFER_DATA(buffer), GST_BUFFER_SIZE (buffer));
	}
// 	gboolean ret = false;
// 	VolumeNormalizer *self = GST_VOLUME_NORMALIZER(filter);
// 	res = volume_update_volume (this, volume, mute);
// 	if (!res) {
// 		GST_ELEMENT_ERROR (this, CORE, NEGOTIATION,
// 					 ("Invalid incoming format"), (NULL));
// 	}
// 	this->negotiated = res;
	return true;
}

}
