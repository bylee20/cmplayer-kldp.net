#ifndef GSTVOLUMENORMALIZER_H
#define GSTVOLUMENORMALIZER_H

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/audio/audio.h>
#include <gst/audio/gstaudiofilter.h>

namespace Gst {

struct VolumeNormalizerClass {
	GstAudioFilterClass parent;
};

class VolumeNormalizer{
public:
	GstAudioFilter parent;
	static GType gtype();
	static GstFlowReturn transform(GstBaseTransform *base, GstBuffer *buffer);
	static gboolean setup(GstAudioFilter *filter, GstRingBufferSpec *format);
	template<typename T> void normalize(gpointer buffer, int size);
	float mul, lastavg;
};

}

#define GST_TYPE_VOLUME_NORMALIZER (volume_normalizer_get_type())
#define GST_VOLUME_NORMALIZER(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_VOLUME_NORMALIZER, VolumeNormalizer))
#define GST_VOLUME_NORMALIZER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_VOLUME_NORMALIZER, VolumeNormalizerClass))
#define GST_IS_VOLUME_NORMALIZER(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_VOLUME_NORMALIZER))
#define GST_IS_VOLUME_NORMALIZER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_VOLUME_NORMALIZER))

#endif
