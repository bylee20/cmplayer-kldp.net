#include "glrenderer_overlay.hpp"
#include "osdrenderer.hpp"
#include "pixmapoverlay.hpp"
#include "videorenderer.hpp"
#include "videoframe.hpp"
#include "logodrawer.hpp"
#include "pixelbufferoverlay.hpp"
#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLShaderProgram>
#include <QtGui/QPainter>
#include <QtCore/QCoreApplication>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QSize>
#include <qmath.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419717
#endif

#define USE_HIGH_SHADER 0

#ifdef USE_HIGH_SHADER
const char *VideoRenderer::yv12ToRgb = 0;
#else
const char *GLRenderer::yv12ToRgb =
	"!!ARBfp1.0"
	"PARAM c[5] = {"
	"	program.local[0],program.local[1],"
	"	{ 1.164, 0, 1.596, 0.5 },"
	"	{ 0.0625, 1.164, -0.391, -0.81300002 },"
	"	{ 1.164, 2.0179999, 0 }"
	"};"
	"TEMP R0;"
	"TEX R0.x, fragment.texcoord[0], texture[1], 2D;"
	"ADD R0.y, R0.x, -c[2].w;"
	"TEX R0.x, fragment.texcoord[0], texture[2], 2D;"
	"ADD R0.x, R0, -c[2].w;"
	"MUL R0.z, R0.y, c[0].w;"
	"MAD R0.z, R0.x, c[0], R0;"
	"MUL R0.w, R0.x, c[0];"
	"MUL R0.z, R0, c[0].y;"
	"TEX R0.x, fragment.texcoord[0], texture[0], 2D;"
	"MAD R0.y, R0, c[0].z, R0.w;"
	"ADD R0.x, R0, -c[3];"
	"MUL R0.y, R0, c[0];"
	"MUL R0.z, R0, c[1].x;"
	"MAD R0.x, R0, c[0].y, c[0];"
	"MUL R0.y, R0, c[1].x;"

	"DP3 result.color.x, R0, c[2];"
	"DP3 result.color.y, R0, c[3].yzww;"
	"DP3 result.color.z, R0, c[4];"
	"MOV result.color.w, c[1].y;"
	"END";
#endif

struct VideoRenderer::Data {
	LogoDrawer logo;
	bool logoOn, frameIsSet, hasPrograms;
	Overlay *overlay;
	GLuint texture[3];
	double crop, aspect, fps;
	float brightness, contrast, saturation, sinhue, coshue;
	ColorProperty color;
	QSize renderSize;
	QMutex mutex;
	VideoFrame buffer, frame;
#ifdef USE_HIGH_SHADER
	QGLShaderProgram *shader;
#else
	GLuint shader;
#endif
};

QGLFormat VideoRenderer::makeFormat() {
//	return QGLFormat();
	QGLFormat format;
	return format;
}

VideoRenderer::VideoRenderer(QWidget *parent)
: QGLWidget(makeFormat(), parent), d(new Data) {
	setMinimumSize(QSize(200, 100));
	setColorProperty(d->color);
	d->frameIsSet = d->logoOn = false;
	d->fps = d->crop = d->aspect = -1.0;
	d->overlay = new FramebufferObjectOverlay(this);
//	d->overlay = new PixelBufferOverlay(this);
//	d->overlay = new PixmapOverlay(this);

	makeCurrent();
	glGenTextures(3, d->texture);
#define GET_PROC_ADDRESS(func) func = (_##func)context()->getProcAddress(QLatin1String(#func))
#ifndef USE_HIGH_SHADER
	GET_PROC_ADDRESS(glProgramStringARB);
	GET_PROC_ADDRESS(glBindProgramARB);
	GET_PROC_ADDRESS(glDeleteProgramsARB);
	GET_PROC_ADDRESS(glGenProgramsARB);
	GET_PROC_ADDRESS(glProgramLocalParameter4fARB);
#endif
	GET_PROC_ADDRESS(glActiveTexture);
#undef GET_PROC_ADDRESS

	d->hasPrograms = glActiveTexture;
#ifdef USE_HIGH_SHADER
	d->hasPrograms = d->hasPrograms && QGLShaderProgram::hasOpenGLShaderPrograms(context());
	if (d->hasPrograms) {
		d->shader = new QGLShaderProgram(context(), this);
		d->shader->addShaderFromSourceFile(QGLShader::Fragment, "/Users/xylosper/cmplayer-vlc/src/glrenderer.fs");
		d->shader->link();
	} else
		d->shader = 0;
#else
	d->hasPrograms = glProgramStringARB && glBindProgramARB && glDeleteProgramsARB;
	d->hasPrograms = d->hasPrograms && glGenProgramsARB;
	d->hasPrograms = d->hasPrograms && glProgramLocalParameter4fARB;
	if (d->hasPrograms) {
		glGenProgramsARB(1, &d->shader);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->shader);
		const GLbyte *glSrc = reinterpret_cast<const GLbyte *>(yv12ToRgb);
		glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB
			, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(yv12ToRgb), glSrc);
		if (glGetError() != GL_NO_ERROR) {
			glDeleteProgramsARB(1, &d->shader);
			d->hasPrograms = false;
		}
	}

#endif
	doneCurrent();
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

VideoRenderer::~VideoRenderer() {
	delete d->overlay;
	glDeleteTextures(3, d->texture);
	if (d->hasPrograms) {
#ifdef USE_HIGH_SHADER
		delete d->shader;
#else
		glDeleteProgramsARB(1, &d->shader);
#endif
	}
	delete d;
}

void *VideoRenderer::lock(void **plane) {
	d->mutex.lock();
	VideoFrame &frame = d->buffer;
	plane[0] = frame.plane(0);
	plane[1] = frame.plane(1);
	plane[2] = frame.plane(2);
	return 0;
}

void VideoRenderer::unlock(void *id, void *const *plane) {
	Q_UNUSED(id);
	Q_ASSERT(d->buffer.data() == plane[0]);
	VideoFrameEvent *event = new VideoFrameEvent(d->buffer);
	d->mutex.unlock();
	QCoreApplication::postEvent(this, event);
}

void VideoRenderer::display(void *id) {
	Q_UNUSED(id);
}

void VideoRenderer::prepare(quint32 fourcc, int width, int height, double fps) {
//	QMutexLocker locker(&d->mutex);
//	Q_UNUSED(locker);
	d->buffer = VideoFrame(fourcc, width, height);
	d->frame = VideoFrame(fourcc, width, height);
	d->frameIsSet = d->buffer.type() == VideoFrame::YV12 && !d->buffer.isEmpty();
	if (!d->frameIsSet)
		return;
	const int w[3] = {d->frame.width(), d->frame.width()/2, d->frame.width()/2};
	const int h[3] = {d->frame.height(), d->frame.height()/2, d->frame.height()/2};
	const int offset[3] = {0, w[0]*h[0]*5/4, w[0]*h[0]};
	makeCurrent();
	for (int i=0; i<3; ++i) {
		glBindTexture(GL_TEXTURE_2D, d->texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w[i], h[i], 0
			, GL_LUMINANCE, GL_UNSIGNED_BYTE, d->frame.data() + offset[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_SGIS);
	}
	doneCurrent();
	if (!qFuzzyCompare(d->fps, fps))
		emit frameRateChanged(d->fps = fps);
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
	d->frame = static_cast<VideoFrameEvent*>(event)->frame();
	if (!d->frameIsSet)
		return;
	const VideoFrame &frame = d->frame;
	const int w[3] = {frame.width(), frame.width()/2, frame.width()/2};
	const int h[3] = {frame.height(), frame.height()/2, frame.height()/2};
	const int offset[3] = {0, w[0]*h[0]*5/4, w[0]*h[0]};
	makeCurrent();
	for (int i=0; i<3; ++i) {
		glBindTexture(GL_TEXTURE_2D, d->texture[i]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w[i], h[i]
			, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame.constData() + offset[i]);
	}
	doneCurrent();
	update();
}

QSize VideoRenderer::sizeHint() const {
	if (d->frame.isEmpty())
		return QSize(400, 300);
	const QSizeF frame = d->frame.size();
	QSizeF size(d->aspect, 1.0);
	if (d->aspect < 0)
		size = frame;
	else
		size.scale(frame, Qt::KeepAspectRatio);
	if (d->crop < 0.0)
		return size.toSize();
	QSizeF crop(d->crop, 1.0);
	crop.scale(size, Qt::KeepAspectRatio);
	return crop.toSize();
}

static bool isSameRatio(double r1, double r2) {
	return (r1 <= 0.0 && r2 <= 0.0) || qFuzzyCompare(r1, r2);
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
	d->saturation = qBound(0., d->color.saturation() + 1., 2.);
	const double hue = qBound(-M_PI, d->color.hue()*M_PI, M_PI);
	d->sinhue = qSin(hue);
	d->coshue = qCos(hue);
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
		if (d->aspect < 0.0)
			frame = d->frame.size();
		const QSizeF widget = renderableSize();
		QSizeF letter(d->crop, 1.0);
		if (d->crop < 0.0)
			letter = frame;
		letter.scale(widget, Qt::KeepAspectRatio);
		frame.scale(letter, Qt::KeepAspectRatioByExpanding);

		const double x = (widget.width() - frame.width())*0.5;
		const double y = (widget.height() - frame.height())*0.5;
		const double hMargin = (widget.width() - letter.width())*0.5;
		const double vMargin = (widget.height() - letter.height())*0.5;
		const QRectF vx(x, y, frame.width(), frame.height());
		const QRectF tx(0.0, 0.0, 1.0, 1.0);
		const float tx_array[] = {
			tx.left(),	tx.top(),
			tx.right(),	tx.top(),
			tx.right(),	tx.bottom(),
			tx.left(),	tx.bottom()
		};
		const float vx_array[] = {
			vx.left(),	vx.top(),
			vx.right(),	vx.top(),
			vx.right(),	vx.bottom(),
			vx.left(),	vx.bottom()
		};

		painter.beginNativePainting();
		makeCurrent();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#ifdef USE_HIGH_SHADER
		d->shader->bind();
		GLuint bgtex[] = {0, 1, 2};
		d->shader->setUniformValueArray("bgtex", bgtex, 3);
		d->shader->setUniformValue("brightness", d->brightness);
		d->shader->setUniformValue("contrast", d->contrast);
		d->shader->setUniformValue("coshue", d->coshue);
		d->shader->setUniformValue("sinhue", d->sinhue);
		d->shader->setUniformValue("saturation", d->saturation);
#else
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->shader);
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0.0f, d->brightness
			, d->contrast, d->coshue, d->sinhue);
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1.0f, d->saturation
			, 1. /*alpha */, 0. /*dummy*/, 0. /*dummy*/);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
#endif

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, d->texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, d->texture[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, d->texture[2]);
		glActiveTexture(GL_TEXTURE0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, tx_array);
		glVertexPointer(2, GL_FLOAT, 0, vx_array);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

//		glBegin(GL_QUADS);
//		glMultiTexCoord2f(GL_TEXTURE0, tx.left(), tx.top());
//		glVertex2f(vx.left(), vx.top());
//		glMultiTexCoord2f(GL_TEXTURE0, tx.right(), tx.top());
//		glVertex2f(vx.right(), vx.top());
//		glMultiTexCoord2f(GL_TEXTURE0, tx.right(), tx.bottom());
//		glVertex2f(vx.right(), vx.bottom());
//		glMultiTexCoord2f(GL_TEXTURE0, tx.left(), tx.bottom());
//		glVertex2f(vx.left(), vx.bottom());
//		glEnd();

#ifdef USE_HIGH_SHADER
		d->shader->release();
#else
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
#endif
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

