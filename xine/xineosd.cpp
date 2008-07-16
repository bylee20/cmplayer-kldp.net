/*
 * xineosd.cpp
 *
 *  Created on: 2008. 7. 16
 *      Author: xylosper
 */

#include "xineosd.h"

namespace Xine {

XineOsd::XineOsd(XineStream *stream) {
	// TODO Auto-generated constructor stub
//	static osd_object_t *osd = 0;
//	if (osd) {
//		m_stream->osd_renderer->hide(osd, 0);
//		m_stream->osd_renderer->free_object(osd);
//	}
//	osd = m_stream->osd_renderer->new_object(m_stream->osd_renderer, 300, 200);
//	/* set sans serif 24 font */
//	m_stream->osd_renderer->set_font(osd, "sans", 24);
//
//	/* copy pre-defined colors for white, black border, transparent background to
//	  starting at the index used by the first text palette */
//	m_stream->osd_renderer->set_text_palette(osd, TEXTPALETTE_WHITE_BLACK_TRANSPARENT, OSD_TEXT1);
//
//	/* copy pre-defined colors for white, no border, translucid background to
//	  starting at the index used by the second text palette */
//	m_stream->osd_renderer->set_text_palette(osd, TEXTPALETTE_WHITE_NONE_TRANSLUCID, OSD_TEXT2);
//	m_stream->osd_renderer->render_text(osd, 0, 0, QString::number(m_curTime).toLatin1(), OSD_TEXT1);
//	//m_stream->osd_renderer->render_text(osd, 0, 30, "white text, no border", OSD_TEXT2);
//
//	m_stream->osd_renderer->show(osd, 0); /* 0 stands for 'now' */
}

XineOsd::~XineOsd() {
	// TODO Auto-generated destructor stub
}

}
