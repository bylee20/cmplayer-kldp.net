#ifndef DUMMYENGINE_H
#define DUMMYENGINE_H

#include <core/playengine.h>
#include <core/info.h>

class DummyEngine : public Core::PlayEngine {
public:
	class Info : public Core::Info {
	public:
		const QStringList &videoRenderer() const {return d.vr;}
		const QStringList &audioRenderer() const {return d.ar;}
		QString name() const {return d.name;}
		double maximumAmplifyingRate() const {return d.maxAmp;}
		QString compileVersion() const {return d.cv;}
		QString runtimeVersion() const {return d.rv;}
		static void copy(const Core::Info &other) {
			d.vr = other.videoRenderer();
			d.ar = other.audioRenderer();
			d.name = other.name();
			d.maxAmp = other.maximumAmplifyingRate();
			d.cv = other.compileVersion();
			d.rv = other.runtimeVersion();
		}
	private:
		struct Data {
			Data();
			QStringList vr, ar;
			double maxAmp;
			QString name, cv, rv;
		};
		static Data d;
	};
	DummyEngine(QObject *parent = 0);
	~DummyEngine();
	virtual const Core::Info &info() const {return m_info;}
	Info &info() {return m_info;}
private:
// public:
	virtual int currentTime() const {return 0;}
	virtual void play() {}
	virtual void stop() {}
	virtual void pause() {}
	virtual void seek(int /*time*/) {}
// protected:
//	virtual bool updateCurrentSpu(const QString &/*spu*/) {return false;}
//	virtual bool updateCurrentTrack(const QString &/*track*/) {return false;}
	virtual void updateVolume() {}
	virtual void updateSpeed(double /*speed*/) {}
	virtual bool updateVideoRenderer(const QString &/*name*/) {return false;}
	virtual bool updateAudioRenderer(const QString &/*name*/) {return false;}
// private:
	class Renderer;
	class Osd;
	Info m_info;
	Renderer *m_renderer;
};

#endif
