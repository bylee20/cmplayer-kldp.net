#include "audiocontroller.hpp"

struct AudioController::Data {
	GstElement *bin, *volume, *sink;
	double amp;
	int vol;
	bool muted;
};

AudioController::AudioController(): d(new Data) {
	d->bin = gst_bin_new("audio-controller");
	gst_object_ref(GST_OBJECT(d->bin));
	gst_object_sink(GST_OBJECT(d->bin));

	d->volume = gst_element_factory_make("volume", 0);
	d->sink = gst_element_factory_make("autoaudiosink", 0);

	GstElement *queue = gst_element_factory_make("queue", 0);
	GstElement *rg = gst_element_factory_make("rgvolume", 0);

	gst_bin_add_many(GST_BIN(d->bin), rg, queue, d->volume, d->sink, NULL);
	gst_element_link_many(queue, rg, d->volume, d->sink, NULL);

	GstPad *pad = gst_element_get_pad(queue, "sink");
	gst_element_add_pad(d->bin, gst_ghost_pad_new("sink", pad));
	gst_object_unref(GST_OBJECT(pad));

	g_object_get(G_OBJECT(d->volume), "volume", &d->amp, NULL);
	g_object_get(G_OBJECT(d->volume), "mute", &d->muted, NULL);
	d->vol = 100;
}

AudioController::~AudioController() {
	gst_object_unref(GST_OBJECT(d->bin));
	delete d;
}

GstElement *AudioController::bin() const {
	return d->bin;
}

int AudioController::volume() const {
	return d->vol;
}

bool AudioController::isMuted() const {
	return d->muted;
}

double AudioController::gstVolume() {
	return qBound(0.0, d->vol*d->amp/100.0, 10.0);
}

void AudioController::setVolume(int volume) {
	volume = qBound(0, volume, 100);
	if (d->vol != volume) {
		d->vol = volume;
		g_object_set(G_OBJECT(d->volume), "volume", gstVolume(), NULL);
		emit volumeChanged(d->vol);
	}
}

void AudioController::setMuted(bool muted) {
	if (d->muted != muted) {
		d->muted = muted;
		g_object_set(G_OBJECT(d->volume), "mute", d->muted, NULL);
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
		g_object_set(G_OBJECT(d->volume), "volume", gstVolume(), NULL);
//		emit volumeChanged(d->vol);
	}
}

double AudioController::preAmp() const {
	return d->amp;
}

