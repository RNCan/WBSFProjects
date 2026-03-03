//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************

#include <fstream>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "WeatherBased/WeatherStation.h"
#include "Basic/ModelStat.h"
#include "Basic/UtilStd.h"
#include "ModelBased/CommunicationStream.h"
#include "ModelBased/ModelBaseError.h"

using namespace std;

namespace WBSF
{

	static const uint64_t VERSION = 1;

	//*******************************************************************8
	//CTransferInfoIn

	CTransferInfoIn::CTransferInfoIn()
	{
		Reset();
	}

	void CTransferInfoIn::Reset()
	{
		m_transferTypeVersion = -1;
		m_modelName.clear();
		m_inputInfoPath.clear();
		m_inputWeatherFilePath.clear();
		m_outputFilePath.clear();

		m_normalDBFilePath.clear();
		m_dailyDBFilePath.clear();
		m_hourlyDBFilePath.clear();
		m_locationsFilePath.clear();
		m_modelName.clear();
		m_WGInputName.clear();
		m_modelInputName.clear();
		m_parametersVariationsName.clear();
		m_repCounter.Reset();
		m_paramCounter.Reset();
		m_locCounter.Reset();

		m_loc.Reset();
		m_WG.Reset();
		m_inputParameters.clear();
		m_outputVariables.clear();
		m_seed = 0;
		m_TM.clear();
		m_language = 0;

		//m_outputParameters.clear();
	}

	bool CTransferInfoIn::operator ==(const CTransferInfoIn& in)const
	{
		bool bEqual = true;
		if (&in != this)
		{
			if (m_transferTypeVersion != in.m_transferTypeVersion)bEqual = false;
			if (m_modelName != in.m_modelName)bEqual = false;
			if (m_inputInfoPath != in.m_inputInfoPath)bEqual = false;
			if (m_inputWeatherFilePath != in.m_inputWeatherFilePath)bEqual = false;
			if (m_outputFilePath != in.m_outputFilePath)bEqual = false;

			if (m_normalDBFilePath != in.m_normalDBFilePath)bEqual = false;
			if (m_dailyDBFilePath != in.m_dailyDBFilePath)bEqual = false;
			if (m_hourlyDBFilePath != in.m_hourlyDBFilePath)bEqual = false;
			if (m_locationsFilePath != in.m_locationsFilePath)bEqual = false;
			if (m_WGInputName != in.m_WGInputName)bEqual = false;
			if (m_modelInputName != in.m_modelInputName)bEqual = false;
			if (m_parametersVariationsName != in.m_parametersVariationsName)bEqual = false;
			if (m_repCounter != in.m_repCounter)bEqual = false;
			if (m_paramCounter != in.m_paramCounter)bEqual = false;
			if (m_locCounter != in.m_locCounter)bEqual = false;

			if (m_loc != in.m_loc)bEqual = false;
			if (m_WG != in.m_WG)bEqual = false;
			if (m_inputParameters != in.m_inputParameters)bEqual = false;
			if (m_outputVariables != in.m_outputVariables)bEqual = false;
			if (m_seed != in.m_seed)bEqual = false;
			if (m_TM != in.m_TM)bEqual = false;
			if (m_language != in.m_language)bEqual = false;

		}

		return bEqual;
	}


	const char* CTransferInfoIn::XML_FLAG = "TransferInputInfo";
	const char* CTransferInfoIn::MEMBER_NAME[NB_MEMBERS] =
	{
		"TransferTypeVersion", "InputFilePath", "OutputFilePath", "NormalsDBFilePath", "DailyDBFilePath", "HourlyDBFilePath", "LocationsFilePath", "ModelName", "TGInputName", "ModelInputName", "ParametersVariationsName",
		"LocationsCounter", "ParametersVariationsCounter", "ReplicationsCounter",
		"Location", "WGInput", "InputParameters", "OutputVariables", "Seed", "OutputTypeMode", "Language"
	};





	void CTransferInfoIn::write_xml(pugi::xml_node node)const
	{


		node.append_child(GetMemberName(TRANSFER_TYPE)).append_child(pugi::node_pcdata).set_value(to_string(m_transferTypeVersion).c_str());
		node.append_child(GetMemberName(INPUT_FILE_PATH)).append_child(pugi::node_pcdata).set_value(m_inputWeatherFilePath.c_str());
		node.append_child(GetMemberName(OUTPUT_FILE_PATH)).append_child(pugi::node_pcdata).set_value(m_outputFilePath.c_str());
		node.append_child(GetMemberName(NORMAL_FILE_PATH)).append_child(pugi::node_pcdata).set_value(m_normalDBFilePath.c_str());
		node.append_child(GetMemberName(DAILY_FILE_PATH)).append_child(pugi::node_pcdata).set_value(m_dailyDBFilePath.c_str());
		node.append_child(GetMemberName(HOURLY_FILE_PATH)).append_child(pugi::node_pcdata).set_value(m_hourlyDBFilePath.c_str());
		node.append_child(GetMemberName(LOC_FILE_PATH)).append_child(pugi::node_pcdata).set_value(m_locationsFilePath.c_str());
		node.append_child(GetMemberName(MODEL_NAME)).append_child(pugi::node_pcdata).set_value(m_modelName.c_str());
		node.append_child(GetMemberName(TG_INPUT_NAME)).append_child(pugi::node_pcdata).set_value(m_WGInputName.c_str());
		node.append_child(GetMemberName(MODEL_INPUT_NAME)).append_child(pugi::node_pcdata).set_value(m_modelInputName.c_str());
		node.append_child(GetMemberName(PARAMETERS_VARIATIONS_NAME)).append_child(pugi::node_pcdata).set_value(m_parametersVariationsName.c_str());
		node.append_child(GetMemberName(LOC_COUNTER)).append_child(pugi::node_pcdata).set_value(m_locCounter.to_string().c_str());
		node.append_child(GetMemberName(PARAM_COUNTER)).append_child(pugi::node_pcdata).set_value(m_paramCounter.to_string().c_str());
		node.append_child(GetMemberName(REP_COUNTER)).append_child(pugi::node_pcdata).set_value(m_repCounter.to_string().c_str());
		m_loc.write_xml(node.append_child(GetMemberName(LOCATION)));
		m_WG.write_xml(node.append_child(GetMemberName(WG_INPUT)));
		m_inputParameters.write_xml(node.append_child(GetMemberName(INPUT_PARAM)));
		m_outputVariables.write_xml(node.append_child(GetMemberName(OUTPUT_VARIABLES)));
		node.append_child(GetMemberName(SEED)).append_child(pugi::node_pcdata).set_value(to_string(m_seed).c_str());
		node.append_child(GetMemberName(OUTPUT_TM)).append_child(pugi::node_pcdata).set_value(m_TM.to_string().c_str());
		node.append_child(GetMemberName(LANGUAGE)).append_child(pugi::node_pcdata).set_value(to_string(m_language).c_str());

	}




	void CTransferInfoIn::read_xml(pugi::xml_node node)
	{

		m_transferTypeVersion = node.child(GetMemberName(TRANSFER_TYPE)).text().as_int();
		m_inputWeatherFilePath = node.child(GetMemberName(INPUT_FILE_PATH)).text().as_string();
		m_outputFilePath = node.child(GetMemberName(OUTPUT_FILE_PATH)).text().as_string();
		m_normalDBFilePath = node.child(GetMemberName(NORMAL_FILE_PATH)).text().as_string();
		m_dailyDBFilePath = node.child(GetMemberName(DAILY_FILE_PATH)).text().as_string();
		m_hourlyDBFilePath = node.child(GetMemberName(HOURLY_FILE_PATH)).text().as_string();
		m_locationsFilePath = node.child(GetMemberName(LOC_FILE_PATH)).text().as_string();
		m_modelName = node.child(GetMemberName(MODEL_NAME)).text().as_string();
		m_WGInputName = node.child(GetMemberName(TG_INPUT_NAME)).text().as_string();
		m_modelInputName = node.child(GetMemberName(MODEL_INPUT_NAME)).text().as_string();
		m_parametersVariationsName = node.child(GetMemberName(PARAMETERS_VARIATIONS_NAME)).text().as_string();
		m_locCounter = node.child(GetMemberName(LOC_COUNTER)).text().as_string();
		m_paramCounter = node.child(GetMemberName(PARAM_COUNTER)).text().as_string();
		m_repCounter = node.child(GetMemberName(REP_COUNTER)).text().as_string();
		m_loc.read_xml(node.child(GetMemberName(LOCATION)));
		m_WG.read_xml(node.child(GetMemberName(WG_INPUT)));
		m_inputParameters.read_xml(node.child(GetMemberName(INPUT_PARAM)));
		m_outputVariables.read_xml(node.child(GetMemberName(OUTPUT_VARIABLES)));
		m_seed = node.child(GetMemberName(SEED)).text().as_ullong();
		m_TM = node.child(GetMemberName(OUTPUT_TM)).text().as_string();
		m_language = node.child(GetMemberName(LANGUAGE)).text().as_int();


	}


	void CTransferInfoIn::WriteStream(ostream& stream)const
	{
		write_value(stream, VERSION);

		std::string xml = to_xml_string(*this, XML_FLAG);
		WriteBuffer(stream, xml);
	}

	ERMsg CTransferInfoIn::ReadStream(istream& stream)
	{
		ERMsg msg;


		uint64_t version = read_value<uint64_t>(stream);
		if (version == 1)
		{
			std::string xml = ReadBuffer(stream);
			msg = from_xml_string(xml, *this, XML_FLAG);
		}
		else
		{
			msg = GetErrorMessage(ERROR_BAD_STREAM_FORMAT);
		}

		return msg;
	}

	ERMsg CTransferInfoIn::Load(const string& filePath)
	{
		ERMsg msg;
		//msg = zen::LoadXML(filePath, XML_FLAG, "1", *this);;
		return msg;
	}

	ERMsg CTransferInfoIn::Save(const string& filePath)const
	{
		ERMsg msg;

		//    if (m_transferTypeVersion == VERSION_TEXT)
		//    {
		//        msg = SaveV1(filePath);
		//    }
		//    else if (m_transferTypeVersion == VERSION_XML)
		//    {
		//        msg = zen::SaveXML(filePath, XML_FLAG, "1", *this);
		//    }
		//    else
		//    {
		//        assert(false);
		//    }
		//
		//    if (msg)
		//        const_cast<CTransferInfoIn*>(this)->m_filePath = filePath;

		return msg;
	}

	ERMsg CTransferInfoIn::SaveV1(const string& filePath)const
	{
		ERMsg msg;

		ofStream file;
		msg = file.open(filePath);
		if (msg)
		{
			file << m_inputWeatherFilePath << endl;
			file << m_outputFilePath << endl;
			file << "\"" << m_loc.m_name << "\" " << m_loc.m_lat << " " << m_loc.m_lon << " " << m_loc.m_elev << " " << m_loc.GetSlope() << " " << m_loc.GetAspect() << endl;
			file << m_locCounter.GetNo() << " " << m_locCounter.GetTotal() << endl;
			file << m_language << endl;

			for (size_t i = 0; i < m_inputParameters.size(); i++)
				file << m_inputParameters[i].GetString() << endl;
		}

		return msg;
	}
	//
	//ERMsg CTransferInfoIn::SaveV2(const string& filePath)const
	//{
	//	ERMsg msg;
	//
	//	CStdStdioFile file;
	//	msg = file.Open( convert(filePath).c_str(), CStdStdioFile::modeCreate|CStdStdioFile::modeWrite);
	//	if( msg )
	//	{
	//		CStdString line;
	//		line.Format(_T("MODEL_TRANSFER_FILE %d\n"), 2);
	//		file.WriteString(line);
	//
	//		line.Format(_T("%d\n"), m_ioFileVersion);//data type : 0=binary; 1=texte; 2=texte with header
	//		file.WriteString(line);
	//
	//		file.WriteString( m_inputWeatherFilePath +"\n");
	//        file.WriteString( m_outputFilePath +"\n");
	//
	//		file.WriteString( m_normalDBFilePath +"\n");
	//        file.WriteString( m_dailyDBFilePath +"\n");
	//		file.WriteString( m_locationsFilePath +"\n");
	//
	//		file.WriteString( m_modelName+"\n");
	//		file.WriteString( m_WGInputName+"\n");
	//		file.WriteString( m_modelInputName+"\n");
	//
	//
	//
	//		assert( m_locCounter.GetNo() >= 0 && m_locCounter.GetTotal() > 0);
	//		line.Format(_T("%d %d\n"), m_locCounter.GetNo(), m_locCounter.GetTotal());
	//		file.WriteString( line );
	//
	//		assert( m_paramCounter.GetNo() >= 0 && m_paramCounter.GetTotal() > 0);
	//		line.Format(_T("%d %d\n"), m_paramCounter.GetNo(), m_paramCounter.GetTotal());
	//		file.WriteString( line );
	//
	//		assert( m_repCounter.GetNo() >= 0 && m_repCounter.GetTotal() > 0);
	//		line.Format(_T("%d %d\n"), m_repCounter.GetNo(), m_repCounter.GetTotal());
	//		file.WriteString( line );
	//
	//		line.Format(_T("\"%s\" %lf %lf %lf %lf %lf\n"),
	//		m_loc.m_name.c_str(),
	//        m_loc.m_lat,
	//		m_loc.m_lon,
	//        m_loc.m_elev,
	//		m_loc.GetSlope(),
	//        m_loc.GetAspect());
	//
	//		file.WriteString( line );
	//
	//		//TGinput
	//		line.Format(_T("%d %d %d %d %d %d %d \"%s\" \"%s\" %d %d\n"),
	//			m_WG.GetFirstYear(),
	//			m_WG.GetNbYears(),
	//			m_WG.m_nbNormalsStations,
	//			m_WG.m_nbDailyStations,
	//			0,
	//			m_WG.m_albedoType,
	//			m_WG.GetSeedType(),
	//			m_WG.m_normalsDBName,
	//			m_WG.m_dailyDBName,
	//			m_WG.GetCategory().HaveCat(WEATHER::PRECIPITATION),
	//			0);
	//
	//		file.WriteString( line );
	//		//language
	//		line.Format(_T("%d\n"), m_language);
	//		file.WriteString( line );
	//
	//		//output ref type
	//		line.Format(_T("%d %d\n"), m_outputType, m_TM);
	//		file.WriteString( line );
	//
	//		//input var
	//		line.Format(_T("%d\n"), m_inputParameters.size());
	//		file.WriteString( line );
	//
	//
	//		for (int i=0; i<(int)m_inputParameters.size(); i++)
	//	    {
	//			// pour être compatible avec l'ancienne version
	//			//if( GetSize() == inputDefArray.GetSize() )
	//			//{
	//				CStdString tmp;
	//				tmp.Format(_T("%s;%s;%s;"),
	//					m_inputParameters[i].GetType(),
	//					m_inputParameters[i].GetName(),
	//					m_inputParameters[i].GetString() );
	//
	//				file.WriteString( tmp + "\n");
	//			//}
	//			//else
	//			//{
	//			//	file.WriteString( GetAt(i).GetStr() + "\n");
	//			//}
	//	    }
	//
	//		//output var
	//		//const CModelOutputVariableDefVector& outputArray = m_model.GetOutputDefinition();
	//		line.Format(_T("%d\n"), m_outputParameters.size());
	//		file.WriteString( line );
	//		for (int i=0; i<(int)m_outputParameters.size(); i++)
	//	    {
	//			file.WriteString( "0;"+m_outputParameters[i].GetString()+ ";0;\n");
	//	    }
	//
	//	}
	//
	//	return msg;
	//}
	//
	//ERMsg CTransferInfoIn::SaveV3(const string& filePath)const
	//{
	//	ERMsg msg;
	//
	//	msg = XSave(filePath, *this );
	//
	//	return msg;
	//}

	//*******************************************************************
	//CTransferInfoOut

	const char* CTransferInfoOut::XML_FLAG = "TransferOutputInfo";
	const char* CTransferInfoOut::MEMBER_NAME[NB_MEMBER] = { "LocationsCounter", "ParametersVariationsCounter", "ReplicationsCounter", "OutputTypeMode", "ErrorMsg" };

	CTransferInfoOut::CTransferInfoOut()
	{
		Reset();
	}

	void CTransferInfoOut::Reset()
	{
		m_repCounter.Reset();
		m_paramCounter.Reset();
		m_locCounter.Reset();
		m_TM.clear();
	}

	bool CTransferInfoOut::operator ==(const CTransferInfoOut& in)const
	{
		bool bEqual = true;
		if (&in != this)
		{
			if (m_repCounter != in.m_repCounter)bEqual = false;
			if (m_paramCounter != in.m_paramCounter)bEqual = false;
			if (m_locCounter != in.m_locCounter)bEqual = false;
			if (m_TM != in.m_TM)bEqual = false;
		}

		return bEqual;
	}

	size_t CTransferInfoOut::GetSectionNo()const
	{
		return
			m_locCounter.GetNo() * m_paramCounter.GetTotal() * m_repCounter.GetTotal() +
			m_paramCounter.GetNo() * m_repCounter.GetTotal() +
			m_repCounter.GetNo();

	}

	void CTransferInfoOut::WriteStream(ostream& stream)const
	{
		write_value(stream, VERSION);

		string xml = to_xml_string(*this, XML_FLAG);
		WriteBuffer(stream, xml);
	}

	ERMsg CTransferInfoOut::ReadStream(istream& stream)
	{
		ERMsg msg;

		uint64_t version = read_value<uint64_t>(stream);
		if (version == 1)
		{
			msg = from_xml_string(ReadBuffer(stream), *this, XML_FLAG);
		}
		else
		{
			msg.ajoute("Model error: error reading input stream. Verify BioSIM/model versions.");
		}

		return msg;
	}


	void CTransferInfoOut::write_xml(pugi::xml_node node)const
	{
		node.append_child(GetMemberName(LOC_COUNTER)).append_child(pugi::node_pcdata).set_value(m_locCounter.to_string().c_str());
		node.append_child(GetMemberName(PARAM_COUNTER)).append_child(pugi::node_pcdata).set_value(m_paramCounter.to_string().c_str());
		node.append_child(GetMemberName(REP_COUNTER)).append_child(pugi::node_pcdata).set_value(m_repCounter.to_string().c_str());
		node.append_child(GetMemberName(OUTPUT_TM)).append_child(pugi::node_pcdata).set_value(m_TM.to_string().c_str());
		node.append_child(GetMemberName(ERROR_MSG)).append_child(pugi::node_pcdata).set_value(m_msg.to_string("¦").c_str());
	}

	void CTransferInfoOut::read_xml(pugi::xml_node node)
	{
		m_locCounter = node.child(GetMemberName(LOC_COUNTER)).text().as_string();
		m_paramCounter = node.child(GetMemberName(PARAM_COUNTER)).text().as_string();
		m_repCounter = node.child(GetMemberName(REP_COUNTER)).text().as_string();
		m_TM = node.child(GetMemberName(OUTPUT_TM)).text().as_string();
		m_msg.from_string( node.child(GetMemberName(ERROR_MSG)).text().as_string(), "¦" );
	}

	//****************************************************************
	//Static data stream
	ERMsg CFileStaticDataVector::Load(const std::vector<std::string>& filePath)
	{
		ERMsg msg;

		try
		{
			resize(filePath.size());

			for (size_t i = 0; i < filePath.size() && msg; i++)
			{
				if (!filePath[i].empty())
				{
					std::ifstream in(filePath[i], std::ios::in | std::ios::binary);

					if (in)
					{
						at(i).m_filePath = filePath[i];
						//std::string contents;
						in.seekg(0, std::ios::end);
						at(i).m_data.resize((size_t)in.tellg());
						in.seekg(0, std::ios::beg);
						in.read(&at(i).m_data[0], at(i).m_data.size());
						in.close();
					}
					else
					{
						msg.ajoute("Unable to load file: " + filePath[i]);
					}
				}
			}
		}
		catch (...)
		{
			msg.ajoute("Unable to load input files");
		}

		return msg;
	}


	int CFileStaticDataVector::Find(const std::string& filePath)const
	{
		int index = -1;
		CFileStaticDataVector::const_iterator it = find(begin(), end(), filePath);
		assert(it != end());

		index = int(it - begin());

		return index;
	}


	ERMsg CStaticDataStream::WriteStream(ostream& stream)
	{
		ERMsg msg;
		//compute size
		try
		{
			//stream.exceptions(std::ios::badbit|std::ios::failbit);

			uint64_t nbSections = NB_SECTIONS;
			uint64_t sectionsNo = FILES;
			uint64_t nbFiles = m_files.size();
			uint64_t size = 0;
			for (size_t i = 0; i < m_files.size(); i++)
			{
				size += 2 * sizeof(uint64_t);
				size += m_files[i].m_filePath.size();
				size += m_files[i].m_data.size();
			}

			write_value(stream, VERSION);
			write_value(stream, nbSections);
			write_value(stream, sectionsNo);
			write_value(stream, nbFiles);
			write_value(stream, size);


			for (size_t i = 0; i < m_files.size(); i++)
			{
				WriteBuffer(stream, m_files[i].m_filePath);
				WriteBuffer(stream, m_files[i].m_data);

				if (stream.bad())
				{
					msg.ajoute("Unable to transfer input file: " + m_files[i].m_filePath);
				}
			}
		}
		catch (...)
		{
			msg.ajoute("Unable to transfer input files");
		}

		return msg;
	}

	ERMsg CStaticDataStream::ReadStream(istream& stream)
	{

		ERMsg msg;

		stream.seekg(0, std::istream::beg);

		uint64_t version = read_value<uint64_t>(stream);
		uint64_t nbSections = read_value<uint64_t>(stream);
		uint64_t sectionsNo = read_value<uint64_t>(stream);
		uint64_t nbFiles = read_value<uint64_t>(stream);
		uint64_t totalSize = read_value<uint64_t>(stream);
		assert(version == 1);
		assert(nbSections == NB_SECTIONS);
		assert(sectionsNo == FILES);

		m_files.resize((size_t)nbFiles);
		for (size_t i = 0; i < (size_t)nbFiles; i++)
		{
			WBSF::ReadBuffer(stream, m_files[i].m_filePath);
			WBSF::ReadBuffer(stream, m_files[i].m_data);
		}

		return msg;
	}

	const std::string& CStaticDataStream::GetFileData(const string& filePath)const
	{
		int index = m_files.Find(filePath);
		assert(index != -1);

		return GetFileData(index);
	}

	const std::string& CStaticDataStream::GetFileData(int index)const
	{
		assert(index >= 0 && index < (int)m_files.size());
		return m_files[index].m_data;
	}


	//****************************************************************
	//Stream communication

	void CCommunicationStream::WriteInputStream(const CTransferInfoIn& info, const CWeatherStation& weather, ostream& stream)
	{
		write_value(stream, VERSION);
		info.WriteStream(stream);
		weather.WriteStream(stream);
	}

	ERMsg CCommunicationStream::ReadInputStream(istream& stream, CTransferInfoIn& info, CWeatherStation& weather)
	{
		ERMsg msg;

		uint64_t version = read_value<uint64_t>(stream);
		assert(version == VERSION);

		msg = info.ReadStream(stream);

		if (msg)
		{
			assert(info.m_transferTypeVersion == CTransferInfoIn::VERSION_STREAM);
			weather.ReadStream(stream);
		}

		return msg;
	}


	void CCommunicationStream::WriteOutputStream(const CTransferInfoOut& info, const CModelStatVector& data, ostream& stream)
	{
		write_value(stream, VERSION);
		info.WriteStream(stream);
		data.WriteStream(stream);
	}

	ERMsg CCommunicationStream::ReadOutputStream(std::istream& stream, CTransferInfoOut& info)
	{
		ERMsg msg;

		uint64_t version = read_value<uint64_t>(stream);
		assert(version == VERSION);

		msg = info.ReadStream(stream);

		return msg;
	}

	ERMsg CCommunicationStream::ReadOutputStream(istream& stream, CTransferInfoOut& info, CModelStatVector& data)
	{
		ERMsg msg;

		uint64_t version = read_value<uint64_t>(stream);
		assert(version == VERSION);

		msg = info.ReadStream(stream);

		if (msg)
			msg = data.ReadStream(stream);

		return msg;
	}

	ERMsg CCommunicationStream::GetErrorMessage(std::istream& stream)
	{
		ERMsg msg;

		uint64_t version = read_value<uint64_t>(stream);
		assert(version == VERSION);

		CTransferInfoOut info;
		msg = info.ReadStream(stream);
		if (msg)
			msg = info.m_msg;

		return msg;
	}

}
