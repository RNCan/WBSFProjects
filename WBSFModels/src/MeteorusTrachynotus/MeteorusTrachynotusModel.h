#pragma once

#include "ModelBased/BioSIMModelBase.h"
#include "MeteorusTrachynotus.h"

namespace WBSF
{
	class CMeteorusTrachynotusModel : public CBioSIMModelBase
	{

	public:

		CMeteorusTrachynotusModel();
		virtual ~CMeteorusTrachynotusModel();

		virtual ERMsg ProcessParameters(const CParameterVector& parameters);
		virtual ERMsg OnExecuteDaily();
		virtual ERMsg OnExecuteAnnual();
		virtual ERMsg OnExecuteAtemporal();

		static CBioSIMModelBase* CreateObject(){ return new CMeteorusTrachynotusModel; }

		//function for simulated annealing
		//virtual void AddDailyResult(const StringVector& header, const StringVector& data);
		//virtual void GetFValueDaily(CFL::CStatisticXY& stat);

	protected:

		//void GetSpruceBudwormBiology(CWeatherStation& weather, CModelStatVector& SBWStat);
		void ExecuteDailyAllGenerations(CModelStatVector& SBWStat, std::vector<CModelStatVector>& stat);


		//bool m_bHaveAttrition;
		//double m_generationAttrition;
		//double m_diapauseAge;
		//double m_lethalTemp;
		//double m_criticalDaylength;
		//bool m_bOnGround;

	};




}