#ifndef FRAGMENTPROGRAM_HPP
#define FRAGMENTPROGRAM_HPP

#include <QtOpenGL/QGLWidget>
#include <QtCore/QDebug>

class Interpreter {
public:
	Interpreter();
	QByteArray code() const {return m_code;}
	bool interpret(const QByteArray &src);
	QString errorMessage() const {
		if (!m_hasError)
			return QString();
		QString ret = "Inst : ";
		ret += m_errorInst;
		ret += "\nError: ";
		ret += m_errorMsg;
		return ret;
	}
private:
	void reset() {
		m_hasError = false;
		m_code.clear();
	}
	static bool isSeperator(char c) {
		return c == ' '|| c == '\n' || c == '\t' || c== '\r';
	}
	static bool skipSeperator(int &idx, const QByteArray &data) {
		for (; idx <data.size() && isSeperator(data[idx]); ++idx) ;
		return idx >= data.size();
	}
	static QByteArray midRef(const QByteArray &data, int from, int count) {
		return QByteArray::fromRawData(data.data() + from, count);
	}
//	void addWarning(const QString &msg) {

//	}
	int indexOf(const QByteArray &data, int from, const char *match, int len = -1) {
		if (len < 0)
			len = qstrlen(match);
		for (int i=from; i<data.size(); ++i) {
			const char c = data[i];
			for (int j=0; j<len; ++j) {
				if (c == match[j])
					return i;
			}
		}
		return -1;
	}


	void setError(const QString &msg) {
		m_errorMsg = msg;
		m_hasError = true;
	}
	static QList<QByteArray> split(const QByteArray &data, const char *sep) {
		const int len = qstrlen(sep);
		QList<QByteArray> ret;
		int from = -1;
		int count = 0;
		for (int i=0; i<data.size(); ++i) {
			const char c = data[i];
			bool isSep = false;
			for (int j=0; j<len; ++j) {
				if (c == sep[j]) {
					isSep = true;
					break;
				}
			}
			if (isSep) {
				if (from != -1) {
					const QByteArray token = data.mid(from, count).trimmed();
					if (!token.isEmpty())
						ret.push_back(token);
					from = -1;
					count = 0;
				}
			} else {
				if (from < 0)
					from = i;
				++count;
			}

		}
		if (from != -1) {
			const QByteArray token = data.mid(from, count).trimmed();
			if (!token.isEmpty())
				ret.push_back(token);
		}
		return ret;
	}

	bool parseDeclaration(const QByteArray &src);
	bool parseAssignment(const QByteArray &src);
	bool parseInstruction(const QByteArray &src);
	static int indexOfSeperator(const QByteArray &src, int from = 0) {
		for (int i=from; i<src.size(); ++i) {
			if (isSeperator(src[i]))
				return i;
		}
		return -1;
	}
	struct VarInfo {
		QByteArray type, name, init;
	};

	QByteArray m_code;
	bool m_hasError;
//	QByteArray m_curLine;
//	int m_errorLine, m_curLineNumber;
	QString m_errorMsg;
	QByteArray m_errorInst;
	QHash<QByteArray, VarInfo> m_var;
	QHash<char, QByteArray> op;
	QSet<QByteArray> cmd;
};

class FragmentProgram {
public:
	FragmentProgram(const QByteArray &code, QGLWidget *gl);
	~FragmentProgram();
	bool isAvailable() const;
	bool hasError() const;
private:
	typedef void (*_glProgramStringARB) (GLenum, GLenum, GLsizei, const GLvoid *);
	typedef void (*_glBindProgramARB) (GLenum, GLuint);
	typedef void (*_glDeleteProgramsARB) (GLsizei, const GLuint *);
	typedef void (*_glGenProgramsARB) (GLsizei, GLuint *);
	typedef void (*_glProgramLocalParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
	_glProgramStringARB glProgramStringARB;
	_glBindProgramARB glBindProgramARB;
	_glDeleteProgramsARB glDeleteProgramsARB;
	_glGenProgramsARB glGenProgramsARB;
	_glProgramLocalParameter4fARB glProgramLocalParameter4fARB;
	struct Data;
	Data *d;
};

#endif // FRAGMENTPROGRAM_HPP
