//**********************************************************************
// 14/04/2022	1.0.0	Rémi Saint-Amant    Model create for Kishan
//**********************************************************************
#include <cstdio>
#include <cmath>
#include <cassert>
#include <cfloat>
#include <limits>

#include "WeatherBased/WeatherDefine.h"
#include "ModelBased/EntryPoint.h"
#include "ClimaticMPB.h"


using namespace std;
using namespace WBSF::HOURLY_DATA;


namespace WBSF
{
	//this line link this model with the EntryPoint of the DLL
	static const bool bRegistred =
		CModelFactory::RegisterModel(CClimaticMPB::CreateObject);



	CClimaticMPB::CClimaticMPB()
	{
		NB_INPUT_PARAMETER = 0;
		VERSION = "1.0.0 (2022)";

		// initialize your variable here (optional)
	}

	CClimaticMPB::~CClimaticMPB()
	{}


	//	nbDay annual lowest Tmin lower than -20°C
	//	percent of days when lowest Tmin lower than -20°C
	//	nbDay annual highest Tmax greater than 32°C
	//	percent of days when highest Tmax greater than 32°C
	
	enum TOutput
	{
		O_NB_DAYS_LOWER20, O_PERCENT_LOWER20, NB_DAYS_GREATER32, NB_PERCENT_GREATER32,	NB_OUTPUTS
	};

	//this method is call to load your parameter in your variable
	ERMsg CClimaticMPB::ProcessParameters(const CParameterVector& parameters)
	{
		ERMsg message;

		//transfer your parameter here
		int c = 0;
		return message;
	}


	ERMsg CClimaticMPB::OnExecuteAnnual()
	{
		ERMsg message;

		CTPeriod p = m_weather.GetEntireTPeriod(CTM(CTM::ANNUAL));
		p.begin().m_year++;

		m_output.Init(p, NB_OUTPUTS, -999);


		for (size_t y = 1; y < m_weather.size(); y++)
		{
			int year = m_weather[y].GetTRef().GetYear();
			CTPeriod winterPeriod(CTRef(year-1, DECEMBER, DAY_01), CTRef(year, FEBRUARY, DAY_28));
			CTPeriod summerPeriod(CTRef(year, MAY, DAY_01), CTRef(year, SEPTEMBER, DAY_30));

			

			int nbDaysLower20 = 0;
			for (CTRef TRef = winterPeriod.begin(); TRef <= winterPeriod.end(); TRef++)
			{
				double T = m_weather[TRef][H_TMIN][LOWEST];
				if (T <= -20.0)
					nbDaysLower20 ++;
			}
			
			int nbDaysGreater32 = 0;
			for (CTRef TRef = summerPeriod.begin(); TRef <= summerPeriod.end(); TRef++)
			{
				double T = m_weather[TRef][H_TMAX][HIGHEST];
				if (T >= 32.0)
					nbDaysGreater32++;
			}

			m_output[y-1][O_NB_DAYS_LOWER20] = nbDaysLower20;
			m_output[y-1][O_PERCENT_LOWER20] = 100.0 * nbDaysLower20/ winterPeriod.length(CTM::DAILY); //[%]
			m_output[y-1][NB_DAYS_GREATER32] = nbDaysGreater32;
			m_output[y-1][NB_PERCENT_GREATER32] = 100.0*nbDaysGreater32 / summerPeriod.length(CTM::DAILY);//[%]
			
			HxGridTestConnection();
		}



		return message;
	}




}