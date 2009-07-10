#ifndef CORE_COLORPROPERTY_H
#define CORE_COLORPROPERTY_H

#include <QtGlobal>

namespace Core {

class ColorProperty {
public:
	static bool isSame(double v1, double v2) {return qAbs(v1-v2) < 1.0e-5;}
	enum Value {Brightness = 0, Saturation, Contrast, Hue};
	ColorProperty(double b, double s, double c, double h) {
		m_value[Brightness] = b;
		m_value[Saturation] = s;
		m_value[Contrast] = c;
		m_value[Hue] = h;
	}
	ColorProperty() {
		m_value[Brightness] = m_value[Saturation]
				= m_value[Contrast] = m_value[Hue] = 0.0;
	}
	ColorProperty(const ColorProperty &other) {
		m_value[Brightness] = other.m_value[Brightness];
		m_value[Saturation] = other.m_value[Saturation];
		m_value[Contrast] = other.m_value[Contrast];
		m_value[Hue] = other.m_value[Hue];
	}
	~ColorProperty() {}
	ColorProperty &operator = (const ColorProperty &rhs) {
		if (this != &rhs) {
			m_value[Brightness] = rhs.m_value[Brightness];
			m_value[Saturation] = rhs.m_value[Saturation];
			m_value[Contrast] = rhs.m_value[Contrast];
			m_value[Hue] = rhs.m_value[Hue];
		}
		return *this;
	}
	bool operator == (const ColorProperty &rhs) const {
		return isSame(m_value[Brightness], rhs.m_value[Brightness])
				&& isSame(m_value[Saturation], rhs.m_value[Saturation])
				&& isSame(m_value[Contrast], rhs.m_value[Contrast])
				&& isSame(m_value[Hue], rhs.m_value[Hue]);
	}
	bool operator != (const ColorProperty &rhs) const {
		return !operator==(rhs);
	}
	double &operator [] (Value p) {return m_value[p];}
	const double &operator [] (Value p) const {return m_value[p];}
	double value(Value v) const {return m_value[v];}
	double brightness() const {return m_value[Brightness];}
	double saturation() const {return m_value[Saturation];}
	double contrast() const {return m_value[Contrast];}
	double hue() const {return m_value[Hue];}
	void setValue(Value p, double val) {m_value[p] = isSame(val, 0.0) ? 0.0 : val;}
	void setBrightness(double v) {m_value[Brightness] = v;}
	void setSaturation(double v) {m_value[Saturation] = v;}
	void setContrast(double v) {m_value[Contrast] = v;}
	void setHue(double v) {m_value[Hue] = v;}
private:
	double m_value[4];
};

}

#endif
