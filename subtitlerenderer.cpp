#include "subtitlerenderer.hpp"
#include "textosdrenderer.hpp"
#include <QtCore/QDebug>

struct SubtitleRenderer::Data {
	TextOsdRenderer *osd;
	Subtitle sub;
	double frameRate;
	Subtitle::Component comp;
	Subtitle::Component::const_iterator prev;
	int delay;
};

SubtitleRenderer::SubtitleRenderer(): d(new Data) {
	d->osd = new TextOsdRenderer(Qt::AlignBottom | Qt::AlignHCenter);
	d->frameRate = 30;
	d->delay = 0;
	d->prev = d->comp.end();

	OsdStyle style = d->osd->style();
	style.font.setBold(true);
	d->osd->setStyle(style);
}

SubtitleRenderer::~SubtitleRenderer() {
	delete d->osd;
	delete d;
}

OsdRenderer *SubtitleRenderer::osd() const {
	return d->osd;
}

void SubtitleRenderer::setSubtitle(const Subtitle &subtitle) {
	d->sub = subtitle;
	d->comp = d->sub.component(d->frameRate);
	d->prev = d->comp.end();
}

void SubtitleRenderer::setFrameRate(double frameRate) {
	if (d->frameRate != frameRate) {
		d->comp = d->sub.component(d->frameRate = frameRate);
		d->prev = d->comp.end();
	}
}

void SubtitleRenderer::render(int ms) {
	if (d->comp.isEmpty())
		return;
	Subtitle::Component::const_iterator it = d->comp.start(ms - d->delay, d->frameRate);
	if (it != d->prev) {
		d->prev = it;
		if (it != d->comp.end())
			d->osd->showText(it.value());
	}
}
