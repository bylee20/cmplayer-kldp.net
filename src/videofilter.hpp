#ifndef VIDEOFILTER_HPP
#define VIDEOFILTER_HPP

#include <gst/gst.h>

class I420Picture;		class VideoManipulator;

class VideoFilter {
public:
	VideoFilter(VideoManipulator *man = 0);
	virtual ~VideoFilter() {}
	virtual bool transform(I420Picture */*out*/, const I420Picture &/*in*/) {return true;}
	virtual bool transform(I420Picture */*pic*/) {return true;}
	virtual bool isInPlace() const = 0;
	void setManipulator(VideoManipulator *man);
	void reconfigure();
	void rerender();
private:
	VideoManipulator *m_man;
};

#endif // VIDEOFILTER_HPP
