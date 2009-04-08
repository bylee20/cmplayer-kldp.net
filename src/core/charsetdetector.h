#ifndef CORECHARSETDETECTOR_H
#define CORECHARSETDETECTOR_H

class QString;			class QByteArray;

namespace Core {

class CharsetDetector{
public:
	CharsetDetector(const QByteArray &data);
	~CharsetDetector();
	bool isDetected() const;
	QString encoding() const;
	double confidence() const;
	static QString detect(const QString &fileName
			, double confidence = 0.6, int size = 1024*500);
	static QString detect(const QByteArray &data, double confidence = 0.6);
private:
	struct Data;
	Data *d;
};

}

#endif
