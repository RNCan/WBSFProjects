#include "ModelBase/BioSIMModelBase.h"




namespace WBSF
{
	
	class CLaricobiusModel : public CBioSIMModelBase
	{

	public:

		enum TSpecies {S_LN, S_LO, NB_SPECIES};
		enum TParam { Τᴴ¹, Τᴴ², delta, μ, ѕ, NB_CDD_PARAMS };
		enum TAllParam { P_LN= NB_CDD_PARAMS * S_LN, P_LO= NB_CDD_PARAMS* S_LO, P_EOD_A= NB_SPECIES * NB_CDD_PARAMS, P_EOD_B, P_EOD_C, NB_PARAMS};

		CLaricobiusModel();
		virtual ~CLaricobiusModel();

		virtual ERMsg OnExecuteDaily()override;
		virtual ERMsg ProcessParameters(const CParameterVector& parameters)override;

		static CBioSIMModelBase* CreateObject(){ return new CLaricobiusModel; }

		virtual void AddDailyResult(const StringVector& header, const StringVector& data)override;
		virtual bool GetFValueDaily(CStatisticXY& stat)override;

		protected:

		std::array<double, NB_PARAMS > m_P;
		

		bool m_bCumul;
		void CalibrateAdultEmergence(CStatisticXY& stat);
		void GetCDD(const CWeatherYears& weather, std::array < CModelStatVector, NB_SPECIES>& output);
		void GetPobs(CModelStatVector& P);

		bool IsParamValid()const;
	};

}