#ifndef VIDEOFRAME_HPP
#define VIDEOFRAME_HPP

#include <QtCore/QByteArray>
#include <QtCore/QSize>

class VideoFrame : public QByteArray {
public:
	enum Type {NoFrame, I420, YV12, YUY2, RGB};
	struct Info {
		Info(): type(NoFrame), pixelAspectRatio(1.0) {}
		bool operator == (const Info &rhs) const {
			return type == rhs.type && size == rhs.size
				&& qFuzzyCompare(pixelAspectRatio, rhs.pixelAspectRatio);
		}
		bool operator != (const Info &rhs) const {
			return !operator == (rhs);
		}
		void setSize(int w, int h) {size.setWidth(w); size.setHeight(h);}
		QSize size;
		Type type;
		double pixelAspectRatio;
	};
	VideoFrame() {}
	void setData(const void *data, int length) {
		static_cast<QByteArray&>(*this) = QByteArray((const char*)data, length);
	}
	bool isNull() const {return isEmpty() || m_info.size.isEmpty();}
	const Info &info() const {return m_info;}
	void setInfo(const Info &info) {m_info = info;}
private:
	Info m_info;
};

#endif // VIDEOFRAME_HPP
