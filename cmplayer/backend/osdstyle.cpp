#include "osdstyle.h"
#include <QDebug>

namespace Backend {

OsdStyle::Clut *OsdStyle::Clut::get() {
	static Clut clut;
	return &clut;
}

OsdStyle::Clut::Clut(): m_clut(256) {
	int i=0;
	for (int r=0; r<8; ++r) {for (int g=0; g<8; ++g) {
	for (int b=0; b<4; ++b) {
		static const QRgb key = qRgb(0x92, 0xff, 0x00);
		static const double RFactor = 255.0/7.0;
		static const double GFactor = 255.0/7.0;
		static const double BFactor = 255.0/3.0;
		const int red = qMin(255, qRound(r*RFactor));
		const int green = qMin(255, qRound(g*GFactor));
		const int blue = qMin(255, qRound(b*BFactor));
		const QRgb rgb = qRgb(red, green, blue);
		m_colorMap[i] = toYuv(rgb);
		if (rgb == key) {
			m_transMap[i] = 0;
			m_clut[i++] = qRgba(0, 0, 0, 0);
		} else {
			m_transMap[i] = 255;
			m_clut[i++] = rgb;
		}
	}}}
}

}
