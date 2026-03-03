//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
#pragma once

#include "Basic/GeoBasic.h"

namespace WBSF
{

	//**************************************************************************************************************************
	class CProjection
	{
	public:

        static const char* PRJ_WGS_84_WKT;
        static TProjection GetPrjID(const std::string& srs);

		CProjection(const std::string& srs);
		CProjection(bool bWGS84=false);
		CProjection(CProjection const& in);
		~CProjection();

		CProjection& operator =(CProjection const& in);


		bool operator ==(CProjection const& in)const;
		bool operator !=(CProjection const& in)const{ return !operator==(in);}

		void clear();

		ERMsg Load(const std::string& name);
		ERMsg Save(const std::string& name);

		const std::string& GetPrjStr() {return m_srs;}
		void SetPrjStr(const std::string& in){m_srs = in;m_prj_type=GetPrjID(m_srs);}

		std::ostream& operator>>(std::ostream &s)const{	s << m_srs;	return s;	}
		std::istream& operator<<(std::istream &s){s >> m_srs; return s;}
		friend std::ostream& operator<<(std::ostream &s, CProjection const & pt){ pt>>s; return s;}
		friend std::istream& operator>>(std::istream &s, CProjection& pt){ pt<<s;	return s; }



		std::string GetPrjName()const;
		TProjection GetPrjID()const{return m_prj_type;}

	protected:

		std::string m_srs;
		TProjection m_prj_type;
	};

	typedef std::shared_ptr<CProjection> CProjectionPtr;



}
