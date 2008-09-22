#ifndef BACKEND_OSDSTYLE_H
#define BACKEND_OSDSTYLE_H

#include <QFont>
#include <QColor>
#include <QVector>

namespace Backend {

struct OsdStyle {
	class Clut {
	public:
		static Clut *get();
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
	enum Scale {FitToHeight, FitToWidth, FitToDiagonal};
	OsdStyle(): font(), border_color(Qt::black), text_color(Qt::white)
	, size(0.05), scale(FitToHeight), high_quality(false) {}
	bool operator != (const OsdStyle &rhs) const {
		return (font != rhs.font) || (text_color != rhs.text_color)
				|| (border_color != rhs.border_color) || (size != rhs.size)
				|| (scale != rhs.scale) || (high_quality != rhs.high_quality);
	}
	bool operator == (const OsdStyle &rhs) const {return !((*this) != rhs);}
	QFont font; QColor border_color, text_color; double size;
	Scale scale; bool high_quality;
};

}

#endif
