#ifndef XINEPOST_H_
#define XINEPOST_H_

#include <QString>
#include <QStringList>
#include <xine.h>

namespace Xine {

class XineEngine;

class XinePost {
public:
	class Param;
	class ParamEnum;
	class ParamInt;
	class ParamDouble;
	class ParamString;
	class ParamBool;
	XinePost(XineEngine *engine, const QString &name);
	~XinePost();
	void dumpInfo() const;
	xine_post_in_t *input() {return m_input;}
	xine_post_out_t *output() {return m_output;}
private:
	XineEngine *m_engine;
	xine_post_t *m_post;
	xine_post_api_t * m_api;
	QString m_name;
	char *m_data;
	xine_post_in_t *m_input;
	xine_post_out_t *m_output;
	QList<Param*> m_params;
};

class XinePost::Param {
public:
	enum Type {Enum, Int, Double, String, Bool};
	virtual ~Param() {}
	Type type() const {return m_type;}
	const QString &name() const {return m_name;}
	int size() const {return m_size;}
	int offset() const {return m_offset;}
	bool isReadOnly() const {return m_readOnly;}
	const QString &description() const {return m_descr;}
protected:
	Param(Type type): m_type(type) {}
private:
	friend class XinePost;
	Type m_type;
	int m_internalType;
	QString m_name;
	int m_size;
	int m_offset;
	bool m_readOnly;
	QString m_descr;
};

class XinePost::ParamInt : public XinePost::Param {
public:
	int minimum() const {return m_min;}
	int maximum() const {return m_max;}
	operator int () {return m_value;}
	void setValue(int value) {if(!isReadOnly()) m_value = qBound(m_min, value, m_max);}
	int value() const {return m_value;}
private:
	friend class XinePost;
	ParamInt(): Param(Int) {}
	int m_value;
	int m_min;
	int m_max;
};

class XinePost::ParamEnum : public XinePost::Param {
public:
	const QStringList &enumList() const {return m_enums;}
	const QString &stringValue() const {return m_enums[m_value];}
	int enumValue() const {return m_value;}
	void setEnumValue(int value) {
		if (!isReadOnly() && 0<=value && value < m_enums.size()) m_value = value;
	}
	void setStringValue(const QString &value) {
		if (!isReadOnly()) {int idx = m_enums.indexOf(value); if (idx!=-1) m_value = idx;}
	}
private:
	friend class XinePost;
	ParamEnum(): Param(Enum) {}
	QStringList m_enums;
	int m_value;
};

class XinePost::ParamDouble : public XinePost::Param {
public:
	double minimum() const {return m_min;}
	double maximum() const {return m_max;}
	operator double () {return m_value;}
	void setValue(double value) {if (!isReadOnly())	m_value = qBound(m_min, value, m_max);}
	double value() const {return m_value;}
private:
	friend class XinePost;
	ParamDouble(): Param(Double) {}
	double m_value;
	double m_min;
	double m_max;
};

class XinePost::ParamString : public XinePost::Param {
public:
	const QString &value() const {return m_value;}
	void setValue(const QString &value) {if (!isReadOnly()) m_value = value;}
private:
	friend class XinePost;
	ParamString(): Param(String) {}
	QString m_value;
};

class XinePost::ParamBool : public XinePost::Param {
public:
	operator int () {return static_cast<int>(m_value);}
	operator bool () {return m_value;}
	void setValue(bool value) {if (!isReadOnly()) m_value = value;}
	bool value() const {return m_value;}
private:
	friend class XinePost;
	ParamBool(): Param(Bool) {}
	bool m_value;
};


}

#endif /* XINEPOST_H_ */
