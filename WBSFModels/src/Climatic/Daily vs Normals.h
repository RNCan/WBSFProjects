//*****************************************************************************
// File: DegreeDay.h
//
// Class: CTemperatureIndexModel
//*****************************************************************************

#pragma once

#include "Basic/ModelStat.h"
#include "WeatherBased/DegreeDays.h"
#include "ModelBased/BioSIMModelBase.h"



namespace WBSF
{

	//**********************************************************
	class CTemperatureIndexModel : public CBioSIMModelBase
	{
	public:

		CTemperatureIndexModel();
		virtual ~CTemperatureIndexModel();

		
		virtual ERMsg OnExecuteDaily ();
		virtual ERMsg ProcessParameters(const CParameterVector& parameters);


		CModelStatVector GenerateNormals();
		static CBioSIMModelBase* CreateObject(){ return new CTemperatureIndexModel; }

	};

}