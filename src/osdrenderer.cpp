#include "osdrenderer.hpp"
#include "osdstyle.hpp"

struct OsdRenderer::Data {
	OsdStyle style;
};

OsdRenderer::OsdRenderer()
: d(new Data) {
}

OsdRenderer::~OsdRenderer() {
	delete d;
}

void OsdRenderer::setStyle(const OsdStyle &style) {
	d->style = style;
//	updateStyle(d->style);
	emit styleChanged(d->style);
	emit needToRerender();
}

const OsdStyle &OsdRenderer::style() const {
	return d->style;
}
