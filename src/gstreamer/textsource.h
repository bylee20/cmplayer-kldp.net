#ifndef GSTTEXTSOURCE_H
#define GSTTEXTSOURCE_H

#include <gst/base/gstbasesrc.h>
#include <gst/gst.h>
#include <QtCore/QString>
#include <QtCore/QByteArray>

namespace Gst {

struct TextSourceClass {
	GstBaseSrcClass parent;
// 	void initialize();
};

class TextSource {
public:
	GstBaseSrc parent;
	static GType gtype();
	static GstStaticPadTemplate templ;
	static void dispose(GObject *obj);
	static GstFlowReturn create(GstBaseSrc *base
			, guint64 offset, guint length, GstBuffer **buffer);
	QByteArray *data;
// 	int pos;
// 	static void initializeIface(GstImplementsInterfaceClass *klass);
// 	static void initialize(QWidgetVideoSinkClass *klass);
};

/* Standard macros for defining types for this element.  */
#define GST_TYPE_TEXT_SOURCE (text_source_get_type())
#define GST_TEXT_SOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_TEXT_SOURCE, TextSource))
#define GST_TEXT_SOURCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_TEXT_SOURCE, TextSourceClass))
#define GST_IS_TEXT_SOURCE_(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_TEXT_SOURCE))
#define GST_IS_TEXT_SOURCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_TEXT_SOURCE))

}

#endif
