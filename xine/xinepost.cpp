#include "xinepost.h"
#include "xineengine.h"
#include "xineaudio.h"
#include "xinevideo.h"
#include <QDebug>

namespace Xine {

XinePost::XinePost(XineEngine *engine, const QString &name)
: m_engine(engine), m_post(0), m_api(0), m_data(0), m_input(0), m_output(0) {
	if (!engine->xine() || !engine->audio()->port() || !engine->video()->port())
		return;
	m_post = xine_post_init(engine->xine(), name.toAscii(), 0,
			&engine->audio()->port(), &engine->video()->port());
	if (!m_post)
		return;
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
				ParamEnum *pEnum = new ParamEnum;
				pEnum->m_value = *reinterpret_cast<int*>(m_data+apiParam->offset);
				for (int i = 0; apiParam->enum_values[i]; ++i)
					pEnum->m_enums.append(apiParam->enum_values[i]);
				param = pEnum;
			} else {
				ParamInt *pInt = new ParamInt;
				pInt->m_value = *reinterpret_cast<int*>(m_data+apiParam->offset);
				pInt->m_min = apiParam->range_min;
				pInt->m_max = apiParam->range_max;
				param = pInt;
			}
			param->m_internalType = POST_PARAM_TYPE_INT;
			break;
		} case POST_PARAM_TYPE_DOUBLE: {
			ParamDouble *pDouble = new ParamDouble;
			pDouble->m_value = *reinterpret_cast<double*>(m_data+apiParam->offset);
			pDouble->m_min = apiParam->range_min;
			pDouble->m_max = apiParam->range_max;
			pDouble->m_internalType = POST_PARAM_TYPE_DOUBLE;
			param = pDouble;
			break;
		} case POST_PARAM_TYPE_CHAR:
		case POST_PARAM_TYPE_STRING: {
			ParamString *pString = new ParamString;
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
			ParamBool *pBool = new ParamBool;
			pBool->m_value = *reinterpret_cast<int*>(m_data+apiParam->offset) != 0;
			pBool->m_internalType = POST_PARAM_TYPE_BOOL;
			param = pBool;
			break;
		} default:
			break;
		}
		if (!param) {
			param->m_name = apiParam->name;
			param->m_descr = apiParam->description;
			param->m_offset = apiParam->offset;
			param->m_readOnly = apiParam->readonly;
			param->m_size = apiParam->size;
			m_params.append(param);
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
	for (int i=0; i<m_params.size(); ++i)
		delete m_params[i];
	delete[] m_data;
	if (m_post)
		xine_post_dispose(m_engine->xine(), m_post);
}

void XinePost::dumpInfo() const {
	qDebug() << qPrintable(m_name);
	if (!m_post) {
		qDebug() << "Invalid Post!";
		return;
	} else if (m_params.isEmpty()) {
		qDebug() << "No Parameters.";
		return;
	}
	for (int i=0; i<m_params.size(); ++i) {
		qDebug() << "Parameter" << i+1 << ":";
		qDebug() << "Name:" << qPrintable(m_params[i]->name());
		qDebug() << "Description:" << qPrintable(m_params[i]->description());
		qDebug() << "Is Read Only:" << m_params[i]->isReadOnly();
		qDebug() << "Size:" << m_params[i]->size();
		switch (m_params[i]->type()) {
		case Param::Enum: {
			qDebug() << "Type: Enum";
			ParamEnum *pEnum = static_cast<ParamEnum*>(m_params[i]);
			qDebug() << "EnumList:" << pEnum->enumList();
			qDebug() << "CurrentValue:" << pEnum->stringValue();
			break;
		} case Param::Int: {
			qDebug() << "Type: Int";
			ParamInt *pInt = static_cast<ParamInt*>(m_params[i]);
			qDebug() << "Range:" << pInt->minimum() << "~" << pInt->maximum();
			qDebug() << "CurrentValue:" << pInt->value();
			break;
		} case Param::Double: {
			qDebug() << "Type: Double";
			ParamDouble *pDouble = static_cast<ParamDouble*>(m_params[i]);
			qDebug() << "Range:" << pDouble->minimum() << "~" << pDouble->maximum();
			qDebug() << "CurrentValue:" << pDouble->value();
			break;
		} case Param::String: {
			qDebug() << "Type: String";
			ParamString *pString = static_cast<ParamString*>(m_params[i]);
			qDebug() << "CurrentValue:" << pString->value();
			break;
		} case Param::Bool: {
			qDebug() << "Type: Bool";
			ParamBool *pBool = static_cast<ParamBool*>(m_params[i]);
			qDebug() << "CurrentValue:" << pBool->value();
			break;
		} default:
			qDebug() << "Type: Invalid";
		}
	}
}

}
