#ifndef XINE_XINEOSD_CLUT_H
#define XINE_XINEOSD_CLUT_H

#include "xineosd.h"
#include <QMap>
#include <QVector>

namespace Xine {
class XineOsd::Clut {
public:
	static Clut *get() {static Clut clut; return &clut;}
	static int toYuv(int r, int g, int b) {
	#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	#define Yuv(R,G,B) ((((((66*R+129*G+25*B+128)>>8)+16)<<8)\
		|(((112*R-94*G-18*B+128)>>8)+128))<<8|(((-38*R-74*G+112*B+128)>>8)+128))
	#else
	#define Yuv(R,G,B) (((((((-38*R-74*G+112*B+128)>>8)+128)<<8)\
		|(((112*R-94*G-18*B+128)>>8)+128))<<8|(((66*R+129*G+25*B+128)>>8)+16))<<8)
	#endif
		return Yuv(r, g, b);
	#undef Yuv
	}
	static int toYuv(const QRgb &rgb) {
		return toYuv(qRed(rgb), qBlue(rgb), qGreen(rgb));
	}
	const uint32_t *colorMap() const {return m_colorMap;}
	const uint8_t *transMap() const {return m_transMap;}
	int index(const QRgb &rgb) const {
#define Conv(val) qMin(3, qRound(double(val)/RgbFactor))
		int r = Conv(qRed(rgb));
		int g = Conv(qGreen(rgb));
		int b = Conv(qBlue(rgb));
		int a = 0;
		if (r == 3 && g == 3 && b == 3)
			a = Conv(qAlpha(rgb));
		else if ((a = qMin(4, qRound(double(qAlpha(rgb))/AlphaFactor))) == 0)
			return 255;
#undef Conv
		QRgb color = qRgba(r, g, b, a);
		return m_clut.contains(color) ? m_clut[color] : -1;
	}
	const QVector<QRgb> &palette() const {
		static bool init = false;
		if (!init) {
			QMap<QRgb, int>::const_iterator it = m_clut.begin();
			for (int i=0; it != m_clut.end(); ++it, ++i) {
				int r = qRed(it.key());
				int g = qGreen(it.key());
				int b = qBlue(it.key());
				int a = qAlpha(it.key());
				if (r == 3 && g == 3 && b == 3) {
					a = qMin(255, qRound(a*RgbFactor));
				} else
					a = qMin(255, qRound(a*AlphaFactor));
				r = qMin(255, qRound(r*RgbFactor));
				g = qMin(255, qRound(g*RgbFactor));
				b = qMin(255, qRound(b*RgbFactor));
				m_palette[it.value()] = qRgba(r, g, b, a);
			}
		}
		return m_palette;
	}
private:
	Clut(): m_palette(256) {
		int i=0;
		for (int r=0; r<4; ++r) {for (int g=0; g<4; ++g) {
		for (int b=0; b<4; ++b) {for (int a=1; a<5; ++a) {
			if (r == 3 && g == 3 && b == 3)
				break;
			m_colorMap[i] = toYuv(qMin(255, qRound(r*RgbFactor))
					, qMin(255, qRound(g*RgbFactor)), qMin(255, qRound(b*RgbFactor)));
			m_transMap[i] = qMin(255, qRound(a*AlphaFactor));
			m_clut[qRgba(r, g, b, a)] = i++;
		}}}}
		for (int a = 3; a>=0; --a) {
			m_colorMap[i] = toYuv(255, 255, 255);
			m_transMap[i] = qMin(255, qRound(a*RgbFactor));
			m_clut[qRgba(3, 3, 3, a)] = i++;
		}
	}
	static const double AlphaFactor = 255.0/4.0;
	static const double RgbFactor = 255.0/3.0;
	QMap<QRgb, int> m_clut;
	uint32_t m_colorMap[256];
	uint8_t m_transMap[256];
	mutable QVector<QRgb> m_palette;
};

}

#endif
