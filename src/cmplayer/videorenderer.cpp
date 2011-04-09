#include "osdrenderer.hpp"
#include "fragmentprogram.hpp"
#include "events.hpp"
#include <QtGui/QMouseEvent>
#include "libvlc.hpp"
#include "pixmapoverlay.hpp"
#include "videorenderer.hpp"
#include "videoframe.hpp"
#include "logodrawer.hpp"
#include "pixelbufferoverlay.hpp"
#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QPainter>
#include <QtCore/QCoreApplication>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QSize>
#include <math.h>

#include "shader/i420_to_rgb_simple.hpp"
#include "shader/i420_to_rgb_filter.hpp"
#include "shader/i420_to_rgb_kernel.hpp"

static const int i420ToRgbSimple = 0;
static const int i420ToRgbFilter = 1;
static const int i420ToRgbKernel = 2;

struct VideoRenderer::Data {
	LogoDrawer logo;
	bool logoOn, frameIsSet, hasPrograms;
	bool binding;
	Overlay *overlay;
	GLuint texture[3];
	double crop, aspect, fps, sar;
	float brightness, contrast, sat_con, sinhue, coshue;
	ColorProperty color;
	QSize renderSize;
	QMutex mutex;
	VideoFrame *buffer, *frame, *temp;
	VideoFrame buf[3];
	VideoUtil *util;
	QRectF vtx;
	Effects effects;
	double kern_d, kern_c, kern_n;
	bool hasKernel, prepared;
	void **planes[VIDEO_FRAME_MAX_PLANE_COUNT];
	QList<FragmentProgram*> shaders;
	FragmentProgram *shader;
	double rgb_base;
	double rgb_c_r;
	double rgb_c_g;
	double rgb_c_b;
};

QGLFormat VideoRenderer::makeFormat() {
	QGLFormat format;
	return format;
}

VideoRenderer::VideoRenderer(QWidget *parent)
: QGLWidget(makeFormat(), parent), d(new Data) {
	d->rgb_base = 0.0;
	d->rgb_c_r = d->rgb_c_g = d->rgb_c_b = 1.0;
	d->kern_c = 1.0;
	d->kern_d = d->kern_n = 0.0;
	d->effects = 0;
	d->shader = 0;
	d->frame = &d->buf[0];
	d->temp = &d->buf[1];
	d->buffer = &d->buf[2];
	d->prepared = false;
	d->hasKernel = false;
	d->binding = false;
	setMinimumSize(QSize(200, 100));
	setColorProperty(d->color);
	d->frameIsSet = d->logoOn = false;
	d->fps = d->crop = d->aspect = -1.0;
	d->overlay = Overlay::create(this);
	for (int i=0; i<VIDEO_FRAME_MAX_PLANE_COUNT; ++i)
		d->planes[i] = 0;
	qDebug() << "Overlay:" << Overlay::typeToString(d->overlay->type());
	makeCurrent();
	glGenTextures(3, d->texture);
#define GET_PROC_ADDRESS(func) func = (_##func)context()->getProcAddress(QLatin1String(#func))
	GET_PROC_ADDRESS(glActiveTexture);
#undef GET_PROC_ADDRESS
	d->hasPrograms = glActiveTexture;
	QList<QByteArray> codes;
#define GET_CODE(name) codes.push_back(QByteArray((char*)name##_fp, name##_fp_len))
	GET_CODE(i420_to_rgb_simple);
	GET_CODE(i420_to_rgb_filter);
	GET_CODE(i420_to_rgb_kernel);
#undef GET_CODE
	for (int i=0; i<codes.size(); ++i) {
		FragmentProgram *fp = new FragmentProgram(codes[i]);
		Q_ASSERT(fp->isAvailable() && !fp->hasError());
		d->shaders.push_back(fp);
	}
	d->shader = d->shaders.first();
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMouseTracking(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
}

VideoRenderer::~VideoRenderer() {
	qDeleteAll(d->shaders);
	delete d->overlay;
	glDeleteTextures(3, d->texture);
	delete d;
}

static inline void setRgbFromYuv(uchar *&r, int y, int _u, int _v) {
	y -= 16;	y *= 298;
	*r++ = qBound(0, (y + 409*_v + 128) >> 8, 255);
	*r++ = qBound(0, (y - 100*_u - 208*_v + 128) >> 8, 255);
	*r++ = qBound(0, (y + 516*_u + 128) >> 8, 255);
}

QImage VideoRenderer::frameImage() const {
	if (!d->frameIsSet || !d->frame->isPlanar())
		return QImage();
	const VideoFrame frame = *d->frame;
	QImage image(frame.size(), QImage::Format_RGB888);
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
			const int _u = *u++ - 128;
			const int _v = *v++ - 128;
			setRgbFromYuv(r1, *y1++, _u, _v);
			setRgbFromYuv(r1, *y1++, _u, _v);
			setRgbFromYuv(r2, *y2++, _u, _v);
			setRgbFromYuv(r2, *y2++, _u, _v);
		}
		r1 += dr;		y0 += dy2;
		u0 += duv;		v0 += duv;
	}
	return image;
}

bool VideoRenderer::hasFrame() const {
	return d->frameIsSet;
}

void VideoRenderer::setUtil(VideoUtil *util) {
	d->util = util;
}

void VideoRenderer::setPlanes() {
	for (int i=0; i<VIDEO_FRAME_MAX_PLANE_COUNT; ++i)
		*d->planes[i] = d->buffer->data(i);
}

void *VideoRenderer::lock(void ***planes) {
	d->mutex.lock();
	d->planes[0] = planes[0];
	d->planes[1] = planes[1];
	d->planes[2] = planes[2];

	*d->planes[0] = d->buffer->data(0);
	*d->planes[1] = d->buffer->data(1);
	*d->planes[2] = d->buffer->data(2);
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
		qSwap(d->temp, d->buffer);
		QCoreApplication::postEvent(this, new NewVideoFrameEvent);
		*d->planes[0] = d->buffer->data(0);
		*d->planes[1] = d->buffer->data(1);
		*d->planes[2] = d->buffer->data(2);
	}
}

void VideoRenderer::prepare(const VideoFormat *format) {
	d->prepared = false;
	*d->buffer = VideoFrame(*format);
	*d->frame = VideoFrame(*format);
	*d->temp = VideoFrame(*format);
	QCoreApplication::postEvent(this, new VideoPrepareEvent(format));
}

double VideoRenderer::frameRate() const {
	return d->fps;
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
		const VideoFrame &frame = *d->frame;
		makeCurrent();
		uchar *data[3] = {frame.data(0), frame.data(1), frame.data(2)};
		switch (frame.type()) {
		case VideoFrame::YV12:
			qSwap(data[1], data[2]);
		case VideoFrame::I420:
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
		const VideoFormat format = static_cast<VideoPrepareEvent*>(event)->format();
		d->sar = format.sar;
		if (!qFuzzyCompare(d->fps, format.fps))
			emit frameRateChanged(d->fps = format.fps);
		if (!d->frame->isPlanar())
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
		return true;
	} else
		return QGLWidget::event(event);
}

double VideoRenderer::targetAspectRatio() const {
	if (d->aspect > 0.0)
		return d->aspect;
	if (d->aspect == 0.0)
		return widgetRatio();
	return (double)d->frame->width()*d->sar/(double)d->frame->height();
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
	size.scale(d->frame->size(), Qt::KeepAspectRatioByExpanding);
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
	d->brightness = qBound(-1.0, d->color.brightness(), 1.0);
	d->contrast = qBound(0., d->color.contrast() + 1., 2.);
	if (d->effects & Grayscale)
		d->sat_con = 0.0;
	else
		d->sat_con = qBound(0., d->color.saturation() + 1., 2.)*d->contrast;
	const double hue = qBound(-M_PI, d->color.hue()*M_PI, M_PI);
	d->sinhue = sin(hue);
	d->coshue = cos(hue);
}

const ColorProperty &VideoRenderer::colorProperty() const {
	return d->color;
}

void VideoRenderer::setFixedRenderSize(const QSize &size) {
	if (d->renderSize != size) {
		d->renderSize = size;
		if (this->size() != size)
			updateSize();
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
	if (!d->logoOn && d->hasPrograms && d->frameIsSet) {
		const double aspect = targetAspectRatio();
		const QSizeF widget = renderableSize();
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
		const double dx = 1.0/(double)d->frame->dataPitch(0);
		const double dy = 1.0/(double)d->frame->dataLines(0);
		painter.beginNativePainting();
		makeCurrent();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		d->shader->bind();
		d->shader->setLocalParam(0, d->contrast, d->sat_con, d->coshue, d->sinhue);
		d->shader->setLocalParam(1, d->rgb_c_r, d->rgb_c_g, d->rgb_c_b, d->rgb_base);
		d->shader->setLocalParam(2, dx, dy, -dx, 0.0);
		d->shader->setLocalParam(3, d->kern_c, d->kern_n, d->kern_d, 0.0f);

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
		d->logo.draw(&painter, rect());
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
		pos.rx() *= (double)d->frame->width()/d->vtx.width();
		pos.ry() *= (double)d->frame->height()/d->vtx.height();
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
	update();
}

void VideoRenderer::setEffect(Effect effect, bool on) {
	Effects effects = d->effects;
	if (on)
		effects |= effect;
	else
		effects &= ~effect;
	setEffects(effects);
}

void VideoRenderer::setEffects(Effects effects) {
	if (d->effects == effects)
		return;
	int idx = i420ToRgbSimple;
	d->effects = effects;
	d->rgb_base = 0.0;
	d->rgb_c_r = d->rgb_c_g = d->rgb_c_b = 1.0;
	if (d->effects & InvertColor) {
		idx = i420ToRgbFilter;
		d->rgb_base = 1.0;
		d->rgb_c_r = d->rgb_c_g = d->rgb_c_b = -1.0;
	}
	d->hasKernel = (d->effects & Blur) || (d->effects & Sharpen);
	if (d->hasKernel) {
		idx = i420ToRgbKernel;
		d->kern_c = d->kern_d = d->kern_n = 0.0;
		if (d->effects & Blur) {
			d->kern_c += 1.0; // center
			d->kern_n += 2.0; // neighbor
			d->kern_d += 1.0; // diagonal
		}
		if (d->effects & Sharpen) {
			d->kern_c += 5.0;
			d->kern_n += -1.0;
			d->kern_d += 0.0;
		}
		const double den = 1.0/(d->kern_c + d->kern_n*4.0 + d->kern_d*4.0);
		d->kern_c *= den;
		d->kern_d *= den;
		d->kern_n *= den;
	}
	d->shader = d->shaders.value(idx, d->shaders.first());
	if (d->effects & Grayscale)
		d->sat_con = 0.0;
	else
		d->sat_con = qBound(0., d->color.saturation() + 1., 2.)*d->contrast;
	update();
}
