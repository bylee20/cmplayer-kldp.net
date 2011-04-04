#include "osdrenderer.hpp"
#include "osdstyle.hpp"
#include <math.h>

int OsdRenderer::cachedSize(double size) {
	return qMax(128, 1 << qRound(log2(size) + 0.5));
}

