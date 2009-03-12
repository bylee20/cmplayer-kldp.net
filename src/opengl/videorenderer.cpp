#include "videorenderer.h"
#include "osdrenderer.h"
#include <core/newframeevent.h>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <qmath.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419717
#endif

namespace OpenGL {

const char *VideoRenderer::yv12ToRgb =
	"!!ARBfp1.0"
	"PARAM c[5] = {"
	"	program.local[0..1],"
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

const char *VideoRenderer::yuy2ToRgb =
	"!!ARBfp1.0"
	"PARAM c[5] = { program.local[0..1],"
	"                { 0.5, 2, 1, 0.0625 },"
	"                { 1.164, 0, 1.596, 2.0179999 },"
	"                { 1.164, -0.391, -0.81300002 } };"
	"TEMP R0;"
	"TEMP R1;"
	"TEMP R2;"
	"FLR R1.z, fragment.texcoord[0].x;"
	"ADD R0.x, R1.z, c[2];"
	"ADD R1.z, fragment.texcoord[0].x, -R1;"
	"MUL R1.x, fragment.texcoord[0].z, R0;"
	"MOV R1.y, fragment.texcoord[0];"
	"TEX R0, R1, texture[0], 2D;"
	"ADD R1.y, R0.z, -R0.x;"
	"MUL R2.x, R1.z, R1.y;"
	"MAD R0.x, R2, c[2].y, R0;"
	"MOV R1.y, fragment.texcoord[0];"
	"ADD R1.x, fragment.texcoord[0].z, R1;"
	"TEX R1.xyw, R1, texture[0], 2D;"
	"ADD R2.x, R1, -R0.z;"
	"MAD R1.x, R1.z, c[2].y, -c[2].z;"
	"MAD R0.z, R1.x, R2.x, R0;"
	"ADD R1.xy, R1.ywzw, -R0.ywzw;"
	"ADD R0.z, R0, -R0.x;"
	"SGE R1.w, R1.z, c[2].x;"
	"MAD R0.x, R1.w, R0.z, R0;"
	"MAD R0.yz, R1.z, R1.xxyw, R0.xyww;"
	"ADD R0.xyz, R0, -c[2].wxxw;"
	"MUL R0.w, R0.y, c[0];"
	"MAD R0.w, R0.z, c[0].z, R0;"
	"MUL R0.z, R0, c[0].w;"
	"MAD R0.y, R0, c[0].z, R0.z;"
	"MUL R0.w, R0, c[0].y;"
	"MUL R0.y, R0, c[0];"
	"MUL R0.z, R0.w, c[1].x;"
	"MAD R0.x, R0, c[0].y, c[0];"
	"MUL R0.y, R0, c[1].x;"
	"DP3 result.color.x, R0, c[3];"
	"DP3 result.color.y, R0, c[4];"
	"DP3 result.color.z, R0, c[3].xwyw;"
	"MOV result.color.w, c[1].y;"
	"END";

struct VideoRenderer::Data {
	GLuint texture[3];
	bool hasPrograms, frameSet;
	GLuint program[3];
	double brightness, contrast, saturation, hue;
	QByteArray data;
	QImage image;
	QList<OsdRenderer*> osds;
	QSizeF visual, widget;
	double vMargin, hMargin;
};

QGLFormat VideoRenderer::makeFormat() {
	QGLFormat format = QGLFormat::defaultFormat();
	format.setSwapInterval(1);
	return format;
}

VideoRenderer::VideoRenderer(QWidget *parent)
: QGLWidget(makeFormat(), parent), OpenGLVideoRendererIface(this), d(new Data) {
	setAttribute(Qt::WA_NativeWindow);
	d->vMargin = d->hMargin = 0.0;
	d->brightness = d->hue = 0.0;
	d->contrast = d->saturation = 1.0;
	d->frameSet = false;
	d->widget = size();
	makeCurrent();
	glGenTextures(3, d->texture);
#define GET_PROC_ADDRESS(func) func = (_##func)context()->getProcAddress(QLatin1String(#func))
	GET_PROC_ADDRESS(glProgramStringARB);
	GET_PROC_ADDRESS(glBindProgramARB);
	GET_PROC_ADDRESS(glDeleteProgramsARB);
	GET_PROC_ADDRESS(glGenProgramsARB);
	GET_PROC_ADDRESS(glActiveTexture);
	GET_PROC_ADDRESS(glProgramLocalParameter4fARB);
#undef GET_PROC_ADDRESS

	d->hasPrograms = glProgramStringARB && glBindProgramARB && glDeleteProgramsARB;
	d->hasPrograms = d->hasPrograms && glGenProgramsARB && glActiveTexture;
	d->hasPrograms = d->hasPrograms && glProgramLocalParameter4fARB;

	if (d->hasPrograms) {
		glGenProgramsARB(2, d->program);
		const char *code[] = {yv12ToRgb, yuy2ToRgb};
		bool error = false;
		for(int i = 0; i < 2 && !error;  ++i) {
			glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->program[i]);
			const GLbyte *gl_src = reinterpret_cast<const GLbyte *>(code[i]);
			glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB
					, strlen(code[i]), gl_src);
			if (glGetError() != GL_NO_ERROR)
				error = true;
		}
		if (error) {
			glDeleteProgramsARB(2, d->program);
			d->hasPrograms = false;
		}
	}
}

VideoRenderer::~VideoRenderer() {
	makeCurrent();
	glDeleteTextures(3, d->texture);
	for (int i=0; i<d->osds.size(); ++i)
		delete d->osds[i];
	delete d;
}

void VideoRenderer::setFrame(const Core::VideoFrame &frame) {
	QApplication::postEvent(this, new Core::NewFrameEvent(frame));
}

void VideoRenderer::setVideoProperties(double b, double c, double s, double h) {
	d->brightness = qBound(-0.5, b*0.5, 0.5);
	d->contrast = qBound(0., c + 1., 2.);
	d->saturation = qBound(0., s + 1., 2.);
	d->hue = qBound(-M_PI, h*M_PI, M_PI);
	update();
}

void VideoRenderer::resizeEvent(QResizeEvent */*event*/) {
	d->widget = size();
	calculate();
}

void VideoRenderer::customEvent(QEvent *event) {
	if (static_cast<Core::BaseEvent*>(event)->type() != Core::BaseEvent::NewFrame)
		return;
	const Core::VideoFrame frame = ((const Core::NewFrameEvent*)event)->frame();
	d->frameSet = !frame.isNull() && frame.info().type != Core::VideoFrame::NoFrame;
	updateFrameInfo(frame.info(), true);
	if (!d->frameSet)
		return;
	const int width = frameInfo().size.width();
	const int height = frameInfo().size.height();
	makeCurrent();
	switch (frameInfo().type) {
	case Core::VideoFrame::I420:
	case Core::VideoFrame::YV12: {
		const int w[3] = {width, width/2, width/2};
		const int h[3] = {height, height/2, height/2};
		int offset[3] = {0};
		if (frameInfo().type == Core::VideoFrame::I420) {
			offset[1] = w[0]*h[0]*5/4;
			offset[2] = w[0]*h[0];
		} else {
			offset[1] = w[0]*h[0];
			offset[2] = w[0]*h[0]*5/4;
		}
		for (int i=0; i<3; ++i) {
			glBindTexture(GL_TEXTURE_2D, d->texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w[i], h[i], 0
					, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame.data() + offset[i]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		break;
	} case Core::VideoFrame::YUY2: {
		glBindTexture(GL_TEXTURE_2D, d->texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width/2, height, 0
				, GL_RGBA, GL_UNSIGNED_BYTE, frame.data());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	} default:
		d->frameSet = false;
		break;
	}
	update();
}

void VideoRenderer::paintEvent(QPaintEvent */*event*/) {
	QPainter painter;
	painter.begin(this);
	painter.fillRect(rect(), Qt::black);
	if (!d->frameSet)
		return;
	if (frameInfo().type == Core::VideoFrame::I420 || frameInfo().type == Core::VideoFrame::YV12)
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->program[0]);
	else if (frameInfo().type == Core::VideoFrame::YUY2)
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, d->program[1]);
	else
		return;
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0.0f, d->brightness
			, d->contrast, qCos(d->hue), qSin(d->hue));
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1.0f, d->saturation
			, 1. /*alpha */, 0. /*dummy*/, 0. /*dummy*/);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	float tx_array[12] = {0., 0., 0., 1., 0., 0., 1., 1., 0., 0., 1., 0.};
	const float v_array[] = { imageRect().left(), imageRect().top(), imageRect().right() + 1.
		, imageRect().top(), imageRect().right() + 1., imageRect().bottom() + 1.
			, imageRect().left(), imageRect().bottom() + 1. };
	if (frameInfo().type == Core::VideoFrame::YUY2) {
		const float w = frameInfo().size.width() / 2;
		const float iw = 1./w;
		tx_array[3] = w;
		tx_array[6] = w;
		for (int i = 0; i < 4; ++i)
			tx_array[3*i + 2] = iw;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->texture[0]);
	if (frameInfo().type == Core::VideoFrame::I420 || frameInfo().type == Core::VideoFrame::YV12) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, d->texture[2]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, d->texture[1]);
		glActiveTexture(GL_TEXTURE0);
	}
	glVertexPointer(2, GL_FLOAT, 0, v_array);
	glTexCoordPointer(3, GL_FLOAT, 0, tx_array);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	if (d->vMargin > 0.5) {
		QRectF rect(0.0, 0.0, width(), d->vMargin);
		painter.fillRect(rect, Qt::black);
		rect.moveTop(height() - d->vMargin);
		painter.fillRect(rect, Qt::black);
	}
	if (d->hMargin > 0.5) {
		QRectF rect(0.0, 0.0, d->hMargin, height());
		painter.fillRect(rect, Qt::black);
		rect.moveLeft(width() - d->hMargin);
		painter.fillRect(rect, Qt::black);
	}
	painter.fillRect(QRect(0, 0, 0, 0), Qt::white);
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->renderContents(&painter);
	if (object())
		object()->overdraw(&painter);
	painter.end();
}

void VideoRenderer::rerender() {
	calculate();
	update();
}

Core::AbstractOsdRenderer *VideoRenderer::createOsd() {
	OsdRenderer *osd = new OsdRenderer(this);
	d->osds.push_back(osd);
	return osd;
}

void VideoRenderer::calculate() {
	QSizeF video(aspectRatioF(), 1.0);
	QSizeF visual = video;
	if (cropRatio() > 0.0) {
		visual.setWidth(cropRatio());
		visual.setHeight(1.0);
	}
	visual.scale(d->widget, Qt::KeepAspectRatio);
	video.scale(visual, Qt::KeepAspectRatioByExpanding);
	const double x = (d->widget.width() - video.width())/2.0;
	const double y = (d->widget.height() -video.height())/2.0;
	setImageRect(x, y, video);
	d->hMargin = (d->widget.width() - visual.width())/2.0;
	d->vMargin = (d->widget.height() - visual.height())/2.0;
	hscale() = video.width() / double(videoSize().width());
	vscale() = video.height() / double(videoSize().height());
	const double pixel = frameInfo().pixelAspectRatio;
	if (pixel > 1.)
		hscale() *= pixel;
	else
		vscale() /= pixel;
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->setSize(visual, d->widget);
}

void VideoRenderer::mouseMoveEvent(QMouseEvent *event) {
	if (object())
		object()->mouseMoveEvent(event->pos());
	QGLWidget::mouseMoveEvent(event);
}

void VideoRenderer::mousePressEvent(QMouseEvent *event) {
	if (object())
		object()->mousePresseEvent(event->pos(), event->button());
	QGLWidget::mousePressEvent(event);
}

}
