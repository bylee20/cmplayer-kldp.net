#ifndef CHARSETDETECTOR_HPP
#define CHARSETDETECTOR_HPP

class QString;			class QByteArray;

class CharsetDetector{
public:
	CharsetDetector(const QByteArray &data);
	~CharsetDetector();
	bool isDetected() const;
	QString encoding() const;
	double accuracy() const;
	static QString detect(const QString &fileName
			, double accuracy = 0.6, int size = 1024*500);
	static QString detect(const QByteArray &data, double accuracy = 0.6);
private:
	struct Data;
	Data *d;
};

#endif // CHARSETDETECTOR_HPP
