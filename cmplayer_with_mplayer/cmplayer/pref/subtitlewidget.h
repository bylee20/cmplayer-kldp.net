#ifndef PREFSUBTITLEWIDGET_H
#define PREFSUBTITLEWIDGET_H

#include <QWidget>

namespace Pref {

class Subtitle;
namespace Ui {class Ui_SubtitleWidget;}

class SubtitleWidget : public QWidget {
	Q_OBJECT
public:
	SubtitleWidget(const Subtitle &subtitle, QWidget *parent = 0);
	~SubtitleWidget();
	const Subtitle &subtitle() const;
private:
	Ui::Ui_SubtitleWidget *ui;
	mutable Subtitle *m_subtitle;
};

}

#endif
