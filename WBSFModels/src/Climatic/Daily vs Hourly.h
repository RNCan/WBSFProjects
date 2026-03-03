#pragma once
#include <array>
#include "ModelBased/BioSIMModelBase.h"


namespace WBSF
{

	class CDaily_vs_Hourly : public CBioSIMModelBase
	{
	public:


		CDaily_vs_Hourly();
		virtual ~CDaily_vs_Hourly();

		virtual ERMsg ProcessParameters(const CParameterVector& parameters);

		//virtual ERMsg OnExecuteAnnual();
		//virtual ERMsg OnExecuteMonthly();
		virtual ERMsg OnExecuteDaily();
		virtual ERMsg OnExecuteHourly();
		
		//virtual void AddSAResult(const std::vector<std::string>& header, const std::vector<std::string>& data);
		//virtual void GetFValueHourly(CStatisticXY& stat);
		//virtual void GetFValueDaily(CStatisticXY& stat);
		//virtual void GetFValueMonthly(CStatisticXY& stat);

		static CBioSIMModelBase* CreateObject(){ return new CDaily_vs_Hourly; }
	};
}