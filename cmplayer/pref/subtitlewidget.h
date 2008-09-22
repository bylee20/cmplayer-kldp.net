#ifndef PREFSUBTITLEWIDGET_H
#define PREFSUBTITLEWIDGET_H

#include <QWidget>

namespace Pref {

class Subtitle;

class SubtitleWidget : public QWidget {
	Q_OBJECT
public:
	SubtitleWidget(const Subtitle &subtitle, QWidget *parent = 0);
	~SubtitleWidget();
	const Subtitle &subtitle() const;
private:
	struct Data;
	Data *d;
};

}

#endif
