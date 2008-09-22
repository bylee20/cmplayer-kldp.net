#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Pref {

class Preferences;				class GeneralWidget;
class SubtitleWidget;		class InterfaceWidget;

namespace Ui {class Ui_PreferencesDialog;}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();
private slots:
	void save();
private:
	Ui::Ui_PreferencesDialog *ui;
	Preferences *m_pref;
	GeneralWidget *m_general;
	SubtitleWidget *m_subtitle;
	InterfaceWidget *m_interface;
};

}

#endif
