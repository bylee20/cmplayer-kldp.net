#ifndef BUTTON_H
#define BUTTON_H

#include <QtGui/QToolButton>

class Button : public QToolButton {
	Q_OBJECT
public:
	Button(QWidget *parent);
	Button(const QIcon &icon, QWidget *parent);
	Button(const QString &text, QWidget *parent);
	~Button();
	void setIconSize(int extent);
	void setAction(QAction *action, bool update);
	void setBlock(bool block);
private slots:
	void toggleAction(bool checked);
private:
	void init();
	struct Data;
	Data *d;
};

#endif
