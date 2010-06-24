#include "videoeqfilter.hpp"
#include "i420picture.hpp"
#include <cmath>

VideoEqFilter::VideoEqFilter() {
	m_pass = true;
	m_enabled = false;
}

void VideoEqFilter::setEnabled(bool enabled) {
	if (m_enabled != enabled) {
		m_enabled = enabled;
		rerender();
	}
}

bool VideoEqFilter::transform(I420Picture *pic) {
	if (m_pass || !m_enabled)
		return true;
	uchar *y = pic->y;
	const int max_y = pic->size_y();
	for (int i=0; i<max_y; ++i) {
		*y = m_tabley[*y];
		++y;
	}
	const int max_uv = pic->size_uv();
	uchar *u = pic->u;
	uchar *v = pic->v;
	uchar temp_u, temp_v;
	for (int i=0; i<max_uv; ++i) {
		temp_u = *u;
		temp_v = *v;
		*u = m_tableu[temp_u][temp_v];
		*v = m_tablev[temp_u][temp_v];
		++u;
		++v;
	}
	return true;
}

void VideoEqFilter::setProperty(ColorProperty prop) {
	prop.clamp();
	if (m_prop == prop)
		return;
	m_prop = prop;
	const double b = prop.brightness();
	const double h = prop.hue();
	const double s = prop.saturation() + 1.0;
	const double c = prop.contrast() + 1.0;
	m_pass = qFuzzyCompare(b, 1.0) && qFuzzyCompare(h, 1.0)
		&& qFuzzyCompare(s, 1.0) && qFuzzyCompare(c, 1.0);
#define clamp(value) ((value) < 0 ? 0 : ((value) > 255 ? 255 : value))
	for (int i=0; i<256; ++i) {
		const int y = 16.0 + ((i - 16.0)*c + b*255.0) + 0.5;
		m_tabley[i] = clamp(y);
	}
	const double cos_h = std::cos(M_PI*h);
	const double sin_h = std::sin(M_PI*h);
	for (int i=-128; i<128; ++i) {
		for (int j=-128; j<128; ++j) {
			const int u = 128.0 + (i*cos_h + j*sin_h)*s + 0.5;
			const int v = 128.0 + (-i*sin_h + j*cos_h)*s + 0.5;
			m_tableu[i + 128][j + 128] = clamp(u);
			m_tablev[i + 128][j + 128] = clamp(v);
		}
	}
#undef clamp
	rerender();
}
