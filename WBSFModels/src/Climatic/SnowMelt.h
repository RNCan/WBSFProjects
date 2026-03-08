#pragma once
#include <array>
#include "ModelBased/BioSIMModelBase.h"


namespace WBSF
{

	class CSnowMeltModel : public CBioSIMModelBase
	{
	public:


		CSnowMeltModel();
		virtual ~CSnowMeltModel();

		virtual ERMsg ProcessParameters(const CParameterVector& parameters);

		//virtual ERMsg OnExecuteAnnual();
		virtual ERMsg OnExecuteMonthly();
		//virtual ERMsg OnExecuteDaily();
		//virtual ERMsg OnExecuteHourly();

		static CBioSIMModelBase* CreateObject(){ return new CSnowMeltModel; }
	};
}