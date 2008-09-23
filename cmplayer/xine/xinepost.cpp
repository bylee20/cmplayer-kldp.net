#include "xinepost.h"
#include "xineengine.h"
#include <QtCore/QDebug>
#include <QtGui/QLineEdit>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

namespace Backend {

namespace Xine {

PostWidget::PostWidget(XinePost *post, QWidget *parent)
: QWidget(parent), m_post(post) {
	QVBoxLayout *vbox = new QVBoxLayout;
	XinePost::ParamListIterator it(m_post->m_params);
	while(it.hasNext()) {
		vbox->addWidget(new QLabel(it.peekNext().value()->description()));
		QWidget *control = 0;
		QHBoxLayout *hbox = new QHBoxLayout;
		switch(it.peekNext().value()->type()) {
		case XinePost::Param::Bool: {
			XinePost::BoolParam *p = static_cast<XinePost::BoolParam*>(it.peekNext().value());
			QCheckBox *check = new QCheckBox(p->name(), this);
			check->setChecked(p->value());
			hbox->addWidget(check);
			control = check;
			break;
		} case XinePost::Param::Double: {
			XinePost::DoubleParam *p = static_cast<XinePost::DoubleParam*>(it.peekNext().value());
			hbox->addWidget(new QLabel(p->name(), this));
			QDoubleSpinBox *spin = new QDoubleSpinBox(this);
			spin->setMinimum(p->minimum());
			spin->setMaximum(p->maximum());
			spin->setAccelerated(true);
			spin->setValue(p->value());
			hbox->addWidget(spin);
			control = spin;
			break;
		} case XinePost::Param::Enum: {
			XinePost::EnumParam *p = static_cast<XinePost::EnumParam*>(it.peekNext().value());
			hbox->addWidget(new QLabel(p->name()));
			QComboBox *combo = new QComboBox(this);
			combo->addItems(p->enumList());
			combo->setCurrentIndex(p->enumValue());
			hbox->addWidget(combo);
			control = combo;
			break;
		} case XinePost::Param::Int: {
			XinePost::IntParam *p = static_cast<XinePost::IntParam*>(it.peekNext().value());
			hbox->addWidget(new QLabel(p->name(), this));
			QSpinBox *spin = new QSpinBox(this);
			spin->setMinimum(p->minimum());
			spin->setMaximum(p->maximum());
			spin->setAccelerated(true);
			spin->setValue(p->value());
			hbox->addWidget(spin);
			control = spin;
			break;
		} case XinePost::Param::String: {
			XinePost::StringParam *p = static_cast<XinePost::StringParam*>(it.peekNext().value());
			hbox->addWidget(new QLabel(p->name()));
			QLineEdit *edit = new QLineEdit(p->value());
			edit->setMaxLength(p->size());
			hbox->addWidget(edit);
			break;
		} default:
			hbox->addWidget(new QLabel(it.peekNext().value()->name()));
			break;
		}
		vbox->addLayout(hbox);
		it.next();
		if (it.hasNext()) {
			QFrame *line = new QFrame(this);
			line->setFrameShape(QFrame::HLine);
			line->setFrameShadow(QFrame::Sunken);
			vbox->addWidget(line);
		}
		m_controls.append(control);
	}
	setLayout(vbox);
}

XinePost::XinePost(xine_post_t *post, const QString &name)
: m_post(post), m_api(0), m_name(name), m_data(0), m_input(0), m_output(0), m_widget(0) {
	xine_post_in_t *apiIn = xine_post_input(m_post, "parameters");
	if (!apiIn)
		return;
	m_api = reinterpret_cast<xine_post_api_t*>(apiIn->data);
	xine_post_api_descr_t *apiDescr = m_api->get_param_descr();
	xine_post_api_parameter_t *apiParam = apiDescr->parameter;

	m_data = new char[apiDescr->struct_size];
	m_api->get_parameters(m_post, m_data);
	while (apiParam->type != POST_PARAM_TYPE_LAST) {
		Param *param = 0;
		switch (apiParam->type) {
		case POST_PARAM_TYPE_INT: {
			if (apiParam->enum_values) {
				EnumParam *pEnum = new EnumParam(this);
				pEnum->m_value = *reinterpret_cast<int*>(m_data+apiParam->offset);
				for (int i = 0; apiParam->enum_values[i]; ++i)
					pEnum->m_enums.append(apiParam->enum_values[i]);
				param = pEnum;
			} else {
				IntParam *pInt = new IntParam(this);
				pInt->m_value = *reinterpret_cast<int*>(m_data+apiParam->offset);
				pInt->m_min = apiParam->range_min;
				pInt->m_max = apiParam->range_max;
				param = pInt;
			}
			param->m_internalType = POST_PARAM_TYPE_INT;
			break;
		} case POST_PARAM_TYPE_DOUBLE: {
			DoubleParam *pDouble = new DoubleParam(this);
			pDouble->m_value = *reinterpret_cast<double*>(m_data+apiParam->offset);
			pDouble->m_min = apiParam->range_min;
			pDouble->m_max = apiParam->range_max;
			pDouble->m_internalType = POST_PARAM_TYPE_DOUBLE;
			param = pDouble;
			break;
		} case POST_PARAM_TYPE_CHAR:
		case POST_PARAM_TYPE_STRING: {
			StringParam *pString = new StringParam(this);
			pString->m_value = (m_data + apiParam->offset);
			if (apiParam->type == POST_PARAM_TYPE_CHAR)
				pString->m_internalType = POST_PARAM_TYPE_CHAR;
			else
				pString->m_internalType = POST_PARAM_TYPE_STRING;
			param = pString;
			break;
		} case POST_PARAM_TYPE_STRINGLIST:
			qWarning("not implemented\n");
			break;
		case POST_PARAM_TYPE_BOOL: {
			BoolParam *pBool = new BoolParam(this);
			pBool->m_value = *reinterpret_cast<int*>(m_data+apiParam->offset) != 0;
			pBool->m_internalType = POST_PARAM_TYPE_BOOL;
			param = pBool;
			break;
		} default:
			break;
		}
		if (param) {
			param->m_name = apiParam->name;
			param->m_descr = apiParam->description;
			param->m_offset = apiParam->offset;
			param->m_readOnly = apiParam->readonly;
			param->m_size = apiParam->size;
			m_params.insert(param->m_name, param);
		}
		++apiParam;
	}
	m_input = xine_post_input(m_post, "video");
	if (!m_input)
		m_input = xine_post_input(m_post, "video in");
	if(!m_input)
		m_input = xine_post_input(m_post, "audio");
	if(!m_input)
		m_input = xine_post_input(m_post, "audio in");
	m_output = xine_post_output(m_post, "video");
	if(!m_output)
		m_output = xine_post_output(m_post, "video out");
	if(!m_output)
		m_output = xine_post_output(m_post, "audio");
	if(!m_output)
		m_output = xine_post_output(m_post, "audio out");
	if(!m_output)
		m_output = xine_post_output(m_post, xine_post_list_outputs(m_post)[0]);
}

XinePost::~XinePost() {
	if (m_widget)
		delete m_widget;
	for (ParamList::iterator it = m_params.begin(); it != m_params.end(); ++it)
		delete it.value();
	delete[] m_data;
	if (m_post)
		xine_post_dispose(XineEngine::get()->xine(), m_post);
}

PostWidget *XinePost::widget(QWidget *parent) {
	if (!parent) {
		if (!m_widget)
			m_widget = new PostWidget(this, 0);
		return m_widget;
	}
	return new PostWidget(this, parent);
}

void XinePost::dumpInfo() const {
	qDebug() << "Post:" << qPrintable(m_name);
	if (!m_post) {
		qDebug() << "Invalid Post!";
		return;
	} else if (m_params.isEmpty()) {
		qDebug() << "No Parameters.";
		return;
	}
	ParamList::const_iterator it = m_params.begin();
	for (int i=0; it != m_params.end(); ++it) {
		qDebug() << "Parameter" << ++i << ":";
		qDebug() << "Name:" << qPrintable((*it)->name());
		qDebug() << "Description:" << qPrintable((*it)->description());
		qDebug() << "IsReadOnly:" << (*it)->isReadOnly();
		qDebug() << "Size:" << (*it)->size();
		switch ((*it)->type()) {
		case Param::Enum: {
			qDebug() << "Type: Enum";
			EnumParam *pEnum = static_cast<EnumParam*>(*it);
			qDebug() << "EnumList:" << pEnum->enumList();
			qDebug() << "CurrentValue:" << pEnum->stringValue();
			break;
		} case Param::Int: {
			qDebug() << "Type: Int";
			IntParam *pInt = static_cast<IntParam*>(*it);
			qDebug() << "Range:" << pInt->minimum() << "~" << pInt->maximum();
			qDebug() << "CurrentValue:" << pInt->value();
			break;
		} case Param::Double: {
			qDebug() << "Type: Double";
			DoubleParam *pDouble = static_cast<DoubleParam*>(*it);
			qDebug() << "Range:" << pDouble->minimum() << "~" << pDouble->maximum();
			qDebug() << "CurrentValue:" << pDouble->value();
			break;
		} case Param::String: {
			qDebug() << "Type: String";
			qDebug() << "CurrentValue:" << static_cast<StringParam*>(*it)->value();
			break;
		} case Param::Bool: {
			qDebug() << "Type: Bool";
			qDebug() << "CurrentValue:" << static_cast<BoolParam*>(*it)->value();
			break;
		} default:
			qDebug() << "Type: Invalid";
		}
	}
}

}

}
