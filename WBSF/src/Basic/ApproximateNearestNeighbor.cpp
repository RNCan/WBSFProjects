//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************



#include <boost/locale.hpp>
#include <boost/format.hpp>

#include "Basic/UtilStd.h"
#include "Basic/Shore.h"
#include "ApproximateNearestNeighbor.h"


//#include "WeatherBasedSimulationString.h"

using namespace std;
//namespace bl = boost::locale;
//using boost::locale::translate;


namespace WBSF
{

    std::mutex CApproximateNearestNeighbor::Mutex;


double GetShoreDistance(const CLocation& pt)
{
    double shore = pt.GetShoreDistance();
    //if (shore < 0)
    //shore = CShore::GetShoreDistance(pt);
    return shore;
}

double GetFactor(size_t d)
{
    //assert(false);//todo

    double f = 1;
    if (d == 3)
        f = 100;//WEATHER::ELEV_FACTOR;
    else if (d == 4)
        f = 1;//WEATHER::SHORE_DISTANCE_FACTOR;

    return f;
}
//*****************************************************************
constexpr  int64_t CApproximateNearestNeighbor::VERSION = 1;
CApproximateNearestNeighbor::CApproximateNearestNeighbor()
{
    m_pTreeRoot = NULL;
    m_pDataPts = NULL;
    m_nSize = 0;
    m_nbDimension = 0;
}


CApproximateNearestNeighbor::CApproximateNearestNeighbor(const CApproximateNearestNeighbor& in)
{
    m_pTreeRoot = NULL;
    m_pDataPts = NULL;
    operator=(in);
}


CApproximateNearestNeighbor& CApproximateNearestNeighbor::operator=(const CApproximateNearestNeighbor& in)
{
    if (&in != this)
    {
        clear();

        m_nbDimension = in.m_nbDimension;
        m_nSize = in.m_nSize;
        if (m_nSize*m_nbDimension > 0)
        {
            m_pDataPts = annAllocPts(m_nSize, m_nbDimension);

            size_t the_size = m_nSize*m_nbDimension*sizeof(ANNcoord);
            std::memcpy(m_pDataPts[0], in.m_pDataPts[0], the_size);
            m_positions = in.m_positions;
        }
    }

    return *this;
}



CApproximateNearestNeighbor::~CApproximateNearestNeighbor()
{
    clear();
}


void CApproximateNearestNeighbor::clear()
{
//		MUTEX.Enter();//?????a vérifier

    if (m_pTreeRoot)
    {
        delete m_pTreeRoot;
        m_pTreeRoot = NULL;
        //annClose();//delete global variable: in multi-thread is not a good idea
    }

    if (m_pDataPts)
    {
        annDeallocPts(m_pDataPts);
        m_pDataPts = NULL;
    }

    m_nSize = 0;
    m_nbDimension = 0;
    m_positions.clear();

    //MUTEX.Leave();
}

ostream& CApproximateNearestNeighbor::operator >> (ostream& stream)const
{
    assert(m_nbDimension == 3 || m_nbDimension == 4 || m_nbDimension == 5);
    stream.write((char*)&VERSION, sizeof(VERSION));
    stream.write((char*)&m_nSize, sizeof(m_nSize));
    stream.write((char*)&m_nbDimension, sizeof(m_nbDimension));
    if (m_pDataPts)
        stream.write((char*)(&(m_pDataPts[0][0])), m_nSize*m_nbDimension*sizeof(ANNcoord));//used adress of point vector

    stream << (int64_t)m_positions.size();
    if (!m_positions.empty())
        stream.write((char*)&(m_positions[0]), m_positions.size()*sizeof(int64_t));

    return stream;
}


istream& CApproximateNearestNeighbor::operator << (istream& stream)
{
    clear();

    int64_t version = 0;
    stream.read((char*)&version, sizeof(version));
    assert(version == VERSION);

    stream.read((char*)&m_nSize, sizeof(m_nSize));
    stream.read((char*)&m_nbDimension, sizeof(m_nbDimension));
    assert(m_nbDimension == 3 || m_nbDimension == 4 || m_nbDimension == 5);
    if (m_nSize*m_nbDimension > 0)
    {
        m_pDataPts = annAllocPts(m_nSize, m_nbDimension);
        stream.read((char*)(&(m_pDataPts[0][0])), m_nSize*m_nbDimension*sizeof(ANNcoord));//used adress of point vector
    }

    int64_t posSize = 0;
    stream >> posSize;
    m_positions.resize(posSize);
    if (!m_positions.empty())
        stream.read((char*)&(m_positions[0]), posSize*sizeof(int64_t));

    return stream;
}

void CApproximateNearestNeighbor::set(const CLocationVector& locations, bool bUseElevation, bool bUseShoreDistance, const vector<int64_t>& positions)
{
    assert(positions.empty() || positions.size() == locations.size());

    clear();

    m_nbDimension = (bUseShoreDistance /*&&WEATHER::SHORE_DISTANCE_FACTOR > 0*/) ? 5 : bUseElevation ? 4 : 3;

    m_nSize = locations.size();
    if (m_nbDimension*m_nSize > 0)
        m_pDataPts = annAllocPts(m_nSize, m_nbDimension);

    m_positions = positions;

    size_t i = 0;
    for (CLocationVector::const_iterator it = locations.begin(); it != locations.end(); it++, i++)
    {
        for (int64_t d = 0; d < m_nbDimension; d++)
            m_pDataPts[i][d] = it->GetGeocentricCoord(d)*GetFactor(d);
    }
}

CLocation CApproximateNearestNeighbor::at(size_t i)const
{
    CLocation pt;
    pt.SetXY(m_pDataPts[i][0], m_pDataPts[i][1], m_pDataPts[i][2]);
    if (m_nbDimension >= 4)
        pt.m_z = m_pDataPts[i][3] / GetFactor(3);
    if (m_nbDimension >= 5)
        pt.SetShoreDistance(m_pDataPts[i][4] / GetFactor(4));

    return pt;
}



void CApproximateNearestNeighbor::init()
{

    if (m_pTreeRoot == NULL)
    {

        if (m_pTreeRoot)
        {
            delete m_pTreeRoot;
            m_pTreeRoot = NULL;
        }

        if (m_nSize*m_nbDimension > 0)
        {
            assert(m_pDataPts);

            Mutex.lock();
            m_pTreeRoot = (ANNkd_tree*)new ANNkd_tree(m_pDataPts, m_nSize, m_nbDimension);
            Mutex.unlock();
        }
    }

}

ERMsg CApproximateNearestNeighbor::search(const CLocation& pt, int64_t nbPoint, CSearchResultVector& result, double eps)const
{
    ERMsg msg;

    CApproximateNearestNeighbor& me = const_cast<CApproximateNearestNeighbor&>(*this);
    me.init();

    result.clear();

    //We make the search event if they are not enough points
    int64_t nbPointSearch = min(nbPoint, m_nSize);
    assert(nbPointSearch <= m_nSize);

    if (m_nSize > 0)
    {
        assert(m_nbDimension >= 3 && m_nbDimension <= 5);

        result.resize(nbPointSearch);

        ANNidxArray	nn_idx = new ANNidx[nbPointSearch];		// allocate near neigh indices
        ANNdistArray	dd = new ANNdist[nbPointSearch];	// allocate near neighbor dists
        ANNpoint	q = annAllocPt(m_nbDimension);			// query point

        for (int64_t d = 0; d < m_nbDimension; d++)
            q[d] = pt.GetGeocentricCoord(d)*GetFactor(d);//elevation have scaled by 100

        m_pTreeRoot->annPkSearch(q, nbPointSearch, nn_idx, dd, eps);
        for (int64_t i = 0; i < nbPointSearch; i++)
        {


            size_t index = nn_idx[i];
            result[i].m_index = SearchPos2LocPos(index);
            //result[i].m_distance = sqrt(dd[i]);
            result[i].m_location.SetXY(m_pDataPts[index][0], m_pDataPts[index][1], m_pDataPts[index][2]);
            result[i].m_distance = pt.GetDistance(result[i].m_location, false, false);

            //if (m_nbDimension == 3)
            //{
            //    static const double _2x6371x1000_ = 2 * 6371 * 1000;
//          //      double test = _2x6371x1000_*asin( sqrt(dd[i]) / _2x6371x1000_);//apply sphere curve to distance
            //  //  int uu;
            //    //uu = 0;
            //    //result[i].m_distance = _2x6371x1000_*asin(result[i].m_distance / _2x6371x1000_);//apply sphere curve to distance
//
            //}



            if (m_nbDimension >= 4)
            {
                result[i].m_location.m_z = m_pDataPts[index][3] / GetFactor(3);
                result[i].m_deltaElev = result[i].m_location.m_z - pt.m_z;
            }

            if (m_nbDimension >= 5)
            {
                double shore = m_pDataPts[index][4] / GetFactor(4);
                result[i].m_location.SetShoreDistance(shore);
                result[i].m_deltaShore = shore - GetShoreDistance(pt);
            }

            //compute virtual distance only after setted elevation and shore distance
            result[i].m_virtual_distance = pt.GetDistance(result[i].m_location, m_nbDimension >= 4, m_nbDimension >= 5);
        }

        annDeallocPt(q);
        delete[] nn_idx;
        delete[] dd;

        assert(result.size() == (size_t)nbPointSearch);
    }

    if (result.size() != (size_t)nbPoint)
    {
        //boost::format fmt = boost::format("The number of neighbor found (%1%) is lower than the number requested (%2%).") % result.size() % nbPoint;
        msg.ajoute(FormatMsg("The number of neighbor found (%1%) is lower than the number requested (%2%).", to_string(result.size()), to_string(nbPoint)));
    }

    return msg;
}

}//namespace WBSF
