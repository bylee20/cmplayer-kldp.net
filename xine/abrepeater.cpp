//#include "subtitle.h"
//#include "subtitleoutput.h"
//#include "playengine.h"
//#include "abrepeater.h"
//
//namespace MPlayer {
//
//struct ABRepeater::Data {
//	PlayEngine *engine;
//	qint64 a, b;
//	bool repeating;
//	int times, nth;
//};
//
//ABRepeater::ABRepeater(PlayEngine *parent)
//: QObject(parent), d(new Data) {
//	d->engine = parent;
//	d->a = d->b = -1;
//	d->repeating = false;
//	d->times = d->nth = 0;
//}
//
//ABRepeater::~ABRepeater() {
//	delete d;
//}
//
//void ABRepeater::slotTick(qint64 time) {
//	if (d->repeating && time > d->b) {
//		d->engine->seek(d->a - 1000);
//		if (d->times > 0 && d->times == ++d->nth)
//			stop();
//	}
//}
//
//qint64 ABRepeater::setAToCurrentTime() {
//	return (d->a = d->engine->currentTime());
//}
//
//qint64 ABRepeater::setBToCurrentTime() {
//	return (d->b = d->engine->currentTime());
//}
//
//qint64 ABRepeater::setAToSubtitleTime() {
//	SubtitleOutput *subout = d->engine->subtitleOutput();
//	if (!subout)
//		return d->a = -1;
//	qint64 curTime = d->engine->currentTime();
//	const Subtitle &sub = subout->currentSubtitle();
//	Subtitle::const_iterator it = sub.find(curTime);
//	if (it == sub.end() && (it = sub.upperBound(curTime)) != sub.begin())
//		--it;
//	return (d->a = it.key());
//}
//
//qint64 ABRepeater::setBToSubtitleTime() {
//	SubtitleOutput *subout = d->engine->subtitleOutput();
//	if (!subout)
//		return d->b = -1;
//	qint64 curTime = d->engine->currentTime();
//	const Subtitle &sub = subout->currentSubtitle();
//	Subtitle::const_iterator it = sub.upperBound(curTime);
//	return (d->a = it == sub.end() ? d->engine->totalTime() : it.key());
//}
//
//void ABRepeater::setA(qint64 a) {
//	d->a = a;
//}
//
//void ABRepeater::setB(qint64 b) {
//	d->b = b;
//}
//
//bool ABRepeater::repeat(qint64 a, qint64 b, int times) {
//	d->a = a;
//	d->b = b;
//	return start(times);
//}
//
//bool ABRepeater::isRepeating() {
//	return d->repeating;
//}
//
//qint64 ABRepeater::a() const {
//	return d->a;
//}
//
//qint64 ABRepeater::b() const {
//	return d->b;
//}
//
//bool ABRepeater::start(int times) {
//	if (d->repeating)
//		stop();
//	//d->timer->stop();
//	d->times = times;
//	d->nth = 0;
//	if (d->repeating = (d->a >= 0 && d->b > d->a))
//		connect(d->engine, SIGNAL(tick(qint64)), this, SLOT(slotTick(qint64)));
//	return d->repeating;
//}
//
//void ABRepeater::stop() {
//	disconnect(d->engine, 0, this, 0);
//	d->repeating = false;
//}
//
//}
//
