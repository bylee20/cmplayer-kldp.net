#ifndef PLAYLISTDOCK_P_H
#define PLAYLISTDOCK_P_H

#include <QDialog>

class QComboBox;

class ShutdownDialog : public QDialog {
	Q_OBJECT
public:
	ShutdownDialog(QWidget *parent);
	void accept();
	QStringList command() const;
private:
	QComboBox *combo;
};

#endif
