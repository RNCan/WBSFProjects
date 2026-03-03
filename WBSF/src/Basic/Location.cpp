//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//
//  Class:	CLocation: hold geographic location and attributes
//
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
// 12-11-2013   Rémi Saint-Amant    Creation from existing code.
//*********************************************************************



#include <algorithm>
#include <string>
#include <cstdio>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/locale.hpp>



#include "Basic/Shore.h"
#include "Basic/CallcURL.h"
#include "Basic/json/json11.hpp"
#include "Basic/nlohmann/json.hpp"
#include "Basic/CSV.h"

#include "Location.h"

//#include "WeatherBasedSimulationString.h"


using namespace std;
//using namespace boost;
using namespace json11;

namespace WBSF
{
//************************************************************************
//CLocation Class


const char* CLocation::MEMBER_NAME[NB_MEMBER] = { "ID", "Name", "Latitude", "Longitude", "Elevation", "SiteSpecificInformation" };
const char* CLocation::DEFAULT_SSI_NAME[NB_DEFAULT_SSI] =
{
    "MergedStationIDs", "Slope", "Aspect", "UseIt", "DataFileName", "TimeZone", "Variables", "Years", "ShoreDistance", "eHorizon", "wHorizon"
};

vector<string> CLocation::MEMBER_TITLE;
vector<string> CLocation::DEFAULT_SSI_TITLE;


const char* CLocation::GetMemberTitle(size_t i)
{
    assert(i >= 0 && i < NB_MEMBER);

    if (MEMBER_TITLE.empty())
    {
        string str = boost::locale::translate("KeyID|Name|Latitude|Longitude|Elevation|Specific Site Information");
        MEMBER_TITLE = Tokenize(str, "|");
    }

    return MEMBER_TITLE.empty() ? GetMemberName(i) : MEMBER_TITLE[i].c_str();
}

const char* CLocation::GetDefaultSSITitle(size_t i)
{
    assert(i >= 0 && i < NB_DEFAULT_SSI);

    if (DEFAULT_SSI_TITLE.empty())
    {
        string str = boost::locale::translate("Optional ID|Slope|Aspect|Use It|Data File Name|Time Zone|Variables|Year|Shore Distance|East Horizon|West Horizon");
        DEFAULT_SSI_TITLE = Tokenize(str, "|");
    }

    return DEFAULT_SSI_TITLE.empty() ? GetDefaultSSITitle(i) : DEFAULT_SSI_TITLE[i].c_str();
}



CLocation::CLocation(string name, string ID, double lat, double lon, double alt) :
    CGeoPoint3D(lon, lat, alt, PRJ_GEOGRAPHIC),
    m_name(name),
    m_ID(ID)
{}

void CLocation::Reset()
{
    m_name.clear();
    m_ID.clear();
    m_prjID = PRJ_GEOGRAPHIC;
    m_lat = -999;
    m_lon = -999;
    m_elev = -999;
    m_siteSpeceficInformation.clear();
}

CLocation& CLocation::operator=(const CLocation& in)
{
    if (&in != this)
    {
        CGeoPoint3D::operator=(in);
        m_name = in.m_name;
        m_ID = in.m_ID;
        //m_prjID=in.m_prjID;
        //m_elev = in.m_elev;
        m_siteSpeceficInformation.clear();

        if (!in.m_siteSpeceficInformation.empty())
            m_siteSpeceficInformation.insert(in.m_siteSpeceficInformation.begin(), in.m_siteSpeceficInformation.end());
    }

    return *this;
}

bool CLocation::operator ==(const CLocation& in)const
{
    bool bEqual = true;

    if (CGeoPoint3D::operator!=(in))bEqual = false;
    if (m_name != in.m_name)bEqual = false;
    if (m_ID != in.m_ID)bEqual = false;
    if (m_siteSpeceficInformation != in.m_siteSpeceficInformation)bEqual = false;

    return bEqual;
}

void CLocation::Init(const std::string& name, const std::string& ID, double lat, double lon, double alt)
{
    m_prjID = PRJ_GEOGRAPHIC;
    m_name = name;
    m_ID = ID;
    m_lat = lat;
    m_lon = lon;
    m_elev = alt;
    m_siteSpeceficInformation.clear();
}


std::istream& CLocation::LoadV2(std::istream& io)
{
    if (io)
    {
        char tmp[FILENAME_MAX] = { 0 };
        io.ignore(1);
        io.get(tmp, FILENAME_MAX, '"');
        io.ignore(1);

        m_name = tmp;
        m_prjID = PRJ_GEOGRAPHIC;
        io >> m_lat;
        io >> m_lon;
        io >> m_elev;
        float slope = 0;
        float aspect = 0;
        io >> slope;
        io >> aspect;
        SetDefaultSSI(SLOPE, to_string(slope));
        SetDefaultSSI(ASPECT, to_string(aspect));
        //take the line feed
        io.getline(tmp, FILENAME_MAX);

    }

    return io;
}


string CLocation::GetMember(size_t i)const
{
    assert(i < NB_MEMBER);

    string str;
    switch (i)
    {
    case ID:
        str = m_ID;
        break;
    case NAME:
        str = m_name;
        break;
    case LAT:
        str = to_string(m_lat);
        break;
    case LON:
        str = to_string(m_lon);
        break;
    case ELEV:
        str = to_string(m_elev);
        break;
    case SITE_SPECIFIC_INFORMATION:
    {
        for (SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.begin(); it != m_siteSpeceficInformation.end(); it++)
            str += "{" + it->first + ":" + it->second + "}";
        break;
    }
    default:
        assert(false);
    }

    return str;
}


void CLocation::SetMember(size_t i, const string& str)
{
    assert(i < NB_MEMBER);

    switch (i)
    {
    case ID:
        m_ID = str;
        break;
    case NAME:
        m_name = str;
        break;
    case LAT:
        m_lat = ToDouble(str);
        break;
    case LON:
        m_lon = ToDouble(str);
        break;
    case ELEV:
        m_elev = ToDouble(str);
        break;
    case SSI:
    {
        vector<string> SSIs = Tokenize(str.c_str(), "{}");
        for (vector<string>::const_iterator it = SSIs.begin(); it != SSIs.end(); it++)
        {
            string::size_type pos = it->find(':', 0);
            if (pos < string::npos)
                SetSSI(it->substr(0, pos), it->substr(pos + 1));
        }

        break;
    }

    default:
        assert(false);
    }
}

double CLocation::GetSlope()const
{
    double slope = -999;
    SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.find(GetDefaultSSIName(SLOPE));
    if (it != m_siteSpeceficInformation.end())
        slope = ToDouble(it->second);

    return slope;
}

double CLocation::GetSlopeInDegree()const
{
    double slope = GetSlope();
    if (slope == -999)
        return -999;

    assert(slope >= 0);
    assert(Rad2Deg(atan(GetSlope() / 100))  >= 0);
    assert(Rad2Deg(atan(GetSlope() / 100)) <= 90);
    return float(Rad2Deg(atan(GetSlope() / 100)));
}

double CLocation::GetAspect()const
{
    double aspect = -999;
    SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.find(GetDefaultSSIName(ASPECT));
    if (it != m_siteSpeceficInformation.end())
        aspect = ToDouble(it->second);

    assert(aspect == -999 || (aspect >= 0 && aspect <= 360));
    return aspect;
}

double CLocation::GetDayLength(size_t d)const
{
    return WBSF::GetDayLength(m_lat, d);
}

double CLocation::GetDayLength(CTRef d)const
{
    return WBSF::GetDayLength(m_lat, d.GetDOY());
}

double CLocation::GetPressure()const
{
    return WBSF::GetPressure(m_elev);
}

double CLocation::GetTimeZone()const
{
    double timeZone = std::round(m_lon / 15.0);
    std::string tz = GetDefaultSSI(CLocation::TIME_ZONE);
    if (!tz.empty())
        timeZone = ToDouble(tz);

    return timeZone;
}


std::string CLocation::GetDefaultSSI(size_t i)const
{
    return GetSSI(GetDefaultSSIName(i));
}

void CLocation::SetDefaultSSI(size_t i, const std::string& SSI)
{
    SetSSI(GetDefaultSSIName(i), SSI);
}

std::string CLocation::GetSSI(const std::string& name)const
{
    SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.find(name);
    if (it != m_siteSpeceficInformation.end())
        return it->second;

    return "";
}

void CLocation::SetSSI(const std::string& name, const std::string& SSI)
{
    if (SSI.empty())
    {
        //size_t pos = UNKNOWN_POS;
        //SiteSpeceficInformationMap::iterator it = m_siteSpeceficInformation.find(name);
       // if (it != m_siteSpeceficInformation.end())
         //   pos = it->second.second;

        m_siteSpeceficInformation.erase(name);
        //update all position after pos

        //for (SiteSpeceficInformationMap::iterator it = m_siteSpeceficInformation.begin(); it != m_siteSpeceficInformation.end(); it++)
          //  if (it->second.second > pos)
            //    it->second.second--;
    }
    else
    {
        m_siteSpeceficInformation[name] = SSI;
    }

}

std::string CLocation::GetDataFileName()const
{
    std::string tmp = GetDefaultSSI(CLocation::DATA_FILE_NAME);
    if (tmp.empty())
        tmp = PurgeFileName(m_name + " [" + m_ID + "]") + ".csv";

    return tmp;
}



ERMsg CLocation::IsValid(bool bExludeUnknownElev)const
{
    ERMsg msg;

    if (m_ID.empty())
    {
        //msg.ajoute(locale::translate("The primary key (ID) is missing. Specify an ID."));
    }

    if (m_lat < -90 || m_lat > 90)
    {
        msg.ajoute(boost::locale::translate("Invalid latitude. Latitude must be in range -90 and 90 degrees."));
    }

    if (m_lon < -360 || m_lon > 360)
    {
        msg.ajoute(boost::locale::translate("Invalid longitude. Longitude must be in range  -360 and 360 degrees."));
    }

    //some stations have under sea level atitude
    if (m_elev < -450 || m_elev > 9000)
    {
        if (m_elev != -999 || !bExludeUnknownElev)
        {
            msg.ajoute(boost::locale::translate("Invalid elevation. Elevation must be in range -350 and 10000 metres."));
        }
    }

    SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.find(GetDefaultSSIName(SLOPE));
    if (it != m_siteSpeceficInformation.end())
    {
        double slope = ToDouble(it->second);
        if (slope < 0)
        {
            msg.ajoute(boost::locale::translate("Invalid slope. Slope must be in percent and must be greater than or equal to 0."));
        }
    }


    it = m_siteSpeceficInformation.find(GetDefaultSSIName(ASPECT));
    if (it != m_siteSpeceficInformation.end())
    {
        double aspect = ToDouble(it->second);

        if (aspect < 0 || aspect > 360)
        {
            msg.ajoute("Invalid aspect. Aspect must be between  0 (north) and 360 degrees.");
        }
    }

    if (!msg)
    {
        boost::format fmt = boost::format("Invalid location %1% [%2%].") % m_name % m_ID;
        msg.ajoute(fmt.str());
    }

    return msg;

}

void CLocation::AppendMergeID(const std::string& mergeID2)
{
    string mergeID1 = GetDefaultSSI(OPTIONAL_ID);
    vector<string> IDs1 = Tokenize(mergeID1, "+");

    vector<string> IDs2 = Tokenize(mergeID2, "+");

    for (vector<string>::const_iterator it = IDs2.begin(); it != IDs2.end(); it++)
    {
        if ( /**it!=m_ID && */find(IDs1.begin(), IDs1.end(), *it) == IDs1.end())
            IDs1.push_back(*it);
    }

    mergeID1.clear();
    for (vector<string>::const_iterator it = IDs1.begin(); it != IDs1.end(); it++)
    {
        mergeID1 += mergeID1.empty() ? "" : "+";
        mergeID1 += *it;
    }

    SetDefaultSSI(OPTIONAL_ID, mergeID1);
}

void CLocation::AppendMergeID(const CLocation& ID)
{
    AppendMergeID(ID.m_ID);
    string mergeID2 = ID.GetDefaultSSI(OPTIONAL_ID);
    AppendMergeID(mergeID2);
}

double CLocation::GetDistance(const CLocation& in, bool bTakeElevation, bool bTakeShoreDistance)const
{
    double d = WBSF::GetDistance(in.m_lat, in.m_lon, m_lat, m_lon);
    double e = (in.m_elev - m_elev) * ELEV_FACTOR;


    if (bTakeElevation && bTakeShoreDistance)
    {
        assert(GetShoreDistance() > -999);
        assert(in.GetShoreDistance() > -999);
        double s = (in.GetShoreDistance() - GetShoreDistance())*SHORE_DISTANCE_FACTOR;
        d = sqrt(d*d + e * e + s * s);
    }
    else if (bTakeElevation && !bTakeShoreDistance)
    {
        d = sqrt(d*d + e * e);
    }
    else if (!bTakeElevation && bTakeShoreDistance)
    {
        assert(GetShoreDistance() > -999);
        assert(in.GetShoreDistance() > -999);
        double s = (in.GetShoreDistance() - GetShoreDistance())*SHORE_DISTANCE_FACTOR;
        d = sqrt(d*d + s * s);
    }

    //double e = (in.m_elev - m_elev) * ELEV_FACTOR;

    //if (bTakeElevation )
    //d = sqrt(d*d + e*e);

    return d;
}

double CLocation::GetXTemp(const CLocation& station, bool bTakeElevation, bool bTakeShoreDistance)const
{
    double d = std::max(GetDistance(station, bTakeElevation, bTakeShoreDistance), 0.0000000001);
    return pow(d, -2);
}

bool CLocation::is_init()const
{
    return (!m_name.empty() || !m_ID.empty()) && m_lat != -999 && m_lon != -999 && m_elev != -999;
}

size_t CLocation::GetMemberFromName(const string& headerIn)
{
    size_t member = SSI;

    string header(headerIn);
    header.erase(std::remove(header.begin(), header.end(), '"'), header.end());
    header.erase(std::remove(header.begin(), header.end(), '\''), header.end());


    string::size_type pos = header.find('(');
    if (pos != string::npos)
        header = TrimConst(header.substr(0, pos));


    if (boost::iequals(header, "KeyID") || boost::iequals(header, "ID") || boost::iequals(header, "UniqueID") || boost::iequals(header, "CleNo")|| boost::iequals(header, "Site"))
        member = ID;
    else if (boost::iequals(header, "Name") || boost::iequals(header, "Nom") || boost::iequals(header, "StationName") || boost::iequals(header, "NomStation") || boost::iequals(header, "LOC_FILE 4") || boost::iequals(header, "LOC_FILE 4 1") || boost::iequals(header, "LOC_FILE 4 2") || boost::iequals(header, "LOC_FILE 3"))
        member = NAME;
    else if (boost::iequals(header, "Latitude") || boost::iequals(header, "Lat") || boost::iequals(header, "Y"))
        member = LAT;
    else if (boost::iequals(header, "Longitude") || boost::iequals(header, "Lon") || boost::iequals(header, "Long") || boost::iequals(header, "X"))
        member = LON;
    else if (boost::iequals(header, "Elevation") || boost::iequals(header, "Élévation") || boost::iequals(header, "Elev") || boost::iequals(header, "Élév") || boost::iequals(header, "Alt") || boost::iequals(header, "Altitude") || boost::iequals(header, "Z"))
        member = ELEV;

    return member;
}

vector<size_t> CLocation::GetMembers(const vector<string>& header)
{
    vector<size_t> members(header.size());

    for (size_t i = 0; i < header.size(); i++)
        members[i] = GetMemberFromName(header[i]);

    return members;
}

//return the eader in the same position as they was added
vector<string> CLocation::GetSSIHeader()const
{
    vector<string> header;
    for (auto SSI: m_siteSpeceficInformation)
        header.push_back(SSI.first);

    return header;


   /* vector<pair<size_t, string>> orderPair;
    for (SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.begin(); it != m_siteSpeceficInformation.end(); it++)
        orderPair.push_back(make_pair(it->second.second, it->first));

    std::sort(orderPair.begin(), orderPair.end());

    vector<string> header;
    for (vector<pair<size_t, string>>::const_iterator it = orderPair.begin(); it != orderPair.end(); it++)
        header.push_back(it->second);

    return header;*/

    //return GetSSIOrder();

    //vector<string> header;
    //for( SiteSpeceficInformationMap::const_iterator it = m_siteSpeceficInformation.begin(); it!=m_siteSpeceficInformation.end(); it++)
    //for (vector<string>::const_iterator it = order.begin(); it != order.end(); it++)
    //header.push_back(m_siteSpeceficInformation[*it]. );

    //return header;
}

//Out : shore distance [m]
double CLocation::GetShoreDistance()const
{
    double shore_distance = -999;
    string sd_str = GetSSI(GetDefaultSSIName(SHORE_DIST));
    if (!sd_str.empty())
        shore_distance = ToDouble(sd_str) * 1000;//[km] to [m]
    else
        shore_distance = CShore::GetShoreDistance(*this);

    return shore_distance;
}

void CLocation::SetShoreDistance(double shore_distance)
{
    if (shore_distance > -999)
        SetSSI(GetDefaultSSIName(SHORE_DIST), to_string(shore_distance / 1000, 1)); //[m] to [km]
    else
        SetSSI(GetDefaultSSIName(SHORE_DIST), ""); //reset
}

double CLocation::GetGeocentricCoord(size_t i)const
{
    assert(i < 5);
    assert(m_lat >= -90 && m_lat <= 90);

    //double y = 360.0;
    double lon = m_lon;
    if (lon > 180)
        lon -= 360;
    if (lon < -180)
        lon += 360;

    //modf(m_lon+720,&y)-180;
    assert(lon >= -180 && lon <= 180);

    double xx = Deg2Rad(lon + 180);
    double yy = Deg2Rad(90 - m_lat);

    double v = 0;
    switch (i)
    {
    case 0:
        v = 6371 * 1000 * cos(xx)*sin(yy);
        break;
    case 1:
        v = 6371 * 1000 * sin(xx)*sin(yy);
        break;
    case 2:
        v = 6371 * 1000 * cos(yy);
        break;
    case 3:
        v = m_elev;
        break;
    case 4:
        v = GetShoreDistance();
        break;
    case 5:
        v = GetExposition(m_lat, GetSlope(), GetAspect());
        break;
    default:
        assert(false);
    }
    return v;
}


void CLocation::write_xml(pugi::xml_node node)const
{
    
    node.append_child(GetMemberName(NAME)).append_child(pugi::node_pcdata).set_value(m_name.c_str());
    node.append_child(GetMemberName(ID)).append_child(pugi::node_pcdata).set_value(m_ID.c_str());
    node.append_child(GetMemberName(LAT)).append_child(pugi::node_pcdata).set_value(to_string(m_lat).c_str());
    node.append_child(GetMemberName(LON)).append_child(pugi::node_pcdata).set_value(to_string(m_lon).c_str());
    node.append_child(GetMemberName(ELEV)).append_child(pugi::node_pcdata).set_value(to_string(m_elev).c_str());

//    vector<string> header = GetSSIHeader();
  //  assert(header.size() == m_siteSpeceficInformation.size());
    
    for (auto SSI : m_siteSpeceficInformation)
    {
        node.append_child(SSI.first.c_str()).append_child(pugi::node_pcdata).set_value(SSI.second.c_str());
    }
}


void CLocation::read_xml(pugi::xml_node node)
{
    //node.child_value(GetMemberName(NAME));

    for (auto child: node.children())
    {
        size_t i = GetMemberFromName(child.name());
        SetMember(i, child.text().as_string());
    }
}


//******************************************************************************************************
const char* CLocationVector::XML_FLAG = "Location";

vector<string> CLocationVector::GetHeaderFromData()const
{
    vector<string> header;
    for (CLocationVector::const_iterator it = begin(); it != end(); it++)
    {
        vector<string> SSIHeader = it->GetSSIHeader();
        for (vector<string>::const_iterator it2 = SSIHeader.begin(); it2 != SSIHeader.end(); it2++)
        {
            vector<string>::const_iterator it3 = find(header.begin(), header.end(), *it2);
            if (it3 == header.end())
                header.push_back(*it2);
        }
    }

    return header;
}

std::string CLocationVector::GetFileFormat(const std::string& file_path)
{
    std::string format = "UNKNOWN";
    
    if (WBSF::Find(file_path, ".csv") != string::npos)
    {
        format = "CSV";
    }
    else if (WBSF::Find(file_path, ".json") != string::npos)
    {
        format = "JSON";
    }
    else
    {
        ifStream file;
        auto myloc = std::locale();//by RSA 18-02-2017, une bonne chose ou non???
        file.imbue(myloc);
        if (file.open(file_path))
        {
            string line;
            ///str.resize(1024+1);
            //file.read(&(str[0]), 1024);
            //file.realine
            std::getline(file, line);
            assert(false);//todo
        }
    }


    return format;
}

ERMsg CLocationVector::Load(const std::string& file_path, const char* separator, CCallback& callback)
{
    ERMsg msg;

    m_filePath.clear();
    clear();


    if (msg)
    {

        if (WBSF::FileExists(file_path))
        {
            string format = GetFileFormat(file_path);
        	//if (WBSF::Find(file_path, ".csv") != string::npos)
            if (format == "CSV" || format == "JSON")
        	{

                ifStream file;
                auto myloc = std::locale();//by RSA 18-02-2017, une bonne chose ou non???
                file.imbue(myloc);
                msg = file.open(file_path);

                if (format == "CSV" )
                    msg = LoadCSV(file, separator, callback);
                else //format == "JSON"
                    msg = LoadJSON(file, callback);
        	}
        	else
        	{
        		msg.ajoute("Unrecognized location list file format. Only .csv or .json file are accepted");
        	}
        }
        else
        {
        	msg.ajoute("Unable to open file: " + file_path);
        	msg.ajoute("file doesn't exist");
        }
        
        if (msg)
            m_filePath = file_path;

    }

    return msg;
}

ERMsg CLocationVector::LoadCSV(std::istream& stream, const char* separator, CCallback& callback)
{
    ERMsg msg;

    //estimate file size to reserve memory
    std::streampos ffirst = stream.tellg();
    stream.seekg(0, std::ios::end);
    std::streampos size = stream.tellg() - ffirst;
    reserve(size_t(size / 40));

    callback.PushTask("Load locations", size);

    //begin to read
    stream.seekg(0);
    vector<size_t> members;


    for (CSVIterator loop(stream, separator); loop != CSVIterator() && msg; ++loop)
    {
        if (members.empty())
        {
            vector<string> head = loop.Header();
            members = CLocation::GetMembers(head);
        }


        if (loop->size() > 1)//can be empty line or some spaces...
        {
            size_t i = this->size();
            resize(i + 1);

            for (size_t j = 0; j < members.size() && j < (*loop).size(); j++)
            {
                switch (members[j])
                {
                case CLocation::ID:
                    at(i).m_ID = (*loop)[j];
                    break;
                case CLocation::NAME:
                    at(i).m_name = (*loop)[j];
                    break;
                case CLocation::LAT:
                    at(i).m_lat = ToDouble((*loop)[j]);
                    break;
                case CLocation::LON:
                    at(i).m_lon = ToDouble((*loop)[j]);
                    break;
                case CLocation::ELEV:
                    at(i).m_elev = ToDouble((*loop)[j]);
                    break;
                case CLocation::SSI:
                    at(i).SetSSI(loop.Header()[j], (*loop)[j]);
                    break;
                default:
                    assert(false);
                }
            }
        }
        msg += callback.SetCurrentStepPos((double)stream.tellg());
    }

    callback.PopTask();
    return msg;
}

ERMsg CLocationVector::LoadJSON(std::istream& stream, CCallback& callback)
{
    ERMsg msg;

    assert(false);//todo
    //const char8_t* test = u8"";
    nlohmann::json data = nlohmann::json::parse(stream);
    ////nlohmann::json data = nlohmann::json::parse(loc_str);
    ////auto l = data.at("Location");
    ////location.m_ID = l.at("ID");
    //
    //nlohmann::json locations = data.at("Locations");
    //std::cout << locations << endl;
    //
    //
    //size_t test = locations.size();
    //for (nlohmann::json::iterator it = locations.begin(); it != locations.end(); ++it)
    //{
    //	std::cout << it->is_object() << endl;
    //	std::cout << it->is_array() << endl;
    //	std::cout << it->is_string() << endl;
    //
    //	std::cout << it->at("KeyID");
    //	//std::cout << locations;
    //	//std::cout << "Key: " << it->key() << ", Value: " << it.value() << std::endl;
    //}
    //try
                //{

                    //const char8_t* utf8_str_char8 = u8"Hello, world! \u03A9";

                    //const char8_t* test = u8"{\"Locations\":["
                    //	u8"{\"Location\":{\"KeyID\":\"1\", \"Name\":\"Québec\",\"Lat\":45.4,\"Lon\":-71.2}},"
                    //	u8"{\"KeyID\":\"2\", \"Name\":\"Montréal\",\"Lat\":42.4,\"Lon\":-73.2,\"Alt\":38}"
                    //	u8"]}";

                    //const char8_t* test = u8"";
                    //nlohmann::json data = nlohmann::json::parse(test);
                    ////nlohmann::json data = nlohmann::json::parse(loc_str);
                    ////auto l = data.at("Location");
                    ////location.m_ID = l.at("ID");
                    //
                    //nlohmann::json locations = data.at("Locations");
                    //std::cout << locations << endl;
                    //
                    //
                    //size_t test = locations.size();
                    //for (nlohmann::json::iterator it = locations.begin(); it != locations.end(); ++it)
                    //{
                    //	std::cout << it->is_object() << endl;
                    //	std::cout << it->is_array() << endl;
                    //	std::cout << it->is_string() << endl;
                    //
                    //	std::cout << it->at("KeyID");
                    //	//std::cout << locations;
                    //	//std::cout << "Key: " << it->key() << ", Value: " << it.value() << std::endl;
                    //}
                //loc.m_ID = data["Location"]["ID"];
                    //loc.m_name = data["Location"]["Name"];
                    //loc.m_lat = data["Location"]["Latitude"];
                    //loc.m_lon = data["Location"]["Longitude"];
                    //loc.m_elev = data["Location"]["Elevation"];

    //}
                //catch (nlohmann::json::exception& e)
                //{
                //	msg.ajoute(e.what());
                //}
    return msg;
}

ERMsg CLocationVector::Save(const std::string& filePath, char separator, CCallback& callback)const
{
    ERMsg msg;

    CLocationVector& me = const_cast<CLocationVector&>(*this);
    me.m_filePath.clear();

    ofStream file;
    msg = file.open(filePath);

    if (msg)
    {
        msg = Save(file, separator, callback);
        if (msg)
            me.m_filePath = filePath;

        file.close();
    }

    return msg;
}


ERMsg CLocationVector::Save(std::ostream& file, char separator, CCallback& callback)const
{
    ERMsg msg;

    callback.PushTask("Save locations", size());
    //callback.SetNbStep(size());

    string header = "KeyID"; //Excel have problem with file begginning by ID
    for (size_t i = 1; i < CLocation::SSI; i++)
    {
        header += separator;
        header += CLocation::GetMemberName(i);
    }

    vector<string> SSIHeader = GetHeaderFromData();
    for (vector<string>::const_iterator it = SSIHeader.begin(); it != SSIHeader.end(); it++)
    {
        header += separator;
        header += *it;
    }

    header += "\n";
    file << header;
    //file.write(header.c_str(), header.length());



    for (CLocationVector::const_iterator it = begin(); it != end() && msg; it++)
    {
        string str;
        str.reserve(75);

        for (size_t j = 0; j < 5; j++)
        {
            switch (j)
            {
            case CLocation::ID:
                str += it->m_ID + separator;
                break;
            case CLocation::NAME:
                str += it->m_name + separator;
                break;
            case CLocation::LAT:
                str += to_string(it->m_lat, 10) + separator;
                break;
            case CLocation::LON:
                str += to_string(it->m_lon, 10) + separator;
                break;
            case CLocation::ELEV:
                str += to_string(it->m_elev, 1);
                break;
            default:
                assert(false);
            }
        }

        for (size_t j = 0; j < SSIHeader.size(); j++)
        {
            str += separator + it->GetSSI(SSIHeader[j]);
        }

        str += "\n";
        file.write(str.c_str(), str.length());

        msg += callback.StepIt();
    }

    callback.PopTask();
    return msg;
}

ERMsg CLocationVector::IsValid(bool bExludeUnknownElev)const
{
    ERMsg msg;

    for (CLocationVector::const_iterator it = begin(); it != end(); it++)
        msg += it->IsValid(bExludeUnknownElev);

    return msg;
}

size_t CLocationVector::FindPosByID(const std::string& ID, bool bCase)const
{
    CLocationVector::const_iterator it = std::find_if(begin(), end(), FindLocationByID(ID, bCase));
    return (it == end()) ? UNKNOWN_POS : it - begin();
}
CLocationVector::const_iterator CLocationVector::FindByID( const std::string& ID, bool bCase)const
{
    return std::find_if(begin(), end(), FindLocationByID(ID, bCase));
}

CLocationVector::iterator CLocationVector::FindByName( const std::string& name, bool bCase)
{
    return std::find_if(begin(), end(), FindLocationByName(name, bCase));
}
CLocationVector::const_iterator CLocationVector::FindByName(const std::string& name, bool bCase)const
{
    return std::find_if(begin(), end(), FindLocationByName(name, bCase));
}

CLocationVector::iterator CLocationVector::FindByID(const std::string& ID, bool bCase)
{
    return std::find_if(begin(), end(), FindLocationByID(ID, bCase));
}
CLocationVector::const_iterator CLocationVector::FindBySSI(const std::string& SSI, const std::string& value, bool bCase)const
{
    return std::find_if(begin(), end(), FindLocationBySSI(SSI, value, bCase));
}

CLocationVector::iterator CLocationVector::FindBySSI(const std::string& SSI, const std::string& value, bool bCase)
{
    return std::find_if(begin(), end(), FindLocationBySSI(SSI, value, bCase));
}

void CLocationVector::TrimData(char separator)
{
    for (iterator it = begin(); it != end(); it++)
        it->m_name = PurgeFileName(it->m_name);

    //trim SSI???
}


ERMsg CLocationVector::ExtractNominatimName(CLocationVector& locations, bool bReplaceAll, bool bName, bool bState, bool bCountry, CCallback& callback)
{
    ERMsg msg;


    //http://nominatim.openstreetmap.org/reverse?format=json&lat=46.736497&lon=-71.450790

    callback.PushTask("Extract location name from nominatim", locations.size());


    //CHttpConnectionPtr pConnection;
    //CInternetSessionPtr pSession;
    //msg += GetHttpConnection("nominatim.openstreetmap.org", pConnection, pSession, PRE_CONFIG_INTERNET_ACCESS, "", "", true);


    if (msg)
    {
        size_t miss = 0;
        for (size_t i = 0; i < locations.size() && msg; i++)
        {
            bool bMissName = bName && (bReplaceAll || locations[i].m_name.empty());
            bool bMissState = bState && (bReplaceAll || locations[i].GetSSI("State").empty());
            bool bMissCountry = bCountry && (bReplaceAll || locations[i].GetSSI("Country").empty());


            if (bMissName || bMissState || bMissCountry)
            {

                string strGeo;
                string URL = "https://nominatim.openstreetmap.org/reverse?zoom=18&format=geojson&lat=" + to_string(locations[i].m_lat) + "&lon=" + to_string(locations[i].m_lon);

                string argument = "-s -k \"" + URL + "\"";
                //string exe = GetApplicationPath() / "curl.exe";
                CCallcURL cURL;
                msg = cURL.get_text(argument, strGeo);
                if (msg)
                {
                    //extract name from nominatim
                    string error;
                    Json geojson = Json::parse(strGeo, error);

                    if (error.empty())
                    {
                        assert(geojson.is_object());
                        assert(geojson["features"].is_array());
                        Json::array features = geojson["features"].array_items();
                        if (features.size() == 1)
                        {
                            assert(features[0].is_object());

                            Json::object feature0 = features[0].object_items();
                            Json::object properties = feature0["properties"].object_items();
                            Json::object address = properties["address"].object_items();

//                            string village = ANSI_2_ASCII(address["village"].string_value());
//                            string town = ANSI_2_ASCII(address["town"].string_value());
//                            string suburb = ANSI_2_ASCII(address["suburb"].string_value());
//                            string city = ANSI_2_ASCII(address["city"].string_value());
//                            string county = ANSI_2_ASCII(address["county"].string_value());
//                            string region = ANSI_2_ASCII(address["region"].string_value());
//
//                            string state = ANSI_2_ASCII(address["state"].string_value());
//                            string country = ANSI_2_ASCII(address["country"].string_value());

                            string village = address["village"].string_value();
                            string town = address["town"].string_value();
                            string suburb = address["suburb"].string_value();
                            string city = address["city"].string_value();
                            string county = address["county"].string_value();
                            string region = address["region"].string_value();

                            string state = address["state"].string_value();
                            string country = address["country"].string_value();

                            string name;
                            if (!village.empty())
                                name = village;
                            else if (!town.empty())
                                name = town;
                            else if (!suburb.empty())
                                name = suburb;
                            else if (!city.empty())
                                name = city;
                            else if (!county.empty())
                                name = county;
                            else if (!region.empty())
                                name = region;


                            if (bMissName && !name.empty())
                                locations[i].m_name = name;

                            if (bMissState && !state.empty())
                                locations[i].SetSSI("State", state);

                            if (bMissCountry && !country.empty())
                                locations[i].SetSSI("Country", country);
                        }
                        else
                        {
                            miss++;
                        }
                    }
                    else
                    {
                        if (error.empty())
                            error = geojson["error"]["message"].string_value();

                        msg.ajoute(error);
                    }
                }//if msg
            }//if empty name

            msg += callback.StepIt();
        }//for all locations

        //pConnection->Close();
        //pSession->Close();
    }//if msg

    callback.PopTask();

    return msg;
}





ERMsg CLocationVector::ExtractOpenTopoDataElevation(CLocationVector& locations, bool bReplaceAll, size_t eProduct, size_t eInterpol, CCallback& callback)
{
    assert(false);
    //assert(eProduct < COpenTopoDataElevation::NB_PRODUCTS);
    //assert(eInterpol < COpenTopoDataElevation::NB_INTERPOL);


    ERMsg msg;

    //http://api.opentopodata.org/v1/test-dataset?locations=56.35,123.90
    //API limits
    //Max 100 locations per request.
    //Max 1 call per second.
    //Max 1000 calls per day.


    //NOAA etopo1 1.8 km (Global, including bathymetry and ice surface elevation near poles)
    //NASA srtm	90 m (Latitudes -60 to 60)
    //NASA srtm 30 m (Latitudes -60 to 60)
    //EEA eudem 25 m (Europe)
    //NASA aster 30 m (Global)
    //USGS ned 10 m (Continental USA, Hawaii, parts of Alaska)

    /*
        string product = COpenTopoDataElevation::PROPDUCT_NAME[eProduct];
        string interpol = COpenTopoDataElevation::INTERPOL_NAME[eInterpol];

        //if (msg)
        //	{
        string URL;

        vector<size_t> loc_to_update;
        //select locations to uptade
        for (size_t i = 0; i < locations.size() && msg; i++)
        {
            if (locations[i].m_elev == -999 || bReplaceAll)
            {
                bool bLimit60 = locations[i].m_lat >= -60 && locations[i].m_lat <= 60;
                bool bNoLimits = eProduct != COpenTopoDataElevation::NASA_SRTM90M && eProduct != COpenTopoDataElevation::NASA_SRTM30M;
                if(bLimit60 || bNoLimits)
                    loc_to_update.push_back(i);
            }
        }

        callback.PushTask("Extract elevation from Open Topo Data (" + to_string(loc_to_update.size()) + " locations)", ceil(loc_to_update.size() / 100));
        callback.AddMessage("Extract elevation from Open Topo Data (" + to_string(loc_to_update.size()) + " locations)");

        size_t ii = 0;
        size_t iii = 0;
        for (size_t i = 0; i < loc_to_update.size() && msg; i++)
        {
            if (ii == 0)
                URL = "https://api.opentopodata.org/v1/" + product + "?&interpolation=" + interpol + "&locations=";
            else
                URL += "|";

            size_t index = loc_to_update[i];
            URL += to_string(locations[index].m_lat) + "," + to_string(locations[index].m_lon);
            ii++;

            if (ii == 100 || i == loc_to_update.size() - 1)
            {
                ii = 0;
                string strGeo;

                string argument = "-s -k \"" + URL + "\"";
                string exe = GetApplicationPath() + "External\\curl.exe";
                CCallcURL cURL(exe);
                msg = cURL.get_text(argument, strGeo);

                //msg = UtilWWW::GetPageText(pConnection, URL, strGeo, false, INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
                if (msg)
                {
                    //extract elevation from google
                    string error;
                    Json json = Json::parse(strGeo, error);
                    assert(json.is_object());

                    if (error.empty() && json["status"] == "OK")
                    {
                        assert(json["results"].is_array());

                        Json::array result = json["results"].array_items();
                        for (size_t i = 0; i < result.size(); i++, iii++)
                        {
                            locations[loc_to_update[iii]].m_elev = result[i]["elevation"].number_value();

                        }
                    }
                    else
                    {
                        if (error.empty())
                            error = json["error"].string_value();

                        msg.ajoute(error);
                    }
                }

                msg += callback.StepIt();
                if (i < (loc_to_update.size() - 1))
                    Sleep(1500);//sleep (API limits)

            }//each 100 locations
        }//for all locations
        //}//if msg

        callback.PopTask();
    */
    return msg;

}


ERMsg CLocationVector::ExtractShoreDistance(CLocationVector& locations, bool bReplaceAll, CCallback& callback)
{
    ERMsg msg;

    callback.PushTask("Extract shore distance", locations.size());


    //process all point
    for (size_t i = 0; i < locations.size() && msg; i++)
    {
        if (bReplaceAll || locations[i].GetDefaultSSI(CLocation::SHORE_DIST).empty())
        {
            double d = CShore::GetShoreDistance(locations[i]) / 1000.0;//distance in km
            locations[i].SetSSI(CLocation::GetDefaultSSIName(CLocation::SHORE_DIST), to_string(d, 1));
        }

        msg += callback.StepIt();
    }//for all locations

    callback.PopTask();

    return msg;
}


//ERMsg CLocationMap::Load(const std::string& filePath)
//{
//    assert(false);
//    //return zen::LoadXML(filePath, "Locations", "1", *this);
//    ERMsg msg;
//    return msg;
//}
//
//ERMsg CLocationMap::Save(const std::string& filePath)
//{
//    assert(false);
//    //return zen::SaveXML(filePath, "Locations", "1", *this);
//    ERMsg msg;
//    return msg;
//}
//
//ERMsg CLocationMap::LoadFromCSV(const std::string& filePath)
//{
//    ERMsg msg;
//
//    clear();
//
//    CLocationVector locations;
//    msg = locations.Load(filePath);
//    if (msg)
//    {
//        for (CLocationVector::const_iterator it = locations.begin(); it != locations.end(); it++)
//        {
//            assert(find(it->m_ID) == end());//no duplication
//            (*this)[it->m_ID] = *it;
//        }
//    }
//
//    return msg;
//}
//
//ERMsg CLocationMap::SaveToCSV(const std::string& filePath)
//{
//    CLocationVector locations;
//    locations.reserve(size());
//
//    for (CLocationMap::const_iterator it = begin(); it != end(); ++it)
//        locations.push_back(it->second);
//
//
//
//    return locations.Save(filePath);
//}

std::ostream& CLocation::operator >> (std::ostream& stream)const
{
    //CGeoPoint3D::operator >> (stream);
    stream.write((char*)(&m_x), sizeof(m_x));
    stream.write((char*)(&m_y), sizeof(m_y));
    stream.write((char*)(&m_z), sizeof(m_z));
    WBSF::WriteBuffer(stream, m_ID);
    WBSF::WriteBuffer(stream, m_name);
    stream << m_siteSpeceficInformation;
    return stream;
}

std::istream& CLocation::operator << (std::istream& stream)
{
    //CGeoPoint3D::operator << (stream);
    stream.read((char*)(&m_x), sizeof(m_x));
    stream.read((char*)(&m_y), sizeof(m_y));
    stream.read((char*)(&m_z), sizeof(m_z));
    WBSF::ReadBuffer(stream, m_ID);
    WBSF::ReadBuffer(stream, m_name);

    m_siteSpeceficInformation >> stream;
    return stream;
}

std::istream& CLocationVector::operator >> (std::istream& stream)
{
    size_t s = 0;
    stream.read((char*)(&s), sizeof(s));
    resize(s);
    for (iterator it = begin(); it != end(); it++)
        stream >> *it;

    return stream;
}

//static bool by_index(std::pair < std::string, std::pair<std::string, size_t>> i, std::pair < std::string, std::pair<std::string, size_t>> j) { return i.second.second < j.second.second; }
std::ostream& CLocationVector::operator << (std::ostream& stream)const
{
    size_t s = size();
    stream.write((char*)(&s), sizeof(s));

    for (const_iterator it = begin(); it != end(); it++)
        stream << *it;

    return stream;
}


std::istream& SiteSpeceficInformationMap::operator >> (std::istream& stream)
{
    size_t s = 0;
    stream.read((char*)(&s), sizeof(s));
    for (size_t i = 0; i < s; i++)
    {
        string key;
        string value;

        WBSF::ReadBuffer(stream, key);
        WBSF::ReadBuffer(stream, value);
        //insert( make_pair(key, make_pair(value, i) ) );
        insert(make_pair(key, value));
    }

    return stream;
}

//static bool by_index(std::pair < std::string, std::pair<std::string, size_t>> i, std::pair < std::string, std::pair<std::string, size_t>> j) { return i.second.second < j.second.second; }
std::ostream& SiteSpeceficInformationMap::operator << (std::ostream& stream)const
{
    size_t s = size();
    stream.write((char*)(&s), sizeof(s));

    //sort(begin(), end(), by_index);
    for (const_iterator it = begin(); it != end(); it++)
    {
        WBSF::WriteBuffer(stream, it->first);
        WBSF::WriteBuffer(stream, it->second);
    }

    return stream;
}




}//namespace WBSF
