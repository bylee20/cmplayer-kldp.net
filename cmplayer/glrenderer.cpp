#include "glrenderer_osd.hpp"
#include "osdrenderer.hpp"
#include "glrenderer.hpp"
#include "videoframe.hpp"
#include "logodrawer.hpp"
#include <QtOpenGL/QGLShaderProgram>
#include <QtGui/QPainter>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QSize>
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include <qmath.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419717
#endif

#define USE_HIGH_SHADER 1

#ifdef USE_HIGH_SHADER
const char *GLRenderer::yv12ToRgb = 0;
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

struct GLRenderer::Data {
	LogoDrawer logo;
	bool logoOn, frameIsSet, hasPrograms;
	QList<Osd*> osds;
	GLuint texture[3];
	double crop, aspect;
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

QGLFormat GLRenderer::makeFormat() {
	QGLFormat format = QGLFormat::defaultFormat();
	format.setSwapInterval(1);
	format.setAlpha(true);
	return format;
}

GLRenderer::GLRenderer(QWidget *parent)
: QGLWidget(makeFormat(), parent), d(new Data) {

	setColorProperty(d->color);
	d->frameIsSet = d->logoOn = false;
	d->crop = d->aspect = -1.0;

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

GLRenderer::~GLRenderer() {
	glDeleteTextures(3, d->texture);
	qDeleteAll(d->osds);
	if (d->hasPrograms) {
#ifdef USE_HIGH_SHADER
		delete d->shader;
#else
		glDeleteProgramsARB(1, &d->shader);
#endif
	}
	delete d;
}

void *GLRenderer::lock(void **plane) {
	d->mutex.lock();
	VideoFrame &frame = d->buffer;
	plane[0] = frame.plane(0);
	plane[1] = frame.plane(1);
	plane[2] = frame.plane(2);
	return 0;
}

void GLRenderer::unlock(void *id, void *const *plane) {
	Q_UNUSED(id);
	Q_ASSERT(d->buffer.data() == plane[0]);
	VideoFrameEvent *event = new VideoFrameEvent(d->buffer);
	d->mutex.unlock();
	QCoreApplication::postEvent(this, event);
}

void GLRenderer::display(void *id) {
	Q_UNUSED(id);
}

void GLRenderer::prepare(quint32 fourcc, int width, int height) {
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
}

void GLRenderer::addOsd(OsdRenderer *osd) {
	d->osds.append(new Osd(osd, this));
}

void GLRenderer::customEvent(QEvent *event) {
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

QSize GLRenderer::sizeHint() const {
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

void GLRenderer::setAspectRatio(double ratio) {
	if (!isSameRatio(d->aspect, ratio)) {
		d->aspect = ratio;
		update();
	}
}

double GLRenderer::aspectRatio() const {
	return d->aspect;
}

void GLRenderer::setCropRatio(double ratio) {
	if (!isSameRatio(d->crop, ratio)) {
		d->crop = ratio;
		update();
	}
}

double GLRenderer::cropRatio() const {
	return d->crop;
}

void GLRenderer::setLogoMode(bool on) {
	d->logoOn = on;
}

void GLRenderer::setColorProperty(const ColorProperty &prop) {
	d->color = prop;
	d->brightness = qBound(-1.0, d->color.brightness(), 1.0);
	d->contrast = qBound(0., d->color.contrast() + 1., 2.);
	d->saturation = qBound(0., d->color.saturation() + 1., 2.);
	const double hue = qBound(-M_PI, d->color.hue()*M_PI, M_PI);
	d->sinhue = qSin(hue);
	d->coshue = qCos(hue);
}

const ColorProperty &GLRenderer::colorProperty() const {
	return d->color;
}

void GLRenderer::setFixedRenderSize(const QSize &size) {
	d->renderSize = size;
	updateSize();
}

QSize GLRenderer::renderableSize() const {
//	return size();
	return d->renderSize.isEmpty() ? size() : d->renderSize;
}

void GLRenderer::updateSize() {
	const QSize size = renderableSize();
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->setBackgroundSize(size);
	update();
}

void GLRenderer::resizeEvent(QResizeEvent *event) {
	QGLWidget::resizeEvent(event);
	updateSize();
}

void GLRenderer::paintEvent(QPaintEvent */*event*/) {
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
		const QRectF r(x, y, frame.width(), frame.height());

		float expandV = (widget.height() - frame.height())*0.5/frame.height();
		float expandH = (widget.width() - frame.width())*0.5/frame.width();
		qDebug() << expandV << expandH;
//		expandV = expandH = 6e-5;//0.0;
//		expandV = 0;
//		expandH = 6.13949e-05;
		static const float tx_array[12] = {
			-expandH,	-expandV,
			1.0f + expandH,	-expandV,
			1.0f + expandH,	1.0f + expandV,
			-expandH,	1.0f + expandV
		};
		const float v_array[] = {
			0.0,		0.0,
			width(),	0.0,
			width(),	height(),
			0.0,		height()
		};

//		static const float tx_array[12] = {
//			0.0,	0.0,
//			1.0,	0.0,
//			1.0,	1.0,
//			0.0,	1.0
//		};

//		const float v_array[] = {
//			r.left(),	r.top(),
//			r.right(),	r.top(),
//			r.right(),	r.bottom(),
//			r.left(),	r.bottom()
//		};

		painter.beginNativePainting();
		makeCurrent();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_HIGH_SHADER
		d->shader->bind();
		d->shader->setUniformValue("tex0", 0);
		d->shader->setUniformValue("tex1", 1);
		d->shader->setUniformValue("tex2", 2);
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

		glVertexPointer(2, GL_FLOAT, 0, v_array);
		glTexCoordPointer(2, GL_FLOAT, 0, tx_array);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

#ifdef USE_HIGH_SHADER
		d->shader->release();
#else
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
#endif
		doneCurrent();
		painter.endNativePainting();
//		if (vMargin >= 0.0) {
//			QRectF rect(0.0, 0.0, widget.width(), vMargin);
//			painter.fillRect(rect, Qt::black);
//			rect.moveTop(widget.height() - vMargin);
//			painter.fillRect(rect, Qt::black);
//		}
//		if (hMargin >= 0.0) {
//			QRectF rect(0.0, 0.0, hMargin, widget.height());
//			painter.fillRect(rect, Qt::black);
//			rect.moveLeft(widget.width() - hMargin);
//			painter.fillRect(rect, Qt::black);
//		}
	} else
		d->logo.draw(&painter, rect());
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->render();
}

