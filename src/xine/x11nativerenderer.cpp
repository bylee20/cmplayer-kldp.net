// #include "x11nativerenderer.h"
// #include <QtGui/QApplication>
// #include <X11/Xlib.h>
// 
// namespace Xine {
// 
// X11NativeRenderer::X11NativeRenderer(Core::PlayEngine *engine, Xine::XineStream *stream)
// : NativeRenderer(engine, stream) {
// 	XInitThreads();
// 	m_display = XOpenDisplay(NULL);
// 	m_visual.display = m_display;
// 	m_visual.screen = DefaultScreen(m_display);
// 	m_visual.d = screenWinId();
// 	m_visual.user_data = this;
// 	m_visual.dest_size_cb = cbDestSize;
// 	m_visual.frame_output_cb = cbFrameOutput;
// 	QApplication::syncX();
// }
// 
// X11NativeRenderer::~X11NativeRenderer() {
// 	if (m_display)
// 		XCloseDisplay(m_display);
// }
// 
// int X11NativeRenderer::xineType() const {
// 	return XINE_VISUAL_TYPE_X11;
// }
// 
// void *X11NativeRenderer::visual() {
// 	return &m_visual;
// }
// 
// bool X11NativeRenderer::screenX11Event(XEvent *event) {
// 	if (stream()->videoPort && event->type == Expose && event->xexpose.count == 0)
// 		xine_port_send_gui_data(stream()->videoPort, XINE_GUI_SEND_EXPOSE_EVENT, event);
// 	return false;
// }
// 
// 
// }
