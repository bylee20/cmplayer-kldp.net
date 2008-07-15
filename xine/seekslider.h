//#ifndef MPLAYERSEEKSLIDER_H
//#define MPLAYERSEEKSLIDER_H
//
//#include "cslider.h"
//
//namespace MPlayer {
//
//class PlayEngine;
//
//class SeekSlider : public CSlider {
//	Q_OBJECT
//public:
//	SeekSlider(PlayEngine *m_engine, QWidget *parent = 0);
//public slots:
//	void setTotalTime(qint64 ms);
//	void seek(int msec);
//private slots:
//	inline void slotTick(qint64 ms) {m_tick = true; if (!m_drag) setValue(ms); m_tick = false;}
//	inline void setDraggingStarted() {m_drag = true;}
//	void setDraggingEnded();
//private:
//	PlayEngine *m_engine;
//	bool m_tick;
//	bool m_drag;
//	int m_dragPos;
//};
//
//}
//
//#endif
