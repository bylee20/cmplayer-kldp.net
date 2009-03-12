#ifndef GETSHORTCUTDIALOG_H
#define GETSHORTCUTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {class Ui_GetShortcutDialog;}

class GetShortcutDialog : public QDialog {
	Q_OBJECT
public:
	GetShortcutDialog(const QKeySequence &shortcut, QWidget *parent = 0);
	GetShortcutDialog(QWidget *parent = 0);
	~GetShortcutDialog();
	const QKeySequence &shortcut() const;
	void setShortcut(const QKeySequence &shortcut);
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
private slots:
	void setGetting(bool on);
	void erase();
private:
	void init();
	static const int MaxKeyCount = 4;
	void getShortcut(QKeyEvent *event);
	struct Data;
	Data *d;
};

#endif
