#include "playinfoview.hpp"
#include "global.hpp"
#include "osdstyle.hpp"
#include "richstring.hpp"
#include "audiocontroller.hpp"
#include "avmisc.hpp"
#include "videoscene.hpp"
#include "textosdrenderer.hpp"
#include "application.hpp"
#include <stdio.h>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QStringBuilder>

typedef QLatin1String _LS;
typedef QLatin1Char _LC;

struct PlayInfoView::Data {
	QTimer *timer;
	VideoFormat vfmt;
	AudioFormat afmt;
	const VideoScene *video;
	const AudioController *audio;
	QString vfps, cpu, mem;
	TextOsdRenderer *osd;
	bool visible;
	QString vinput, voutput;
	QString size;
};

PlayInfoView::PlayInfoView(QObject *parent)
: QObject(parent), d(new Data) {
	d->osd = new TextOsdRenderer(Qt::AlignTop | Qt::AlignLeft);
	OsdStyle style = d->osd->style();
	style.color_fg = QColor(Qt::yellow);
	style.text_scale = 0.02;
	d->osd->setStyle(style);
	d->size = d->mem = d->cpu = d->vfps = QLatin1String("--");
	d->visible = false;
	d->timer = new QTimer(this);
	d->timer->setInterval(500);
	d->video = 0;
	d->vinput = d->voutput = "--";
	d->audio = 0;
	connect(app(), SIGNAL(gotProcInfo(double,int,double)), this, SLOT(setProcInfo(double,int,double)));
	connect(d->timer, SIGNAL(timeout()), this, SLOT(update()));
}

PlayInfoView::~PlayInfoView() {
	delete d->timer;
	delete d;
}

OsdRenderer *PlayInfoView::osd() const {
	return d->osd;
}

void PlayInfoView::setVideo(const VideoScene *video) {
	d->video = video;
	connect(d->video, SIGNAL(formatChanged(VideoFormat)), this, SLOT(setVideoFormat(VideoFormat)));
}

void PlayInfoView::setVideoFormat(const VideoFormat &vfmt) {
	d->vfmt = vfmt;
	d->size = Global::toString(vfmt.size());
	const double sar = (double)d->vfmt.width/(double)d->vfmt.height;
	const double dar = sar*d->vfmt.sar;
	const QString fmt("fourcc: %1, fps: %3, aspect ratio: %2");
	d->vinput = fmt.arg(VideoFormat::fourccToString(d->vfmt.source_fourcc))
		.arg(toString(sar, false, 2)).arg(toString(d->vfmt.fps, false, 2));
	d->voutput = fmt.arg(VideoFormat::fourccToString(d->vfmt.output_fourcc)).arg(toString(dar, false, 2));
	update();
}

void PlayInfoView::setProcInfo(double cpu, int rss, double mem) {
	d->cpu = toString(cpu, false) % _LC('%');
	d->mem = toString(rss/1024.0, false) % _LS("MB (") % toString(mem, false) % _LS("%)");
}

void PlayInfoView::setVisible(bool visible) {
	if (d->visible != visible) {
		d->visible = visible;
		if (visible) {
			app()->getProcInfo();
			d->timer->start();
		} else {
			d->timer->stop();
			d->osd->clear();
			d->cpu = d->vfps = -1;
		}
	}
}

void PlayInfoView::update() {
	static const QString br("<br>");
	static const QString colon(": ");
	if (d->visible) {
		app()->getProcInfo();
		const QString text =
			tr("CPU Usage") % colon % d->cpu % br %
			tr("Memory Usage") % colon % d->mem % br % br %
			tr("Video Information") % br %
			tr("Pixel Size") % colon % d->size % br %
			tr("Input") % colon % d->vinput % br %
			tr("Output") % colon % d->voutput.arg(toString(d->video->outputFrameRate(), false, 2)) % br % br %
			tr("Audio Information") % br %
			tr("Normalizer") % colon % toString(d->audio->gain()*100.0, false) % _LS("%<br>");
		d->osd->showText(RichString(text, text));
	}
}

void PlayInfoView::setAudio(const AudioController *audio) {
	d->audio = audio;
	connect(d->audio, SIGNAL(formatChanged(AudioFormat)), this, SLOT(setAudioFormat(AudioFormat)));
}

void PlayInfoView::setAudioFormat(const AudioFormat &afmt) {
	d->afmt = afmt;
}
