//****************************************************************************
// File:	WGInput.cpp
// Class:	CWGInput
//****************************************************************************
// 15/09/2008	Rémi Saint-Amant	Created from old file
//****************************************************************************


#include "Basic/UtilMath.h"
//#include "Basic/utilzen.h"
#include "ModelBased/WGInput.h"

//#include "WeatherBasedSimulationString.h"

using namespace std;
using namespace WBSF::HOURLY_DATA;

namespace WBSF
{

const short CWGInput::NB_STATION_SEARCH_MAX = 1000;
const char * CWGInput::FILE_EXT = ".wgs";

const char* CWGInput::XML_FLAG = "WGInput";
const char* CWGInput::MEMBERS_NAME[NB_MEMBERS] = { "Variables", "SourceType", "GenerationType", "NbNormalsYears", "FirstYear", "LastYear", "UseForecast", "UseRadarPrcp", "NormalDBName", "NbNormalsStations",
                                                   "DailyDBName", "NbDailyStations", "HourlyDBName", "NbHourlyStations", "UseGribs", "GribDBName", "NbGribPoints", "Albedo", "Seed", "AllowedDerivedVariables", "Xvalidation", "SkipVerify", "SearchRadius", "NoFillMissing", "UseShore"
                                                 };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWGInput::CWGInput()
{
    Reset();
}

CWGInput::CWGInput(const CWGInput& TGInput)
{
    operator = (TGInput);
}


void CWGInput::clear()
{
    m_variables = "TN T TX P";
    m_sourceType = FROM_DISAGGREGATIONS;
    m_generationType = GENERATE_DAILY;
    m_nbNormalsYears = 1;
    m_firstYear = CTRef::GetCurrentTRef().GetYear();
    m_lastYear = CTRef::GetCurrentTRef().GetYear();
    m_bUseForecast = true;
    m_bUseRadarPrcp = false;

    m_normalsDBName.clear();
    m_nbNormalsStations = 4;
    m_dailyDBName.clear();
    m_nbDailyStations = 4;
    m_hourlyDBName.clear();
    m_nbHourlyStations = 4;
    m_gribsDBName.clear();
    m_nbGribPoints = 4;
    m_bUseGribs = false;
    //m_bAtSurfaceOnly = true;
    m_albedo = CANOPY;
    m_seed = CRandomGenerator::RANDOM_SEED;
    m_allowedDerivedVariables = "T WD R Z S SD SWE WS2 A1 A2";
    m_bXValidation = false;
    m_bSkipVerify = false;
    m_bNoFillMissing = false;
    m_bUseShore = true;
    m_searchRadius.reset();

    m_filePath.clear();
}

CWGInput& CWGInput::operator=(const CWGInput& in)
{
    if (&in != this)
    {
        m_variables = in.m_variables;
        m_sourceType = in.m_sourceType;
        m_generationType = in.m_generationType;
        m_nbNormalsYears = in.m_nbNormalsYears;
        m_firstYear = in.m_firstYear;
        m_lastYear = in.m_lastYear;
        m_bUseForecast = in.m_bUseForecast;
        m_bUseRadarPrcp = in.m_bUseRadarPrcp;

        m_normalsDBName = in.m_normalsDBName;
        m_nbNormalsStations = in.m_nbNormalsStations;
        m_dailyDBName = in.m_dailyDBName;
        m_nbDailyStations = in.m_nbDailyStations;
        m_hourlyDBName = in.m_hourlyDBName;
        m_nbHourlyStations = in.m_nbHourlyStations;
        m_nbGribPoints = in.m_nbGribPoints;
        m_gribsDBName = in.m_gribsDBName;
        m_bUseGribs = in.m_bUseGribs;
        //m_bAtSurfaceOnly = in.m_bAtSurfaceOnly;
        m_albedo = in.m_albedo;
        m_seed = in.m_seed;
        m_allowedDerivedVariables = in.m_allowedDerivedVariables;
        m_bXValidation = in.m_bXValidation;
        m_filePath = in.m_filePath;
        m_bSkipVerify = in.m_bSkipVerify;
        m_bNoFillMissing = in.m_bNoFillMissing;
        m_bUseShore = in.m_bUseShore;
        m_searchRadius = in.m_searchRadius;
    }

    assert(operator==(in));

    return *this;
}

string CWGInput::GetMember(size_t i)const
{
    assert(i < NB_MEMBERS);

    string str;
    switch (i)
    {
    case VARIABLES:
        str = m_variables.to_string();
        break;
    case SOURCE_TYPE:
        str = std::to_string(m_sourceType);
        break;
    case GENERATION_TYPE:
        str = to_string(m_generationType);
        break;
    case NB_NORMALS_YEARS:
        str = to_string(m_nbNormalsYears);
        break;
    case FIRST_YEAR:
        str = to_string(m_firstYear);
        break;
    case LAST_YEAR:
        str = to_string(m_lastYear);
        break;
    case USE_FORECAST:
        str = to_string(m_bUseForecast);
        break;
    case USE_RADAR_PRCP:
        str = to_string(m_bUseRadarPrcp);
        break;
    case NORMAL_DB_NAME:
        str = m_normalsDBName;
        break;
    case NB_NORMAL_STATION:
        str = to_string(m_nbNormalsStations);
        break;
    case DAILY_DB_NAME:
        str = m_dailyDBName;
        break;
    case NB_DAILY_STATION:
        str = to_string(m_nbDailyStations);
        break;
    case HOURLY_DB_NAME:
        str = m_hourlyDBName;
        break;
    case NB_HOURLY_STATION:
        str = to_string(m_nbHourlyStations);
        break;
    case NB_GRIB_POINTS:
        str = to_string(m_nbGribPoints);
        break;
    case GRIBS_DB_NAME:
        str = m_gribsDBName;
        break;
    case USE_GRIBS:
        str = to_string(m_bUseGribs);
        break;
    //case ATSURFACE_ONLY:	str = to_string(m_bAtSurfaceOnly); break;
    case ALBEDO:
        str = to_string(m_albedo);
        break;
    case SEED:
        str = to_string(m_seed);
        break;
    case ALLOWED_DERIVED_VARIABLES:
        str = m_allowedDerivedVariables.to_string();
        break;
    case XVALIDATION:
        str = to_string(m_bXValidation);
        break;
    case SKIP_VERIFY:
        str = to_string(m_bSkipVerify);
        break;
    case NO_FILL_MISSING:
        str = to_string(m_bNoFillMissing);
        break;
    case USE_SHORE:
        str = to_string(m_bUseShore);
        break;
    case SEARCH_RADIUS:
        assert(false);//to do
//        str = to_string(m_searchRadius, "," );
        break;
    default:
        assert(false);
    }

    return str;
}

bool CWGInput::operator==(const CWGInput& in)const
{
    bool bEqual = true;

    if (m_variables != in.m_variables)bEqual = false;
    if (m_sourceType != in.m_sourceType)bEqual = false;
    if (m_generationType != in.m_generationType)bEqual = false;
    if (m_nbNormalsYears != in.m_nbNormalsYears)bEqual = false;
    if (m_firstYear != in.m_firstYear)bEqual = false;
    if (m_lastYear != in.m_lastYear)bEqual = false;
    if (m_bUseForecast != in.m_bUseForecast)bEqual = false;
    if (m_bUseRadarPrcp != in.m_bUseRadarPrcp)bEqual = false;

    if (m_normalsDBName != in.m_normalsDBName)bEqual = false;
    if (m_nbNormalsStations != in.m_nbNormalsStations)bEqual = false;
    if (m_dailyDBName != in.m_dailyDBName)bEqual = false;
    if (m_nbDailyStations != in.m_nbDailyStations)bEqual = false;
    if (m_hourlyDBName != in.m_hourlyDBName)bEqual = false;
    if (m_nbHourlyStations != in.m_nbHourlyStations)bEqual = false;
    if (m_nbGribPoints != in.m_nbGribPoints)bEqual = false;
    if (m_gribsDBName != in.m_gribsDBName)bEqual = false;
    if (m_bUseGribs != in.m_bUseGribs)bEqual = false;
//		if (m_bAtSurfaceOnly != in.m_bAtSurfaceOnly)bEqual = false;
    if (m_albedo != in.m_albedo)bEqual = false;
    if (m_seed != in.m_seed)bEqual = false;
    if (m_allowedDerivedVariables != in.m_allowedDerivedVariables)bEqual = false;
    if (m_bXValidation != in.m_bXValidation)bEqual = false;
    if (m_bSkipVerify != in.m_bSkipVerify)bEqual = false;
    if (m_bNoFillMissing != in.m_bNoFillMissing)bEqual = false;
    if (m_bUseShore != in.m_bUseShore)bEqual = false;
    if (m_searchRadius != in.m_searchRadius)bEqual = false;

    return bEqual;
}

bool CWGInput::operator!=(const CWGInput& TGInput)const
{
    return !operator==(TGInput);
}

void CWGInput::LoadDefaultParameter()
{
    Reset();


    string filePath = GetUserDataPath() + "Default\\DefaultWG.tgs";
    if (FileExists(filePath))
    {
        Load(filePath);
    }
    else
    {
        m_variables = "TN T TX P";
        m_normalsDBName = "Canada-USA 1981-2010";
        m_allowedDerivedVariables = "T R Z S SD SWE ES EA VPD WS2";
    }

    m_filePath = STRDEFAULT;
}

void CWGInput::SetAsDefaultParameter()
{
    string path = GetUserDataPath() + "Default\\";
    if (CreateMultipleDir(path))
        Save(path + "DefaultWG.tgs");
}

ERMsg CWGInput::Load(const string& filePath)
{
    clear();
    return ERMsg();//zen::LoadXML(filePath, GetXMLFlag(), "1", *this);
}

ERMsg CWGInput::Save(const string& filePath)
{
    return ERMsg();//zen::SaveXML(filePath, GetXMLFlag(), "1", *this);
}

ERMsg CWGInput::IsValid()const
{
    ERMsg msg;

    if (m_normalsDBName.empty())
        msg.ajoute("Invalid weather generator input: normals DB name is empty.");


    if (m_sourceType == FROM_OBSERVATIONS)
    {
        if(m_firstYear>m_lastYear)
            msg.ajoute("First year ("+ to_string(m_firstYear) +") must be smaller than last year ("+ to_string(m_lastYear) +")");

        if (m_generationType == GENERATE_DAILY)
        {
            if (m_dailyDBName.empty()&&!m_bUseGribs)
                msg.ajoute("Invalid weather generator input: daily DB name is empty.");
        }
        else if (m_generationType == GENERATE_HOURLY)
        {
            if (m_hourlyDBName.empty()&&!m_bUseGribs)
                msg.ajoute("Invalid weather generator input: hourly DB name is empty.");
        }
    }

    return msg;
}


CModelOutputVariableDefVector CWGInput::GetOutputDefenition()const
{
    CModelOutputVariableDefVector variables;

    for (size_t v = 0; v < m_variables.size(); v++)
    {
        if (m_variables[v])
        {
            string name = HOURLY_DATA::GetVariableName(v);
            string title = HOURLY_DATA::GetVariableTitle(v);
            string units = HOURLY_DATA::GetVariableUnits(v);
            variables.push_back(CModelOutputVariableDef(name, title, units, "", CTM(), 1, "", v));
        }
    }

    return variables;
}

CTPeriod CWGInput::GetTPeriod()const
{
    return CTPeriod(CTRef(GetFirstYear(), JANUARY, DAY_01, 0, GetTM()), CTRef(GetLastYear(), DECEMBER, DAY_31, 23, GetTM()));
}

//todo ; a éclaircir tout cela....
CWVariables CWGInput::GetMissingInputVariables()const
{
    //DerivedVariable
    CWVariables dVariables;
    //if (m_variables[H_TRNG] && m_allowedDerivedVariables[H_TRNG] && !m_variables[H_TAIR])
    //dVariables.set(H_TAIR);

    if ((m_variables[H_TMIN] && !m_variables[H_TAIR]) || (m_variables[H_TMAX] && !m_variables[H_TAIR]))
        dVariables.set(H_TAIR);
    if (m_variables[H_TDEW] && m_allowedDerivedVariables[H_TDEW] && !m_variables[H_TAIR])
        dVariables.set(H_TAIR);
    if (m_variables[H_TDEW] && m_allowedDerivedVariables[H_TDEW] && !m_variables[H_PRCP])
        dVariables.set(H_PRCP);
    if (m_variables[H_RELH] && m_allowedDerivedVariables[H_RELH] && !m_variables[H_TAIR])
        dVariables.set(H_TAIR);
    if (m_variables[H_RELH] && m_allowedDerivedVariables[H_RELH] && !m_variables[H_PRCP])
        dVariables.set(H_PRCP);
    if (m_variables[H_SRAD] && m_allowedDerivedVariables[H_SRAD] && !m_variables[H_TAIR])
        dVariables.set(H_TAIR);
    if (m_variables[H_SRAD] && m_allowedDerivedVariables[H_SRAD] && !m_variables[H_PRCP])
        dVariables.set(H_PRCP);
    if (m_variables[H_SNOW] && m_allowedDerivedVariables[H_SNOW] && !m_variables[H_TAIR])
        dVariables.set(H_TAIR);
    if (m_variables[H_SNOW] && m_allowedDerivedVariables[H_SNOW] && !m_variables[H_PRCP])
        dVariables.set(H_PRCP);
    if (m_variables[H_SNDH] && m_allowedDerivedVariables[H_SNDH] && !m_variables[H_TAIR])
        dVariables.set(H_TAIR);
    if (m_variables[H_SNDH] && m_allowedDerivedVariables[H_SNDH] && !m_variables[H_PRCP])
        dVariables.set(H_PRCP);
    if (m_variables[H_SWE] && m_allowedDerivedVariables[H_SWE] && !m_variables[H_TAIR])
        dVariables.set(H_TAIR);
    if (m_variables[H_SWE] && m_allowedDerivedVariables[H_SWE] && !m_variables[H_PRCP])
        dVariables.set(H_PRCP);
    /*if (m_variables[H_EA] && m_allowedDerivedVariables[H_EA] && !m_variables[H_TDEW])
    	dVariables.set(H_TDEW);
    if (m_variables[H_ES] && m_allowedDerivedVariables[H_ES] && !m_variables[H_TAIR])
    	dVariables.set(H_TAIR);
    if (m_variables[H_VPD] && m_allowedDerivedVariables[H_VPD] && !m_variables[H_TAIR])
    	dVariables.set(H_TAIR);*/
    //if (m_variables[H_VPD] && m_allowedDerivedVariables[H_VPD] && !m_variables[H_TDEW])
    //dVariables.set(H_TDEW);
    if (m_variables[H_WND2] && m_allowedDerivedVariables[H_WND2] && !m_variables[H_WNDS])
        dVariables.set(H_WNDS);




    /*if (m_variables[H_TAIR] && (!m_variables[H_TMIN] || !m_variables[H_TMAX]))
    {
    	dVariables.set(H_TMIN);
    	dVariables.set(H_TMAX);
    }*/

    if (dVariables[H_TAIR])
    {
        dVariables.set(H_TMIN);
        dVariables.set(H_TMAX);
    }


    return dVariables;
}



CWVariables CWGInput::GetMandatoryVariables()const
{
    CWVariables vars(m_variables);
    vars |= GetMissingInputVariables();

    return vars;
}


CWVariables CWGInput::GetNormalMandatoryVariables()const
{
    CWVariables mVariables(m_variables);
    mVariables |= GetMissingInputVariables();

    /*if (mVariables[H_TAIR] && !mVariables[H_TMIN])
    	mVariables.set(H_TMIN);

    if (mVariables[H_TAIR] && !mVariables[H_TMAX])
    	mVariables.set(H_TMAX);*/

    if (mVariables[H_PRCP] && ! mVariables[H_TAIR] )
        mVariables.set(H_TAIR);

    if (mVariables[H_TDEW] && !mVariables[H_RELH])
        mVariables.set(H_RELH);

    if (mVariables[H_TDEW] && mVariables[H_TAIR] )
        mVariables.set(H_TAIR);

    //if (mVariables[H_TDEW] && !mVariables[H_TMAX])
    //mVariables.set(H_TMAX);

    if (mVariables[H_WND2] && !mVariables[H_WNDS])
        mVariables.set(H_WNDS);

    if (mVariables[H_WNDS] && !mVariables[H_PRCP])
        mVariables.set(H_PRCP);

    if (mVariables[H_TAIR])
    {
        mVariables.set(H_TMIN);
        mVariables.set(H_TMAX);
    }

    return mVariables;
}


void CWGInput::write_xml(pugi::xml_node node)const
{

    node.append_child(GetMemberName(VARIABLES)).append_child(pugi::node_pcdata).set_value(m_variables.to_string().c_str());
    node.append_child(GetMemberName(SOURCE_TYPE)).append_child(pugi::node_pcdata).set_value(to_string(m_sourceType).c_str());
    node.append_child(GetMemberName(GENERATION_TYPE)).append_child(pugi::node_pcdata).set_value(to_string(m_generationType).c_str());
    node.append_child(GetMemberName(NB_NORMALS_YEARS)).append_child(pugi::node_pcdata).set_value(to_string(m_nbNormalsYears).c_str());
    node.append_child(GetMemberName(FIRST_YEAR)).append_child(pugi::node_pcdata).set_value(to_string(m_firstYear).c_str());
    node.append_child(GetMemberName(LAST_YEAR)).append_child(pugi::node_pcdata).set_value(to_string(m_lastYear).c_str());
    node.append_child(GetMemberName(USE_FORECAST)).append_child(pugi::node_pcdata).set_value(to_string(m_bUseForecast).c_str());
    node.append_child(GetMemberName(USE_RADAR_PRCP)).append_child(pugi::node_pcdata).set_value(to_string(m_bUseRadarPrcp).c_str());
    node.append_child(GetMemberName(NB_NORMAL_STATION)).append_child(pugi::node_pcdata).set_value(to_string(m_nbNormalsStations).c_str());
    node.append_child(GetMemberName(NORMAL_DB_NAME)).append_child(pugi::node_pcdata).set_value(m_normalsDBName.c_str());
    node.append_child(GetMemberName(NB_DAILY_STATION)).append_child(pugi::node_pcdata).set_value(to_string(m_nbDailyStations).c_str());
    node.append_child(GetMemberName(DAILY_DB_NAME)).append_child(pugi::node_pcdata).set_value(m_dailyDBName.c_str());
    node.append_child(GetMemberName(NB_HOURLY_STATION)).append_child(pugi::node_pcdata).set_value(to_string(m_nbHourlyStations).c_str());
    node.append_child(GetMemberName(HOURLY_DB_NAME)).append_child(pugi::node_pcdata).set_value(m_hourlyDBName.c_str());
    node.append_child(GetMemberName(NB_GRIB_POINTS)).append_child(pugi::node_pcdata).set_value(to_string(m_nbGribPoints).c_str());
    node.append_child(GetMemberName(GRIBS_DB_NAME)).append_child(pugi::node_pcdata).set_value(m_gribsDBName.c_str());
    node.append_child(GetMemberName(USE_GRIBS)).append_child(pugi::node_pcdata).set_value(to_string(m_bUseGribs).c_str());
    node.append_child(GetMemberName(ALBEDO)).append_child(pugi::node_pcdata).set_value(to_string(m_albedo).c_str());
    node.append_child(GetMemberName(SEED)).append_child(pugi::node_pcdata).set_value(to_string(m_seed).c_str());
    node.append_child(GetMemberName(ALLOWED_DERIVED_VARIABLES)).append_child(pugi::node_pcdata).set_value(m_allowedDerivedVariables.to_string().c_str());
    node.append_child(GetMemberName(XVALIDATION)).append_child(pugi::node_pcdata).set_value(to_string(m_bXValidation).c_str());
    node.append_child(GetMemberName(SKIP_VERIFY)).append_child(pugi::node_pcdata).set_value(to_string(m_bSkipVerify).c_str());
    node.append_child(GetMemberName(NO_FILL_MISSING)).append_child(pugi::node_pcdata).set_value(to_string(m_bNoFillMissing).c_str());
    node.append_child(GetMemberName(USE_SHORE)).append_child(pugi::node_pcdata).set_value(to_string(m_bUseShore).c_str());
    node.append_child(GetMemberName(SEARCH_RADIUS)).append_child(pugi::node_pcdata).set_value(m_searchRadius.to_string().c_str());

}





void CWGInput::read_xml(pugi::xml_node node)
{
    m_variables = node.child(GetMemberName(VARIABLES)).text().as_string();
    m_sourceType = node.child(GetMemberName(SOURCE_TYPE)).text().as_int();
    m_generationType = node.child(GetMemberName(GENERATION_TYPE)).text().as_int();
    m_nbNormalsYears = node.child(GetMemberName(NB_NORMALS_YEARS)).text().as_int();
    m_firstYear = node.child(GetMemberName(FIRST_YEAR)).text().as_int();
    m_lastYear = node.child(GetMemberName(LAST_YEAR)).text().as_int();
    m_bUseForecast = node.child(GetMemberName(USE_FORECAST)).text().as_bool();
    m_bUseRadarPrcp = node.child(GetMemberName(USE_RADAR_PRCP)).text().as_bool();
    m_normalsDBName = node.child(GetMemberName(NORMAL_DB_NAME)).text().as_string();
    m_nbNormalsStations = node.child(GetMemberName(NB_NORMAL_STATION)).text().as_int();
    m_dailyDBName = node.child(GetMemberName(DAILY_DB_NAME)).text().as_string();
    m_nbDailyStations = node.child(GetMemberName(NB_DAILY_STATION)).text().as_int();
    m_hourlyDBName = node.child(GetMemberName(HOURLY_DB_NAME)).text().as_string();
    m_nbHourlyStations = node.child(GetMemberName(NB_HOURLY_STATION)).text().as_int();
    m_gribsDBName = node.child(GetMemberName(GRIBS_DB_NAME)).text().as_string();
    m_nbGribPoints = node.child(GetMemberName(NB_GRIB_POINTS)).text().as_int();
    m_bUseGribs = node.child(GetMemberName(USE_GRIBS)).text().as_string();
    m_albedo = node.child(GetMemberName(ALBEDO)).text().as_ullong();
    m_seed = node.child(GetMemberName(SEED)).text().as_ullong();
    m_allowedDerivedVariables = node.child(GetMemberName(ALLOWED_DERIVED_VARIABLES)).text().as_string();
    m_bXValidation = node.child(GetMemberName(XVALIDATION)).text().as_bool();
    m_bSkipVerify = node.child(GetMemberName(SKIP_VERIFY)).text().as_bool();
    m_bNoFillMissing = node.child(GetMemberName(NO_FILL_MISSING)).text().as_bool();
    m_bUseShore = node.child(GetMemberName(USE_SHORE)).text().as_bool();
    m_searchRadius = node.child(GetMemberName(SEARCH_RADIUS)).text().as_string();

}


}
