#ifndef VIDEOSCENE_P_HPP
#define VIDEOSCENE_P_HPP

#include "videoscene.hpp"
#include <QtGui/QGraphicsSceneWheelEvent>
#include <QtGui/QGraphicsSceneMouseEvent>
#include "menu.hpp"
#include <QtGui/QPainter>
#include <QtGui/QGraphicsView>
#include "fragmentprogram.hpp"
#include "overlay.hpp"
#include "colorproperty.hpp"
#include "events.hpp"
#include "videoframe.hpp"
#include "pref.hpp"
#include "logodrawer.hpp"
#include "skinhelper.hpp"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QSize>
#include <QtCore/QRegExp>
#include <QtCore/QCoreApplication>
#include <math.h>

#include "i420_to_rgb_simple.hpp"
#include "i420_to_rgb_filter.hpp"
#include "i420_to_rgb_kernel.hpp"

class VideoScene::VideoView : public QGraphicsView {
public:
	VideoView(VideoScene *scene, QGLWidget *viewport) {
		m_scene = scene;
		setScene(m_scene);
		setMouseTracking(true);
		setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		setFrameStyle(QFrame::NoFrame | QFrame::Plain);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setMinimumSize(300, 200);
		setContextMenuPolicy(Qt::CustomContextMenu);

		setViewport(viewport);
		setAutoFillBackground(false);
		setAttribute(Qt::WA_OpaquePaintEvent, true);
		viewport->setAutoFillBackground(false);
		viewport->setAttribute(Qt::WA_OpaquePaintEvent, true);
	}
	QSize sizeHint() const {return m_scene->sizeHint(1.0).toSize();}
private:
	VideoScene *m_scene;
	void mouseMoveEvent(QMouseEvent *event) {
		QGraphicsView::mouseMoveEvent(event);
		event->setAccepted(!m_scene->needToPropagate(event->posF()));
	}
	void mousePressEvent(QMouseEvent *event) {
		QGraphicsView::mousePressEvent(event);
		event->setAccepted(!m_scene->needToPropagate(event->posF()));
	}
	void mouseReleaseEvent(QMouseEvent *event) {
		QGraphicsView::mouseReleaseEvent(event);
		event->setAccepted(!m_scene->needToPropagate(event->posF()));
	}
	void resizeEvent(QResizeEvent *event) {
		QGraphicsView::resizeEvent(event);
		m_scene->updateSceneRect();
	}
};

class VideoScene::FrameRateMeasure {
public:
	FrameRateMeasure() {m_drawn = 0; m_prev = 0;}
	void reset() {m_time.restart(); m_drawn = 0; m_prev = 0;}
	int elapsed() const {return m_time.elapsed();}
	void frameDrawn(int id) {
		if (m_prev != id) {
			++m_drawn;
			m_prev = id;
		}
	}
	int drawnFrames() const {return m_drawn;}
	double frameRate() const {return (double)m_drawn/(double)m_time.elapsed()*1e3;}
private:
	int m_drawn;
	int m_prev;
	QTime m_time;
};

struct VideoScene::Data {
	static const int i420ToRgbSimple = 0;
	static const int i420ToRgbFilter = 1;
	static const int i420ToRgbKernel = 2;
	QGLWidget *gl;
	QGraphicsView *view;
	FrameRateMeasure fps;	int frameId;
	QMutex mutex;		QSize renderSize;	ColorProperty color;
	LogoDrawer logo;	Overlay *overlay;	GLuint texture[3];
	VideoFrame *buffer, *frame, *temp, buf[3];	VideoUtil *util;
	QSize size;
	QRectF vtx;		Effects effects;
	void **planes[VIDEO_FRAME_MAX_PLANE_COUNT];	VideoFormat format;
	QList<FragmentProgram*> shaders;		FragmentProgram *shader;
	double rgb_base, rgb_c_r, rgb_c_g, rgb_c_b;
	double crop, aspect, kern_d, kern_c, kern_n;
	double y_min, y_max; double y_min_buffer, y_max_buffer;
	double brightness, contrast, sat_con, sinhue, coshue;
	double cpu;
	bool hasKernel, prepared, logoOn, frameIsSet, hasPrograms, binding;
	SkinHelper *skin;
	bool skinVisible;
// methods

	static double aspect_ratio(const QSizeF &size) {return size.width()/size.height();}
	static double aspect_ratio(const QRectF &rect) {return rect.width()/rect.height();}
	static bool compare_ratio(double r1, double r2) {return (r1<0. && r2<0.) || qFuzzyCompare(r1, r2);}
	static int translate_button(Qt::MouseButton qbutton) {
		switch (qbutton) {
		case Qt::LeftButton:	return 0;	case Qt::MidButton:	return 1;
		case Qt::RightButton:	return 2;	default:		return -1;
		}
	}


	static inline int kbps(double fps, int width, int height, int bpp) {
		return width*height*bpp*fps/1024 + 0.5;
	}

	inline void update_sat_con() {
		sat_con = (!(effects & IgnoreEffect) && (effects & Grayscale)) ? 0.0
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
		if (!(effects & IgnoreEffect)) {
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



#endif // VIDEOSCENE_P_HPP
