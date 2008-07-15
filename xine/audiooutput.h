//#ifndef MPLAYERAUDIOOUTPUT_H
//#define MPLAYERAUDIOOUTPUT_H
//
//#include "controller.h"
//
//namespace MPlayer {
//
//class PlayEngine;
//
//class AudioOutput : public Controller {
//	Q_OBJECT
//public:
//	AudioOutput(QObject *parent = 0);
//	~AudioOutput();
//	int volume() const;
//	bool isMuted() const;
//	const QString &driver() const;
//	qreal volumeAmplification() const;
//	bool isEnabledSoftwareVolume() const;
//	void setDriver(const QString &driver);
//	void setEnabledSoftwareVolume(bool enabled);
//	void setVolumeAmplification(qreal amp);
//	int initialVolume() const;
//public slots:
//	void setInitialVolume(int value);
//	void setVolume(int value, bool relative = false);
//	void setMuted(bool muted);
//signals:
//	void volumeChanged(int value);
//	void mutedChanged(bool muted);
//protected slots:
//	void update();
//protected:
//	void link(Controller *controller);
//	void unlink(Controller *controller);
//private:
//	struct Data;
//	Data *d;
//};
//
//}
//
//#endif
