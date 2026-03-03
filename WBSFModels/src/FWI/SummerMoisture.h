#pragma once
#include "Basic/ERMsg.h"
#include "Basic/ModelStat.h"
#include "WeatherBased/WeatherStation.h"
#include "ModelBased/InputParam.h"


namespace WBSF
{


	class CSummerMoisture
	{
		// Construction
	public:

		ERMsg Execute(const CWeatherStation& weather, CModelStatVector& output);


		bool m_bOverwinter;
		double m_a;
		double m_b;



		static double ComputeIndice(int year, size_t m, double& DCMo, double Rm, double Tm, bool bSouthHemis);
	};

}