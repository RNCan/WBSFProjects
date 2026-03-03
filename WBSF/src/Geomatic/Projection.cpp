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


#include "Basic/UtilStd.h"
#include "Geomatic/Projection.h"


using namespace std;

namespace WBSF
{

    const char* CProjection::PRJ_WGS_84_WKT = "GEOGCRS[\"WGS 84\",DATUM[\"World Geodetic System 1984\",ELLIPSOID[\"WGS 84\", 6378137, 298.257223563,LENGTHUNIT[\"metre\", 1]]],PRIMEM[\"Greenwich\", 0,ANGLEUNIT[\"degree\", 0.0174532925199433]],CS[ellipsoidal, 2],AXIS[\"geodetic latitude (Lat)\", north,ORDER[1],ANGLEUNIT[\"degree\", 0.0174532925199433]],AXIS[\"geodetic longitude (Lon)\", east,ORDER[2],ANGLEUNIT[\"degree\", 0.0174532925199433]],USAGE[SCOPE[\"unknown\"],AREA[\"World\"],BBOX[-90, -180, 90, 180]],ID[\"EPSG\", 4326]]";

    TProjection CProjection::GetPrjID(const std::string& srs)
    {
        assert(false);//todo
    }

	CProjection::CProjection(bool bWGS84)
	{
	    m_prj_type=PRJ_NOT_INIT;
		if (bWGS84)
		{
			m_srs= PRJ_WGS_84_WKT;
            m_prj_type=PRJ_GEOGRAPHIC;
		}
	}

	CProjection::CProjection(const std::string& srs)
	{
        m_srs=srs;
        m_prj_type=GetPrjID(srs);
	}

	CProjection::CProjection(CProjection const& in)
	{
		operator=(in);
	}

	CProjection::~CProjection()
	{
	}

	CProjection& CProjection::operator =(CProjection const& in)
	{
		m_srs=in.m_srs;
		m_prj_type=in.m_prj_type;

		return *this;
	}

	void CProjection::clear()
	{
		m_srs.clear();
		m_prj_type=PRJ_NOT_INIT;

	}


	bool CProjection::operator ==(CProjection const& in)const
	{
	    return m_srs == in.m_srs;
    }




	ERMsg CProjection::Load(const std::string& name)
	{
		ERMsg msg;


		ifStream file;
		msg = file.open(name);
		if (msg)
		{
			m_srs = file.GetText();
            m_prj_type=GetPrjID(m_srs );


			file.close();
		}

		return msg;
	}

	ERMsg CProjection::Save(const std::string& name)
	{
		ERMsg msg;

        fStream file;
        msg = file.open(name);
        if (msg)
        {
            file.write(m_srs.c_str(), m_srs.length());
            file.close();
        }


		return msg;
	}

	string CProjection::GetPrjName()const
	{
		string name = "Unknown";

        switch(m_prj_type)
        {
            case PRJ_NOT_INIT:name = "Unknown";break;
            case PRJ_GEOGRAPHIC:name = "Geographic";break;
            case PRJ_PROJECTED:name = "Projected";break;
            case PRJ_GEOCENTRIC:name = "Geocentric";break;
            default:name = "Unknown";
        }




		return name;
	}


}
