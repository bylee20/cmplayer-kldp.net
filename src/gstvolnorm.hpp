#ifndef GSTVOLNORM_HPP
#define GSTVOLNORM_HPP

#include <gst/gst.h>
#include <gst/audio/gstaudiofilter.h>

class GstVolNorm {
	GstAudioFilter parent;
	void doInt16_1(GstBuffer *outbuf);
	void doFloat32_1(GstBuffer *outbuf);
public:
	static const int NSAMPLES = 128;
	static GType gtype();
//private:
	GstFlowReturn transform_ip(GstBuffer *outbuf);
	bool setup(GstRingBufferSpec *format);
	struct Data {
		float mul, lastAvg;
		bool on;
		int idx;
		struct Mem {
			Mem(): avg(0.0), len(0) {}
			float avg; // average level of the sample
			int len; // sample size (weight)
		};
		Mem mem[NSAMPLES];
	};
	Data *d;
};

struct GstVolNormClass {
	GstAudioFilterClass parent;
};

GType gst_vol_norm_get_type();

#define GST_TYPE_VOL_NORM (gst_vol_norm_get_type())

#define GST_VOL_NORM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VOL_NORM, GstVolNorm))
#define GST_VOL_NORM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VOL_NORM, GstVolNormClass))
#define GST_IS_VOL_NORM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VOL_NORM))
#define GST_IS_VOL_NORM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VOL_NORM))

#endif // GSTVOLNORM_HPP
