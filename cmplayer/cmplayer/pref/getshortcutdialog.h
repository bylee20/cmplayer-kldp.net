#ifndef PREFERENCESGETSHORTCUTDIALOG_H
#define PREFERENCESGETSHORTCUTDIALOG_H

#include <QtGui/QDialog>

namespace Pref {

namespace Ui {class Ui_GetShortcutDialog;}

class GetShortcutDialog : public QDialog {
	Q_OBJECT
public:
	GetShortcutDialog(QWidget *parent = 0);
	~GetShortcutDialog();
	const QKeySequence &key() const;
	void setKey(const QKeySequence &key);
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
private slots:
	void setGetting(bool on);
private:
	static const int MaxKeyCount = 4;
	void getShortcut(QKeyEvent *event);
	Ui::Ui_GetShortcutDialog *ui;
	bool m_getting;
	bool m_pressing;
	int m_curIdx;
	int m_codes[MaxKeyCount];
	QKeySequence m_key;
};

}

#endif
