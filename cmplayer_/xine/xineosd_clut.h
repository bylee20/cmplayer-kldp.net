#ifndef XINE_XINEOSD_CLUT_H
#define XINE_XINEOSD_CLUT_H

#include "xineosd.h"
#include <QVector>
#include <QHash>

namespace Xine {
class XineOsd::Clut {
public:
	static Clut *get() {static Clut clut; return &clut;}
	inline const uint32_t *colorMap() const {return m_colorMap;}
	inline const uint8_t *transMap() const {return m_transMap;}
	inline const QVector<QRgb> &clut() const {return m_clut;}
private:
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
	static int toYuv(const QRgb &rgb) {return toYuv(qRed(rgb), qGreen(rgb), qBlue(rgb));}
	Clut();
	uint32_t m_colorMap[256];
	uint8_t m_transMap[256];
	QVector<QRgb> m_clut;
};

}

#endif
