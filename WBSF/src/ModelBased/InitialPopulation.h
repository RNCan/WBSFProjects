//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************

#pragma once

#include <vector>
#include <cassert>

#include "Basic/ModelStat.h"
#include "ModelBased/WeatherBasedSimulation.h"

namespace WBSF
{
class CIndividualInfo
{
public:
    CIndividualInfo(CTRef creationDate = CTRef(), double age = 0, TSex sex = RANDOM_SEX, bool bFertil = false, size_t generation = 0, double scaleFactor = 0)
    {
        m_creationDate = creationDate;
        m_age = age;
        m_sex = sex;
        m_bFertil = bFertil;
        m_generation = generation;
        m_scaleFactor = scaleFactor;
    }


    CTRef m_creationDate;
    double m_age;
    TSex m_sex;
    bool m_bFertil;
    size_t m_generation;
    double m_scaleFactor;
};

class CInitialPopulation : public std::vector < CIndividualInfo >
{
public:

    CInitialPopulation(size_t nbObjects, double initialPopulation, double age, TSex sex = RANDOM_SEX, bool bFertil = false, size_t generation = 0);
    CInitialPopulation(CTRef peakDay = CTRef(), double sigma = 0, size_t nbObjects = 400, double initialPopulation = 100, double age = 0, TSex sex = RANDOM_SEX, bool bFertil = false, size_t generation = 0);
    void Initialize(size_t nbObjects, double initialPopulation, double age, TSex sex = RANDOM_SEX, bool bFertil = false, size_t generation = 0);
    void Initialize(CTRef peakDay, double sigma = 0, size_t nbObjects = 400, double initialPopulation = 100, double age = 0, TSex sex = RANDOM_SEX, bool bFertil = false, size_t generation = 0);


    void UpdateYear(int year)
    {
        for (iterator it = begin(); it != end(); it++)
            it->m_creationDate.m_year = year;
    }
};


}//namespace WBSF
