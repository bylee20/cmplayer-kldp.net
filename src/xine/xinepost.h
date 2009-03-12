#ifndef XINE_XINEPOST_H
#define XINE_XINEPOST_H

#include <xine.h>
#include <QtGui/QWidget>
#include <QtCore/QStringList>
#include <QtCore/QMap>

namespace Xine {

class PostWidget;

class XinePost : public QObject {
public:
	class Param;
	class EnumParam;
	class IntParam;
	class DoubleParam;
	class StringParam;
	class BoolParam;
	typedef QMap<QString, Param*> ParamList;
	typedef QMapIterator<QString, Param*> ParamListIterator;
	XinePost(xine_post_t *post, const QString &name);
	~XinePost();
	void dumpInfo() const;
	xine_post_in_t *input() {return m_input;}
	xine_post_out_t *output() {return m_output;}
	PostWidget *widget(QWidget *parent);
	ParamList *parameters() {return &m_params;}
private:
	friend class PostWidget;
	friend class Param;
	xine_post_t *m_post;
	xine_post_api_t *m_api;
	QString m_name;
	char *m_data;
	xine_post_in_t *m_input;
	xine_post_out_t *m_output;
	ParamList m_params;
	PostWidget *m_widget;
};

class PostWidget : public QWidget {
public:
	PostWidget(XinePost *post, QWidget *parent);
private:
	XinePost *m_post;
	QList<QWidget*> m_controls;
};

class XinePost::Param {
public:
	enum Type {Enum, Int, Double, String, Bool, Invalid};
	virtual ~Param() {}
	Type type() const {return m_type;}
	const QString &name() const {return m_name;}
	int size() const {return m_size;}
	int offset() const {return m_offset;}
	bool isReadOnly() const {return m_readOnly;}
	const QString &description() const {return m_descr;}
protected:
	Param(Type type, XinePost *post): m_type(type), m_post(post) {}
	template<typename T>
	void apply(T t) {
		*(T*)(m_post->m_data + m_offset) = t;
		m_post->m_api->set_parameters(m_post->m_post, m_post->m_data);
	}
private:
	friend class XinePost;
	Param(): m_type(Invalid) {}
	Type m_type;
	XinePost *m_post;
	int m_internalType;
	QString m_name;
	int m_size;
	int m_offset;
	bool m_readOnly;
	QString m_descr;
};

class XinePost::IntParam : public XinePost::Param {
public:
	int minimum() const {return m_min;}
	int maximum() const {return m_max;}
	operator int () {return m_value;}
	void setValue(int value) {
		if(!isReadOnly() && m_value != value)
			apply<int>(m_value = qBound(m_min, value, m_max));
	}
	int value() const {return m_value;}
private:
	friend class XinePost;
	IntParam(XinePost *post): Param(Int, post) {}
	int m_value;
	int m_min;
	int m_max;
};

class XinePost::EnumParam : public XinePost::Param {
public:
	const QStringList &enumList() const {return m_enums;}
	const QString &stringValue() const {return m_enums[m_value];}
	int enumValue() const {return m_value;}
	void setEnumValue(int value) {
		if (!isReadOnly() && 0<=value && value < m_enums.size() && m_value != value)
			apply<int>(m_value = value);
	}
	void setStringValue(const QString &value) {
		if (!isReadOnly()) {int idx = m_enums.indexOf(value); if (idx!=-1) m_value = idx;}
	}
private:
	friend class XinePost;
	EnumParam(XinePost *post): Param(Enum, post) {}
	QStringList m_enums;
	int m_value;
};

class XinePost::DoubleParam : public XinePost::Param {
public:
	double minimum() const {return m_min;}
	double maximum() const {return m_max;}
	operator double () {return m_value;}
	void setValue(double value) {
		if (!isReadOnly() && m_value != value)
			apply<double>(m_value = qBound(m_min, value, m_max));
	}
	double value() const {return m_value;}
private:
	friend class XinePost;
	DoubleParam(XinePost *post): Param(Double, post) {}
	double m_value;
	double m_min;
	double m_max;
};

class XinePost::StringParam : public XinePost::Param {
public:
	const QString &value() const {return m_value;}
	void setValue(const QString &value) {if (!isReadOnly()) m_value = value;}
private:
	friend class XinePost;
	StringParam(XinePost *post): Param(String, post) {}
	QString m_value;
};

class XinePost::BoolParam : public XinePost::Param {
public:
	operator int () {return static_cast<int>(m_value);}
	operator bool () {return m_value;}
	void setValue(bool value) {
		if (!isReadOnly() && m_value != value)
			apply<int>(m_value = value);
	}
	bool value() const {return m_value;}
private:
	friend class XinePost;
	BoolParam(XinePost *post): Param(Bool, post) {}
	bool m_value;
};

}

#endif /* XINEPOST_H_ */
