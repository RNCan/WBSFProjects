//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
// 10-02-2009 	Remi Saint-Amant	Inherite from vector
// 01-01-2005	Remi Saint-Amant	Creation
//******************************************************************************

#include <algorithm>
#include <cfloat>

#include "Basic/ModelStat.h"
#include "Basic/UtilStd.h"
#include "Basic/UtilMath.h"
//#include "Basic/CSV.hpp"
#include "WeatherBased/WeatherDefine.h"
#include "ModelBased/WeatherBasedSimulation.h"
#include "ModelBased/InitialPopulation.h"

using namespace std;

namespace WBSF
{

CInitialPopulation::CInitialPopulation(size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
{
    Initialize(nbObjects, initialPopulation, age, sex, bFertil, generation);
}

CInitialPopulation::CInitialPopulation(CTRef peakDay, double sigma, size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
{
    Initialize(peakDay, sigma, nbObjects, initialPopulation, age, sex, bFertil, generation);
}
void CInitialPopulation::Initialize(CTRef peakDay, double sigma, size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
{
    assert(nbObjects > 0);
    assert(initialPopulation > 0);

    clear();
    if (peakDay.is_init())
    {
        assert((peakDay.GetDOY() - 3 * sigma) >= 0 && (peakDay.GetDOY() + 3 * sigma) <= 365); //all attack within first year

        size_t nbCreated = 0;
        double cumulCreated = 0;
        double scaleFactor = initialPopulation / nbObjects;

        for (CTRef TRef = peakDay - int(3 * sigma); TRef <= peakDay + int(3 * sigma) && nbCreated < nbObjects; TRef++)
        {
            cumulCreated += sigma > 0 ? nbObjects / (sigma*pow((2 * PI), 0.5)) * exp(-0.5*square((TRef - peakDay) / sigma)) : nbObjects;

            size_t nbObjectToCreate = size_t(max(0, (int)std::round(cumulCreated - nbCreated)));
            for (size_t i = 0; i < nbObjectToCreate; i++)
                push_back(CIndividualInfo(TRef, age, sex, bFertil, generation, scaleFactor));

            nbCreated += nbObjectToCreate;
        }

        assert(abs(int(size()) - int(nbObjects)) <= 1);

        while (size() < nbObjects)
            push_back(CIndividualInfo(peakDay, age, sex, bFertil, generation, scaleFactor));


        assert(size() == nbObjects);
    }
}

void CInitialPopulation::Initialize(size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
{
    assert(nbObjects > 0);
    assert(initialPopulation > 0);

    clear();
    double scaleFactor = initialPopulation / nbObjects;

    for (size_t i = 0; i < nbObjects; i++)
        push_back(CIndividualInfo(CTRef(), age, sex, bFertil, generation, scaleFactor));

}

//**********************************************************************************************

CInitialPopulation CModelStatVector::GetInitialPopulation(size_t var, size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation, CTPeriod p)const
{
    CInitialPopulation population;

    if (!p.is_init())
        p = GetTPeriod();

    CStatistic stat = GetStat(var, p);

    if (stat[SUM] > 100)//need at least 100 eggs
    {
        //Get nbDay with activity
        double scaleFactor = initialPopulation / nbObjects;

        size_t nbCreated = 0;
        double cumulCreated = 0;
        CTPeriod period = GetTPeriod(var, ">", 0, 0, 0, p);
        assert(period.is_init());

        for (CTRef TRef = period.begin(); TRef <= period.end(); TRef++)
        {
            cumulCreated += nbObjects * at(TRef).at(var) / stat[SUM];

            size_t nbObjectToCreate = size_t(max(0, (int)round(cumulCreated - nbCreated)));
            for (size_t i = 0; i < nbObjectToCreate; i++)
                population.push_back(CIndividualInfo(TRef, age, sex, bFertil, generation, scaleFactor));

            nbCreated += nbObjectToCreate;
        }

        assert(population.size() == nbObjects);

        double ipTest = 0;
        for (size_t i = 0; i < population.size(); i++)
            ipTest += population[i].m_scaleFactor;

        assert(fabs(ipTest - initialPopulation) < 0.001);
        //for(size_t i=size(); i<nbObjects; i++)
        //proportionVector.push_back( CProportionStat(peak, nbIndividuPerObject) );
    }

    return population;
}



}//namespace WBSF
