#ifndef VIDEOEQFILTER_HPP
#define VIDEOEQFILTER_HPP

#include "videofilter.hpp"
#include <QtCore/QtGlobal>
#include "colorproperty.hpp"

class VideoEqFilter : public VideoFilter {
public:
	VideoEqFilter();
	void crop(int crop_h, int crop_v);
	bool transform(I420Picture *pic);
	void setProperty(ColorProperty prop);
	const ColorProperty &property() const {return m_prop;}
	bool isInPlace() const {return true;}
	void setEnabled(bool enabled);
	bool isEnabled() const {return m_enabled;}
private:
	bool m_pass, m_enabled;
	ColorProperty m_prop;
	uchar m_tabley[256];
	uchar m_tableu[256][256];
	uchar m_tablev[256][256];
};

#endif // VIDEOEQFILTER_HPP
