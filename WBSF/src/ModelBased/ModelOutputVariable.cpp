//****************************************************************************
// File:	ModelOutputVariable.cpp
// Class:	CModelOutputVariableDef
//****************************************************************************
// 15/09/2008  Rémi Saint-Amant	Created from old file
// 24/11/2008  Rémi Saint-Amant	Add binary serialisation, add before, after value
// 10/02/2010  Rémi Saint-Amant	New functionnality. Add title and equation
//****************************************************************************


#include "ModelOutputVariable.h"

using namespace std;
namespace WBSF
{


	const char* CModelOutputVariableDef::MEMBERS_NAME[NB_MEMBERS] = { "Name", "Title", "Units", "Description", "TimeMode", "Precision", "Equation", "Climatic" };

	CModelOutputVariableDef::CModelOutputVariableDef()
	{
		Reset();
	}


	CModelOutputVariableDef::CModelOutputVariableDef(string name, string title, string units, string description, CTM TM, short precision, string equation, size_t climaticVariable)
	{
		m_name = name;
		m_title = title;
		m_units = units;
		m_description = description;
		m_TM = TM;
		m_precision = precision;
		m_equation = equation;
		m_climaticVariable = climaticVariable;


	}

	CModelOutputVariableDef::CModelOutputVariableDef(const CModelOutputVariableDef& in)
	{
		operator=(in);
	}

	CModelOutputVariableDef::~CModelOutputVariableDef()
	{
	}

	void CModelOutputVariableDef::Reset()
	{
		m_name.clear();
		m_title.clear();
		m_units.clear();
		m_description.clear();
		m_TM = CTM(CTM::ATEMPORAL, CTM::FOR_EACH_YEAR);
		m_precision = 4;
		m_equation.clear();
		m_climaticVariable = UNKNOWN_POS;

	}

	CModelOutputVariableDef& CModelOutputVariableDef::operator = (const CModelOutputVariableDef& in)
	{
		if (&in != this)
		{
			m_name = in.m_name;
			m_title = in.m_title;
			m_units = in.m_units;
			m_description = in.m_description;
			m_TM = in.m_TM;
			m_precision = in.m_precision;
			m_equation = in.m_equation;
			m_climaticVariable = in.m_climaticVariable;
		}

		return *this;
	}

	bool CModelOutputVariableDef::operator == (const CModelOutputVariableDef& in)const
	{
		bool bEqual = true;

		if (m_name != in.m_name)bEqual = false;
		if (m_title != in.m_title)bEqual = false;
		if (m_units != in.m_units)bEqual = false;
		if (m_description != in.m_description)bEqual = false;
		if (m_TM != in.m_TM)bEqual = false;
		if (m_precision != in.m_precision)bEqual = false;
		if (m_equation != in.m_equation)bEqual = false;
		if (m_climaticVariable != in.m_climaticVariable)bEqual = false;


		return bEqual;
	}


	string CModelOutputVariableDef::GetMember(size_t i)const
	{
		assert(i >= 0 && i < NB_MEMBERS);

		string str;
		switch (i)
		{
		case NAME:
			str = m_name;
			break;
		case TITLE:
			str = m_title;
			break;
		case UNITS:
			str = m_units;
			break;
		case DESCRIPTION:
			str = m_description;
			break;
		case TIME_MODE:
			str = m_TM.to_string();
			break;
		case PRECISION:
			str = to_string(m_precision);
			break;
		case EQUATION:
			str = m_equation;
			break;
		case CLIMATIC_VARIABLE:
			str = to_string(m_climaticVariable);
			break;
		default:
			assert(false);
		}

		return str;
	}

	void CModelOutputVariableDef::SetMember(size_t i, const string& str)
	{
		assert(i >= 0 && i < NB_MEMBERS);

		switch (i)
		{
		case NAME:
			m_name = str;
			break;
		case TITLE:
			m_title = str;
			break;
		case UNITS:
			m_units = str;
			break;
		case DESCRIPTION:
			m_description = str;
			break;
		case TIME_MODE:
			m_TM = str;//to_CTM(str);
			break;
		case PRECISION:
			m_precision = ToShort(str);
			break;
		case EQUATION:
			m_equation = str;
			break;
		case CLIMATIC_VARIABLE:
			m_climaticVariable = ToSizeT(str);
			break;
		default:
			assert(false);
		}
	}

	void CModelOutputVariableDef::write_xml(pugi::xml_node node)const
	{
		node.append_child(GetMemberName(NAME)).set_value(m_name.c_str());
		node.append_child(GetMemberName(TITLE)).set_value(m_title.c_str());
		node.append_child(GetMemberName(UNITS)).set_value(m_units.c_str());
		node.append_child(GetMemberName(DESCRIPTION)).set_value(m_description.c_str());
		node.append_child(GetMemberName(TIME_MODE)).set_value(m_TM.to_string().c_str());
		node.append_child(GetMemberName(PRECISION)).set_value(to_string(m_precision).c_str());
		node.append_child(GetMemberName(EQUATION)).set_value(m_equation.c_str());
		node.append_child(GetMemberName(CLIMATIC_VARIABLE)).set_value(to_string(int(m_climaticVariable)).c_str());
	}

	void CModelOutputVariableDef::read_xml(const pugi::xml_node node)
	{
		m_name = node.child(GetMemberName(NAME)).text().as_string();
		m_title = node.child(GetMemberName(TITLE)).text().as_string();
		m_units = node.child(GetMemberName(UNITS)).text().as_string();
		m_description = node.child(GetMemberName(DESCRIPTION)).text().as_string();
		m_TM = node.child(GetMemberName(TIME_MODE)).text().as_string();
		m_precision = node.child(GetMemberName(PRECISION)).text().as_int();
		m_equation = node.child(GetMemberName(EQUATION)).text().as_string();
		m_climaticVariable = node.child(GetMemberName(CLIMATIC_VARIABLE)).text().as_int();
	}

	//********************************************************************************************************
	const char* CModelOutputVariableDefVector::XML_FLAG = "OutputVariable";

	CParameterVector CModelOutputVariableDefVector::GetParametersVector()const
	{
		const CModelOutputVariableDefVector& me = *this;

		CParameterVector out(me.size());
		for (size_t i = 0; i < me.size(); i++)
		{
			std::string type = me[i].m_TM.to_string();
			std::string name = me[i].m_name;
			out[i] = CParameter(name, type, false);
		}

		return out;
	}


	CWVariables CModelOutputVariableDefVector::GetWVariables()const
	{
		CWVariables variables;

		for (const_iterator it = begin(); it != end(); it++)
		{
			if (it->m_climaticVariable != UNKNOWN_POS)
				variables.set(it->m_climaticVariable);
		}


		return variables;
	}
}
