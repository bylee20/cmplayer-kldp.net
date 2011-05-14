#ifndef SKINHELPER_HPP
#define SKINHELPER_HPP

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativePropertyMap>
#include "global.hpp"

class PlaylistModel;		class Mrl;
class QDeclarativePropertyMap;

namespace Skin {

class ValueHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(QVariant plain READ plain WRITE setPlain NOTIFY plainChanged DESIGNABLE false)
	Q_PROPERTY(QVariant disabled READ disabled WRITE setDisabled NOTIFY disabledChanged DESIGNABLE false)
	Q_PROPERTY(QVariant hovered READ hovered WRITE setHovered NOTIFY hoveredChanged DESIGNABLE false)
	Q_PROPERTY(QVariant pressed READ pressed WRITE setPressed NOTIFY pressedChanged DESIGNABLE false)
public:
	ValueHint(QObject *parent = 0): QObject(parent) {}
	QVariant plain() const {return m_plain;}
	QVariant disabled() const {return m_disabled.isValid() ? m_disabled : m_plain;}
	QVariant hovered() const {return m_hovered.isValid() ? m_hovered : m_plain;}
	QVariant pressed() const {return m_pressed.isValid() ? m_pressed : (m_hovered.isValid() ? m_hovered : m_plain);}
	void setPlain(const QVariant &var) {if (m_plain != var) {m_plain = var; emit plainChanged();}}
	void setHovered(const QVariant &var) {if (m_hovered != var) {m_hovered = var; emit hoveredChanged();}}
	void setDisabled(const QVariant &var) {if (m_disabled != var) {m_disabled = var; emit disabledChanged();}}
	void setPressed(const QVariant &var) {if (m_pressed != var) {m_pressed = var; emit pressedChanged();}}
signals:
	void plainChanged();
	void disabledChanged();
	void hoveredChanged();
	void pressedChanged();
private:
	QVariant m_plain, m_hovered, m_pressed, m_disabled;
};

class UrlHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(QUrl plain READ plain WRITE setPlain NOTIFY plainChanged DESIGNABLE false)
	Q_PROPERTY(QUrl disabled READ disabled WRITE setDisabled NOTIFY disabledChanged DESIGNABLE false)
	Q_PROPERTY(QUrl hovered READ hovered WRITE setHovered NOTIFY hoveredChanged DESIGNABLE false)
	Q_PROPERTY(QUrl pressed READ pressed WRITE setPressed NOTIFY pressedChanged DESIGNABLE false)
public:
	UrlHint(QObject *parent = 0): QObject(parent) {}
	QUrl plain() const {return m_plain;}
	QUrl disabled() const {return !m_disabled.isEmpty() ? m_disabled : m_plain;}
	QUrl hovered() const {return !m_hovered.isEmpty() ? m_hovered : m_plain;}
	QUrl pressed() const {return !m_pressed.isEmpty() ? m_pressed : (!m_hovered.isEmpty() ? m_hovered : m_plain);}
	void setPlain(const QUrl &var) {if (m_plain != var) {m_plain = var; emit plainChanged();}}
	void setHovered(const QUrl &var) {if (m_hovered != var) {m_hovered = var; emit hoveredChanged();}}
	void setDisabled(const QUrl &var) {if (m_disabled != var) {m_disabled = var; emit disabledChanged();}}
	void setPressed(const QUrl &var) {if (m_pressed != var) {m_pressed = var; emit pressedChanged();}}
signals:
	void plainChanged();
	void disabledChanged();
	void hoveredChanged();
	void pressedChanged();
private:
	QUrl m_plain, m_hovered, m_pressed, m_disabled;
};

class BackgroundHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(Skin::ValueHint *color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE false)
	Q_PROPERTY(Skin::ValueHint *gradient READ gradient WRITE setGradient NOTIFY gradientChanged DESIGNABLE false)
	Q_PROPERTY(Skin::UrlHint *image READ image WRITE setImage NOTIFY imageChanged DESIGNABLE false)
	Q_PROPERTY(int fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged DESIGNABLE false)
public:
	BackgroundHint(QObject *parent = 0): QObject(parent) {
		m_color = new ValueHint(this);
		m_image = new UrlHint(this);
		m_gradient = new ValueHint(this);
		m_color->setPlain(Qt::transparent);
		m_fillMode = 0;
		m_gradient->setPlain(QVariant::fromValue<void*>(0));
	}
	ValueHint *color() const {return m_color;}
	UrlHint *image() const {return m_image;}
	ValueHint *gradient() const {return m_gradient;}
	void setColor(ValueHint *color) {if (m_color != color) {m_color = color; emit colorChanged();}}
	void setGradient(ValueHint *grad) {if (m_gradient != grad) {m_gradient = grad; emit gradientChanged();}}
	void setImage(UrlHint *image) {if (m_image != image) {m_image = image; emit imageChanged();}}
	int fillMode() const {return m_fillMode;}
	void setFillMode(const int &var) {if (m_fillMode != var) {m_fillMode = var; emit fillModeChanged();}}
signals:
	void fillModeChanged();
	void colorChanged();
	void imageChanged();
	void gradientChanged();
private:
	ValueHint *m_color, *m_gradient;
	UrlHint *m_image;
	int m_fillMode;
};

class BorderHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(Skin::ValueHint *color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE false)
	Q_PROPERTY(Skin::ValueHint *width READ width WRITE setWidth NOTIFY widthChanged DESIGNABLE false)
public:
	BorderHint(QObject *parent = 0): QObject(parent) {
		m_color = new ValueHint(this);
		m_width = new ValueHint(this);
		m_color->setPlain(Qt::transparent);
		m_width->setPlain(0);
	}
	ValueHint *color() const {return m_color;}
	ValueHint *width() const {return m_width;}
	void setColor(ValueHint *color) {if (m_color != color) {m_color = color; emit colorChanged();}}
	void setWidth(ValueHint *w) {if (m_width != w) {m_width = w; emit widthChanged();}}
signals:
	void colorChanged();
	void widthChanged();
private:
	ValueHint *m_color, *m_width;
};

class IconHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(QUrl normal READ normal WRITE setNormal)
	Q_PROPERTY(QUrl checked READ checked WRITE setChecked)
public:
	IconHint(QObject *parent = 0): QObject(parent) {}
	QUrl normal() const {return m_normal;}
	QUrl checked() const {return m_checked;}
	void setNormal(const QUrl &url) {m_normal = url;}
	void setChecked(const QUrl &url) {m_checked = url;}
private:
	QUrl m_normal, m_checked;
};

class Button : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(Skin::IconHint *icon READ icon DESIGNABLE false CONSTANT FINAL)
//	Q_PROPERTY(Skin::BackgroundHint *background READ background DESIGNABLE false CONSTANT FINAL)
//	Q_PROPERTY(Skin::BorderHint *border READ border DESIGNABLE false CONSTANT FINAL)
public:
	Button(QDeclarativeItem *parent = 0): QDeclarativeItem(parent) {
		m_icon = new IconHint(this);
//		m_bg = new BackgroundHint(this);
//		m_border = new BorderHint(this);
	}
	IconHint *icon() const {return m_icon;}
//	BackgroundHint *background() const {return m_bg;}
//	BorderHint *border() const {return m_border;}
private:
	IconHint *m_icon;
//	BackgroundHint *m_bg;
//	BorderHint *m_border;
};

class RectHint: public QObject {
	Q_OBJECT
	Q_PROPERTY(Skin::BackgroundHint *background READ background WRITE setBackground NOTIFY backgroundChanged DESIGNABLE false)
	Q_PROPERTY(Skin::BorderHint *border READ border WRITE setBorder NOTIFY borderChanged DESIGNABLE false)
public:
	RectHint(QObject *parent = 0): QObject(parent) {
		m_bg = new BackgroundHint(this);
		m_border = new BorderHint(this);
	}
	BackgroundHint *background() const {return m_bg;}
	BorderHint *border() const {return m_border;}
	void setBackground(BackgroundHint *bg) {if (m_bg != bg) {m_bg = bg; emit backgroundChanged();}}
	void setBorder(BorderHint *border) {if (m_border != border) {m_border = border; emit borderChanged();}}
signals:
	void borderChanged();
	void backgroundChanged();
private:
	BackgroundHint *m_bg;
	BorderHint *m_border;
};

class HandleHint: public RectHint {
	Q_OBJECT
	Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
	Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
public:
	HandleHint(QObject *parent = 0): RectHint(parent) {m_width = m_height = 1;}
	void setWidth(int w) {if (m_width != w) {m_width = w; emit widthChanged();}}
	void setHeight(int h) {if (m_height != h) {m_height = h; emit heightChanged();}}
	int width() const {return m_width;}
	int height() const {return m_height;}
signals:
	void widthChanged();
	void heightChanged();
private:
	int m_width, m_height;
};

class Rect : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(Skin::BackgroundHint *background READ background WRITE setBackground NOTIFY backgroundChanged DESIGNABLE false)
	Q_PROPERTY(Skin::BorderHint *border READ border WRITE setBorder NOTIFY borderChanged DESIGNABLE false)
public:
	Rect(QDeclarativeItem *parent = 0): QDeclarativeItem(parent) {
		m_bg = new BackgroundHint(this);
		m_border = new BorderHint(this);
	}
	BackgroundHint *background() const {return m_bg;}
	BorderHint *border() const {return m_border;}
	void setBackground(BackgroundHint *bg) {if (m_bg != bg) {m_bg = bg; emit backgroundChanged();}}
	void setBorder(BorderHint *border) {if (m_border != border) {m_border = border; emit borderChanged();}}
signals:
	void borderChanged();
	void backgroundChanged();
private:
	BackgroundHint *m_bg;
	BorderHint *m_border;
};

class GrooveHint : public HandleHint {
	Q_OBJECT
public:
	GrooveHint(QObject *parent = 0): HandleHint(parent) {}
private:
};

class Slider : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(Skin::HandleHint *handle READ handle DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(Skin::GrooveHint *filled READ filled DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(Skin::GrooveHint *unfilled READ unfilled DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged DESIGNABLE false)
	Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged DESIGNABLE false)
	Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged DESIGNABLE false)
	Q_PROPERTY(qreal rate READ rate NOTIFY rateChanged)
public:
	Slider() {
		m_handle = new HandleHint(this);
		m_filled = new GrooveHint(this);
		m_unfilled = new GrooveHint(this);
		m_min = m_max = m_value = 0;
	}
	HandleHint *handle() const {return m_handle;}
	GrooveHint *filled() const {return m_filled;}
	GrooveHint *unfilled() const {return m_unfilled;}
	int minimum() const {return m_min;}
	int maximum() const {return m_max;}
	int value() const {return m_value;}
	qreal rate() const {return m_max <= m_min ? 0.0 : qBound(0.0, (qreal)(m_value - m_min)/(qreal)(m_max - m_min), 1.0);}
	void setMinimum(int min) {if (m_min != min) {m_min = min; emit minimumChanged(); emit rateChanged();}}
	void setMaximum(int max) {if (m_max != max) {m_max = max; emit maximumChanged(); emit rateChanged();}}
	void setValue(int value) {
		if (value != m_value && m_min <= value && value <= m_max) {
			m_value = value; emit valueChanged(); emit rateChanged();
		}
	}
signals:
	void rateChanged();
	void minimumChanged();
	void maximumChanged();
	void valueChanged();
private:
	HandleHint *m_handle;
	GrooveHint *m_filled, *m_unfilled;
	int m_min, m_max, m_value;
};

class Media : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString info READ info DESIGNABLE false)
	Q_PROPERTY(int index READ index DESIGNABLE false)
	Q_PROPERTY(int nth READ nth DESIGNABLE false)
public:
	Media(QObject *parent = 0): QObject(parent) {m_index = 0;}
	~Media();
	QString info() const {return m_info;}
	int index() const {return m_index;}
	int nth() const {return m_index + 1;}
	void setIndex(int index) {m_index = index;}
	void setInfo(const QString &info) {m_info = info;}
private:
	int m_index;
	QString m_info;
};

class Storage : public QDeclarativePropertyMap {
	Q_OBJECT
public:
	void setName(const QString &name) {m_name = name;}
	Q_INVOKABLE void save();
	Q_INVOKABLE void load();
private:
	QString m_name;
};

class Screen : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
public:
	static const int Type = UserType+1;
	Screen(QDeclarativeItem *parent = 0);
	QRectF geometry() const {return m_rect;}
	int type() const {return Type;}
	void setGeometry(const QRectF &rect);
signals:
	void geometryChanged();
private:
	void geometryChanged(const QRectF &newOne, const QRectF &old);
	QRectF m_rect;
};

class Helper : public QDeclarativeItem {
	Q_OBJECT
	Q_ENUMS(PlayerState)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
	Q_PROPERTY(int position READ position NOTIFY positionChanged)
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
	Q_PROPERTY(QSizeF size READ size NOTIFY sizeChanged)
	Q_PROPERTY(bool muted READ isMuted NOTIFY mutedChanged)
	Q_PROPERTY(PlayerState playerState READ playerState NOTIFY playerStateChanged)
	Q_PROPERTY(bool fullscreen READ isFullscreen NOTIFY fullscreenChanged)
	Q_PROPERTY(Skin::Storage *storage READ storage)
	Q_PROPERTY(Skin::Screen *screen READ screen)
	Q_PROPERTY(Skin::Media *currentMedia READ currentMedia NOTIFY currentMediaChanged)
	Q_PROPERTY(int mediaCount READ mediaCount NOTIFY mediaCountChanged)
public:
	enum PlayerState {
		PlayingState = Global::PlayingState,
		PausedState = Global::PausedState,
		StoppedState = Global::StoppedState
	};
	Helper();
	~Helper();
	QRectF screenGeometry() const {return m_screenGeometry;}
	QSizeF size() const {return QSizeF(width(), height());}
	QString name() const {return m_name;}
	int duration() const {return m_duration;}
	int position() const {return m_position;}
	int volume() const;
	bool isMuted() const;
	PlayerState playerState() const;
	int mediaCount() const;
	bool isFullscreen() const {return m_fullscreen;}
	Media *currentMedia() const {return m_media;}
	void resize(const QSizeF &size);
	void setName(const QString &name);
	Screen *screen() const {return m_screen;}
	Storage *storage() const {return m_storage;}
	Q_INVOKABLE bool exec(const QString &id);
	Q_INVOKABLE void seek(int msec);
	Q_INVOKABLE void setVolume(int volume);
	Q_INVOKABLE QString formatMSec(int msec, const QString &format) const;
//	Q_INVOKABLE bool contains(double x, double y) const {return boundingRect().contains(x, y) && !m_screen->geometry().contains(x, y);}
signals:
	void storageCreated();
	void fullscreenChanged();
	void durationChanged();
	void positionChanged();
	void volumeChanged();
	void screenChanged();
	void sizeChanged();
	void mutedChanged();
	void playerStateChanged();
	void mediaCountChanged();
	void currentMediaChanged();
	void screenGeometryChanged();
	void nameChanged();
private slots:
	void __updateDuration(int duration);
	void __updatePosition(int position);
	void __updateMrl(const Mrl &mrl);
	void __updateScreenGeometry();
	void __updateFullscreen(bool full);
private:
	void componentComplete();
	const PlaylistModel *playlist() const;
	int m_duration, m_position, m_mediaCount;
	QRectF m_screenGeometry;
	Screen *m_screen;
	bool m_fullscreen;
	Storage *m_storage;
	QString m_name;
	Media *m_media;
};

class Manager {
public:
	static QString defaultSkinName();
	static QStringList avaiableSkinNames();
	static QList<QUrl> avaiableSkinUrls();
	static Helper *load(const QString &name);
	static Helper *load(const QUrl &url, const QString &nameHint = QString());
private:
	struct Data;
	static Data &d();
};

}

Q_DECLARE_METATYPE(Skin::Helper::PlayerState)

#endif // SKINHELPER_HPP
