#ifndef DESIGNED_BUTTONS_H
#define DESIGNED_BUTTONS_H

#include <QtGui/QToolButton>
#include <QtGui/QPushButton>

class ToolButton : public QToolButton {
	Q_OBJECT
public:
	ToolButton(QWidget *parent)
	: QToolButton(parent) {
		init();
	}
	ToolButton(const QString &toolTip, QWidget *parent)
	: QToolButton(parent) {
		init(toolTip);
	}
	ToolButton(const QIcon &icon, QWidget *parent)
	: QToolButton(parent) {
		init(QString(), icon);
	}
	ToolButton(const QString &toolTip, const QIcon &icon, QWidget *parent)
	: QToolButton(parent) {
		init(toolTip, icon);
	}
private:
	void init(const QString &toolTip = QString(), const QIcon &icon = QIcon()) {
		setFocusPolicy(Qt::NoFocus);
		setAutoRaise(true);
		setToolTip(toolTip);
		setIcon(icon);
	}
};

class PushButton : public QPushButton {
	Q_OBJECT
public:
	PushButton(QWidget *parent): QPushButton(parent) {}
	PushButton(const QString &text, QWidget *parent)
	: QPushButton(text, parent) {}
};

#endif
