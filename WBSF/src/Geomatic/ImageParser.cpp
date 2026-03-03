//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************


#include <cassert>

#include "Basic/OpenMP.h"


#include "Geomatic/ImageParser.h"



using namespace std;

namespace WBSF
{
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c)
    {
        return !std::isdigit(c);
    }) == s.end();
}

CImageBand GetImageBand(const std::string& name)
{
    CImageBand bi;


    //assert(var.m_name[0] == 'i');
    string::size_type pos_i = name.find('i');
    string::size_type pos_b = name.find('b');

    if (pos_i == 0 && pos_b != string::npos)
    {
        string str_i = name.substr(pos_i + 1, pos_b - 1);
        string str_b = name.substr(pos_b + 1);

        if (is_number(str_i) && is_number(str_b))
        {
            int i = std::stoi(str_i);
            int b = std::stoi(str_b);

            bi.m_image = size_t(i - 1);//zero base pos
            bi.m_band = size_t(b - 1);//zero base pos
        }
    }

    return bi;

}

// Factory function for creating new parser variables
// This could as well be a function performing database queries.
//double* CVarFactory::AddVariableCallback(const char* a_szName, void* pVarFactory)
//{
//    //static double afValBuf[100];
//    //static int iVal = -1;
//
//
//    // You can also use custom factory classes like for instance:
//    CVarFactory* pFactory = (CVarFactory*)pVarFactory;
//
//    //look to see if it's an equation result
//    /*for (size_t i = 0; i < pFactory->GetEquations().size(); i++)
//    {
//    	if (a_szName == pFactory->GetEquations()[i].m_name)
//    	{
//    		std::cout << "Equation variable \""
//    			<< a_szName << endl;
//
//    		return &(pFactory->GetEquations()[i].m_value);
//    	}
//    }*/
//
//    CImageBand bi = GetImageBand(a_szName);
//
//    //		pFactory->CreateNewVariable(a_szName, bi.is_init());
//
//    double* pReturn = nullptr;
//
//
//    /*if (bi.is_init())
//    {
//    	pFactory->m_vars.push_back({ a_szName,0 });
//    	pReturn = &pFactory->m_vars.back().m_value;
//    }
//    else
//    {
//    	pFactory->m_tmp_vars.push_back({ a_szName,0 });
//    	pReturn = &pFactory->m_tmp_vars.back().m_value;
//    }*/
//
//    return pReturn;
//}





//************************************************************************
//CImageParser

CImageParser::CImageParser()
{
}

CImageParser::CImageParser(const CImageParser& in)
{
    operator = (in);
}

CImageParser& CImageParser::operator = (const CImageParser& in)
{
    if (&in != this)
    {
        //m_parser.SetVarFactory(CVarFactory::AddVariableCallback, &m_factory);
        m_parser.SetExpr(in.m_parser.GetExpr());
        m_vars = in.m_vars;
        m_tmp_vars = in.m_tmp_vars;

        //set variables
        for (size_t v = 0; v < m_vars.size(); v++)
        {
            m_parser.DefineVar(m_vars[v].m_name, &m_vars[v].m_value);
        }
        //set tmp variables
        for (size_t v = 0; v < m_tmp_vars.size(); v++)
        {
            m_parser.DefineVar(m_tmp_vars[v].m_name, &m_tmp_vars[v].m_value);
        }


        //m_parser.Eval();
    }

    return *this;
}

//string CImageParser::ReplaceOldStyle(string formula)
//{
//	string newFormula;

//	string::size_type pos = 0;
//	bool bContinue = true;
//	//int newPos = formula.Find("GetPixel(", pos);
//	//int newPos = formula.Find("Pixel(", pos);

//	while (bContinue)
//	{
//		string::size_type pos2 = formula.find("][", pos);
//		if (pos2 != string::npos)
//		{
//			string::size_type pos1 = formula.find_last_of("[", pos2);
//			string::size_type pos3 = formula.find("]", pos2 + 2);
//			//assert( newPos2-newPos1 <= 3 );//up to 999 image
//			//assert( newPos3-newPos2 <= 5 );//up tyo 99 999 bands
//			if (pos1 != string::npos && pos3 != string::npos)
//			{
//				newFormula += formula.substr(pos, pos1 - pos);

//				string strVar = formula.substr(pos1, pos3 - pos1 + 1);
//				string::size_type subPos = 0;

//				string posI = Tokenize(strVar, "[]", subPos, true);
//				if (subPos != string::npos)
//				{
//					string posB = Tokenize(strVar, "[]", subPos, true);
//					//newFormula += formula.substr(pos, pos-pos1);
//					newFormula += "i" + posI + "b" + posB;
//					pos = pos3 + 1;
//				}
//				else
//				{
//					//????? probably invalid formula...will be detected later
//					newFormula += formula.substr(pos);
//					bContinue = false;
//				}
//			}
//			else
//			{
//				//????? probably invalid formula...will be detected later
//				newFormula += formula.substr(pos);
//				bContinue = false;
//			}
//		}
//		else
//		{
//			newFormula += formula.substr(pos);
//			bContinue = false;
//		}
//	}

//	return newFormula;
//}




/*mu::value_type CImageParser::GetPixel(int nBulkIdx, int nThreadIdx, mu::value_type v1in, mu::value_type v2in)
{
int v1 = (int)v1in;
int v2 = (int)v2in;
if( v1>=0 && v1<m_variablePos.size() &&
v2>=0 && v2<m_variablePos[v1].size() )
{
int pos = m_variablePos[v1][v2];
m_vars[pos].
m_pBandHolder.get()->GetPixel();
}


return nBulkIdx + nThreadIdx + v1;
}
*/
//static double * pVal1[2] = {0};
/*mu::value_type* CImageParser::VariableFactory(const mu::char_type* pName, void* pData)
{
CMTParserVariable var(pName);

CImageParser* pMe = static_cast<CImageParser*>(pData);
pMe->m_vars.push_back(var);

//pVal1[pMe->m_vars.size()-1] = &(pMe->m_vars.back().m_value);
return &(pMe->m_vars.back().m_value);
}
*/





string GetOldStyle(const string& name)
{
    assert(name[0] == 'i');
    string::size_type pos = name.find('b');
    assert(pos != string::npos);

    return string("[") + name.substr(1, pos - 1) + "][" + name.substr(pos + 1) + "]";
}


//---------------------------------------------------------------------------
mu::value_type BulkFun1(int nBulkIdx, int nThreadIdx, mu::value_type v1)
{
    // Note: I'm just doing something with all three parameters to shut
    // compiler warnings up!
    return nBulkIdx + nThreadIdx + v1;
}




ERMsg CImageParser::Compile(std::string formula, const vector <CGDALDatasetEx>& inputDSVector)
{
    ERMsg msg;


    if (formula.empty())
        return msg;

    try
    {

        m_parser.DefineFun("round", WBSFMathImpl<mu::value_type>::round, false);
        m_parser.SetExpr(formula);

        for (auto v : m_parser.GetUsedVar())
        {
            string name = v.first;
            CImageBand ib = GetImageBand(name);

            if (ib.is_init())
            {
                if (ib.m_image < inputDSVector.size())
                {
                    if (ib.m_band < inputDSVector[ib.m_image].GetRasterCount())
                    {
                        //init noData value
                        double noData = MISSING_NO_DATA;
                        if (inputDSVector[ib.m_image].HaveNoData(ib.m_band))
                            noData = inputDSVector[ib.m_image].GetNoData(ib.m_band);

                        m_vars.push_back({ name, 0, noData, ib });
                    }
                    else
                    {
                        //msg.ajoute(string("ERROR: band index (1..") + to_string(inputDSVector[ib.m_image].GetRasterCount()) + ") is incorrecte in expression: " + (bOldStyle ? GetOldStyle(vars[v].m_name) : vars[v].m_name));
                        msg.ajoute(string("ERROR: band index (1..") + to_string(inputDSVector[ib.m_image].GetRasterCount()) + ") is incorrecte in expression: " + name);
                    }
                }
                else
                {
                    //msg.ajoute(string("ERROR: image index (1..") + to_string(inputDSVector.size()) + ") is incorrecte in expression: " + (bOldStyle ? GetOldStyle(vars[v].m_name) : vars[v].m_name));
                    msg.ajoute(string("ERROR: image index (1..") + to_string(inputDSVector.size()) + ") is incorrecte in expression: " + name);
                }
            }
            else
            {
                m_tmp_vars.push_back({ name, 0, 0, ib });
            }
        }

        if (msg)
        {
            //set variables
            for (size_t v = 0; v < m_vars.size(); v++)
            {
                m_parser.DefineVar(m_vars[v].m_name, &m_vars[v].m_value);
            }
            //set tmp variables
            for (size_t v = 0; v < m_tmp_vars.size(); v++)
            {
                m_parser.DefineVar(m_tmp_vars[v].m_name, &m_tmp_vars[v].m_value);
            }

            //now test to see if all is OK
            try
            {
                auto var = m_parser.GetVar();
                auto usedVar = m_parser.GetUsedVar();
                m_parser.Eval();
            }
            catch (mu::ParserError& e)
            {
                msg.ajoute(e.GetMsg());
            }


        }
        /*for (size_t t = 1; t < m_factories.size(); t++)
        {

        	std::deque<CVarDef>& vars_0 = m_factories[0].m_vars;
        	std::deque<CVarDef>& vars_t = m_factories[t].m_vars;
        	assert(vars_t.size() == vars_0.size());

        	for (size_t v = 0; v < vars_t.size(); v++)
        	{
        		vars_t[v].m_ib = vars_0[v].m_ib;
        		vars_t[v].m_noData = vars_0[v].m_noData;
        	}

        }*/
    }
    catch (mu::ParserError e)
    {
        msg.ajoute(e.GetMsg());
    }


    return msg;
}

//void CImageParser::Evaluate(const std::deque<CRasterWindow>& windows, std::vector<float>& outputs)
//{
//	assert(!windows.empty());
//	assert(!windows.front().empty());
//	assert(!outputs.empty());
//	assert(!windows.front().size() == outputs.size());
//	//assert(thread < m_factories.size());
//
//	//	int thread = omp_get_thread_num();
//	double value = MISSING_NO_DATA;
//	//bool bMissingValue = false;
//
//	//assert(m_factory.m_vars.size() == var_values.size());
//
//	try
//	{
//		for (size_t v = 0; v < m_vars.size(); v++)
//		{
//			size_t i = m_vars[v].m_ib.m_image;
//			size_t b = m_vars[v].m_ib.m_band;
//			m_parser.DefineVar(m_vars[v].m_name, windows[i][b].data().data());
//		}
//
//		//m_parser.EnableDebugDump(true, true);
//		value = m_parser.Eval(outputs.data(), outputs.size());
//	}
//	catch (mu::ParserError& e)
//	{
//		assert(false);//already verify
//		string msg = e.GetMsg();
//	}
//
//	//}
//
//	return value;
//}

double CImageParser::Evaluate(const std::vector<double>& var_values)
{
    assert(m_vars.size() == var_values.size());

    double value = MISSING_NO_DATA;

    for (size_t v = 0; v < m_vars.size(); v++)
    {
        m_vars[v].m_value = var_values[v];
    }

    try
    {
        value = m_parser.Eval();
    }
    catch (mu::ParserError& )
    {
        assert(false);//already verify
    }


    return value;
}




//***************************************************************************

//MTDOUBLE GetPixelFct::evaluate(unsigned int nbArgs, const MTDOUBLE *pArg)
//{

//	assert(nbArgs == 2);
//	size_t imageNo = size_t(pArg[0]) - 1;
//	size_t bandNo = size_t(pArg[1]) - 1;

//	MTDOUBLE val = 0;
//	/*
//	//convert
//	if( imageNo>=0 && imageNo<m_DS.size() )
//	{
//	if( !m_DS[imageNo].IsOpen() )
//	{
//	ERMsg msg = m_DS[imageNo].Open(m_filePaths[imageNo] );
//	if( !msg )
//	{
//	MTExcepData data( MTPARSINGEXCEP_InternalError );
//	MTParserException exceps;
//	string desc;
//	desc.Format( "Unable to open image %s", m_filePaths[imageNo] );
//	//SYGetMessage(msg);
//	exceps.add(__LINE__,  _T(__FILE__), 0, data, desc.c_str());
//	throw(exceps);
//	}

//	}

//	if( bandNo>=0 && bandNo<m_DS[imageNo].GetRasterCount() )
//	{
//	val = (MTDOUBLE) m_DS[imageNo]->RasterIO(>GetPixel(bandNo, m_x);
//	}
//	else
//	{
//	MTExcepData data( MTPARSINGEXCEP_InternalError );
//	MTParserException exceps;
//	string desc;
//	desc.Format( "Invalid argument in function GetPixel(%d,%d)", imageNo+1, bandNo+1);

//	exceps.add(__LINE__,  _T(__FILE__), 0, data, desc.c_str());
//	throw(exceps);
//	}
//	}
//	else
//	{
//	MTExcepData data( MTPARSINGEXCEP_InternalError );
//	MTParserException exceps;
//	string desc;
//	desc.Format( "Invalid argument in function GetPixel(%d,%d)", imageNo+1, bandNo+1);

//	exceps.add(__LINE__,  _T(__FILE__), 0, data, desc.c_str());
//	throw(exceps);
//	}
//	*/

//	return val;
//}



//*****************************************************************************************************************
}
