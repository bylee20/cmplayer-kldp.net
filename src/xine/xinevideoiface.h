#ifndef XINE_XINEVIDEOIFACE_H
#define XINE_XINEVIDEOIFACE_H

class QPoint;

namespace Xine {

class XineStream;

class XineVideoIface {
public:
	XineVideoIface(XineStream *stream): m_stream(stream), m_tracking(false) {}
	virtual ~XineVideoIface() {}
	XineStream *stream() const {return m_stream;}
	virtual int xineType() const = 0;
	virtual void *visual() = 0;
	void activateMouseTracking(bool active) {m_tracking = active;}
	bool isMouseTrackingActivated() const {return m_tracking;}
protected:
	void mouseMoved(const QPoint &pos);
	void mouseClicked(const QPoint &pos, int button);
private:
	XineStream *m_stream;
	bool m_tracking;
};

}

#endif
