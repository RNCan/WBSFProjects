#pragma once

#include "Basic/ERMsg.h"
#include "WeatherBased/WeatherStation.h"
#include "Basic/ModelStat.h"

namespace WBSF
{

	class CPlantHardinessUSA
	{
	public:

		CPlantHardinessUSA()
		{}

		void Compute(const CWeatherStation& weather, CModelStatVector& result);


		static double GetSuitabilityIndex(const CWeatherStation& weather);
	};

}
