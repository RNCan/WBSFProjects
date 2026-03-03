//***************************************************************************
// File:        TGInput.h
//
// Class:       CWGInput
//
// Abstract:    Manage input parameters for TemGen
//
// Description: A container class with XML serialisation for TemGen Input
//
// Note:
//***************************************************************************
#pragma once

#include "ModelBased/ModelInput.h"
#include "ModelBased/WGInput.h"
#include "ModelBased/ModelOutputVariable.h"
#include "ModelBased/ModelInputParameter.h"

namespace WBSF
{

void WGInput2ModelInput(const CWGInput& WGInput, CModelInput& modelInput);
void ModelInput2WGInput(const CModelInput& modelInput, CWGInput& WGInput);

}
