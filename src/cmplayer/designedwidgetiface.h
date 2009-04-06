#ifndef DESIGNEDWIDGETIFACE_H
#define DESIGNEDWIDGETIFACE_H

#include <QtGui/QWidget>

class ToolButton;

class DesignedWidgetIface {
public:
	class TitleBar;
	DesignedWidgetIface();
	virtual ~DesignedWidgetIface();
protected:
	TitleBar *titleBar();
	void drawBackground(QPainter *painter, QWidget *widget);
private:
	TitleBar *m_titleBar;
};

class DesignedWidgetIface::TitleBar : public QWidget {
	Q_OBJECT
public:
	TitleBar(QWidget *parent);
	~TitleBar();
	void connect(QWidget *widget);
	void setTitle(const QString &title);
	QString title() const;
	void setIcon(const QIcon &icon);
	ToolButton *addButton(const QIcon &icon, QObject *object, const char *slot);
private slots:
	void toggleMaximized();
private:
	void setSpacerSize(int size);
	void paintEvent(QPaintEvent */*event*/);
	struct Data;
	Data *d;
};

#endif
