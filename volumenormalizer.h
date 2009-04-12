#ifndef GSTVOLUMENORMALIZER_H
#define GSTVOLUMENORMALIZER_H

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/audio/audio.h>
#include <gst/audio/gstaudiofilter.h>

namespace Gst {

// struct _GstVolume {
//   GstAudioFilter element;
// 
//   void (*process)(GstVolume*, gpointer, guint);
// 
//   gboolean mute;
//   gfloat volume;
// 
//   gboolean current_mute;
//   gfloat current_volume;
// 
//   gint   current_vol_i32;
//   gint   current_vol_i24; /* the _i(nt) values get synchronized with the */
//   gint   current_vol_i16; /* the _i(nt) values get synchronized with the */
//   gint   current_vol_i8;   /* the _i(nt) values get synchronized with the */
//   
//   GList *tracklist;
//   gboolean negotiated;
// };



// struct _GstVolumeClass {
//   GstAudioFilterClass parent_class;
// };

// GType volume_normalizer_get_type (void);

struct VolumeNormalizerClass {
	GstAudioFilterClass parent;
	VolumeNormalizerClass(): m_init(false) {}
	void initialize();
private:
	bool m_init;
};

class VolumeNormalizer{
public:
	GstAudioFilter parent;
	static GType gtype();
	VolumeNormalizer();
	static GstFlowReturn transform(GstBaseTransform *base, GstBuffer *buffer);
	static gboolean setup(GstAudioFilter *filter, GstRingBufferSpec *format);
	static void initialize(VolumeNormalizerClass *klass);
// 	static void additionalInitialize(GType type);
	static GstAudioFilterClass *parentClass;
	template<typename T> void normalize(gpointer buffer, int size);
	
	float mul, lastavg;
//     // method 2
// 	int idx;
// 	struct {
// 		float avg; // average level of the sample
// 		int len; // sample size (weight)
// 	} mem[NSAMPLES];
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
