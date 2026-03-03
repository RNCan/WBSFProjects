//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
// 15-09-2008	Rémi Saint-Amant	Created from old file
//****************************************************************************


#include "WeatherBased/AmountPpt.h"

namespace WBSF
{
	//***********************************************************************
	void CBetaDistribution::SetTable(double alpha, double beta)
	{

		int i;
		double tmp;
		double sum_P = 0;

		//Compute Beta cumulative probability distribution

		m_XP[0].m_x = 0;
		m_XP[0].m_p = 0;
		for (i = 1; i <= 50; ++i)
		{
			tmp = (double)i / 50.;
			m_XP[i].m_x = tmp;
			if (i == 50) tmp = 0.995;
			sum_P += pow(tmp, alpha - 1) * pow((1 - tmp), beta - 1);
			m_XP[i].m_p = sum_P;
		}
		//Scale so P is [0,1]
		for (i = 0; i <= 50; ++i)
		{
			m_XP[i].m_p /= sum_P;
		}
	}

	double CBetaDistribution::XfromP(double p)const
	{
		assert(p >= 0.0 && p <= 1.0);
		double x = 0;
		for (int i = 49; i >= 0; --i)
		{
			if (p > m_XP[i].m_p)
			{
				double slope = (m_XP[i + 1].m_x - m_XP[i].m_x) / (m_XP[i + 1].m_p - m_XP[i].m_p);
				x = m_XP[i].m_x + (p - m_XP[i].m_p)* slope;
				break;
			}
		}

		assert(!std::isnan(x));
		assert(x >= 0);
		return x;
	}

	//***********************************************************************
	//CAmountPpt

	CAmountPpt::CAmountPpt()
	{
		static const double m[6] = { 0.2, 29.6347, 4.0251, 0.2837, 0.0392, 0.0353 };
		static const double SP_CENTER[NUMBER_SPCLASS] = {
			0.25, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.65, 0.75,
			0.85, 0.95, 1.125, 1.375, 1.625, 1.875, 2.25, 2.75, 3.25, 3.5 };

		for (size_t i = 0; i < NUMBER_SPCLASS; i++)
		{
			//Equations for v and w of the Beta(v,w) distribution
			double w = m[0] + m[1] / pow((SP_CENTER[i] + 1), m[2]); //Régnière (2007) Equ. [18]
			double v = m[3] + m[4] * w + m[5] / w; //Régnière (2007) Equ. [19]

			m_beta[i].SetTable(v, w);
		}
	}

	CAmountPpt::~CAmountPpt()
	{
	}

	size_t CAmountPpt::GetClass(double Sp)const
	{
		assert(Sp >= 0);

		size_t p_class = 0;
		if (Sp > 0.25)
			p_class = 1;
		if (Sp > 0.3)
			p_class = 2;
		if (Sp > 0.35)
			p_class = 3;
		if (Sp > 0.4)
			p_class = 4;
		if (Sp > 0.45)
			p_class = 5;
		if (Sp > 0.5)
			p_class = 6;
		if (Sp > 0.55)
			p_class = 7;
		if (Sp > .6)
			p_class = 8;
		if (Sp > .7)
			p_class = 9;
		if (Sp > .8)
			p_class = 10;
		if (Sp > .9)
			p_class = 11;
		if (Sp > 1.)
			p_class = 12;
		if (Sp > 1.25)
			p_class = 13;
		if (Sp > 1.5)
			p_class = 14;
		if (Sp > 1.75)
			p_class = 15;
		if (Sp > 2.)
			p_class = 16;
		if (Sp > 2.5)
			p_class = 17;
		if (Sp > 3.)
			p_class = 18;
		if (Sp > 3.5)
			p_class = 19;

		assert( p_class < NUMBER_SPCLASS);
		return (p_class);
	}

	double CAmountPpt::GetPprecip(double min, double max, double tot_prec, double Sp)
	{
		Sp = std::max(Sp, 0.25);

		// Régnière (2007) Equ. [14]-[17]
		double k[8] = { 1.5440, 1.0229, 1.4592, -1.1396, -0.9252, 4.8753, -0.1660, 1.6168 };

		//	Equ. [17]
		double w1 = k[4] + k[5] * Sp;
		double w2 = k[6] + k[7] * Sp;
		assert(w1>0 && w2 > 0);

		double R = std::min(25., std::max(3., (max - min))) + 1.0f;

		//	Weibull probability density function (equation [16])
		double tmp2 = pow(R / w1, w2);
		double tmp1 = tmp2 / (R / w1); //same as pow(R/w1,w2-1)
		double W = log10(w2 / w1*tmp1*exp(-tmp2));

		//Logistic regression results (equation [14])
		tmp1 = log10(tot_prec + 1);
		double egP = exp(k[0] + k[1] * tmp1 + k[2] * W + k[3] * Sp);

		double pprec = egP / (1 + egP); // Equation [15]
		assert(!std::isnan(pprec));


		return (pprec);
	}

}
