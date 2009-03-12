#ifndef CHOOSER_H
#define CHOOSER_H

#include <QWidget>

class Chooser : public QWidget {
	Q_OBJECT
public:
	Chooser(QWidget *parent = 0);
	~Chooser();
	void setChoices(const QStringList choices, int chosen = 0);
	void setChosenText(const QString &text);
	void clear();
	int chosenIndex() const;
	QString chosenText() const;
private:
	struct Data;
	Data *d;
};

#endif
