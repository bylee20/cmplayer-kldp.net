#include "videofilter.hpp"
#include "videomanipulator.hpp"

VideoFilter::VideoFilter(VideoManipulator *man) {
	m_man = man;
}

void VideoFilter::reconfigure() {
	m_man->reconfigure();
}

void VideoFilter::rerender() {
	m_man->rerender();
}

void VideoFilter::setManipulator(VideoManipulator *man) {
	m_man = man;
}
