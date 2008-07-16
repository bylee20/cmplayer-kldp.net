#ifndef XINE_XINEOSD_H_
#define XINE_XINEOSD_H_

namespace Xine {

class XineStream;

class XineOsd {
public:
	XineOsd(XineStream *stream);
	virtual ~XineOsd();
};

}

#endif
