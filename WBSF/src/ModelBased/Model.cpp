//****************************************************************************
// File:	Model.cpp
// Class:	CTemporalRef, CModel
//****************************************************************************
// 08/12/2014  Rémi Saint-Amant	Replace CWCategory by CWVariables
// 04/07/2013  Rémi Saint-Amant	Add SetStaticData
// 14/11/2011  Rémi Saint-Amant Add THREAD_SAFE code
// 03/08/2011  Rémi Saint-Amant Add behavior info
// 01/06/2010  Rémi Saint-Amant	Add m_bOutputJulianDay to manage old model compatibility
// 10/05/2010  Rémi Saint-Amant	Replace outputType by Type/Mode
// 11/12/2008  Rémi Saint-Amant	push_back simulated Annealing functionality
// 15/09/2008  Rémi Saint-Amant	Created from old file
//****************************************************************************


//f you want something simple and lightweight try: https://pocoproject.org/docs/package-Foundation.SharedLibrary.html
//
//Using the SharedLibrary class it takes three lines to call a function in a C shared library:
//
//Poco::SharedLibrary lib("libfoo.so");
//int (* foo)(int) = reinterpret_cast<int (*)(int)>(lib.getSymbol("foo"));
//printf("answer %d\n", foo(5));


//https://www.boost.org/doc/libs/1_66_0/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.plugin_basics


#include <cfloat>
#include <thread>

#include "Basic/xml.hpp"

#include "ModelBased/ModelInput.h"
#include "ModelBased/WGInput.h"
#include "ModelBased/CommunicationStream.h"

#include "ModelBased/Model.h"

//#include "WeatherBasedSimulationString.h"

using namespace std;
using namespace WBSF::WEATHER;
using namespace WBSF::HOURLY_DATA;



namespace WBSF
{


#define SUCCESS						0x00000000
	// general error
#define UNKNOW_ERROR				0xffffffff


#define FIRST_MODEL_ERROR			0x00000021
#define MODEL_ERROR1				0x00000021 //bad input file
#define MODEL_ERROR2				0x00000022 //unable read ids file
#define MODEL_ERROR3				0x00000023 //unable read tgo file
#define MODEL_ERROR4				0x00000024 //unable write ido file
#define MODEL_ERROR5				0x00000025 //not enough memory
#define MODEL_ERROR7				0x00000027 //disk space
#define LAST_MODEL_ERROR			0x00000027


	static const char* TNAME(size_t i)
	{
		return GetTemporalName(i);
	}



	const char* CModel::FILE_EXT = ".mdl";
	const char* CModel::XML_FLAG = "BioSIMModel";
	const char* CModel::MEMBER_NAME[NB_MEMBER] = { "Title", "Version", "ID", "EXE", "Behaviour", "Description", "WindowRect", "SimulatedCategory", "TransferFileVersion", "IOFileVersion", "NbYearMin", "NbYearMax", "ThreadSafe", "InputVariableArray", "SSI", "OutputTypeMode", "JulianDay", "MissingValue", "OutputVariableArray", "Credit", "Copyright", "HelpType", "HelpFileName", "HelpText" };
	const short CModel::TRANSLATED_MEMBER[NB_TRANSLATED_MEMBER] = { TITLE, DESCRIPTION, CREDIT, COPYRIGHT, HELP_FILENAME, HELP_TEXT };
#if defined(__linux__)
    const std::string CModel::PREFIX = "lib";
    const std::string CModel::EXTENTION = ".so";

#else //Windows
    const std::string CModel::PREFIX = "";
    const std::string CModel::EXTENTION = ".dll";

#endif // _linux_
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////


	CModel::CModel()
	{
		m_RunModelStream = nullptr;
		Reset();
	}

	CModel::CModel(const CModel& in)
	{
		operator = (in);
	}

	CModel::~CModel()
	{
		UnloadLibrary();
	}



	void CModel::Reset()
	{
		UnloadLibrary();
		assert(m_RunModelStream == nullptr);

		m_windowRect = { 0, 0, 250, 100 };
		m_SetStaticData = nullptr;
		m_GetSimulatedAnnealingSize = nullptr;
		m_SetSimulatedAnnealingSize = nullptr;
		m_InitSimulatedAnnealing = nullptr;
		m_GetFValue = nullptr;

		m_filePath.clear();
		m_title.clear();
		m_version = "1.0";
		m_behaviour = DETERMINISTIC;
		m_variables = "T";
		m_outputTM = CTM(CTM::DAILY);
		m_missValue = MISSING;

		m_transferFileVersion = VERSION_XML;
		//m_IOFileVersion=VERSION_ASCII_WITH_HEADER;
		m_nbYearMin = 1;
		m_nbYearMax = 999;
		m_bThreadSafe = true;
		m_SSI.clear();

		m_inputList.clear();
		m_outputList.clear();
	}

	ERMsg CModel::Load(const string& filePath)
	{
		assert(!filePath.empty());

		ERMsg msg;

		m_filePath = filePath;
		if (FileExists(filePath))
		{
			int version = GetFileVersion(filePath);

			if (version == 3)
			{
				xml_document doc;
				msg = doc.load_file(filePath);
				read_xml(doc.document_element());
			}
			else
			{
				msg.ajoute("Unsupported old model version. Ask conversion to developer");
			}
		}
		else
		{
			msg.ajoute("file doesn't exist: "+ filePath);
		}

		return msg;
	}

	ERMsg CModel::Save(const string& filePath)
	{
		ERMsg msg;

		xml_document doc;

		pugi::xml_node root = doc.append_child("Model");
		root.append_attribute("version").set_value("3");

		write_xml(root);
		msg = doc.save_file(filePath);

		return msg;//zen::SaveXML(filePath, "Model", "3", *this);
	}

	void CModel::write_xml(pugi::xml_node node)const
	{
		node.append_child(GetMemberName(TITLE)).set_value(m_title.c_str());
		node.append_child(GetMemberName(VERSION)).set_value(m_version.c_str());
		node.append_child(GetMemberName(EXTENSION)).set_value(m_extension.c_str());
		node.append_child(GetMemberName(LIBRARY_NAME)).set_value(m_libraryName.c_str());
		node.append_child(GetMemberName(BEHAVIOUR)).set_value(to_string(m_behaviour).c_str());
		node.append_child(GetMemberName(DESCRIPTION)).set_value(m_description.c_str());
		node.append_child(GetMemberName(WINDOW_RECT)).set_value(to_string(m_windowRect).c_str());
		node.append_child(GetMemberName(SIMULATED_CATEGORY)).set_value(m_variables.to_string().c_str());
		node.append_child(GetMemberName(TRANSFER_FILE_VERSION)).set_value(to_string(m_transferFileVersion).c_str());
		node.append_child(GetMemberName(SSI)).set_value(m_SSI.c_str());
		node.append_child(GetMemberName(NB_YEAR_MIN)).set_value(to_string(m_nbYearMin).c_str());
		node.append_child(GetMemberName(NB_YEAR_MAX)).set_value(to_string(m_nbYearMax).c_str());
		node.append_child(GetMemberName(THREAD_SAFE)).set_value(to_string(m_bThreadSafe).c_str());
		m_inputList.write_xml(node.append_child(GetMemberName(INPUT_VARIABLE)));
		node.append_child(GetMemberName(OUTPUT_TM)).set_value(m_outputTM.to_string().c_str());
		node.append_child(GetMemberName(MISSING_VALUE)).set_value(to_string(m_missValue).c_str());
		m_outputList.write_xml(node.append_child(GetMemberName(OUTPUT_VARIABLE)));
		node.append_child(GetMemberName(COPYRIGHT)).set_value(m_copyright.c_str());
		node.append_child(GetMemberName(CREDIT)).set_value(m_credit.c_str());
		node.append_child(GetMemberName(HELP_FILENAME)).set_value(m_helpFileName.c_str());

	}

	void CModel::read_xml(pugi::xml_node node)
	{
		m_title = node.child(GetMemberName(TITLE)).text().as_string();
		m_version = node.child(GetMemberName(VERSION)).text().as_string();
		m_extension = node.child(GetMemberName(EXTENSION)).text().as_string();
		m_libraryName = node.child(GetMemberName(LIBRARY_NAME)).text().as_string();
		m_behaviour = node.child(GetMemberName(BEHAVIOUR)).text().as_int();
		m_description = node.child(GetMemberName(DESCRIPTION)).text().as_string();
		m_windowRect = to_rect(node.child_value(GetMemberName(WINDOW_RECT)));
		m_variables = node.child(GetMemberName(SIMULATED_CATEGORY)).text().as_string();
		m_transferFileVersion = node.child(GetMemberName(TRANSFER_FILE_VERSION)).text().as_int();
		m_SSI = node.child(GetMemberName(SSI)).text().as_string();
		m_nbYearMin = node.child(GetMemberName(NB_YEAR_MIN)).text().as_int();
		m_nbYearMax = node.child(GetMemberName(NB_YEAR_MAX)).text().as_int();
		m_bThreadSafe = node.child(GetMemberName(THREAD_SAFE)).text().as_bool();
		m_inputList.read_xml(node.child(GetMemberName(INPUT_VARIABLE)));
		m_outputTM = node.child(GetMemberName(OUTPUT_TM)).text().as_string();
		m_missValue = node.child(GetMemberName(MISSING_VALUE)).text().as_double();
		m_outputList.read_xml(node.child(GetMemberName(OUTPUT_VARIABLE)));
		m_copyright = node.child(GetMemberName(COPYRIGHT)).text().as_string();
		m_credit = node.child(GetMemberName(CREDIT)).text().as_string();
		m_helpFileName = node.child(GetMemberName(HELP_FILENAME)).text().as_string();

		//remove ".dll" form old model
		size_t pos = Find(m_libraryName, ".dll");
		if(pos !=string::npos)
            m_libraryName = m_libraryName.substr(0,pos);
	}

	//********************************************************

	bool CModel::operator == (const CModel& in)const
	{
		bool bEqual = true;

		if (m_title != in.m_title) bEqual = false;
		if (m_version != in.m_version) bEqual = false;
		if (m_extension != in.m_extension) bEqual = false;
		if (m_libraryName != in.m_libraryName) bEqual = false;
		if (m_behaviour != in.m_behaviour) bEqual = false;
		if (m_description != in.m_description) bEqual = false;
		if (m_windowRect.left != in.m_windowRect.left) bEqual = false;
		if (m_windowRect.right != in.m_windowRect.right) bEqual = false;
		if (m_windowRect.top != in.m_windowRect.top) bEqual = false;
		if (m_windowRect.bottom != in.m_windowRect.bottom) bEqual = false;
		if (m_credit != in.m_credit) bEqual = false;
		if (m_transferFileVersion != in.m_transferFileVersion)bEqual = false;
		if (m_SSI != in.m_SSI)bEqual = false;
		if (m_outputTM != in.m_outputTM)bEqual = false;
		if (m_missValue != in.m_missValue)bEqual = false;
		if (m_nbYearMin != in.m_nbYearMin)bEqual = false;
		if (m_nbYearMax != in.m_nbYearMax)bEqual = false;
		if (m_bThreadSafe != in.m_bThreadSafe)bEqual = false;
		if (m_variables != in.m_variables)bEqual = false;
		if (m_copyright != in.m_copyright)bEqual = false;
		if (m_helpFileName != in.m_helpFileName)bEqual = false;
		if (m_inputList != in.m_inputList)bEqual = false;;
		if (m_outputList != in.m_outputList)bEqual = false;;

		return bEqual;
	}

	bool CModel::operator != (const CModel& model)const
	{
		return !(*this == model);
	}

	CModel& CModel::operator = (const CModel& in)
	{
		if (&in != this)
		{
			//m_name = in.m_name;
			m_filePath = in.m_filePath;
			m_title = in.m_title;
			m_version = in.m_version;
			m_extension = in.m_extension;
			m_libraryName = in.m_libraryName;
			m_behaviour = in.m_behaviour;
			m_description = in.m_description;


			m_windowRect = in.m_windowRect;
			m_credit = in.m_credit;

			m_inputList = in.m_inputList;
			m_outputList = in.m_outputList;

			m_variables = in.m_variables;
			m_transferFileVersion = in.m_transferFileVersion;
			m_outputTM = in.m_outputTM;
			m_SSI = in.m_SSI;
			//m_bOutputJulianDay = in.m_bOutputJulianDay;

			m_missValue = in.m_missValue;
			//m_IOFileVersion = in.m_IOFileVersion;
			m_nbYearMin = in.m_nbYearMin;
			m_nbYearMax = in.m_nbYearMax;
			m_bThreadSafe = in.m_bThreadSafe;

			m_copyright = in.m_copyright;
			// m_helpType = in.m_helpType;
			m_helpFileName = in.m_helpFileName;
			//m_helpText = in.m_helpText;

			UnloadLibrary();
		}

		assert(in == *this);

		return *this;
	}

	void CModel::GetDefaultParameter(CModelInput& modelInput)const
	{
		modelInput.clear();
		modelInput.SetExtension(m_extension);
		modelInput.SetName(STRDEFAULT);

		for (size_t i = 0; i < m_inputList.size(); i++)
		{
			int type = m_inputList[i].GetType();
			if (type != CModelInputParameterDef::kMVTitle &&
				type != CModelInputParameterDef::kMVLine &&
				type != CModelInputParameterDef::kMVStaticText)
			{
				CModelInputParam p;
				p.m_name = m_inputList[i].m_name;
				//			p.m_type = m_inputList[i].m_type;
				p.m_value = m_inputList[i].m_default;
				modelInput.push_back(p);
			}
		}
	}

	void CModel::SetDefaultParameter(const CModelInput& modelInput)
	{
		for (size_t i = 0, p = 0; i < m_inputList.size() && p < modelInput.size(); i++)
		{
			int type = m_inputList[i].GetType();
			if (type != CModelInputParameterDef::kMVTitle &&
				type != CModelInputParameterDef::kMVLine &&
				type != CModelInputParameterDef::kMVStaticText)
			{
				m_inputList[i].m_name = modelInput[p].m_name;
				m_inputList[i].m_default = modelInput[p].m_value;
				p++;
			}
		}
	}

	ERMsg CModel::LoadLibrary()
	{

		ERMsg msg;

		if (!m_shared_library.is_loaded())
		{
			string filePath = GetLibraryFilePath();

			assert (Find(filePath, ".dll") != string::npos || Find(filePath, ".so") != string::npos);

            try
            {
                m_shared_library.load(filePath.c_str());
            }
            catch (boost::exception& e)
            {
                msg.ajoute( diagnostic_information(e));
                msg.ajoute(FormatMsg("Cannot load library \"%1%\".", filePath));
            }

		}


		return msg;
	}

	void CModel::UnloadLibrary()
	{
		if (m_shared_library.is_loaded())
		{
			m_shared_library.unload();
			m_RunModelStream = nullptr;
			m_SetStaticData = nullptr;
			m_GetSimulatedAnnealingSize = nullptr;
			m_SetSimulatedAnnealingSize = nullptr;
			m_InitSimulatedAnnealing = nullptr;
			m_GetFValue = nullptr;
		}
	}


	ERMsg CModel::RunModel(std::istream& inStream, std::iostream& outStream)
	{
		ERMsg msg;

		if (m_RunModelStream == nullptr)
		{
			msg = LoadLibrary();
			if (msg)
			{
				assert(IsLoaded());
				m_RunModelStream = m_shared_library.get<bool(std::istream & inStream, std::ostream & outStream)>("RunModelStream");

				if (m_RunModelStream == nullptr)
				{
					msg.ajoute(FormatMsg("Cannot obtain method '%1%' in library \"%2%\".", "RunModelStream", GetLibraryFilePath()));
				}
			}
		}

		if (msg)
		{
			if (!m_RunModelStream(inStream, outStream))
				msg = CCommunicationStream::GetErrorMessage(outStream);
		}

		return msg;
	}

	ERMsg CModel::SetStaticData(std::istream& inStream)
	{
		ERMsg msg;

		//set static data on if they have data
		inStream.seekg(0, std::istream::end);
		if (inStream.tellg() > 0)
		{
			if (m_SetStaticData == nullptr)
			{
				msg = LoadLibrary();
				if (msg)
				{
					//assert(m_hDll != nullptr);
					//                m_SetStaticData = (SetStaticDataF)GetProcAddress(m_hDll, "SetStaticData");
					m_SetStaticData = m_shared_library.get<SetStaticDataF>("SetStaticData");

					if (m_SetStaticData == nullptr)
					{
						msg.ajoute(FormatMsg("Cannot obtain method '%1%' in library \"%2%\".", "SetStaticData", GetLibraryFilePath()));
					}
				}
			}

			if (msg)
			{
				inStream.seekg(0, std::istream::beg);
				m_SetStaticData(inStream);
			}
		}

		return msg;
	}

	//ERMsg CModel::RunModelEXE(const string& nameInputFile)
	//{
	//	ERMsg msg;
	//
	//	string IDsFileName(nameInputFile);
	//
	//	string exeName = GetLibraryFilePath();
	//
	//
	//	IDsFileName = '"' + IDsFileName + '"';
	//	string command = exeName + " " + IDsFileName;
	//
	//	WinExecWait(exeName, IDsFileName);
	//
	//	// STARTUPINFO si;
	//	// ::ZeroMemory(&si, sizeof(STARTUPINFO));
	//	// si.cb = sizeof(STARTUPINFO);
	//	// si.dwFlags = STARTF_USESHOWWINDOW;
	//	// si.wShowWindow = SW_HIDE;
	//	//
	//	//
	//	// PROCESS_INFORMATION pi;
	//	//
	//	// //char* pCommand = (char*) command.c_str();//.GetBuffer(1);
	//	// wstring commandW = convert(command);
	//	// if (::CreateProcessW(nullptr, &(commandW[0]), nullptr, nullptr,
	//	//                      FALSE, nullptr, nullptr, nullptr, &si, &pi))//NORMAL_PRIORITY_CLASS
	//	// {
	//	//     ::CloseHandle(pi.hThread);
	//	//     ::WaitForSingleObject(pi.hProcess, INFINITE);
	//	//
	//	//
	//	//     DWORD code;
	//	//     GetExitCodeProcess(pi.hProcess, &code);
	//	//
	//	//     if (code != SUCCESS)
	//	//     {
	//	//         msg = FormatModelError(code);
	//	//
	//	//         if (!GetDiskSpaceOk(nameInputFile))
	//	//         {
	//	//             msg.ajoute(GetString(IDS_BSC_DISK_FULL));
	//	//         }
	//	//     }
	//	//
	//	//     ::CloseHandle(pi.hProcess);
	//	// }
	//	// else
	//	// {
	//	//     DWORD err = GetLastError();
	//	//     switch (err)
	//	//     {
	//	//     case ERROR_FILE_NOT_FOUND:
	//	//     {
	//	//         msg.ajoute(FormatMsg(IDS_BSC_EXE_NOTFOUND, exeName));
	//	//         break;
	//	//     }
	//	//     case ERROR_BAD_EXE_FORMAT:
	//	//     {
	//	//         msg.ajoute(FormatMsg(IDS_BSC_ERROR_BAD_EXE_FORMAT, exeName));
	//	//         break;
	//	//     }
	//	//     case ERROR_ACCESS_DENIED:
	//	//     default:
	//	//     {
	//	//         PSTR lpMsgBuf = nullptr;
	//	//         FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
	//	//                        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
	//	//                        err,
	//	//                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	//	//                        (LPSTR)&lpMsgBuf, 0, nullptr);
	//	//
	//	//         msg.ajoute(FormatMsg(IDS_BSC_UNKNOW_ERROR, lpMsgBuf, exeName));
	//	//
	//	//         break;
	//	//     }
	//	//     }
	//	// }
	//
	//	return msg;
	//}

	//ERMsg CModel::RunModelDLL(const string& nameInputFile)
	//{
	//
	//	//string IDsFileName(nameInputFile);
	//	ERMsg msg;
	//
	//	assert(m_RunModelFile != nullptr);
	//
	//	char errorMessage[1024] = { 0 };
	//	if (!m_RunModelFile(nameInputFile.c_str(), errorMessage))
	//	{
	//		msg.ajoute(errorMessage);
	//
	//		if (!GetDiskSpaceOk(nameInputFile))
	//		{
	//			msg.ajoute("Disk may be full or write-protected. Check disk space and permissions.");
	//		}
	//	}
	//
	//	return msg;
	//}



	//bool CModel::GetDiskSpaceOk(const string& nameInputFile)
	//{
	//	//  // receives the number of bytes on disk available to the caller
	//	//  ULARGE_INTEGER FreeBytesAvailableToCaller;
	//	//  // receives the number of bytes on disk
	//	//  ULARGE_INTEGER TotalNumberOfBytes;
	//	//  // receives the free bytes on disk
	//	//  ULARGE_INTEGER TotalNumberOfFreeBytes;
	//	//  string path = WBSF::GetPath(nameInputFile);
	//	//
	//	//  //verification supplémentaire pour vérifier qu'il y a assez d'espace sur le disque
	//	//  GetDiskFreeSpaceExW(convert(path).c_str(), &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
	//
	//	assert(false);//todo
	//	return true;//FreeBytesAvailableToCaller.QuadPart > 500000;
	//}

	ERMsg CModel::VerifyModelInput(const CModelInput& modelInput)const
	{
		return modelInput.IsValid(m_inputList);
	}


	ERMsg CModel::FormatModelError(int code)
	{
		string filePath = GetLibraryFilePath(filePath);

		ERMsg msg;

		string error;

		//if (m_RunModelFile == nullptr)
		//error = FormatMsg(IDS_SIM_ERRORINMODEL, filePath, to_string(code) );
		//else
		error = FormatMsg("DLL \"%1%\" returned error code '%2%'.", m_libraryName, to_string(code));

		msg.ajoute(error);

		switch (code)
		{
			//no input file
		case MODEL_ERROR1:
			error = "Unable to find input file.";
			break;
			//unable read ids file
		case MODEL_ERROR2:
			error = "Unable to read input file.";
			break;
			//unable read tgo file
		case MODEL_ERROR3:
			error = "Unable to read correctly temperature generation file. ";
			break;
			//unable write ido file
		case MODEL_ERROR4:
			error = "Unable to write output file.";
			break;
			//not enought memory
		case MODEL_ERROR5:
			error = "Not enough memory.";
			break;
		case MODEL_ERROR7:
			error = "Disk may be full or write-protected. Check disk space and permissions.";
			break;
		default:
			break;
		}

		msg.ajoute(error);

		return msg;
	}


	string CModel::GetLibraryVersion(const string& filePath)
	{
		string version;

		boost::dll::shared_library shared_library;

		try
		{
			shared_library.load(filePath);

			//GetModelVersionF GetModelVersion = shared_library.get<GetModelVersionF>("GetModelVersion");
			auto GetModelVersion = shared_library.get<const char* ()>("GetModelVersion");
			if (GetModelVersion)
				version = GetModelVersion();

			shared_library.unload();
		}
		catch (const boost::system::system_error& e)
		{
			std::cerr << "Error loading library: " << e.what() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
		}


		return version;
	}


	CTRef CModel::GetTRef(CTM TM, const std::vector<double>& in)
	{
		//assert( in.size() == GetNbOutputReference(TM) );

		short p[4] = { 0 };
		p[0] = in.size() > 0 ? short(in[0]) : 0;
		p[1] = in.size() > 1 ? short(in[1]) : 0;
		p[2] = in.size() > 2 ? short(in[2]) : 0;
		p[3] = in.size() > 3 ? short(in[3]) : 0;

		CTRef ref;
		ref.Set(p[0], p[1] - 1, p[2] - 1, p[3] % 24, TM);

		return ref;
	}


	void CModel::SetExtension(const string& ext)
	{
		assert(!ext.empty());
		assert(ext[0] == '.');

		m_extension = ext;
	}

	CModelInputParameterDefVector CModel::GetInputDefinition(bool RemoveNonVariable)const
	{
		CModelInputParameterDefVector varList;
		if (RemoveNonVariable)
		{
			varList.clear();
			for (size_t i = 0; i < m_inputList.size(); i++)
			{
				if (m_inputList[i].IsAVariable())
					varList.push_back(m_inputList[i]);
			}
		}
		else
		{
			varList = m_inputList;
		}

		return varList;
	}


	short CModel::GetFileVersion(const string& filePath)
	{
		short version = -1;

		ifStream file;
		if (file.open(filePath))
		{
			string buffer;
			std::getline(file, buffer);
			MakeLower(buffer);

			if (buffer.find("<?xml") != string::npos)
				version = 3;
			else version = 2;

			file.close();
		}

		return version;
	}

	string CModel::GetDocumentation()const
	{
		string doc;

		std::vector<std::string> title = Tokenize("Documentation for model:\t|Copyright:\t|Model input extension:\t|Description:\n|Weather Input Variables:\t|Input:\n|Output type:\t|Output:\n|Credit:\n|", "|");

		int c = 0;
		doc += title[c++] + GetTitle() + "\n";
		doc += title[c++] + m_copyright + "\n";
		doc += title[c++] + m_extension + "\n";
		doc += title[c++] + m_description + "\n\n";
		doc += title[c++] + m_variables.to_string() + "\n\n";

		doc += title[c++];
		for (size_t i = 0; i < m_inputList.size(); i++)
		{
			if (m_inputList[i].IsAVariable())
				doc += string("\t") + m_inputList[i].GetName() + ":\t" + m_inputList[i].GetDescription() + "\n";
		}

		doc += "\n";
		doc += title[c++] + string(m_outputTM.GetTypeName()) + "\n\n";

		doc += title[c++];
		for (size_t i = 0; i < m_outputList.size(); i++)
			doc += string("\t") + m_outputList[i].m_title + ":\t" + m_outputList[i].m_description + " [" + m_outputList[i].m_units + "]\n";

		doc += "\n";
		doc += title[c++] + m_credit + "\n\n";

		return doc;
	}

	ERMsg CModel::VerifyWGInput(const CWGInput& WGInput)const
	{
		ERMsg msg;

		if (WGInput.GetNbYears() < GetNbYearMin() ||
			WGInput.GetNbYears() > GetNbYearMax())
		{
			msg.ajoute(FormatMsg("The number of year(s) (%1%) defined in the WG Input is invalid for this model. The model \"%2%\" needs at least %3% year(s) and not more than %4% year(s).", to_string(WGInput.GetNbYears()), GetName(), to_string(GetNbYearMin()), to_string(GetNbYearMax())));
		}


		return msg;
	}

	ERMsg CModel::VerifyInputs(const std::vector<std::string>& SSIHeader, CWVariables variables)const
	{
		ERMsg msg;

		//assert(false);
		std::vector<std::string> SSI = WBSF::Tokenize(m_SSI, "|");
		for (size_t i = 0; i < m_SSI.size(); i++)
		{
			//if (!SSIHeader.Find(SSI[i], false))
			//{
			//    msg.ajoute("Missing SSI: " + SSI[i]);
			//    //msg.ajoute(FormatMsg(IDS_BSC_MISSING_WEATHER, HOURLY_DATA::GetVariableName(v)));
			//}
		}


		for (size_t v = 0; v < m_variables.size(); v++)
		{
			if (m_variables[v] && !variables[v])
			{
				msg.ajoute(FormatMsg("The input weather variable %1% is missing.", HOURLY_DATA::GetVariableName(v)));
			}
		}

		return msg;
	}



	//*******************************************************************
	//Simulated Annealing part
	ERMsg CModel::SetSimulatedAnnealingSize(long size, string resultFilePath)
	{
		ERMsg msg;
		if (!IsLoaded())
		{
			msg = LoadLibrary();
		}

		if (!msg)
			return msg;


		m_GetSimulatedAnnealingSize = m_shared_library.get < GetSimulatedAnnealingSizeF>("GetSimulatedAnnealingSize");
		m_SetSimulatedAnnealingSize = m_shared_library.get < SetSimulatedAnnealingSizeF>("SetSimulatedAnnealingSize");
		m_InitSimulatedAnnealing = m_shared_library.get < InitSimulatedAnnealingF>("InitSimulatedAnnealing");
		m_GetFValue = m_shared_library.get < GetFValueF>("GetFValue");



		if (m_GetSimulatedAnnealingSize &&
			m_SetSimulatedAnnealingSize &&
			m_InitSimulatedAnnealing &&
			m_GetFValue)
		{
			m_SetSimulatedAnnealingSize(size, resultFilePath.c_str());
			assert(m_GetSimulatedAnnealingSize() == size);
		}
		else
		{
			msg.ajoute("This dll is not created for SimulatedAnnealing");
		}

		return msg;
	}

	ERMsg CModel::InitSimulatedAnnealing(long ID, const string& IDsFilePath)
	{
		assert(m_SetSimulatedAnnealingSize && m_InitSimulatedAnnealing && m_GetFValue);

		ERMsg msg;

		char error[1024] = { 0 };
		if (!m_InitSimulatedAnnealing(ID, IDsFilePath.c_str(), error))
		{
			msg.ajoute(error);
		}
		//	}
		//	else
		//	{
		//		msg.ajoute("Error loading dll for simulated annealing");
		//	}

		return msg;
	}


	double CModel::GetFValue(const vector<double>& X, CStatisticXYVector& stat)
	{
		assert(m_GetFValue);
		return m_GetFValue((int)X.size(), X.data(), stat);
	}




}
