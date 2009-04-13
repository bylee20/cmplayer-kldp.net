#include "textsource.h"

namespace Gst {

static GType text_source_get_type();

GstStaticPadTemplate TextSource::templ = GST_STATIC_PAD_TEMPLATE("src"
		, GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS ("text/plain; text/x-pango-markup"));

GST_BOILERPLATE(TextSource, text_source, GstBaseSrc, GST_TYPE_BASE_SRC);

GType TextSource::gtype() {return text_source_get_type();}

static void text_source_base_init(gpointer klass) {
	gst_element_class_add_pad_template(GST_ELEMENT_CLASS(klass)
			, gst_static_pad_template_get(&TextSource::templ));
}

static void text_source_class_init(TextSourceClass *klass) {
	G_OBJECT_CLASS(klass)->dispose = TextSource::dispose;
	GST_BASE_SRC_CLASS(klass)->create = TextSource::create;
	parent_class = GST_BASE_SRC_CLASS(g_type_class_peek_parent(klass));
}

static void text_source_init(TextSource *self, TextSourceClass */*klass*/) {
	self->data = new QByteArray("");
}

void TextSource::dispose(GObject *obj) {
	TextSource *self = GST_TEXT_SOURCE(obj);
	delete self->data;
	self->data = 0;
	G_OBJECT_CLASS(parent_class)->dispose(obj);
}

GstFlowReturn TextSource::create(GstBaseSrc *base
		, guint64 offset, guint length, GstBuffer **buffer) {
	TextSource *self = GST_TEXT_SOURCE(base);
	const QByteArray &data = *self->data;
	const int rest = data.size() - offset;
	if (rest <= length)
		length = rest + 1;
	GstBuffer *buf = gst_buffer_new_and_alloc(length);
	memcpy(GST_BUFFER_DATA(buf), data.data()+offset, length);
	GST_BUFFER_SIZE(buf) = length;
	GST_BUFFER_OFFSET(buf) = offset;
	GST_BUFFER_OFFSET_END(buf) = offset + length;
	*buffer = buf;
	return GST_FLOW_OK;
}

}

