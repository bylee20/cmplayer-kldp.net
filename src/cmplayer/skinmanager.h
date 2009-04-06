#ifndef SKINMANAGER_H
#define SKINMANAGER_H

#include <QtGui/QToolButton>
#include <QtGui/QAction>
#include <QtCore/QDebug>

class Button : public QToolButton {
	Q_OBJECT
public:
	Button(QWidget *parent)
	: QToolButton(parent), d(new Data) {
		setObjectName("flat");
		d->action = 0;
		setFocusPolicy(Qt::NoFocus);
		setAutoRaise(true);
	}
	Button(const QString &text, QWidget *parent)
	: QToolButton(parent), d(new Data) {
		setText(text);
	}
	~Button() {delete d;}
	void setIconSize(int extent) {
		QToolButton::setIconSize(QSize(extent, extent));
	}
	void setAction(QAction *action, bool update) {
		if (d->action == action)
			return;
		if (!(d->action = action))
			return;
		setCheckable(action->isCheckable());
		if (isCheckable()) {
			connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleAction(bool)));
			connect(action, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
		} else
			connect(this, SIGNAL(clicked()), action, SLOT(trigger()));
		if (update) {
			if (action->icon().isNull())
				setText(action->text());
			else {
				setIcon(action->icon());
				setToolTip(action->text());
			}
		}
	}
	void setBlock(bool block) {
		setObjectName(block ? "block" : "flat");
	}
private slots:
	void toggleAction(bool checked) {
		if (d->action && d->action->isChecked() != checked)
			d->action->trigger();
	}
private:
	struct Data {
		QAction *action;
	};
	Data *d;
};

class SkinManager {
public:
	enum PanelLayout {OneLine = 1, TwoLine = 2};
	SkinManager();
	~SkinManager();
	void setPanelLayout(PanelLayout layout);
	PanelLayout panelLayout() const;
	static SkinManager &get();
private:
	struct Data;
	Data *d;
};

#endif
