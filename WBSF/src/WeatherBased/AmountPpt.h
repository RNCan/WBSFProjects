//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//
//	CAmountPpt: the amout of precipitation from a beta distribution
//******************************************************************************

#pragma once

#include <cassert>
#include "Basic/UtilMath.h"


namespace WBSF
{

	class CBetaXP
	{
	public:

		CBetaXP(double x = 0, double p = 0){ m_x = x; m_p = p; };

		double m_x;
		double m_p;
	};



	class CBetaDistribution
	{
	public:

		CBetaDistribution()
		{}

		CBetaDistribution(double v, double w)
		{
			SetTable(v, w);
		}

		virtual ~CBetaDistribution()
		{}

		void SetTable(double v, double w);
		double XfromP(double p)const;

	private:

		CBetaXP m_XP[51];
	};



	class CAmountPpt
	{
	public:

		static const size_t NUMBER_SPCLASS = 20;

		CAmountPpt();
		virtual ~CAmountPpt();

		size_t GetClass(double Sp)const;
		double GetPprecip(double min, double max, double tot_prec, double Sp);
		double XfromP(double Sp, double p)const
		{
			return m_beta[GetClass(Sp)].XfromP(p);
		}

		const CBetaDistribution& operator[](size_t index)const
		{
			assert(index >= 0 && index < NUMBER_SPCLASS);
			return m_beta[index];
		}

	private:

		CBetaDistribution m_beta[NUMBER_SPCLASS];
	};

}
