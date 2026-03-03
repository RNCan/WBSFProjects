#pragma once

#include "ogrsf_frmts.h"

#include "ModelBased/BioSIMModelBase.h"
#include "Basic/UtilTime.h"


namespace WBSF
{



	class CWaterBalanceModel : public CBioSIMModelBase
	{
	public:


		CWaterBalanceModel();



		virtual ERMsg OnExecuteMonthly();
		virtual ERMsg ProcessParameters(const CParameterVector& parameters);
		static CBioSIMModelBase* CreateObject(){ return new CWaterBalanceModel; }

	protected:

		static std::mutex m_mutex;
		static GDALDataset* m_poDS;
		double GetAWHC(const CLocation& loc);

	};
}
