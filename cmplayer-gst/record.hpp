#ifndef RECORD_HPP
#define RECORD_HPP

#include <QtCore/QSettings>

class Record : public QSettings {
public:
	Record(QObject *parent = 0);
private:
};

#endif // RECORD_HPP
