//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
#pragma once

#include <vector>
#include <deque>

#include "Basic/muparser/muParser.h"
#include "Geomatic/GDALDatasetEx.h"
#include "Geomatic/LandsatDataset2.h"



namespace WBSF
{

//class CMTParserVariable
//{
//public:
//
//	std::string m_name;
//	mu::value_type m_value;
//};



class CImageBand
{
public:

    CImageBand(size_t image = WBSF::NOT_INIT, size_t band = WBSF::NOT_INIT) :
        m_image(image),
        m_band(band)
    {
    }

    bool is_init()const
    {
        return m_image != WBSF::NOT_INIT && m_band != WBSF::NOT_INIT;
    }

    size_t m_image;
    size_t m_band;
};

class  CVarDef
{
public:
    std::string m_name;
    double m_value;
    double m_noData;
    CImageBand m_ib;
};

//class CEquation
//{
//public:

//	std::string m_name;
//	std::string m_equation;
//	std::string m_pre_compile_equation;


//	std::vector <CLayerDef> m_layers;
//	double m_value;//value of the result for this equation
//};



template<typename T>
struct WBSFMathImpl
{
    static T NBR(T v1, T v2)
    {
        return (v1 - v2) / (v1 + v2);
    }
    static T round(T v1, T digit)
    {
        v1 *= pow(10, digit);
        v1 = T(std::round(v1));
        v1 /= pow(10, digit);
        return v1;
    }
};


class CVarFactory
{
public:

    //void SetEquations(const CEquation& in) { m_equation = in; }
    //const CEquation& GetEquations()const { return m_equation; }
    //CEquation& GetEquations() { return m_equation; }



    static double* AddVariableCallback(const char* a_szName, void* pVarFactory);

    //void CreateNewVariable(const char* a_szName, bool bRealVar = false)
    //{
    //	if (bRealVar)
    //		m_vars.push_back({ a_szName, 0 });
    //	else
    //		m_tmp_vars.push_back({ a_szName, 0 });
    //
    //}

    //const std::vector < CVarDef>& GetVars()const { return m_vars; }
    //std::vector < CVarDef>& GetVars() { return m_vars; }

    std::deque<CVarDef> m_vars;
    std::deque<CVarDef> m_tmp_vars;


    //protected:

    //CEquation m_equation;


};


//typedef std::pair<size_t, size_t>  CImageBandPos;
//typedef std::vector<CImageBandPos> CImageBandPosVector;
//CImageBandPos GetImageBand(const CMTParserVariable& var);

//class ParserFactory
//{
//public:
//
//
//	CVarFactory m_factory;
//};

//typedef std::vector< mu::Parser > ParserVector;
//typedef std::vector<CMTParserVariable> CMTParserVariableVector;
//typedef std::vector<CGDALDatasetEx> CGDALDatasetExVector;


class CImageParser
{
public:

    CImageParser();
    CImageParser(const CImageParser& in);
    CImageParser& operator = (const CImageParser& in);

    ERMsg Compile(std::string formula, const std::vector<CGDALDatasetEx>& inputDSVector);

    //template<typename T>
    //void Evaluate(const std::deque<CRasterWindow>& windows, std::vector<float>& outputs);
    double Evaluate(const std::vector<double>& var_values);


    const std::deque<CVarDef>& GetParsedVars()const
    {
        return m_vars;
    }

protected:


    mu::Parser m_parser;
    std::deque<CVarDef> m_vars;
    std::deque<CVarDef> m_tmp_vars;

//		static std::string ReplaceOldStyle(std::string formula);
};
}
