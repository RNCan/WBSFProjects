//*********************************************************************
//22/05/2017	Rémi Saint-Amant	Creation
//*********************************************************************
#include "WeatherBased/WeatherDefine.h"
#include "ModelBased/EntryPoint.h"
#include "ClimdexVariables.h"
#include "ClimdexModel.h"


using namespace WBSF::HOURLY_DATA;
using namespace std;


namespace WBSF
{


	//this line link this model with the EntryPoint of the DLL
	static const bool bRegistred =
		CModelFactory::RegisterModel(CClimdexModel::CreateObject);


	CClimdexModel::CClimdexModel()
	{
		// initialise your variable here (optionnal)
		NB_INPUT_PARAMETER = 3;
		VERSION = "1.0.0 (2017)";
	}

	CClimdexModel::~CClimdexModel()
	{
	}

	ERMsg CClimdexModel::ProcessParameters(const CParameterVector& parameters)
	{
		ERMsg msg;

		std::vector<std::string> str = Tokenize(parameters[0].GetString(), "-");
		m_nn = parameters[1].GetReal();
		m_bUseBootstrap = parameters[2].GetBool();

		assert(str.size() == 2);
		m_basePeriod = CTPeriod(CTRef(stoi(str[0]), JANUARY, DAY_01), CTRef(stoi(str[1]), DECEMBER, DAY_31));
		CTPeriod simulationP = m_weather.GetEntireTPeriod(CTM::DAILY);
		if (!simulationP.is_inside(m_basePeriod))
			msg.ajoute("The base period " + m_basePeriod.GetFormatedString() + " is not inside the simulation period " + simulationP.GetFormatedString());


		return msg;
	}

	ERMsg CClimdexModel::OnExecuteMonthly()
	{
		ERMsg msg;

		m_weather.SetHourly(false);//force to became daily

		CClimdexVariables climdex;
		climdex.m_basePeriod = m_basePeriod;
		climdex.m_nn = m_nn;
		climdex.m_bUseBootstrap = m_bUseBootstrap;
		msg = climdex.Execute(CTM::MONTHLY, m_weather, m_output);


		return msg;
	}



	//This method is call to compute solution
	ERMsg CClimdexModel::OnExecuteAnnual()
	{
		ERMsg msg;

		m_weather.SetHourly(false);//force to became daily

		CClimdexVariables climdex;
		climdex.m_basePeriod = m_basePeriod;
		climdex.m_nn = m_nn;
		climdex.m_bUseBootstrap = m_bUseBootstrap;
		msg = climdex.Execute(CTM::ANNUAL, m_weather, m_output);

		return msg;
	}

}
