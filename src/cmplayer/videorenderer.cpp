#include "osdrenderer.hpp"
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

const char *VideoRenderer::i420ToRgb =
"!!ARBfp1.0"
"PARAM param0 = program.local[0];"
"PARAM param1 = program.local[1];"
"PARAM coef_r = {1.164,  0.0,       1.1596,     0.0625};"
"PARAM coef_g = {1.164, -0.391,    -0.81300002, 0.5};"
"PARAM coef_b = {1.164,  2.0179999, 0.0,        0.5};"
"TEMP yuv;"
"TEX yuv.x, fragment.texcoord[0], texture[1], 2D;"
"SUB yuv.z, yuv.x, coef_g.a;"
"TEX yuv.x, fragment.texcoord[0], texture[2], 2D;"
"SUB yuv.w, yuv.x, coef_b.a;"
"MUL yuv.x, yuv.z, param1.x;"
"MUL yuv.y, yuv.w, param1.y;"
"ADD yuv.y, yuv.x, yuv.y;"
"MUL yuv.x, yuv.z, param1.y;"
"MUL yuv.w, yuv.w, param1.x;"
"SUB yuv.z, yuv.w, yuv.x;"
"TEX yuv.x, fragment.texcoord[0], texture[0], 2D;"
"SUB yuv.x, yuv, coef_r.a;"
"MUL yuv, yuv, param0;"
"ADD yuv.x, yuv, param0.w;"
"DP3 result.color.r, yuv, coef_r;"
"DP3 result.color.g, yuv, coef_g;"
"DP3 result.color.b, yuv, coef_b;"
"MOV result.color.a, 1.0;"
"END";

const char *VideoRenderer::yuy2ToRgb =
"!!ARBfp1.0"
"PARAM param0 = program.local[0];"
"PARAM param1 = program.local[1];"
"PARAM coef_r = {1.164,  0.0,       1.1596,     0.0625};"
"PARAM coef_g = {1.164, -0.391,    -0.81300002, 0.5};"
"PARAM coef_b = {1.164,  2.0179999, 0.0,        0.5};"
"TEMP yuv;"
"TEX yuv, fragment.texcoord[0], texture[1], 2D;"
"SUB yuv.z, yuv.y, coef_g.a;"
"SUB yuv.w, yuv.w, coef_b.a;"
"MUL yuv.x, yuv.z, param1.x;"
"MUL yuv.y, yuv.w, param1.y;"
"ADD yuv.y, yuv.x, yuv.y;"
"MUL yuv.x, yuv.z, param1.y;"
"MUL yuv.w, yuv.w, param1.x;"
"SUB yuv.z, yuv.w, yuv.x;"
"TEX yuv.x, fragment.texcoord[0], texture[0], 2D;"
"SUB yuv.x, yuv, coef_r.a;"
"MUL yuv, yuv, param0;"
"ADD yuv.x, yuv, param0.w;"
"DP3 result.color.r, yuv, coef_r;"
"DP3 result.color.g, yuv, coef_g;"
"DP3 result.color.b, yuv, coef_b;"
"MOV result.color.a, 1.0;"
"END";

struct VideoRenderer::Data {
	LogoDrawer logo;
	bool logoOn, frameIsSet, hasPrograms;
	bool locked;
	Overlay *overlay;
	GLuint texture[3];
	double crop, aspect, fps, sar;
	float brightness, contrast, sat_con, sinhue, coshue;
	ColorProperty color;
	QSize renderSize;
	QMutex mutex;
	VideoFrame buffer, frame;
	GLuint shader[2];
	VideoUtil *util;
	QRectF vtx;
};

QGLFormat VideoRenderer::makeFormat() {
	QGLFormat format;
	return format;
}

VideoRenderer::VideoRenderer(QWidget *parent)
: QGLWidget(makeFormat(), parent), d(new Data) {
	d->locked = false;
	setMinimumSize(QSize(200, 100));
	setColorProperty(d->color);
	d->frameIsSet = d->logoOn = false;
	d->fps = d->crop = d->aspect = -1.0;
	d->overlay = Overlay::create(this);
	qDebug() << "Overlay:" << Overlay::typeToString(d->overlay->type());

	makeCurrent();
	glGenTextures(3, d->texture);
#define GET_PROC_ADDRESS(func) func = (_##func)context()->getProcAddress(QLatin1String(#func))
	GET_PROC_ADDRESS(glProgramStringARB);
	GET_PROC_ADDRESS(glBindProgramARB);
	GET_PROC_ADDRESS(glDeleteProgramsARB);
	GET_PROC_ADDRESS(glGenProgramsARB);
	GET_PROC_ADDRESS(glProgramLocalParameter4fARB);
	GET_PROC_ADDRESS(glActiveTexture);
#undef GET_PROC_ADDRESS
	d->hasPrograms = glActiveTexture != 0;
	d->hasPrograms = glProgramStringARB && glBindProgramARB && glDeleteProgramsARB;
	d->hasPrograms = d->hasPrograms && glGenProgramsARB;
	d->hasPrograms = d->hasPrograms && glProgramLocalParameter4fARB;
	if (d->hasPrograms) {
		glGenProgramsARB(2, d->shader);
		const char *codes[] = {i420ToRgb, yuy2ToRgb};
		bool error = false;
		for(int i = 0; i < 2 && !error;  ++i) {
			glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->shader[i]);
			const GLbyte *src = reinterpret_cast<const GLbyte *>(codes[i]);
			glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB
					, strlen(codes[i]), src);
			if (glGetError() != GL_NO_ERROR)
				error = true;
		}
		if (error) {
			glDeleteProgramsARB(2, d->shader);
			d->hasPrograms = false;
			qDebug() << "Shader compilation error!";
		}
	}
	if (!d->hasPrograms)
		qDebug() << "Shader program is not available!";

	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMouseTracking(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
}

VideoRenderer::~VideoRenderer() {
	qDebug() << "locked?" << d->locked;
	delete d->overlay;
	glDeleteTextures(3, d->texture);
	if (d->hasPrograms)
		glDeleteProgramsARB(2, d->shader);
	delete d;
}

void VideoRenderer::setUtil(VideoUtil *util) {
	d->util = util;
}

void *VideoRenderer::lock(void **planes) {
	d->mutex.lock();
	d->locked = true;
	for (int i=0; i<d->buffer.planeCount(); ++i)
		planes[i] = d->buffer.data(i);
	return 0;
}

void VideoRenderer::unlock(void *id, void *const *plane) {
	Q_UNUSED(id);
	Q_ASSERT(d->buffer.data() == plane[0]);
	d->mutex.unlock();
	d->locked = false;
}

void VideoRenderer::display(void *id) {
	Q_UNUSED(id);
	VideoFrameEvent *event = new VideoFrameEvent(d->buffer);
	QCoreApplication::postEvent(this, event);
}

void VideoRenderer::prepare(const VideoFormat *format) {
	d->buffer = VideoFrame(*format);
	d->frame = d->buffer;
	d->sar = format->sar;
	if (!qFuzzyCompare(d->fps, format->fps))
		emit frameRateChanged(d->fps = format->fps);
	const VideoFrame &f = d->buffer;
	makeCurrent();
	switch (f.type()) {
	case VideoFrame::I420:
	case VideoFrame::YV12:
		for (int i=0; i<3; ++i) {
			glBindTexture(GL_TEXTURE_2D, d->texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE
				, f.dataPitch(i), f.dataLines(i), 0
				, GL_LUMINANCE, GL_UNSIGNED_BYTE, f.data(i));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		break;
	case VideoFrame::YUY2: {
		glBindTexture(GL_TEXTURE_2D, d->texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA
			, f.dataPitch()/2, f.dataLines(), 0
			, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, f.data());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, d->texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA
			, f.dataPitch()/4, f.dataLines(), 0
			, GL_RGBA, GL_UNSIGNED_BYTE, f.data());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	} default:
		break;
	}
}

double VideoRenderer::frameRate() const {
	return d->fps;
}

void VideoRenderer::addOsd(OsdRenderer *osd) {
	d->overlay->addOsd(osd);
}

void VideoRenderer::customEvent(QEvent *event) {
	if (event->type() != VideoFrameEvent::Id)
		return;
	VideoFrameEvent *e = static_cast<VideoFrameEvent*>(event);
	Q_ASSERT(d->frame.length() == e->length());
	d->frame.setData(e->data());
	d->frameIsSet = !d->frame.isEmpty();
	if (d->frame.isEmpty())
		return;
	makeCurrent();
	uchar *data[3] = {d->frame.data(0), d->frame.data(1), d->frame.data(2)};
	const VideoFrame &f = d->frame;
	switch (d->frame.type()) {
	case VideoFrame::YV12:
		qSwap(data[1], data[2]);
	case VideoFrame::I420:
#define BIND_TEXTURE_I420(idx)\
		glBindTexture(GL_TEXTURE_2D, d->texture[idx]);\
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0\
			, f.dataPitch(idx), f.dataLines(idx)\
			, GL_LUMINANCE, GL_UNSIGNED_BYTE, data[idx]);
		BIND_TEXTURE_I420(0);
		BIND_TEXTURE_I420(1);
		BIND_TEXTURE_I420(2);
#undef BIND_TEXTURE_I420
		break;
	case VideoFrame::YUY2:
		glBindTexture(GL_TEXTURE_2D, d->texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA
			, f.dataPitch()/2, f.dataLines(), 0
			, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, f.data());
		glBindTexture(GL_TEXTURE_2D, d->texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA
			, f.dataPitch()/4, f.dataLines(), 0
			, GL_RGBA, GL_UNSIGNED_BYTE, f.data());
		break;
	default:
		d->frameIsSet = false;
		break;
	}
	update();
}

QSize VideoRenderer::sizeHint() const {
	if (d->frame.isEmpty())
		return QSize(400, 300);
	const QSizeF frame = d->frame.size();
	QSizeF size(d->aspect, 1.0);
	if (d->aspect > 0.0)
		size.scale(frame, Qt::KeepAspectRatio);
	else {
		size = frame;
		size.rwidth() *= d->sar;
	}
	if (d->crop <= 0.0)
		return size.toSize();
	QSizeF crop(d->crop, 1.0);
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
	const double sat = qBound(0., d->color.saturation() + 1., 2.);
	d->sat_con = sat*d->contrast;
	const double hue = qBound(-M_PI, d->color.hue()*M_PI, M_PI);
	d->sinhue = sin(hue);
	d->coshue = cos(hue);
}

const ColorProperty &VideoRenderer::colorProperty() const {
	return d->color;
}

void VideoRenderer::setFixedRenderSize(const QSize &size) {
	d->renderSize = size;
	updateSize();
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
		QSizeF frame(d->aspect, 1.0);
		if (d->aspect < 0.0) {
			frame = d->frame.size();
			frame.rwidth() *= d->sar;
		} else if (d->aspect == 0.0)
			frame = size();
		const QSizeF widget = renderableSize();
		QSizeF letter(d->crop, 1.0);
		if (d->crop < 0.0)
			letter = frame;
		else if (d->crop == 0.0)
			letter = size();
		letter.scale(widget, Qt::KeepAspectRatio);
		frame.scale(letter, Qt::KeepAspectRatioByExpanding);

		const double x = (widget.width() - frame.width())*0.5;
		const double y = (widget.height() - frame.height())*0.5;
		const double hMargin = (widget.width() - letter.width())*0.5;
		const double vMargin = (widget.height() - letter.height())*0.5;
		const QRectF vtx(x, y, frame.width(), frame.height());
		d->vtx = vtx;
		const double txtRight = (double)(d->frame.framePitch(0)-1)/(double)d->frame.dataPitch(0);
		const double txtBottom = (double)(d->frame.frameLines(0)-1)/(double)d->frame.dataLines(0);
		const QRectF txt(0.0, 0.0, txtRight, txtBottom);
		const float textureCoords[] = {
			txt.left(),	txt.top(),
			txt.right(),	txt.top(),
			txt.right(),	txt.bottom(),
			txt.left(),	txt.bottom()
		};
		const float vertexCoords[] = {
			vtx.left(),	vtx.top(),
			vtx.right(),	vtx.top(),
			vtx.right(),	vtx.bottom(),
			vtx.left(),	vtx.bottom()
		};
		painter.beginNativePainting();
		makeCurrent();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		const bool planar = d->frame.type() == VideoFrame::YV12
			|| d->frame.type() == VideoFrame::I420;
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->shader[!planar]);
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0.0f
			, d->contrast, d->sat_con, d->sat_con, d->brightness);
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1.0f
			, d->coshue, d->sinhue, 1.0, 0.0);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, d->texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, d->texture[1]);
		if (planar) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, d->texture[2]);
		}
		glActiveTexture(GL_TEXTURE0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
		glVertexPointer(2, GL_FLOAT, 0, vertexCoords);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_FRAGMENT_PROGRAM_ARB);

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
	if (d->util->vd && d->vtx.isValid() && !d->frame.isEmpty()) {
		QPointF pos = event->posF();
		pos -= d->vtx.topLeft();
		pos.rx() *= (double)d->frame.width()/d->vtx.width();
		pos.ry() *= (double)d->frame.height()/d->vtx.height();
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
