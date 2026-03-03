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

#include "Basic/UtilStd.h"
#include "Basic/UtilMath.h"
#include "Basic/CSV.h"
#include "WeatherBased/WeatherDefine.h"

#include "ModelStat.h"


using namespace std;

namespace WBSF
{

	//	CInitialPopulation::CInitialPopulation(size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
	//	{
	//		Initialize(nbObjects, initialPopulation, age, sex, bFertil, generation);
	//	}
	//
	//	CInitialPopulation::CInitialPopulation(CTRef peakDay, double sigma, size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
	//	{
	//		Initialize(peakDay, sigma, nbObjects, initialPopulation, age, sex, bFertil, generation);
	//	}
	//	void CInitialPopulation::Initialize(CTRef peakDay, double sigma, size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
	//	{
	//		assert(nbObjects > 0);
	//		assert(initialPopulation > 0);
	//		assert((peakDay.GetDOY() - 3 * sigma) >= 0 && (peakDay.GetDOY() + 3 * sigma) <= 365); //all attack within first year
	//
	//		clear();
	//		if (peakDay.is_init())
	//		{
	//			size_t nbCreated = 0;
	//			double cumulCreated = 0;
	//			double scaleFactor = initialPopulation / nbObjects;
	//
	//			for (CTRef TRef = peakDay - int(3 * sigma); TRef <= peakDay + int(3 * sigma) && nbCreated < nbObjects; TRef++)
	//			{
	//				cumulCreated += sigma > 0 ? nbObjects / (sigma*pow((2 * PI), 0.5)) * exp(-0.5*square((TRef - peakDay) / sigma)) : nbObjects;
	//
	//				size_t nbObjectToCreate = size_t(max(0, round<int>(cumulCreated - nbCreated)));
	//				for (size_t i = 0; i < nbObjectToCreate; i++)
	//					push_back(CIndividualInfo(TRef, age, sex, bFertil, generation, scaleFactor));
	//
	//				nbCreated += nbObjectToCreate;
	//			}
	//
	//			assert(abs(int(size()) - int(nbObjects)) <= 1);
	//
	//			while (size() < nbObjects)
	//				push_back(CIndividualInfo(peakDay, age, sex, bFertil, generation, scaleFactor));
	//
	//
	//			assert(size() == nbObjects);
	//		}
	//	}
	//
	//	void CInitialPopulation::Initialize(size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation)
	//	{
	//		assert(nbObjects > 0);
	//		assert(initialPopulation > 0);
	//
	//		clear();
	//		double scaleFactor = initialPopulation / nbObjects;
	//
	//		for (size_t i = 0; i < nbObjects; i++)
	//			push_back(CIndividualInfo(CTRef(), age, sex, bFertil, generation, scaleFactor));
	//
	//	}

	//**********************************************************************************************


	CModelStatVector::CModelStatVector(const CModelStatVector& in, CTM TM, size_t s)
	{
		Init(in, TM, s);
	}

	CModelStatVector::CModelStatVector(const CModelStatVector& in, const CTTransformation& TT, size_t s)
	{
		Init(in, TT, s);
	}

	CModelStatVector::CModelStatVector(const CTStatMatrix& in, size_t s)
	{
		Init(in, s);
	}

	void CModelStatVector::Init(const CModelStatVector& in, const CTM& TM, size_t s)
	{
		CTPeriod p1 = in.GetTPeriod();
		CTPeriod p2 = p1.as(TM);
		Init(in, CTTransformation(p1, p2), s);
	}

	void CModelStatVector::Init(const CModelStatVector& in, const CTTransformation& TT, size_t s)
	{
		operator=(in);
		Transform(TT, s);
	}



	void CModelStatVector::Init(const CTStatMatrix& in, size_t s)
	{
		Init(in.m_period, in.size_x(), -999);
		for (size_t i = 0; i < in.size(); i++)
		{
			for (size_t j = 0; j < in[i].size(); j++)
			{
				if (in[i][j].is_init())
					at(i).at(j) = in[i][j][s];
			}
		}
	}

	CModelStatVector& CModelStatVector::operator=(const CModelStatVector& in)
	{
		if (this != &in)
		{
			CModelStatVectorBase::operator =(in);
			m_firstTRef = in.m_firstTRef;
			m_nbStat = in.m_nbStat;
			m_missingValue = in.m_missingValue;
			m_header = in.m_header;
		}

		return *this;
	}


	CModelStatVector& CModelStatVector::swap(CModelStatVector& in)
	{
		if (this != &in)
		{
			CModelStatVectorBase::swap(in);
			std::swap(m_firstTRef, in.m_firstTRef);
			std::swap(m_nbStat, in.m_nbStat);
			std::swap(m_missingValue, in.m_missingValue);
			m_header.swap(in.m_header);

		}

		return *this;
	}


	bool CModelStatVector::HaveData()const
	{
		bool bHaveData = false;
		for (size_t i = 0; i < size() && !bHaveData; i++)
			for (size_t j = 0; j < at(i).size() && !bHaveData; j++)
				bHaveData = at(i).at(j) > m_missingValue;


		return bHaveData;
	}

	bool CModelStatVector::test_op(double value, const std::string& op, double threshold)
	{
		bool bOp = false;
		if (op == "==")
			bOp = fabs(value - threshold) < EPSILON_DATA;
		else if (op == "!=")
			bOp = fabs(value - threshold) >= EPSILON_DATA;
		else if (op == "<=")
			bOp = (value <= threshold);
		else if (op == "<")
			bOp = (value < threshold);
		else if (op == ">=")
			bOp = (value >= threshold);
		else if (op == ">")
			bOp = (value > threshold);
		else
			assert(false);

		return bOp;

	}

	size_t CModelStatVector::GetFirstIndex(size_t s, const std::string& op, double threshold, int nbDayBefore, const CTPeriod& pIn)const
	{
		const CModelStatVector& me = *this;

		CTPeriod p = pIn;
		if (!p.is_init())
			p = GetTPeriod();

		assert(GetTPeriod().is_inside(p));

		int32_t fd = p.begin() - m_firstTRef;
		int32_t ld = p.end() - m_firstTRef;

		size_t firstDay = NOT_INIT;
		for (int32_t i = fd; i <= ld && firstDay == NOT_INIT; i++)
		{
			if (test_op(me[i][s], op, threshold))
			{
				firstDay = max(0, i - nbDayBefore);
			}
		}

		assert(firstDay == NOT_INIT || firstDay < size());

		return firstDay;
	}


	size_t CModelStatVector::GetLastIndex(size_t s, const std::string& op, double threshold, int nbDayAfter, const CTPeriod& pIn)const
	{
		const CModelStatVector& me = *this;
		size_t lastDay = NOT_INIT;

		CTPeriod p = pIn;
		if (!p.is_init())
			p = GetTPeriod();

		assert(GetTPeriod().is_inside(p));

		int64_t fd = p.begin() - m_firstTRef;
		int64_t ld = p.end() - m_firstTRef;
		for (int64_t i = ld; i >= fd && lastDay == NOT_INIT; i--)
		{
			if (test_op(me[i][s], op, threshold))
				lastDay = std::min(int64_t(size()) - 1, i + nbDayAfter);
		}

		assert(lastDay == NOT_INIT || lastDay < size());

		return lastDay;
	}

	CStatistic CModelStatVector::GetStat(size_t s, const CTPeriod& period)const
	{
		const CModelStatVector& me = *this;
		CStatistic stat;

		for (size_t i = 0; i < size(); i++)
		{
			if (period.is_inside(m_firstTRef + int32_t(i)))
			{
				stat += me[i][s];
			}
		}

		return stat;
	}

	void CModelStatVector::WriteStream(std::ostream& stream)const
	{
		const CModelStatVector& me = *this;

		int64_t version = 1;
		int64_t nbRows = (int64_t)size();//GetNbUsed();
		int64_t nbCols = (nbRows > 0) ? m_nbStat + 1 : 0;
		int64_t rawSize = nbRows * nbCols * sizeof(float);
		int64_t realSize = (int64_t)size();
		int64_t nbStat = m_nbStat;
		int64_t type = GetFirstTRef().TM().Type();
		int64_t mode = GetFirstTRef().TM().Mode();
		int64_t Uref = GetFirstTRef().GetRef();

		//CTRef ref(d.GetYear(),d.GetMonth(),d.GetDay());

		stream.write((char*)(&version), sizeof(version));
		stream.write((char*)(&rawSize), sizeof(rawSize));
		stream.write((char*)(&nbRows), sizeof(nbRows));
		stream.write((char*)(&nbCols), sizeof(nbCols));
		stream.write((char*)(&realSize), sizeof(realSize));
		stream.write((char*)(&nbStat), sizeof(nbStat));
		stream.write((char*)(&type), sizeof(type));
		stream.write((char*)(&mode), sizeof(mode));
		stream.write((char*)(&Uref), sizeof(Uref));

		for (size_t i = 0; i < size(); i++)
		{
			for (size_t j = 0; j < m_nbStat; j++)
			{
				assert(me[i][j] < FLT_MAX);
				float value = float(me[i][j]);
				stream.write((char*)(&value), sizeof(value));
			}
		}
	}

	ERMsg CModelStatVector::ReadStream(std::istream& stream)
	{
		ERMsg msg;

		CModelStatVector& me = *this;

		int64_t version = -1;
		int64_t rawSize = 0;
		int64_t nbRows = 0;
		int64_t nbCols = 0;
		int64_t realSize = 0;
		int64_t nbStat = 0;
		int64_t type = 0;
		int64_t mode = 0;
		int64_t Uref = 0;

		stream.read((char*)(&version), sizeof(version));
		stream.read((char*)(&rawSize), sizeof(rawSize));
		stream.read((char*)(&nbRows), sizeof(nbRows));
		stream.read((char*)(&nbCols), sizeof(nbCols));
		stream.read((char*)(&realSize), sizeof(realSize));
		stream.read((char*)(&nbStat), sizeof(nbStat));
		stream.read((char*)(&type), sizeof(type));
		stream.read((char*)(&mode), sizeof(mode));
		stream.read((char*)(&Uref), sizeof(Uref));
		resize((size_t)realSize, (size_t)nbStat, 0);
		m_firstTRef.SetRef(Uref, CTM(type, mode));

		for (size_t i = 0; i < size(); i++)
		{
			for (size_t j = 0; j < m_nbStat; j++)
			{
				float value = 0;
				stream.read((char*)(&value), sizeof(value));
				me[i][j] = value;
			}
		}


		assert(version == 1);
		assert((size_t)rawSize == nbRows * nbCols * sizeof(float));
		assert(m_firstTRef.is_valid());

		return msg;
	}



	ERMsg CModelStatVector::Load(const std::string& filePath)
	{
		ERMsg msg;


		ifStream file;
		msg = file.open(filePath);

		if (msg)
		{
			msg += Load(file);
			if (!msg)
				msg.ajoute("Problem reading result file " + filePath);

			file.close();
		}

		return msg;
	}

	ERMsg CModelStatVector::Load(std::istream& stream)
	{
		ERMsg msg;


		CSVIterator loop(stream);
		m_header = to_string(loop.Header());
		assert(m_header.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos);

		CWeatherFormat format(m_header.c_str());
		
		size_t nbReference = CTRefFormat::GetTimeReference(loop.Header()).size();//format.TM().GetNbTimeReferences();
		m_nbStat = loop.Header().size() - nbReference;
		assert(m_nbStat > 0 && m_nbStat <= loop.Header().size());

		size_t line = 1;
		for (; loop != CSVIterator(); ++loop, line++)
		{
			//resize(size() + 1);
			if (!(*loop).empty())
			{
				CTRef TRef = format.GetTRef(*loop);
				if (msg)
				{
					if ((*loop).size() == nbReference + m_nbStat)
					{
						CModelStat row(m_nbStat);
						for (size_t j = nbReference; j < (*loop).size(); j++)
						{
							row[j - nbReference] = ToDouble((*loop)[j]);
						}

						Insert(TRef, row);
					}
					else
					{
						msg.ajoute("Problem reading line " + to_string(line));
						msg.ajoute((*loop).GetLastLine());
					}
				}
			}//no empty
		}//for all lines


		return msg;
	}

	ERMsg CModelStatVector::Save(const std::string& filePath)const
	{
		ERMsg msg;
		ofStream file;
		msg = file.open(filePath);
		if (msg)
		{
			msg += Save(file);
			file.close();
		}

		return msg;
	}

	ERMsg CModelStatVector::Save(std::ostream& stream)const
	{
		ERMsg msg;

		//save header if any
		if (!m_header.empty())
		{
			assert(TokenizeQuoted(m_header, ",").size() == m_nbStat);
			assert(GetFirstTRef().is_init());

			CTemporal temporalRef(GetFirstTRef().TM());
			stream << temporalRef.GetHeader(",");
			stream << "," << m_header;
			stream << std::endl;
		}


		CTRef d = GetFirstTRef();
		for (const_iterator it = begin(); it != end(); it++, d++)
		{
			if (d.TM().Mode() == CTM::FOR_EACH_YEAR)
			{
				if (d.TM().Type() != CTM::ATEMPORAL)
					stream << d.GetYear();
				else
					stream << d.m_ref;
			}

			switch (d.TM().Type())
			{
			case CTM::HOURLY: stream << ',' << d.GetMonth() + 1 << ',' << d.GetDay() + 1 << ',' << d.GetHour(); break;
			case CTM::DAILY: stream << ',' << d.GetMonth() + 1 << ',' << d.GetDay() + 1; break;
			case CTM::MONTHLY: stream << ',' << d.GetMonth() + 1; break;
			case CTM::ANNUAL: break;
			case CTM::ATEMPORAL: break;
			default: assert(false);
			}

			for (CModelStat::const_iterator it2 = it->begin(); it2 != it->end(); it2++)
				stream << ',' << *it2;

			stream << std::endl;
		}


		return msg;
	}

	int32_t CModelStatVector::GetNbDay(size_t v, const string& op_str, const CTPeriod& p, bool bConsecutive, double MISSING)const
	{
		assert(GetFirstTRef().TM() == p.TM());

		CMathEvaluation op(op_str.c_str());


		const CModelStatVector& me = *this;
		CTRef firstDate = max(GetFirstTRef(), p.begin());
		CTRef lastDate = min(GetLastTRef(), p.end());

		int32_t nbDay = 0;
		CTRef firstDWP = firstDate;
		for (CTRef d = firstDate; d <= lastDate; d++)
		{
			if (me[d][v] > MISSING)
			{
				if (bConsecutive)
				{
					if (!op.Evaluate(me[d][v]))
					{
						nbDay = max(nbDay, d - firstDWP);
						firstDWP = d + 1;
					}
					else if (d == lastDate)
					{
						//the last day, we verify the number of day
						nbDay = max(nbDay, d - firstDWP + 1);
					}
				}
				else
				{
					if (op.Evaluate(me[d][v]))
						nbDay++;
				}
			}
			else firstDWP = d + 1;
		}

		assert(nbDay >= 0 && (size_t)nbDay <= p.size());

		return nbDay;
	}

	size_t CModelStatVector::Insert(CTRef r, const CModelStat& row)
	{
		assert(size() == 0 || m_firstTRef.is_init());

		if (!m_firstTRef.is_init())
		{
			m_nbStat = row.size();
			m_firstTRef = r;
		}


		int index = r - m_firstTRef;
		assert(index >= 0);

		if (index < 0)
		{
			insert(begin(), -index - 1, CModelStat(m_nbStat, m_missingValue));
			insert(begin(), row);
			m_firstTRef = m_firstTRef + index;
		}
		else if (index < int(size()))
		{
			insert(begin() + index, row);
		}
		else if ((size_t)index == size())
		{
			push_back(row);
		}
		else
		{
			insert(end(), index - size(), CModelStat(m_nbStat, m_missingValue));
			push_back(row);
		}

		return GetRows();
	}

	void CModelStatVector::ConvertValue(float value1, float value2)
	{
		CModelStatVector& me = *this;
		for (size_t d = 0; d < size(); d++)
		{
			for (size_t v = 0; v < me[d].size(); v++)
			{
				if (me[d][v] == value1)
					me[d][v] = value2;
			}
		}

	}

	//	CInitialPopulation CModelStatVector::GetInitialPopulation(size_t var, size_t nbObjects, double initialPopulation, double age, TSex sex, bool bFertil, size_t generation, CTPeriod p)const
	//	{
	//		CInitialPopulation population;
	//
	//		if (!p.is_init())
	//			p = GetTPeriod();
	//
	//		CStatistic stat = GetStat(var, p);
	//
	//		if (stat[SUM] > 100)//need at least 100 eggs
	//		{
	//			//Get nbDay with activity
	//			double scaleFactor = initialPopulation / nbObjects;
	//
	//			size_t nbCreated = 0;
	//			double cumulCreated = 0;
	//			CTPeriod period = GetTPeriod(var, ">", 0, 0, 0, p);
	//			assert(period.is_init());
	//
	//			for (CTRef TRef = period.begin(); TRef <= period.end(); TRef++)
	//			{
	//				cumulCreated += nbObjects * at(TRef).at(var) / stat[SUM];
	//
	//				size_t nbObjectToCreate = size_t(max(0, round<int>(cumulCreated - nbCreated)));
	//				for (size_t i = 0; i < nbObjectToCreate; i++)
	//					population.push_back(CIndividualInfo(TRef, age, sex, bFertil, generation, scaleFactor));
	//
	//				nbCreated += nbObjectToCreate;
	//			}
	//
	//			assert(population.size() == nbObjects);
	//
	//			double ipTest = 0;
	//			for (size_t i = 0; i < population.size(); i++)
	//				ipTest += population[i].m_scaleFactor;
	//
	//			assert(fabs(ipTest - initialPopulation) < 0.001);
	//			//for(size_t i=size(); i<nbObjects; i++)
	//			//proportionVector.push_back( CProportionStat(peak, nbIndividuPerObject) );
	//		}
	//
	//		return population;
	//	}
	//

	//void CModelStatVector::Transform(const CTTransformation& TT, size_t s)
	//{
	//CTStatMatrix output(*this, TT);
	//CModelStatVector& me = *this;
	//CTPeriod pIn = TT.GetPeriodIn().is_init() ? TT.GetPeriodIn() : GetTPeriod();
	//CTPeriod pOut = TT.GetPeriodOut();
	//output.Init(pOut, GetNbStat());

	//for (CTRef TRefIn = pIn.begin(); TRefIn <= pIn.end(); TRefIn++)
	//{
	//	size_t c = TT.GetCluster(TRefIn);
	//	if (c != UNKNOWN_POS)
	//	{
	//		for (size_t v = 0; v < GetNbStat(); v++)
	//		{
	//			double var = me[TRefIn][v];
	//			assert(var > -9999);
	//			CTRef TRefOut = TT.GetClusterTRef(c);
	//			output(TRefOut, v) += var;
	//		}
	//	}
	//}



	//CModelStatVector& me = *this;
	//CTPeriod pIn = TT.GetPeriodIn().is_init() ? TT.GetPeriodIn() : GetTPeriod();
	//CTPeriod pOut = TT.GetPeriodOut();
	//output.Init(pOut, GetNbStat());

	//for (CTRef TRefIn = pIn.begin(); TRefIn <= pIn.end(); TRefIn++)
	//{
	//	size_t c = TT.GetCluster(TRefIn);
	//	if (c != UNKNOWN_POS)
	//	{
	//		for (size_t v = 0; v < GetNbStat(); v++)
	//		{
	//			double var = me[TRefIn][v];
	//			assert(var > -9999);
	//			CTRef TRefOut = TT.GetClusterTRef(c);
	//			output(TRefOut, v) += var;
	//		}
	//	}
	//}
	//}

	void CModelStatVector::Transform(const CTM& TMi, size_t s)
	{
		if (TMi != TM())
		{
			CTTransformation TT(GetTPeriod(), TMi);
			Transform(TT, s);
		}
	}

	void CModelStatVector::Transform(const CTTransformation& TT, size_t s)
	{
		CTStatMatrix stat(*this, TT);

		CTPeriod p = stat.m_period;
		Init(p, stat.size_x(), -999);
		for (size_t i = 0; i < stat.size(); i++)
		{
			for (size_t j = 0; j < stat[i].size(); j++)
			{
				if (stat[i][j].is_init())
					at(i).at(j) = stat[i][j][s];
			}
		}
	}


	//*************************************************************************************************
	CTStatMatrix::CTStatMatrix(CTPeriod period, size_t nbVars) :
		CTStatMatrixBase(period, nbVars)
	{
	}


	CTStatMatrix::CTStatMatrix(const CModelStatVector& in, const CTM& TM)
	{
		Init(in, TM);
	}

	CTStatMatrix::CTStatMatrix(const CModelStatVector& in, const CTTransformation& TT)
	{
		Init(in, TT);
	}
	//
	//CTStatMatrix(const CTStatMatrix& in, const CTM& TM)
	//{
	//}

	//CTStatMatrix(const CTStatMatrix& in, const CTTransformation& TT)
	//{
	//	operator=(in);
	//	Transform(TT);
	//}

	//void CTStatMatrix::Init(CTStatMatrix& in, const CTM& TM)
	//{
	//	CTPeriod p1 = input.GetTPeriod();
	//	CTPeriod p2 = p1.Transform(TM);
	//	Init(input, CTTransformation(p1, p2));
	//}

	//void CTStatMatrix::Init(CTStatMatrix& in, const CTTransformation& TT)
	//{
	//	operator=(in);
	//	Transform(TT);
	//}

	void CTStatMatrix::Init(const CModelStatVector& in, const CTM& TM)
	{
		CTPeriod p1 = in.GetTPeriod();
		CTPeriod p2 = p1.as(TM);
		Init(in, CTTransformation(p1, p2));
	}

	void CTStatMatrix::Init(const CModelStatVector& in, const CTTransformation& TTin)
	{
		CTStatMatrix& me = *this;
		CTPeriod pIn = TTin.GetPeriodIn().is_init() ? TTin.GetPeriodIn() : in.GetTPeriod();
		CTPeriod pOut = TTin.GetPeriodOut().is_init() ? TTin.GetPeriodOut() : pIn;

		CTTransformation TT(pIn, pOut);
		CTStatMatrixBase::Init(pOut, in.GetNbStat());

		for (CTRef TRefIn = pIn.begin(); TRefIn <= pIn.end(); TRefIn++)
		{
			size_t c = TT.GetCluster(TRefIn);
			if (c != UNKNOWN_POS)
			{
				for (size_t v = 0; v < in.GetNbStat(); v++)
				{
					double var = in[TRefIn][v];
					if (var != -999)
					{
						CTRef TRefOut = TT.GetClusterTRef(c);

						me[TRefOut][v] += var;
					}
				}
			}
		}
	}

	void CTStatMatrix::Transform(const CTM& TM)
	{
		if (TM.is_init())
		{
			CTPeriod p1 = m_period;
			CTPeriod p2 = p1.as(TM);
			Transform(CTTransformation(p1, p2));
		}
	}

	void CTStatMatrix::Transform(const CTTransformation& TT)
	{
		if (TT.is_init())
		{
			CTStatMatrix& me = *this;
			CTPeriod pIn = TT.GetPeriodIn();
			CTPeriod pOut = TT.GetPeriodOut();

			CTStatMatrix output(pOut, size_x());

			for (CTRef TRefIn = pIn.begin(); TRefIn <= pIn.end(); TRefIn++)
			{
				size_t c = TT.GetCluster(TRefIn);
				if (c != UNKNOWN_POS)
				{
					for (size_t v = 0; v < size_x(); v++)
					{
						CTRef TRefOut = TT.GetClusterTRef(c);
						output[TRefOut][v] += me[TRefIn][v];
					}
				}
			}

			operator=(output);
		}
	}

	void ModelInterface::Transform(const CTTransformation& TT, const CModelStatVector& in, CModelStatVector& out, size_t s)
	{
		CTStatMatrix tmp;
		Transform(TT, in, tmp);
		out.Init(tmp, s);
	}

	void ModelInterface::Transform(const CTTransformation& TT, const CModelStatVector& in, CTStatMatrix& out)
	{
		out.Init(in, TT);


		//CTPeriod pIn = TT.GetPeriodIn().is_init() ? TT.GetPeriodIn():in.GetTPeriod();
		//CTPeriod pOut = TT.GetPeriodOut().is_init()? TT.GetPeriodOut():pIn;

		//out.Init(pOut, NB_OUTPUT);

		//for (CTRef TRefIn = pIn.begin(); TRefIn <= pIn.end(); TRefIn++)
		//{
		//	double DD = input[TRefIn][S_DD];
		//	assert(DD > -9999);


		//	size_t c = TT.GetCluster(TRefIn);
		//	if (c != UNKNOWN_POS)
		//	{
		//		CTRef TRefOut = TT.GetClusterTRef(c);
		//		output(TRefOut, S_DD) += DD;
		//	}
		//}
	}

}//namespace WBSF
