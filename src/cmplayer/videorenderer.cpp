#include "fragmentprogram.hpp"
#include <QtCore/QTime>
#include "overlay.hpp"
#include "colorproperty.hpp"
#include "events.hpp"
#include "libvlc.hpp"
#include "videorenderer.hpp"
#include "videoframe.hpp"
#include "pref.hpp"
#include "logodrawer.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QSize>
#include <QtCore/QRegExp>
#include <QtCore/QCoreApplication>
#include <math.h>

#include "shader/i420_to_rgb_simple.hpp"
#include "shader/i420_to_rgb_filter.hpp"
#include "shader/i420_to_rgb_kernel.hpp"

#define CLIP(val) (qBound(0, (int)((val)+0.5), 255))

struct VideoRenderer::Data {
	static const int i420ToRgbSimple = 0;
	static const int i420ToRgbFilter = 1;
	static const int i420ToRgbKernel = 2;
	QMutex mutex;		QSize renderSize;	ColorProperty color;
	LogoDrawer logo;	Overlay *overlay;	GLuint texture[3];
	VideoFrame *buffer, *frame, *temp, buf[3];	VideoUtil *util;
	QRectF vtx;		Effects effects;
	void **planes[VIDEO_FRAME_MAX_PLANE_COUNT];	VideoFormat format;
	QList<FragmentProgram*> shaders;		FragmentProgram *shader;
	double rgb_base, rgb_c_r, rgb_c_g, rgb_c_b;
	double crop, aspect, kern_d, kern_c, kern_n;
	double y_min, y_max; double y_min_buffer, y_max_buffer;
	double brightness, contrast, sat_con, sinhue, coshue;
	bool hasKernel, prepared, logoOn, frameIsSet, hasPrograms, binding;
// methods
	inline void update_sat_con() {
		sat_con = (effects & Grayscale) ? 0.0
			: qBound(0.0, color.saturation() + 1.0, 2.0)*contrast;
	}
	inline void update_color_prop() {
		color.clamp();
		brightness = qBound(-1.0, color.brightness(), 1.0);
		contrast = qBound(0., color.contrast() + 1., 2.);
		update_sat_con();
		const double hue = qBound(-M_PI, color.hue()*M_PI, M_PI);
		sinhue = sin(hue);	coshue = cos(hue);
	}
	inline void update_planes() {
		*planes[0] = buffer->data(0);
		*planes[1] = buffer->data(1);
		*planes[2] = buffer->data(2);
	}
	void update_effects() {
		int idx = i420ToRgbSimple;
		rgb_base = 0.0;
		rgb_c_r = rgb_c_g = rgb_c_b = 1.0;
		kern_c = kern_d = kern_n = 0.0;
		if (effects & FilterEffects) {
			idx = i420ToRgbFilter;
			if (effects & InvertColor) {
				rgb_base = 1.0;
				rgb_c_r = rgb_c_g = rgb_c_b = -1.0;
			}
		}
		if ((hasKernel = effects & KernelEffects)) {
			idx = i420ToRgbKernel;
			const Pref &p = Pref::get();
			if (effects & Blur) {
				kern_c += p.blur_kern_c;
				kern_n += p.blur_kern_n;
				kern_d += p.blur_kern_d;
			}
			if (effects & Sharpen) {
				kern_c += p.sharpen_kern_c;
				kern_n += p.sharpen_kern_n;
				kern_d += p.sharpen_kern_d;
			}
			const double den = 1.0/(kern_c + kern_n*4.0 + kern_d*4.0);
			kern_c *= den;
			kern_d *= den;
			kern_n *= den;
		}
		shader = shaders.value(idx, shaders.first());
		update_sat_con();
	}
	void init_program() {
#define GET_CODE(name) (QByteArray((char*)name##_fp, name##_fp_len))
		shaders.push_back(new FragmentProgram(GET_CODE(i420_to_rgb_simple)));
		shaders.push_back(new FragmentProgram(GET_CODE(i420_to_rgb_filter)));
		shaders.push_back(new FragmentProgram(GET_CODE(i420_to_rgb_kernel)));
#undef GET_CODE
		hasPrograms = (shader = shaders.value(0, 0));
		Q_ASSERT(hasPrograms);
	}
};

QGLFormat VideoRenderer::makeFormat() {
	QGLFormat format;
	return format;
}

VideoRenderer::VideoRenderer(QWidget *parent)
: QGLWidget(makeFormat(), parent), d(new Data) {
	makeCurrent();
	glGenTextures(3, d->texture);
#define GET_PROC_ADDRESS(func) func = (_##func)context()->getProcAddress(QLatin1String(#func))
	GET_PROC_ADDRESS(glActiveTexture);
	Q_ASSERT(glActiveTexture != 0);
#undef GET_PROC_ADDRESS
	d->init_program();
	doneCurrent();

	d->update_color_prop();
	d->effects = 0;	d->update_effects();
	d->planes[0] = d->planes[1] = d->planes[2] = 0;
	d->y_max = 1.0; d->y_min = 0.0;
	d->format.fps = d->crop = d->aspect = -1.0;
	d->format.sar = 1.0;
	d->frame = &d->buf[0];	d->temp = &d->buf[1];	d->buffer = &d->buf[2];
	d->frameIsSet = d->logoOn = d->binding = d->hasKernel = d->prepared = false;
	d->overlay = Overlay::create(this);
	qDebug() << "Overlay:" << Overlay::typeToString(d->overlay->type());

	setMinimumSize(QSize(200, 100));
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMouseTracking(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
}

VideoRenderer::~VideoRenderer() {
	makeCurrent();
	qDeleteAll(d->shaders);
	delete d->overlay;
	glDeleteTextures(3, d->texture);
	delete d;
}

static inline void setRgbFromYuv(uchar *&r, int y, int u, int v) {
	y -= 16;	y *= 298;
	u -= 128;	v -= 128;
	*r++ = qBound(0, (y + 409*v + 128) >> 8, 255);
	*r++ = qBound(0, (y - 100*u - 208*v + 128) >> 8, 255);
	*r++ = qBound(0, (y + 516*u + 128) >> 8, 255);
}

QImage VideoRenderer::frameImage() const {
	if (!d->frameIsSet || !VideoFormat::isPlanar(d->format.output))
		return QImage();
	VideoFrame frame = *d->frame;
	QImage image(frame.format().size(), QImage::Format_RGB888);
	const int dy = frame.dataPitch(0);
	const int dy2 = dy << 1;
	const int dr = image.bytesPerLine();
	const int duv = frame.dataPitch(1);
	const uchar *y0 = frame.data(0);
	const uchar *u0 = frame.data(1);
	const uchar *v0 = frame.data(2);
	uchar *r1 = image.bits();
	for (int j = 0; j < image.height()/2; ++j) {
		const uchar *u = u0;	const uchar *v = v0;
		const uchar *y1 = y0;	const uchar *y2 = y1 + dy;
		uchar *r2 = r1 + dr;
		for (int i = 0; i < image.width()/2; ++i) {
			const int _u = *u++;
			const int _v = *v++;
			setRgbFromYuv(r1, *y1++, _u, _v);
			setRgbFromYuv(r1, *y1++, _u, _v);
			setRgbFromYuv(r2, *y2++, _u, _v);
			setRgbFromYuv(r2, *y2++, _u, _v);
		}
		r1 += dr;		y0 += dy2;
		u0 += duv;		v0 += duv;
	}
	return (image);
}

bool VideoRenderer::hasFrame() const {
	return d->frameIsSet;
}

void VideoRenderer::setUtil(VideoUtil *util) {
	d->util = util;
}

void VideoRenderer::render(void **planes) {
	Q_ASSERT(planes[0] == d->buffer->data());
	int min = 0;
	int max = 255;
	if (d->effects & RemapLuma) {
		min = Pref::get().adjust_contrast_min_luma;
		max = Pref::get().adjust_contrast_max_luma;
	}
	if (d->effects & AutoContrast) {
		int his_y[256] = {0};
		uchar *y1, *y2;
		y1 = (uchar*)planes[0];
		y2 = y1 + d->format.planes[0].data_pitch;
		const int lines = d->format.planes[1].frame_lines;
		const int pitch = d->format.planes[1].frame_pitch;
		const int dy = (d->format.planes[0].data_pitch << 1) - d->format.planes[0].frame_pitch;
		for (int j=0; j<lines; ++j) {
			for (int i=0; i<pitch; ++i) {
				++his_y[*y1++];		++his_y[*y1++];
				++his_y[*y2++];		++his_y[*y2++];
			}
			y1 += dy;	y2 += dy;
		}
		const double th = Pref::get().auto_contrast_threshold/100.0;
		int acc = 0;
		const double r_count = 1.0/(double)((lines*pitch) << 2);
		for (int i=min; i<256; ++i) {
			acc += his_y[i];
			if ((double)acc*r_count > th)
				break;
			min = i;
		}
		acc = 0;
		for (int i=max; i>min; --i) {
			acc += his_y[i];
			if ((double)acc*r_count > th)
				break;
			max = i;
		}
	}
	d->y_min_buffer = (double)min/255.0;
	d->y_max_buffer = (double)max/255.0;
}

void VideoRenderer::process(void **planes) {
	Q_UNUSED(planes);
	return;
//	if (d->frame->isEmpty() || !VideoFormat::isPlanar(d->frame->format().source))
//		return;
////	d->y_max = 1.0;
////	d->y_min = 0.0;
//	if (d->filters & AutoContrast) {
//		int his_y[256] = {0};
//		uchar *y1, *y2;
//		y1 = (uchar*)planes[0];
//		y2 = y1 + d->frame->dataPitch(0);
//		QTime t;
//		t.start();
//		const int lines = d->frame->frameLines(1);
//		const int pitch = d->frame->framePitch(1);
//		const int dy = (d->frame->dataPitch(0) << 1) - d->frame->framePitch(0);
//		for (int j=0; j<lines; ++j) {
//			for (int i=0; i<pitch; ++i) {
//				++his_y[*y1++];		++his_y[*y1++];
//				++his_y[*y2++];		++his_y[*y2++];
//			}
//			y1 += dy;	y2 += dy;
//		}
//		const int t1 = t.restart();
//		const double threshold = 0.5/100.0;
//		int min = 0;
//		int acc = 0;
//		const double count = d->frame->frameLines(0)*d->frame->framePitch(0);
//		for (int i=min; i<256; ++i) {
//			acc += his_y[i];
//			if ((double)acc/count > threshold)
//				break;
//			min = i;
//		}
//		int max = 255;
//		acc = 0;
//		for (int i=max; i>min; --i) {
//			acc += his_y[i];
//			if ((double)acc/count > threshold)
//				break;
//			max = i;
//		}
//		y1 = (uchar*)planes[0];
//		y2 = y1 + d->frame->dataPitch(0);
//		const double r = 255.0/(double)(max-min);
//		uchar lut[256];
//		for (int i=0; i<256; ++i)
//			lut[i] = CLIP((double)(i-min)*r);
//		const int t2 = t.restart();
//		for (int j=0; j<lines; ++j) {
//			for (int i=0; i<pitch; ++i) {
//				*y1 = lut[*y1];	++y1;
//				*y1 = lut[*y1];	++y1;
//				*y2 = lut[*y2];	++y2;
//				*y2 = lut[*y2]; ++y2;
//			}
//			y1 += dy;	y2 += dy;
//		}
//		const int t3 = t.restart();
//		qDebug() << t1 << t2 << t3;
//	}
//	if (d->filters & AutoColor) {

//	}
}

void *VideoRenderer::lock(void ***planes) {
	d->mutex.lock();
	d->planes[0] = planes[0];
	d->planes[1] = planes[1];
	d->planes[2] = planes[2];
	d->update_planes();
	return 0;
}

void VideoRenderer::unlock(void *id, void *const *plane) {
	Q_UNUSED(id);
	Q_ASSERT(d->buffer->data() == plane[0]);
	d->mutex.unlock();
}

void VideoRenderer::display(void *id) {
	Q_UNUSED(id);
	if (!d->prepared || d->binding) {
		qDebug() << "drop a frame!";
	} else {
		Q_ASSERT(d->temp->length() == d->buffer->length());
//		d->y_min = d->y_min_buffer;
//		d->y_max = d->y_max_buffer;
		qSwap(d->temp, d->buffer);
		QEvent *event = new NewVideoFrameEvent(d->y_min_buffer, d->y_max_buffer);
		QCoreApplication::postEvent(this, event);
		d->update_planes();
	}
}

void VideoRenderer::prepare(const VideoFormat *format) {
	d->prepared = false;
	d->format = *format;
	*d->buffer = VideoFrame(d->format);
	*d->frame = VideoFrame(d->format);
	*d->temp = VideoFrame(d->format);
	QCoreApplication::postEvent(this, new VideoPrepareEvent);
}

double VideoRenderer::frameRate() const {
	return d->format.fps;
}

void VideoRenderer::addOsd(OsdRenderer *osd) {
	d->overlay->addOsd(osd);
}

bool VideoRenderer::event(QEvent *event) {
	if (event->type() == (int)Event::NewVideoFrame) {
		if (!d->prepared)
			return true;
		if (!(d->frameIsSet = !d->frame->isEmpty()))
			return true;
		d->binding = true;
		qSwap(d->frame, d->temp);
		NewVideoFrameEvent *e = static_cast<NewVideoFrameEvent*>(event);
		d->y_min = e->y_min;
		d->y_max = e->y_max;
		const VideoFrame &frame = *d->frame;
		makeCurrent();
		uchar *data[3] = {frame.data(0), frame.data(1), frame.data(2)};
		switch (frame.format().output) {
		case VideoFormat::YV12:
			qSwap(data[1], data[2]);
		case VideoFormat::I420:
	#define BIND_TEXTURE_I420(idx)\
			glBindTexture(GL_TEXTURE_2D, d->texture[idx]);\
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0\
				, frame.dataPitch(idx), frame.dataLines(idx)\
				, GL_LUMINANCE, GL_UNSIGNED_BYTE, data[idx]);
			BIND_TEXTURE_I420(0);
			BIND_TEXTURE_I420(1);
			BIND_TEXTURE_I420(2);
	#undef BIND_TEXTURE_I420
			break;
		default:
			d->frameIsSet = false;
			break;
		}
		d->binding = false;
		update();
		return true;
	} else if (event->type() == (int)Event::VideoPrepare) {
		d->prepared = false;
		d->y_min = 0.0;
		d->y_max = 1.0;
		emit frameRateChanged(d->format.fps);
		if (!VideoFormat::isPlanar(d->format.output))
			return true;
		makeCurrent();
		for (int i=0; i<3; ++i) {
			glBindTexture(GL_TEXTURE_2D, d->texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE
				, d->frame->dataPitch(i), d->frame->dataLines(i), 0
				, GL_LUMINANCE, GL_UNSIGNED_BYTE, d->frame->data(i));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		d->prepared = true;
		qDebug() << "source" << VideoFrame::fourccToString(d->format.source)
			 << "output" << VideoFrame::fourccToString(d->format.output);
		return true;
	} else
		return QGLWidget::event(event);
}

double VideoRenderer::targetAspectRatio() const {
	if (d->aspect > 0.0)
		return d->aspect;
	if (d->aspect == 0.0)
		return widgetRatio();
	return (double)d->format.width*d->format.sar/(double)d->format.height;
}

double VideoRenderer::targetCropRatio(double fallback) const {
	if (d->crop > 0.0)
		return d->crop;
	if (d->crop == 0.0)
		return widgetRatio();
	return fallback;
}

QSize VideoRenderer::sizeHint() const {
	if (d->frame->isEmpty())
		return QSize(400, 300);
	const double aspect = targetAspectRatio();
	QSizeF size(aspect, 1.0);
	size.scale(d->frame->format().size(), Qt::KeepAspectRatioByExpanding);
	QSizeF crop(targetCropRatio(aspect), 1.0);
	crop.scale(size, Qt::KeepAspectRatio);
	return crop.toSize();
}

static bool isSameRatio(double r1, double r2) {
	return (r1 < 0.0 && r2 < 0.0) || qFuzzyCompare(r1, r2);
}

void VideoRenderer::setAspectRatio(double ratio) {
	if (!isSameRatio(d->aspect, ratio)) {
		d->aspect = ratio;
		update();
	}
}

double VideoRenderer::aspectRatio() const {
	return d->aspect;
}

void VideoRenderer::setCropRatio(double ratio) {
	if (!isSameRatio(d->crop, ratio)) {
		d->crop = ratio;
		update();
	}
}

double VideoRenderer::cropRatio() const {
	return d->crop;
}

void VideoRenderer::setLogoMode(bool on) {
	d->logoOn = on;
}

void VideoRenderer::setColorProperty(const ColorProperty &prop) {
	d->color = prop;
	d->update_color_prop();
	update();
}

const ColorProperty &VideoRenderer::colorProperty() const {
	return d->color;
}

void VideoRenderer::setFixedRenderSize(const QSize &size) {
	if (d->renderSize != size) {
		d->renderSize = size;
		if (this->size() != size)
			updateSize();
		update();
	}
}

QSize VideoRenderer::renderableSize() const {
	return d->renderSize.isEmpty() ? size() : d->renderSize;
}

void VideoRenderer::updateSize() {
	const QSize size = renderableSize();
	d->overlay->setArea(QRect(QPoint(0, 0), size));
}

void VideoRenderer::resizeEvent(QResizeEvent *event) {
	QGLWidget::resizeEvent(event);
	updateSize();
}

void VideoRenderer::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);
	const QSizeF widget = renderableSize();
	if (!d->logoOn && d->hasPrograms && d->frameIsSet && d->prepared) {
		const double aspect = targetAspectRatio();
		QSizeF frame(aspect, 1.0);
		QSizeF letter(targetCropRatio(aspect), 1.0);
		letter.scale(widget, Qt::KeepAspectRatio);
		frame.scale(letter, Qt::KeepAspectRatioByExpanding);

		const double x = (widget.width() - frame.width())*0.5;
		const double y = (widget.height() - frame.height())*0.5;
		const double hMargin = (widget.width() - letter.width())*0.5;
		const double vMargin = (widget.height() - letter.height())*0.5;
		const QRectF vtx(x, y, frame.width(), frame.height());
		d->vtx = vtx;
		double top = 0.0, left = 0.0;
		double bottom = (double)(d->frame->frameLines(0)-1)/(double)d->frame->dataLines(0);
		double right = (double)(d->frame->framePitch(0)-1)/(double)d->frame->dataPitch(0);
		if (d->effects & FlipHorizontally)
			qSwap(left, right);
		if (d->effects & FlipVertically)
			qSwap(top, bottom);
		painter.beginNativePainting();
		makeCurrent();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		d->shader->bind();
		d->shader->setLocalParam(0, d->contrast, d->sat_con, d->coshue, d->sinhue);
		if (d->shader != d->shaders.first()) {
			d->shader->setLocalParam(1, d->rgb_c_r, d->rgb_c_g, d->rgb_c_b, d->rgb_base);
			d->shader->setLocalParam(2, d->y_min, 1.0/(d->y_max - d->y_min));
			if (d->hasKernel) {
				const double dx = 1.0/(double)d->frame->dataPitch(0);
				const double dy = 1.0/(double)d->frame->dataLines(0);
				d->shader->setLocalParam(3, dx, dy, -dx, 0.0f); // 4th value must be zero
				d->shader->setLocalParam(4, d->kern_c, d->kern_n, d->kern_d, 0.0f);
			}
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, d->texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, d->texture[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, d->texture[2]);
		glActiveTexture(GL_TEXTURE0);

		const float textureCoords[] = {
			left,	top,	d->brightness,
			right,	top,	d->brightness,
			right,	bottom,	d->brightness,
			left,	bottom, d->brightness
		};
		const float vertexCoords[] = {
			vtx.left(),	vtx.top(),
			vtx.right(),	vtx.top(),
			vtx.right(),	vtx.bottom(),
			vtx.left(),	vtx.bottom()
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, textureCoords);
		glVertexPointer(2, GL_FLOAT, 0, vertexCoords);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		d->shader->release();

		painter.endNativePainting();

		if (vMargin >= 0.0) {
			QRectF rect(0.0, 0.0, widget.width(), vMargin);
			painter.fillRect(rect, Qt::black);
			rect.moveTop(widget.height() - vMargin);
			painter.fillRect(rect, Qt::black);
		}
		if (hMargin >= 0.0) {
			QRectF rect(0.0, 0.0, hMargin, widget.height());
			painter.fillRect(rect, Qt::black);
			rect.moveLeft(widget.width() - hMargin);
			painter.fillRect(rect, Qt::black);
		}
	} else
		d->logo.draw(&painter, QRectF(QPointF(0, 0), widget));
	d->overlay->render(&painter);
}

int VideoRenderer::translateButton(Qt::MouseButton qbutton) {
	switch (qbutton) {
	case Qt::LeftButton:
		return 0;
	case Qt::MidButton:
		return 1;
	case Qt::RightButton:
		return 2;
	default:
		return -1;
	}
}

void VideoRenderer::mouseMoveEvent(QMouseEvent *event) {
	QGLWidget::mouseMoveEvent(event);
	if (d->util->vd && d->vtx.isValid() && !d->frame->isEmpty()) {
		QPointF pos = event->posF();
		pos -= d->vtx.topLeft();
		pos.rx() *= (double)d->format.width/d->vtx.width();
		pos.ry() *= (double)d->format.height/d->vtx.height();
		d->util->mouseMoveEvent(d->util->vd, pos.x() + 0.5, pos.y() + 0.5);
	}
}

void VideoRenderer::mousePressEvent(QMouseEvent *event) {
	QGLWidget::mousePressEvent(event);
	if (d->util->vd) {
		const int b = translateButton(event->button());
		if (b >= 0)
			d->util->mousePresseEvent(d->util->vd, b);
	}
}

void VideoRenderer::mouseReleaseEvent(QMouseEvent *event) {
	QGLWidget::mouseReleaseEvent(event);
	if (d->util->vd) {
		const int b = translateButton(event->button());
		if (b >= 0)
			d->util->mouseReleaseEvent(d->util->vd, b);
	}
}

void VideoRenderer::clearEffects() {
	d->effects = 0;
	d->update_effects();
	update();
}

void VideoRenderer::setEffect(Effect effect, bool on) {
	setEffects(on ? (d->effects | effect) : (d->effects & ~effect));
}

void VideoRenderer::setEffects(Effects effects) {
	if (d->effects != effects) {
		d->effects = effects;
		d->update_effects();
		update();
	}
}
