//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
// 16-09-2025   Rémi Saint-Amant	Port on Linux
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************


#ifdef _MSC_VER
#include <SDKDDKVer.h>//add this include to avoid warning in boost/local.hpp
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <cstdarg>

#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp> // Or boost::process.hpp
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/crc.hpp>

//#ifdef _MSC_VER
//#if defined(_WIN32)
#include <boost/process/v1.hpp>
//#else
//#include <boost/process.hpp>
//#endif

#include <boost/process.hpp>

#include "WBSFconfig.h"
#include "UtilStd.h"


using namespace std;

using boost::tokenizer;
using boost::escaped_list_separator;
namespace boost_process = boost::process;


//template < >
//boost::filesystem::path& boost::filesystem::path::append< typename boost::filesystem::path::iterator >(typename boost::filesystem::path::iterator begin, typename boost::filesystem::path::iterator end, const codecvt_type& cvt)
//{
//    for (; begin != end; ++begin)
//        *this /= *begin;
//    return *this;
//}



namespace WBSF
{


	// Return path when appended to a_From will resolve to same as a_To
	std::filesystem::path make_relative(std::filesystem::path a_From, std::filesystem::path a_To)
	{
		std::filesystem::path ret;


		a_From = std::filesystem::absolute(a_From);
		a_To = std::filesystem::absolute(a_To);
		//std::filesystem::path ret;
		std::filesystem::path::const_iterator itrFrom(a_From.begin()), itrTo(a_To.begin());
		// Find common base
		for (std::filesystem::path::const_iterator toEnd(a_To.end()), fromEnd(a_From.end()); itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo);
		// Navigate backwards in directory to reach previously found base
		for (std::filesystem::path::const_iterator fromEnd(a_From.end()); itrFrom != fromEnd; ++itrFrom)
		{
			if ((*itrFrom) != ".")
				ret /= "..";
		}
		// Now navigate down the directory branch
		//ret.append(itrTo, a_To.end(), std::filesystem::path::codecvt());

		auto pp = std::mismatch(a_From.begin(), a_From.end(), a_To.begin());
		for (auto iter = pp.second; iter != a_To.end(); ++iter)
			ret /= *iter;


		return ret;
	}


	bool GDALStyleProgressBar(double dfComplete)
	{
		const int nThisTick =
			std::min(40, std::max(0, static_cast<int>(dfComplete * 40.0)));

		// Have we started a new progress run?
		static int nLastTick = -1;
		if (nThisTick < nLastTick && nLastTick >= 39)
			nLastTick = -1;

		if (nThisTick <= nLastTick)
			return true;

		while (nThisTick > nLastTick)
		{
			++nLastTick;
			if (nLastTick % 4 == 0)
				fprintf(stdout, "%d", (nLastTick / 4) * 10);
			else
				fprintf(stdout, ".");
		}

		if (nThisTick == 40)
			fprintf(stdout, " - done.\n");
		else
			fflush(stdout);

		return true;
	}

	//const std::string STRVMISS = "VMiss";
	//const std::string STRDEFAULT = "Default";

	ERMsg RemoveFile(const std::string& filePath)
	{
		ERMsg msg;

		if (FileExists(filePath))
		{
			remove(filePath.c_str());
			if (FileExists(filePath))
				msg.ajoute("Error deleting file: " + filePath);// GetLastErrorMessage();
		}

		return msg;
	}



	ERMsg RemoveDir(const std::string& pathIn)
	{
		ERMsg msg;

		//std::string path(pathIn);

		//if (IsPathEndOk(path))
			//path = path.substr(0, path.size() - 1);

		//	if (DirectoryExists(path))
	//		{
		boost::filesystem::path path = pathIn;

		try
		{
			if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path))
			{
				// Remove the directory and all its contents
				boost::filesystem::remove_all(path);
				//std::cout << "Directory and its contents removed successfully." << std::endl;
			}
			else
			{
				msg.ajoute("Directory does not exist or is not a directory");
			}
		}
		catch (const boost::filesystem::filesystem_error& e)
		{
			msg.ajoute(string("Error removing directory: ") + e.what());
		}
		//}


		return msg;
	}

	ERMsg RenameFile(const std::string& filePath1, const std::string& filePath2)
	{
		ERMsg msg;

		//		if (!MoveFileExW(UTF16(filePath1).c_str(), UTF16(filePath2).c_str(), MOVEFILE_COPY_ALLOWED))
		//		{
		//			msg = GetLastErrorMessage();
		//			msg.ajoute(FormatMsg(IDS_BSC_UNABLE_RENAME, filePath1, filePath2));
		//		}


		return msg;
	}

	ERMsg RenameDir(const std::string& pathIn1, const std::string& pathIn2)
	{
		ERMsg msg;


		std::string path1(pathIn1);

		if (IsPathEndOk(path1))
			path1 = path1.substr(0, path1.size() - 1);

		std::string path2(pathIn1);

		if (IsPathEndOk(path2))
			path2 = path2.substr(0, path2.size() - 1);

		//		if (!MoveFileExW(UTF16(path1).c_str(), UTF16(path2).c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING))
		//		{
		//			msg = GetLastErrorMessage();
		//			msg.ajoute(pathIn1);
		//			msg.ajoute(pathIn2);
		//		}


		return msg;
	}

	ERMsg CopyOneFile(const std::string& filePath1, const std::string& filePath2, bool failIfExist)
	{
		ERMsg msg;

		//		if (!CopyFileA(filePath1.c_str(), filePath2.c_str(), failIfExist))
		//		{
		//			msg = GetLastErrorMessage();
		//			msg.ajoute(FormatMsg(IDS_BSC_UNABLE_COPY_FILE, filePath1, filePath2));
		//		}

		return msg;
	}

	ERMsg CopyDirectory(const std::string& pathIn1, const std::string& pathIn2)
	{
		ERMsg msg;
		//
		//
		//		std::wstring path1(UTF16(pathIn1));
		//
		//		//		if (IsPathEndOk(path1))
		//					//path1 = path1.substr(0, path1.size() - 1);
		//
		//		std::wstring path2(UTF16(pathIn1));
		//
		//		//if (IsPathEndOk(path2))
		//			//path2 = path2.substr(0, path2.size() - 1);
		//
		//		assert(false);
		//		SHFILEOPSTRUCTW s = { 0 };
		//		s.hwnd = NULL;
		//		s.wFunc = FO_COPY;
		//		s.fFlags = FOF_SILENT;
		//		s.pTo = path2.c_str();
		//		s.pFrom = path1.c_str();
		//
		//
		//		//if (SHFileOperation(&s)>0)
		//		//msg.ajoute("Erreur copy dir");
		//

		return msg;
	}
	//
	//	std::string GetErrorString(ERMsg msg, const char* sep)
	//	{
	//		std::string str;
	//		for (int i = 0; i < (int)msg.dimension(); i++)
	//			str += msg[i] + sep;
	//
	//		return str;
	//	}
	//
	//	ERMsg GetErrorMsgFromString(const std::string& str, const char* sep)
	//	{
	//		ERMsg msg;
	//
	//		std::vector<std::string> errors(str.c_str(), sep);
	//		for (std::vector<std::string>::const_iterator it = errors.begin(); it != errors.end(); it++)
	//			msg.ajoute(*it);
	//
	//		return msg;
	//	}
	//

	std::string GetRelativePath(const std::string& base_path_in, const std::string& file_path_in)
	{

		std::filesystem::path base_path(base_path_in);
		std::filesystem::path file_path(file_path_in);



		std::filesystem::path rel_path;
		if (!file_path.empty())
		{
			if (base_path.root_name() == file_path.root_name())
			{
				rel_path = make_relative(base_path, file_path);
				rel_path = rel_path.parent_path();
				if (rel_path.empty())
					rel_path = "./";
			}
			else
			{
				rel_path = file_path;
			}
		}

		return rel_path.string();
	}

#ifdef _MSC_VER
#define realpath( x, yfilename , audit_log );
#endif

	std::filesystem::path SimplifyFilePath(const std::filesystem::path& file_path)
	{

		//std::string tmp;
		//tmp.resize(255);
		//_fullpath(&(tmp[0]), filePath.c_str(), 255);
		//realpath filePath.c_str(), tmp.data());
		//tmp.resize(strlen(tmp.c_str()));


		//return tmp;

		std::filesystem::path canonical_path = std::filesystem::weakly_canonical(file_path);
		return canonical_path.make_preferred();
	}

	std::string GetAbsolutePath(const std::string& base_path_in, const std::string& file_path_in)
	{
		std::filesystem::path base_path(base_path_in);
		std::filesystem::path file_path(file_path_in);


		std::filesystem::path abs_path;
		if (!file_path.empty())
		{
			//std::wstring wBasePath(UTF16(sBasePath));
			//std::wstring wFilePath(UTF16(sFilePath));
			//std::filesystem::path basePath(sBasePath);
			//std::filesystem::path filePath(sFilePath);
			if (file_path.is_relative())
			{
				//std::system::error_code ec;
				//std::filesystem::path abs_path = std::filesystem::absolute(file_path, base_path);
				//std::filesystem::path abs_path = std::filesystem::absolute(file_path, );
				abs_path = file_path / base_path;
				abs_path = SimplifyFilePath(abs_path);

				assert(!abs_path.empty());
			}
			else
			{
				abs_path = file_path;
			}

		}

		return abs_path.string();
	}


	std::string upperCase(std::string input)
	{
		for (std::string::iterator it = input.begin(); it != input.end(); ++it)
			*it = toupper(*it);
		return input;
	}


	string FilePath2SpecialPath(const std::string& filePath, const string& appPath, const string& projectPath)
	{
		string special_path = filePath;
		if (!filePath.empty())
		{
			string::size_type pos = upperCase(filePath).find(upperCase(projectPath));
			if (pos != string::npos)
			{
				special_path.replace(pos, projectPath.length(), "[Project Path]/");
			}
			else
			{
				pos = upperCase(filePath).find(upperCase(appPath));
				if (pos != NOT_INIT)
				{
					special_path.replace(pos, appPath.length(), "[BioSIM Path]/");
				}
			}

		}

		return special_path;
	}

	string SpecialPath2FilePath(const string& specialPath, const string& appPath, const string& projectPath)
	{
		string filePath = specialPath;

		if (!filePath.empty())
		{
			string::size_type pos = filePath.find("[Project Path]\\");
			if (pos != string::npos)
			{
				filePath = ReplaceString(filePath, "[Project Path]\\", projectPath.c_str());
			}

			pos = filePath.find("[BioSIM Path]\\");
			if (pos != NOT_INIT)
			{
				filePath = ReplaceString(filePath, "[BioSIM Path]\\", appPath.c_str());
			}

			pos = filePath.find("[Models Path]\\");
			if (pos != NOT_INIT)
			{
				filePath = ReplaceString(filePath, "[Models Path]\\", (appPath + "Models\\").c_str());
			}
		}

		return filePath;
	}

	bool FileExists(const std::string& file_path)
	{
		return std::filesystem::exists(file_path);
		//		DWORD ftyp = GetFileAttributesA(filePath.c_str());
		//		if (ftyp == INVALID_FILE_ATTRIBUTES)
		//			return false;  //something is wrong with your path!
		//
		//		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		//			return false;   // this is a directory, not a file

			//bool bExists = !(INVALID_FILE_ATTRIBUTES == GetFileAttributesA(filePath.c_str()) && GetLastError() == ERROR_FILE_NOT_FOUND);
			//return true;
	}


	bool DirectoryExists(const std::string& file_path)
	{
		return std::filesystem::exists(file_path);


		//std::string tmp(path);
	//    while (IsPathEndOk(path))
		//      path = path.substr(0, path.length() - 1);

	//		DWORD ftyp = GetFileAttributesA(path.c_str());
	//		if (ftyp == INVALID_FILE_ATTRIBUTES)
	//			return false;  //something is wrong with your path!
	//
	//		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	//			return true;   // this is a directory!

		//return false;    // this is not a directory!
	}

	std::string GetApplicationPath()
	{
		//filesystem::path app_path;

	//    boost::dll::program_location();

		string executable_path = boost::dll::program_location().string();

		//#ifdef _WIN32
		//
		////    dladdr(&GetApplicationPath, &dlInfo);
		//    assert(false);
		//    //Get full path with decoration
		//    std::wstring appPath;
		//    //appPath.resize(MAX_PATH);
		//    // GetModuleFileNameW(NULL, &(appPath[0]), MAX_PATH);
		//    appPath.resize(wcslen(appPath.c_str()));
		//    appPath.shrink_to_fit();
		//
		//    //remove inutile decoration (ie .\)
		//    //char	szAbsolutePath[_MAX_PATH]={0};
		//    //if( PathCanonicalize(szAbsolutePath, appPath) )
		//    //appPath = szAbsolutePath;
		//    //std::string appPath2 = SimplifyFilePath(UTF8(appPath));
		//
		//    //assert(!appPath2.empty());
		//    //path = appPath2.substr(0, appPath2.find_last_of("\\/") + 1);
		//
		//    app_path = filesystem::canonical(appPath);
		//    app_path = app_path.parent_path();
		//
		//#else
		//
		//    dladdr(&X, &dlInfo);
		//    app_path = filesystem::canonical("/proc/self/exe");
		//    app_path = app_path.parent_path();
		//    //path = p.string();
		//#endif


		return executable_path;
	}

	bool is_file_path(const std::string& path_str)
	{
		std::filesystem::path p(path_str);

		// Check if the path exists
		if (!std::filesystem::exists(p))
		{
			return false; // Path does not exist
		}

		// Check if it's a regular file
		if (std::filesystem::is_regular_file(p))
		{
			return true;
		}

		// Check if it's a directory (optional, depending on your definition of "file path")
		if (std::filesystem::is_directory(p))
		{
			// A directory is a valid path, but not a "file path" in the strict sense.
			// You might return false here if you only want to identify regular files.
			return false;
		}

		// Handle other types like symbolic links, etc., if needed
		return false; // Not a regular file or directory
	}

	ERMsg GetFileInfo(const std::string& file_path, CFileInfo& info)
	{
		ERMsg msg;
		info = GetFileInfo(file_path);

		if (!info.is_init())
			msg.ajoute("Unable to get file information for: " + file_path);


		return msg;
	}



	CFileInfo GetFileInfo(const std::string& file_path_in)
	{
		CFileInfo info;


		boost::filesystem::path file_path = file_path_in;
		//time_t curTime = time(NULL);
		if (boost::filesystem::exists(file_path) && boost::filesystem::is_regular_file(file_path))
		{
			info.m_filePath = file_path_in;
			info.m_time = boost::filesystem::last_write_time(file_path);
			info.m_size = boost::filesystem::file_size(file_path);
		}

		return info;
	}

	std::time_t GetFileStamp(const std::string& filePath)
	{
		std::time_t fileStamp = 0;

		CFileInfo info = GetFileInfo(filePath);
		//if (GetFileInfo(filePath, info))
		fileStamp = info.m_time;

		return fileStamp;
	}

	CFileInfoVector GetFilesInfo(const std::vector<std::string>& files_list)
	{
		CFileInfoVector files_info(files_list.size());

		for (size_t i = 0; i < files_list.size(); i++)
		{
			files_info[i] = GetFileInfo(files_list[i]);
		}

		return files_info;
	}

	template<typename DirectoryIterator>
	std::vector<std::string> iter_dir(DirectoryIterator it, const std::string& filter)
	{
		std::vector<std::string> files_list;


		for (const auto& entry : it)
		{
			// Check if the file matches the extension filter
			if (boost::filesystem::is_regular_file(entry.path()) && entry.path().extension() == filter)
			{
				files_list.push_back(entry.path().filename().string());
			}
		}

		return files_list;
	}


	std::vector<std::string> GetFilesList(const std::string& directory_path, const std::string& filter, bool bSubDirSearch)
	{
		if (bSubDirSearch)
			return iter_dir(boost::filesystem::recursive_directory_iterator(directory_path), filter);



		return iter_dir(boost::filesystem::directory_iterator(directory_path), filter);
	}

	//
	//	std::vector<std::string> GetDirectoriesList(const std::string& filter)
	//	{
	//
	//		//std::string filter = filterIn;
	//		//while (IsPathEndOk(filter))
	//			//filter = filter.substr(0, filter.length() - 1);
	//
	//		std::vector<std::string> dirList;
	//
	//		WIN32_FIND_DATA ffd;
	//		HANDLE hFind = FindFirstFileExW(UTF16(filter).c_str(), FindExInfoBasic, &ffd, FindExSearchLimitToDirectories, NULL, FIND_FIRST_EX_LARGE_FETCH);
	//
	//		if (hFind != INVALID_HANDLE_VALUE)
	//		{
	//			do
	//			{
	//				string tmp = UTF8(ffd.cFileName);
	//				bool bDirectory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	//
	//				if (bDirectory && tmp != "." && tmp != "..")
	//					dirList.push_back(tmp);
	//
	//			} while (FindNextFile(hFind, &ffd) != 0);
	//
	//
	//			FindClose(hFind);
	//		}
	//
	//
	//		return dirList;
	//	}
	//
	//	std::string OpenFileName(const char *filter, HWND owner = NULL)
	//	{
	//		OPENFILENAMEW ofn;
	//
	//		std::wstring wfilter = UTF16(filter);
	//		std::replace(wfilter.begin(), wfilter.end(), '|', '\0');
	//		std::wstring wfileName; wfileName.resize(MAX_PATH);
	//
	//		ZeroMemory(&ofn, sizeof(ofn));
	//		ofn.lStructSize = sizeof(OPENFILENAME);
	//		ofn.hwndOwner = owner;
	//		ofn.lpstrFilter = &(wfilter[0]);
	//		ofn.lpstrFile = &(wfileName[0]);
	//		ofn.nMaxFile = MAX_PATH;
	//		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//		ofn.lpstrDefExt = L"";
	//		ofn.lpstrInitialDir = L"";
	//
	//		std::string fileNameStr;
	//		if (GetOpenFileNameW(&ofn))
	//			fileNameStr = UTF8(wfileName);
	//
	//		return fileNameStr;
	//	}
	//
	//	ERMsg AskToFindApplication(std::string appType, std::string appFilePath, HWND hCaller)
	//	{
	//		ERMsg msg;
	//
	//		//assert(hCaller != NULL);
	//
	//		std::string title = GetFileName(GetApplicationPath());
	//		std::string appName = GetFileName(appFilePath);
	//		std::string question = FormatMsg(IDS_BSC_APP_NOTFOUND, appName);
	//
	//		if (MessageBoxA(hCaller, question.c_str(), title.c_str(), MB_YESNO) == IDYES)
	//		{
	//			std::string appFilePath = OpenFileName(GetString(IDS_STR_FILTER_EXECUTABLE).c_str(), hCaller);
	//
	//			if (!appFilePath.empty())
	//			{
	//				CRegistry registry;
	//				registry.SetAppFilePath(appType, appFilePath);
	//			}
	//			else
	//			{
	//				msg.ajoute(FormatMsg(IDS_BSC_APP_NOTEXEC, appFilePath));
	//			}
	//		}
	//		else
	//		{
	//			msg.ajoute(FormatMsg(IDS_BSC_APP_NOTEXEC, appFilePath));
	//		}
	//
	//		return msg;
	//	}
	//
	//	ERMsg CallApplication(std::string appType, std::string argument, HWND hCaller, int showMode, bool bAddCote, bool bWait)
	//	{
	//		ERMsg msg;
	//
	//		CRegistry registry;
	//
	//		std::string appFilePath = registry.GetAppFilePath(appType);
	//
	//		if (GetPath(appFilePath).empty())//if the path is not specified, take the current application path.
	//			appFilePath = GetApplicationPath() + appFilePath;
	//
	//		if (bAddCote)
	//			argument = '\"' + argument + '\"';
	//
	//		std::string command = '\"' + appFilePath + "\" " + argument;
	//
	//		if (bWait)
	//		{
	//			msg = WinExecWait(command.c_str(), "", showMode);
	//		}
	//		else
	//		{
	//			if (WinExec(command.c_str(), showMode) < 31)
	//			{
	//				msg.ajoute(FormatMsg(IDS_BSC_APP_NOTEXEC, appFilePath));
	//			}
	//		}
	//
	//		if (!msg)
	//		{
	//			msg = AskToFindApplication(appType, appFilePath, hCaller);
	//			if (msg)
	//			{
	//				appFilePath = registry.GetAppFilePath(appType);
	//
	//				if (GetPath(appFilePath).empty())//if the path is not specified, take the current application path.
	//					appFilePath = GetApplicationPath() + appFilePath;
	//
	//				command = '\"' + appFilePath + "\" " + argument;
	//
	//				if (bWait)
	//				{
	//					msg = WinExecWait(command, "", showMode);
	//				}
	//				else
	//				{
	//					if (WinExec(command.c_str(), showMode) < 31)
	//					{
	//						msg.ajoute(FormatMsg(IDS_BSC_APP_NOTEXEC, appFilePath));
	//					}
	//				}
	//			}
	//		}
	//
	//		return msg;
	//	}
	//
	//
#ifdef _WIND32
	struct new_console : boost::process::extend::handler
	{
		template <typename Executor>
		void on_setup(Executor& ex)
		{
			// This template will only be instantiated for windows_executor
			// due to SFINAE or specific template specialization in Boost.Process.
			// On POSIX systems, this method will not be called for the handler.
			if constexpr (std::is_same_v<Executor, boost::process::extend::windows_executor<typename Executor::char_type, typename Executor::sequence_type>>)
			{

				ex.creation_flags |= CREATE_NEW_CONSOLE;

			}
		}
	};

#endif


	ERMsg WinExecWait(const std::string& exe_path, const std::string& arg, std::string working_dir, bool bShow, int* pExitCode)
	{
		ERMsg msg;


		//#if defined(__linux__)
		//#if defined(_MSC_VER)
		//#if defined(_WIN32)



				//auto l = boost_process::windows::default_launcher();
				//boost::asio::io_context ctx;


				// forward both stderr & stdout to stdout of the parent process
				//boost::process::v1::proce proc(ctx, exe_path.c_str(), arg.c_str());//, boost_process::process_stdio{{ /* in to default */}, stdout, stdout});
				//proc.wait();

		namespace bp = boost::process::v1;



		bp::filesystem::path the_path = bp::search_path(exe_path);


		try
		{

			std::wstring w_working_dir = bp::filesystem::path(working_dir).wstring();
			if (w_working_dir.empty())
				w_working_dir = bp::filesystem::current_path().wstring();

			// Convert UTF-8 to UTF-16 (std::wstring)
			// stange way to convert but it works
			std::wstring w_arg = std::filesystem::path((const char8_t*)arg.c_str()).wstring();

			bp::ipstream output;
			std::wstring command = the_path.wstring() + L" " + w_arg;


			boost::process::v1::child child;

			if (bShow)
			{
				child = boost::process::v1::child(command, bp::start_dir(w_working_dir));

			}
			else
			{
				child = boost::process::v1::child(command, bp::start_dir(w_working_dir), bp::std_out > output);
			}


			child.wait();
			int exitCode = child.exit_code();
			if (exitCode != 0)
			{
				msg.ajoute(std::string("Unable to execute command: ") + the_path.string() + " " + arg);
			}


			if (pExitCode)
				*pExitCode = exitCode;
		}
		catch (const bp::process_error& e)
		{
			//cout << e..what();
			msg.ajoute(std::string("Unable to execute command: ") + the_path.string() + " " + arg);
			msg.ajoute(string("Boost.Process Error: ") + e.what());
			//std::cerr << "Error Code: " << e.code().value() << std::endl;
			//std::cerr << "Error Category: " << e.code().category().name() << std::endl;
		}
		catch (const std::exception& e)
		{
			msg.ajoute(std::string("Unable to execute command: ") + the_path.string() + " " + arg);
			msg.ajoute(e.what());
			//std::cerr << "General C++ Exception: " << e.what() << std::endl;
		}


		//bp::child c(bp::search_path(exe_path), arg, bp::start_dir(working_dir));
		// ... perform other tasks while the child process runs ...
		//c.wait(); // Wait for the child process to complete if needed


		//int result = bp::system("your_executable", "arg1", "arg2", bp::start_dir("/path/to/working/directory"));

		//namespace bp = boost::process;
		//using namespace boost::process;

		// Or you can get the exit code
		//int exit_code = proc.exit_code();
		// std::cout << "Exit code: " << exit_code << std::endl;

/* #else

		try
		{


			boost_process::ipstream pipe_stream;
			boost_process::child c;

			c = boost_process::child(exe_path, arg, boost_process::std_out > pipe_stream, boost_process::start_dir(working_dir));


			std::string line;

			c.wait();

			if (pExitCode)
				*pExitCode = c.exit_code();
		}
		catch (const boost_process::process_error& e)
		{
			//cout << e..what();
			msg.ajoute(std::string("Unable to execute command: ") + exe_path);
			msg.ajoute(string("Boost.Process Error: ") + e.what());
			//std::cerr << "Error Code: " << e.code().value() << std::endl;
			//std::cerr << "Error Category: " << e.code().category().name() << std::endl;
		}
		catch (const std::exception& e)
		{
			msg.ajoute(std::string("Unable to execute command: ") + exe_path);
			msg.ajoute(e.what());
			//std::cerr << "General C++ Exception: " << e.what() << std::endl;
		}


#endif

*/




//#if _MSC_VER
//    while (IsPathEndOk(inputDir))
//        inputDir = inputDir.substr(0, inputDir.length() - 1);
//
//    STARTUPINFOA si = { 0 };
//    si.cb = sizeof(si);
//    si.dwFlags = STARTF_USESHOWWINDOW;
//    si.wShowWindow = bShow?SW_SHOW:SW_HIDE;
//
//    PROCESS_INFORMATION pi = { 0 };
//
//    //std::wstring wdir(UTF16(inputDir));
//    //std::wstring wcommand = UTF16(command);
//    LPCSTR pDir = inputDir.empty() ? NULL : inputDir.c_str();
//
//    if (::CreateProcessA(NULL, const_cast<LPSTR>(command.c_str()), NULL, NULL, FALSE, NULL, NULL, pDir, &si, &pi))
//    {
//        ::CloseHandle(pi.hThread);
//        ::WaitForSingleObject(pi.hProcess, INFINITE);
//
//        if (pExitCode != NULL)
//        {
//            // DWORD E=0;
//            //::GetExitCodeProcess(pi.hProcess, &E);
//        }
//    }
//    else
//    {
//        //msg = GetLastErrorMessage();
//        msg.ajoute(std::string("Unable to execute command: ") + command);
//    }
//#else
//    system(command.c_str());
//#endif
//

		return msg;
	}

	//	HWND FindMyTopMostWindow()
	//	{
	//		DWORD dwProcID = GetCurrentProcessId();
	//		HWND hWnd = GetTopWindow(GetDesktopWindow());
	//		while (hWnd)
	//		{
	//			DWORD dwWndProcID = 0;
	//			GetWindowThreadProcessId(hWnd, &dwWndProcID);
	//			if (dwWndProcID == dwProcID)
	//				return hWnd;
	//			hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	//		}
	//		return NULL;
	//	}
	//
	//	bool SetClipboardText(const std::string& str)
	//	{
	//		bool bRep = false;
	//
	//		HWND hWnd = FindMyTopMostWindow();
	//		if (OpenClipboard(hWnd))
	//		{
	//			EmptyClipboard();
	//
	//			// Allouer de la mémoire relocalisable globale
	//			//-------------------------------------------
	//			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, str.length() + 1);
	//
	//			// Verrouiller le bloc afin d'obtenir un pointeur éloigné
	//			// sur cette mémoire
	//			char* pBuffer = (char*)GlobalLock(hMem);
	//
	//			// Copier la chaîne dans cette mémoire
	//			strcpy_s(pBuffer, str.length() + 1, str.c_str());
	//
	//			// Relâche la mémoire et copier sur le clipboard
	//			GlobalUnlock(hMem);
	//
	//			SetClipboardData(CF_TEXT, hMem);
	//
	//
	//			bRep = CloseClipboard() != FALSE;
	//		}
	//
	//		return bRep;
	//	}
	//
	//	std::string GetClipboardText()
	//	{
	//		std::string str;
	//
	//		HWND hWnd = FindMyTopMostWindow();
	//		if (OpenClipboard(hWnd))
	//		{
	//			// Mettre la main sur le bloc de mémoire
	//			// référé par le texte
	//			HGLOBAL hMem = GetClipboardData(CF_TEXT);
	//
	//			char* pBuffer = (char*)GlobalLock(hMem);
	//			if (pBuffer)
	//			{
	//				// Verrouiller la mémoiure du Clipboard qu'on puisse reférer
	//				// à la chaîne actuelle
	//				str = pBuffer;
	//				GlobalUnlock(hMem);
	//			}
	//		}
	//
	//		CloseClipboard();
	//
	//		return str;
	//	}
	//
	int GetCrc32(const std::string& str, uint64_t begin, uint64_t end)
	{
		assert(begin <= end);
		boost::crc_32_type result;

		if (begin < str.size() && end < str.size() && begin < end)
		{
			result.process_bytes(&(str.at((uint64_t)begin)), uint64_t(end - begin));
		}
		else
		{
			result.process_bytes(str.data(), str.length());
		}

		return result.checksum();
	}

	int GetEndNumber(std::string name)
	{
		int number = 0;
		if (!name.empty())
		{
			std::string::size_type posBeg = name.find_last_not_of("0123456789");
			std::string::size_type posEnd = name.find_last_of("0123456789");
			if (posBeg != std::string::npos && posEnd != std::string::npos && name[posBeg] == ' ' && posEnd == name.length() - 1)
			{
				number = std::stoi(name.substr(posBeg + 1, posEnd - posBeg));
			}
		}

		return number;
	}

	std::string GenerateNewName(std::string name)
	{
		if (!name.empty())
		{
			int no = GetEndNumber(name);
			if (no == 0)
			{
				name += " 2";
			}
			else
			{
				std::string noStr = to_string(no);
				std::string::size_type pos = name.rfind(noStr);
				assert(pos != std::string::npos);
				name = name.substr(0, pos) + to_string(no + 1);
			}
		}

		return name;
	}

	std::string GenerateNewFileName(std::string name)
	{
		if (!name.empty())
		{
			while (WBSF::FileExists(name))
			{
				std::string title = WBSF::GetFileTitle(name);
				title = GenerateNewName(title);
				WBSF::SetFileTitle(name, title);
			}

		}

		return name;
	}

	ERMsg CreateMultipleDir(const std::string& path)
	{
		ERMsg msg;

		if (path.empty())
			return msg;


		try
		{

			std::string tmp(path);
			while (IsPathEndOk(tmp))//remove \ and //
				tmp = tmp.substr(0, tmp.length() - 1);

			//if (tmp != "c:" && tmp != "C:" && tmp.size() > 2)
			//{
			if (!DirectoryExists(path))
			{
				//std::string::size_type pos = tmp.find_last_of("\\/");
				//if (pos != std::string::npos)
				//{
					//std::string subDir = tmp.substr(0, pos);
					//msg = CreateMultipleDir(subDir);
					//if (msg)
					//{
						//can't create directory when a file without extension with the same name exist!

				if (!boost::filesystem::create_directories(path))
				{
					//DWORD dw = GetLastError();
					//if (dw != ERROR_ALREADY_EXISTS)
					//{
					//	msg = GetLastErrorMessage();
					//	return msg;
					//}
					msg.ajoute( "Directories '" + path + "' could not be created." );
				
				}
				//}
			//}
			}
			//}
		}
		catch (const boost::filesystem::filesystem_error& ex) 
		{
			msg.ajoute(string("File system error: ") + ex.what() );
		}

		return msg;
	}

	size_t GetTotalSystemMemory()
	{
		size_t total = 0;

#ifdef _MSC_VER
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx(&status);
		total = status.ullTotalPhys;
#else

		assert(false);//todo
#endif


		return total;
	}

	//	ERMsg GetLastErrorMessage()
	//	{
	//		ERMsg msg;
	//		LPSTR lpMsgBuf = NULL;
	//
	//		DWORD dw = GetLastError();
	//
	//		FormatMessageA(
	//			FORMAT_MESSAGE_ALLOCATE_BUFFER |
	//			FORMAT_MESSAGE_FROM_SYSTEM |
	//			FORMAT_MESSAGE_IGNORE_INSERTS,
	//			NULL,
	//			dw,
	//			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	//			(LPSTR)&lpMsgBuf,
	//			0, NULL);
	//
	//		// Display the error message and clean up
	//		std::string str = "Failed with error " + std::to_string(dw) + ": " + lpMsgBuf;
	//		msg.ajoute(str);
	//
	//		LocalFree(lpMsgBuf);
	//
	//		return msg;
	//	}
	//
	//
	//
	std::string SecondToDHMS(double time)
	{
		int d = int(time / (24 * 60 * 60));
		time -= d * 24 * 60 * 60;
		int h = int(time / (60 * 60));
		time -= h * 60 * 60;
		int m = int(time / 60);
		time -= m * 60;
		double s = time;

		std::string str;
		str.resize(100);

		if (d > 0)
			snprintf(&(str[0]), 100, "%d, %02d:%02d:%02.3lf", d, h, m, s);
		else if (h > 0 || d > 0)
			snprintf(&(str[0]), 100, "%02d:%02d:%02.3lf", h, m, s);
		else if (m > 0 || h > 0 || d > 0)
			snprintf(&(str[0]), 100, "%02d:%02.3lf", m, s);
		else snprintf(&(str[0]), 100, "%02.3lf", s);
		str.resize(strlen(str.c_str()));
		return str;
	}


	std::string GetTempPath()
	{
		//DWORD size = ::GetTempPathW(0, NULL);
	//
		//std::wstring tmp;
		//tmp.resize(size);
		//::GetTempPathW(size, &(tmp[0]));
		////tmp.ReleaseBuffer();
	//
		//return UTF8(tmp);
		assert(false);
		return "";
	}

	std::string GetUserDataPath()
	{
		//std::wstring path;
		//path.insert(path.begin(), MAX_PATH, 0);
		//HRESULT result = SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, &(path[0]));
		//path.resize(wcslen(path.c_str()));
		//path.shrink_to_fit();
	//
		//if (result == S_OK)
		//	path += L"\\NRCan\\";
		//else
		//	path = UTF16(GetTempPath());
	//
		//return UTF8(path);
		assert(false);
		return "";
	}

	//
	//
	//
	//	std::string GetVersionString(const std::string& filerPath)
	//	{
	//		std::string version;
	//
	//		DWORD dwDummy;
	//		DWORD dwFVISize = GetFileVersionInfoSizeW(UTF16(filerPath).c_str(), &dwDummy);
	//		if (dwFVISize > 0)
	//		{
	//			//LPBYTE lpVersionInfo = new BYTE[dwFVISize];
	//			std::wstring tmp;
	//			tmp.resize(dwFVISize);
	//			VERIFY(GetFileVersionInfoW(UTF16(filerPath).c_str(), 0, dwFVISize, &(tmp[0])));
	//
	//			UINT uLen = 0;
	//			VS_FIXEDFILEINFO *lpFfi = NULL;
	//			VerQueryValueW(tmp.data(), L"\\", (LPVOID *)&lpFfi, &uLen);
	//			DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
	//			DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;
	//			//delete [] lpVersionInfo;
	//
	//			DWORD dwLeftMost = HIWORD(dwFileVersionMS);
	//			DWORD dwSecondLeft = LOWORD(dwFileVersionMS);
	//			DWORD dwSecondRight = HIWORD(dwFileVersionLS);
	//			DWORD dwRightMost = LOWORD(dwFileVersionLS);
	//
	//			version = to_string(dwLeftMost) + "." + to_string(dwSecondLeft) + "." + to_string(dwSecondRight) + "." + to_string(dwRightMost);
	//		}
	//
	//		return version;
	//	}
	//
	//	std::string GetCompilationDateString(char *compilation_date)
	//	{
	//		std::string str;
	//
	//		char *months[12] = { "Jan","Feb","Mar","Apr","May","Jun",
	//							"Jul","Aug","Sep","Oct","Nov","Dec" };
	//
	//
	//		int month = -1;
	//		for (int i = 0; i < 12; i++)
	//		{
	//			if (memcmp(compilation_date, months[i], 3) == 0)
	//			{
	//				month = i;
	//				break;
	//			}
	//		}
	//
	//		if (month != -1)
	//		{
	//			char year[5] = { 0 };
	//			char day[3] = { 0 };
	//			char hour[3] = { 0 };
	//			char minute[3] = { 0 };
	//			char second[3] = { 0 };
	//
	//			memcpy(year, compilation_date + 7, 4);
	//			memcpy(day, compilation_date + 4, 2);
	//
	//			//CTime time(ToInt(year), month+1, ToInt(day), 12,0,0 );
	//			str = FormatTime("%x", ToInt(year), month, ToSizeT(day) - 1);
	//		}
	//
	//		return str;
	//	}

	struct StringComparator
	{
		StringComparator(const std::string& nameToFind) : m_str(nameToFind) {}
		~StringComparator() {}

		bool operator () (const std::string& str) const
		{
			return boost::iequals(str, m_str);
		}

		const std::string& m_str;
	};

	std::vector<std::string>::const_iterator FindStringExact(const std::vector<std::string>& list, const std::string& value, bool bCaseSensitive)
	{
		if (!bCaseSensitive)
			return find_if(list.begin(), list.end(), StringComparator(value));

		return find(list.begin(), list.end(), value);
	}

	//
	//
	//	std::string GetText(ERMsg msg)
	//	{
	//		std::string messStr;
	//
	//		static CCriticalSection CS;
	//
	//		CS.Enter();
	//		for (unsigned int i = 0; i < msg.dimension(); i++)
	//		{
	//			if (!messStr.empty())
	//				messStr += "\n";
	//
	//			messStr += msg[i].data();
	//		}
	//		CS.Leave();
	//
	//		return messStr;
	//	}
	//
	//
	//
	std::string ReplaceString(const std::string& str_in, const std::string& oldStr, const std::string& newStr)
	{
		string str(str_in);
		//int nbReplace = 0;
		size_t pos = 0;
		while ((pos = str.find(oldStr, pos)) != std::string::npos)
		{
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
			//nbReplace++;
		}

		return str;
	}

	//	std::string RemoveAccented(std::string str)
	//	{
	//		for (string::iterator it = str.begin(); it != str.end(); it++)
	//		{
	//			static const char*
	//				//   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
	//				//tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
	//				tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnoooooouuuuypy";
	//			unsigned char ch = *it;
	//			if (ch >= 192)
	//				*it = tr[ch - 192];
	//
	//			//++p; // http://stackoverflow.com/questions/14094621/
	//		}
	//		//ÿ
	//		return str;
	//	}
	//
	std::string PurgeFileName(std::string name)
	{
		std::replace(name.begin(), name.end(), '\\', '-');
		std::replace(name.begin(), name.end(), '/', '-');
		std::replace(name.begin(), name.end(), '\"', '-');
		std::replace(name.begin(), name.end(), ':', '-');
		std::replace(name.begin(), name.end(), '*', '-');
		std::replace(name.begin(), name.end(), '?', '-');
		std::replace(name.begin(), name.end(), '<', '-');
		std::replace(name.begin(), name.end(), '>', '-');
		std::replace(name.begin(), name.end(), '|', '-');
		std::replace(name.begin(), name.end(), '\t', ' ');
		std::replace(name.begin(), name.end(), '.', ' ');
		//, is not a problem for file name but is problem in CSV file
		std::replace(name.begin(), name.end(), ',', '-');
		std::replace(name.begin(), name.end(), ';', '-');
		std::replace(name.begin(), name.end(), '\"', ' ');
		std::replace(name.begin(), name.end(), '\'', ' ');
		name = ReplaceString(name, "œ", "oe");

		name = WBSF::Trim(name);

		return name;
	}

	//	string ANSI_2_ASCII(std::string str)
	//	{
	//		int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	//
	//		std::wstring w_text;
	//		w_text.resize(len);
	//		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &(w_text[0]), len);
	//
	//		//Convert UTF16 to ASCII encoding
	//		static const UINT US_ASCII = 20127;
	//		int newLen = WideCharToMultiByte(US_ASCII, 0, w_text.c_str(), -1, NULL, 0, 0, 0);
	//
	//		str.resize(newLen);
	//		WideCharToMultiByte(US_ASCII, 0, w_text.c_str(), -1, &(str[0]), newLen, 0, 0);
	//		str.resize(strlen(str.c_str()));
	//
	//		return str;
	//	}
	//
	//	//typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> CONVERT_STRING_DEF;
	//	//static CONVERT_STRING_DEF CONVERT_STRING;
	//
	//	std::wstring UTF8_UTF16(const std::string& str)
	//	{
	//		std::wstring out;
	//
	//		try
	//		{
	//			//typedef std::codecvt_utf8<wchar_t> convert_typeX;
	//			//std::wstring_convert<convert_typeX, wchar_t> converterX;
	//
	//			//out = converterX.from_bytes(str);
	//
	//			typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> CONVERT_STRING_DEF;
	//			CONVERT_STRING_DEF CONVERT_STRING;
	//			out = CONVERT_STRING.from_bytes(str);
	//			//out.resize(strlen(out.length()));
	//			out.resize(wcslen(out.c_str()));
	//			//out.shrink_to_fit();
	//		}
	//		catch (...)
	//		{
	//
	//		}
	//
	//		return out;
	//	}
	//
	//	std::string UTF16_UTF8(const std::wstring& str)
	//	{
	//
	//
	//		std::string out;
	//		try
	//		{
	//			//typedef std::codecvt_utf8<wchar_t> convert_typeX;
	//			//std::wstring_convert<convert_typeX, wchar_t> converterX;
	//
	//			//out = converterX.to_bytes(str);
	//
	//			typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> CONVERT_STRING_DEF;
	//			CONVERT_STRING_DEF CONVERT_STRING;
	//			out = CONVERT_STRING.to_bytes(str);
	//			//out.resize(out.length());
	//			out.resize(strlen(out.c_str()));
	//			//out.shrink_to_fit();
	//		}
	//		catch (...)
	//		{
	//			//int i;
	//			//i = 0;
	//			out = std::string(str.begin(), str.end());
	//		}
	//
	//		return out;
	//	}
	//
	//
	//
	//	// change a char's encoding from UTF8 to ANSI
	//	std::string UTF8_ANSI(const std::string& u8)
	//	{
	//		int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, u8.c_str(), (int)u8.length(), NULL, 0);
	//		std::wstring wszString;	wszString.resize(wcsLen);
	//		::MultiByteToWideChar(CP_UTF8, NULL, u8.c_str(), (int)u8.length(), ((wchar_t*)(&wszString[0])), wcsLen);
	//
	//		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString.c_str(), (int)wszString.length(), NULL, 0, NULL, NULL);
	//		std::string szAnsi;	szAnsi.resize(ansiLen);
	//		::WideCharToMultiByte(CP_ACP, NULL, wszString.c_str(), (int)wszString.length(), ((char*)(&szAnsi[0])), ansiLen, NULL, NULL);
	//
	//		return szAnsi;
	//	}
	//
	//	// change a char's encoding from ANSI to UTF8
	//	std::string ANSI_UTF8(const std::string& szAnsi)
	//	{
	//		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), (int)szAnsi.length(), NULL, 0);
	//		std::wstring wszString;	wszString.resize(wcsLen);
	//		::MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), (int)szAnsi.length(), ((wchar_t*)(&wszString[0])), wcsLen);
	//
	//		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString.c_str(), (int)wszString.length(), NULL, 0, NULL, NULL);
	//		std::string szU8; szU8.resize(u8Len);
	//		::WideCharToMultiByte(CP_UTF8, NULL, wszString.c_str(), (int)wszString.length(), ((char*)(&szU8[0])), u8Len, NULL, NULL);
	//
	//		return szU8;
	//	}
	//
	//	std::string Encrypt(const std::string& str, const std::string& key)
	//	{
	//		std::string strOut;
	//		if (!str.empty())
	//		{
	//			try
	//			{
	//				int l = int(ceil((double)str.length() / key.length())*key.length() + 1);
	//
	//				std::string init;
	//				for (int i = 0; i < l; i++)
	//					init += '\0';
	//
	//
	//				std::CRijndael oRijndael;
	//				oRijndael.MakeKey(key.c_str(), init.c_str(), int(key.length()), int(key.length()));
	//
	//				strOut.resize(l);
	//				oRijndael.EncryptBlock(str.c_str(), &(strOut[0]));
	//				assert(strOut.size() == l);
	//				strOut.resize(strlen(strOut.c_str()));
	//			}
	//			catch (...)
	//			{
	//
	//			}
	//		}
	//
	//		return strOut;
	//	}
	//
	//
	//
	//	std::string Decrypt(const std::string& str, const std::string& key)
	//	{
	//		std::string strOut;
	//		if (!str.empty())
	//		{
	//			try
	//			{
	//				int l = int(ceil((double)str.length() / key.length())*key.length() + 1);
	//
	//				std::string init;
	//				for (int i = 0; i < l; i++)
	//					init += '\0';
	//
	//				std::CRijndael oRijndael;
	//				oRijndael.MakeKey(key.c_str(), init.c_str(), int(key.length()), int(key.length()));
	//				strOut.resize(l);
	//				oRijndael.DecryptBlock(str.c_str(), &(strOut[0]));
	//				strOut.resize(strlen(strOut.c_str()));
	//			}
	//			catch (...)
	//			{
	//
	//			}
	//			//strOut.ReleaseBuffer();
	//		}
	//
	//		return strOut;
	//	}
	//
	//
	std::string& UppercaseFirstLetter(std::string& str)
	{

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		bool cap_it = true;
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] < 0 || isalpha(str[i]) || isdigit(str[i]))
			{
				if (cap_it == true)
				{
					str[i] = toupper(str[i]);
					cap_it = false;
				}
			}
			else cap_it = true;
			//if (isspace(str[i]))
		}

		return str;

	}


	//	//std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters, bool bEliminateDuplicateSep, std::string::size_type begin, std::string::size_type end)
	//	//{
	//	//	assert(begin<=end);
	//	//	//assert(begin<str.size());
	//
	//	//	std::vector<std::string> tokens;
	//	//	if( end==std::string::npos )
	//	//		end=str.size();
	//
	//	//	std::string::size_type pos = begin;
	//
	//
	//	//	if(str.length()<1)
	//	//		return tokens;
	//
	//	//	while(pos<end)
	//	//	{
	//	//		std::string::size_type delimPos = std::min(end, str.find_first_of(delimiters, pos));
	//	//		std::string::size_type tokenPos = std::min(end, str.find_first_not_of(delimiters, pos));
	//
	//	//		//if(end != delimPos)
	//	//		//{
	//	//		if(end != tokenPos)
	//	//		{
	//	//			if(tokenPos<delimPos)
	//	//			{
	//	//				tokens.push_back(str.substr(pos,delimPos-pos));
	//	//			}
	//	//			else
	//	//			{
	//	//				tokens.push_back("");
	//	//			}
	//	//		}
	//	//		else
	//	//		{
	//	//			tokens.push_back("");
	//	//		}
	//
	//	//		pos = delimPos+1;
	//	//
	//	//		if( bEliminateDuplicateSep )
	//	//		{
	//	//			assert(str.length()==str.size());
	//	//			while( pos<str.length() && delimiters.find(str[pos])!=std::string::npos )
	//	//				pos++;
	//	//		}
	//	//	//}
	//	//	//else
	//	//	//{
	//	//	//	if(end != tokenPos)
	//	//	//	{
	//	//	//		if(tokenPos<delimPos)
	//	//	//		{
	//	//	//			tokens.push_back(str.substr(pos, delimPos-pos));
	//	//	//		}
	//	//	//	}
	//	//	//	else
	//	//	//	{
	//	//	//		//tokens.push_back("");
	//	//	//	}
	//	//	//	break;
	//	//	//}
	//	//	}
	//
	//	//	return tokens;
	//	//}
	//
	//	std::string GetString(UINT ID) { return LoadString(ID); }
	//
	//	std::string to_stringDMS(double coord, bool bWithFraction)
	//	{
	//		std::string deg;
	//		std::string min;
	//		std::string sec;
	//
	//		std::string str;
	//
	//		int prec = 0;
	//		if (bWithFraction)
	//			prec = 2;
	//
	//		double mult = pow(10.0, prec);
	//		deg = to_string(GetDegrees(coord, mult));
	//		min = to_string(GetMinutes(coord, mult));
	//		sec = to_string(GetSeconds(coord, mult), prec);
	//
	//		if (sec == "0" || sec == "-0")
	//			sec.clear();
	//		if (sec.empty() && (min == "0" || min == "-0"))
	//			min.clear();
	//
	//
	//		return deg + min + sec;
	//	}
	//
	//	//std::string to_string(const CGeoRectIndex& rect )
	//	//{
	//	//
	//	//	return to_string(rect.top) + " " + to_string(rect.left) + " " + to_string( rect.bottom) + " " + to_string(rect.right);
	//	//}
	//
	//	//CGeoRectIndex ToCRect(const std::string& str)
	//	//{
	//	//	CGeoRectIndex rect;
	//
	//	//	sscanf_s( str.c_str(), "%d %d %d %d", &rect.top, &rect.left, &rect.bottom, &rect.right);
	//
	//	//	return rect;
	//	//}
	//
	//	COLORREF ToCOLORREF(const std::string& str)
	//	{
	//		int r = 0;
	//		int g = 0;
	//		int b = 0;
	//		sscanf_s(str.c_str(), "%d %d %d", &r, &g, &b);
	//
	//		return RGB(r, g, b);
	//	}
	//
	//
	//	void ToVector(const std::string& str, std::vector<std::string>& val, const std::string& sep)
	//	{
	//		val = Tokenize(str, sep);
	//	}
	//
	typedef boost::iterator_range<std::string::const_iterator> StringRange;


	std::string FindString(const std::string& source, const std::string& strBegin, const std::string& strEnd, std::string::size_type& posBegin, std::string::size_type& posEnd)
	{
		std::string text;
		posBegin = source.find(strBegin, posEnd);

		if (posBegin != std::string::npos)
		{
			posBegin += strBegin.length();
			posEnd = source.find(strEnd, posBegin);
			assert(posEnd != std::string::npos);
			if (posEnd != std::string::npos)
				text = source.substr(posBegin, posEnd - posBegin);
		}

		return text;
	}

	std::string FindString(const std::string& source, const std::string& strBegin, const std::string& strEnd, std::string::size_type& posBegin)
	{
		assert(posBegin != std::string::npos);

		std::string text;
		posBegin = source.find(strBegin, posBegin);

		if (posBegin != std::string::npos)
		{
			posBegin += strBegin.length();
			std::string::size_type posEnd = source.find(strEnd, posBegin);
			assert(posEnd != std::string::npos);
			if (posEnd != std::string::npos)
			{
				text = source.substr(posBegin, posEnd - posBegin);
				posBegin = posEnd;
			}
		}

		return text;
	}

	std::string FindString(const std::string& source, const std::string& strBegin, const std::string& strEnd)
	{
		std::string::size_type posBegin = 0;
		return FindString(source, strBegin, strEnd, posBegin);
	}

	static size_t FindSensitive(const std::string& str1, const std::string& str2)
	{
		auto it1 = StringRange(str1.begin(), str1.end());
		auto it2 = StringRange(str2.begin(), str2.end());
		size_t pos = std::distance(it1.begin(), boost::ifind_first(it1, it2).begin());
		return pos < str1.length() ? pos : string::npos;
	}

	static size_t FindInsensitive(const std::string& str1, const std::string& str2)
	{
		auto it1 = StringRange(str1.begin(), str1.end());
		auto it2 = StringRange(str2.begin(), str2.end());
		size_t pos = std::distance(it1.begin(), boost::ifind_first(it1, it2).begin());
		return pos < str1.length() ? pos : string::npos;
	}

	size_t Find(const std::string& str1, const std::string& str2, bool bCaseSensitive)
	{
		return bCaseSensitive ? FindSensitive(str1, str2) : FindInsensitive(str1, str2);
	}

	bool IsEqualNoCase(const std::string& in1, const std::string& in2)
	{
		return boost::iequals(in1, in2);
	}

	std::string Tokenize(const std::string& str, const std::string& delimiters, std::string::size_type& pos, bool bRemoveDuplicate, std::string::size_type posEnd)
	{
		assert(pos != std::string::npos);

		if (posEnd == std::string::npos)
			posEnd = str.length();

		if (bRemoveDuplicate)
			pos = str.find_first_not_of(delimiters, pos);

		std::string tmp;
		if (pos < posEnd)//str.length())
		{
			char const* p = str.c_str() + pos;

			char const* q = strpbrk(p, delimiters.c_str());
			if (q != NULL)
			{

				tmp = std::vector<std::string>::value_type(p, q);
				p = q + 1;
				pos = p - str.c_str();
			}
			else
			{
				tmp = str.substr(pos);
				pos = std::string::npos;
			}

			if (bRemoveDuplicate && pos != std::string::npos)
				pos = str.find_first_not_of(delimiters, pos);
		}
		else
		{
			pos = std::string::npos;
		}

		return tmp;
	}

	std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters, bool bRemoveDuplicate, std::string::size_type pos, std::string::size_type posEnd)
	{
		std::vector<std::string> v;

		if (!str.empty())
		{
			while (pos != std::string::npos)
			{
				std::string str2 = WBSF::Tokenize(str, delimiters, pos, bRemoveDuplicate, posEnd);
				if (!bRemoveDuplicate || !str2.empty())
					v.push_back(str2);
			}
		}

		return v;

	}



	//est-ce qu'il y a une différence de performence entre les deux
	typedef tokenizer<escaped_list_separator<char> > so_tokenizer;
	bool TokenizeWithQuote(const std::string& str, char sep, std::vector<std::string>& out)
	{
		try
		{
			out.clear();
			so_tokenizer tok(str, escaped_list_separator<char>('\\', sep, '\"'));
			for (so_tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
				out.push_back(*beg);
		}
		catch (...)
		{
			//        int i;
					//      i = 0;
		}

		return !out.empty();
	}

	bool TokenizeWithQuote(const std::string& str, char* sep, std::vector<std::string>& out)
	{

		try
		{
			out.clear();
			so_tokenizer tok(str, escaped_list_separator<char>("\\", sep, "\""));
			for (so_tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
				out.push_back(*beg);
		}
		catch (...)
		{
			//        int i;
					//      i = 0;
		}

		return !out.empty();
	}

	std::vector<std::string> TokenizeQuoted(std::string command, const std::string& delimiters)
	{
		std::vector<std::string> v;

		int len = int(command.length());
		bool qot = false, sqot = false;
		int arglen;
		for (int i = 0; i < len; i++)
		{
			int start = i;
			if (command[i] == '\"')
				qot = true;
			else if (command[i] == '\'')
				sqot = true;

			if (qot)
			{
				i++;
				start++;
				while (i < len && command[i] != '\"')
					i++;
				if (i < len)
					qot = false;
				arglen = i - start;
				i++;
			}
			else if (sqot)
			{
				i++;
				while (i < len && command[i] != '\'')
					i++;
				if (i < len)
					sqot = false;
				arglen = i - start;
				i++;
			}
			else
			{
				while (i < len && delimiters.find(command[i]) == string::npos)//command[i] != ' ' && command[i] != '-')
					i++;

				arglen = i - start;
			}

			v.push_back(command.substr(start, arglen));
		}

		//if (qot || sqot) std::cout << "One of the quotes is open\n";
		assert(!qot && !sqot);

		return v;
	}



	std::string FormatA(const char* szFormat, ...)
	{
		va_list argList;
		va_start(argList, szFormat);
		std::string strOut = FormatV(szFormat, argList);
		va_end(argList);
		return strOut;
	}

	std::string FormatV(const char* szFormat, va_list argList)
	{
		char* pBuf = NULL;
		int nChars = 1;
		int nUsed = 0;
		std::string::size_type nActual = 0;
		int nTry = 0;

		do
		{
			// Grow more than linearly (e.g. 512, 1536, 3072, etc)

			nChars += ((nTry + 1) * 2048);
			pBuf = reinterpret_cast<char*>(alloca(sizeof(char) * nChars));
			nUsed = vsnprintf(pBuf, nChars - 1, szFormat, argList);

			// Ensure proper NULL termination.

			nActual = nUsed == -1 ? nChars - 1 : std::min(nUsed, nChars - 1);
			pBuf[nActual + 1] = '\0';


		} while (nUsed < 0 && nTry++ < 5);

		// assign whatever we managed to format
		return std::string(pBuf, nActual);
	}

	//	std::string FormatMsgA(PCSTR szFormat, ...) //throw(std::exception)
	//	{
	//		std::string str;
	//		va_list argList;
	//		va_start(argList, szFormat);
	//		PSTR szTemp = NULL;
	//		if (FormatMessageA(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
	//			szFormat, 0, 0,
	//			reinterpret_cast<LPSTR>(&szTemp), 0, &argList) > 0 &&
	//			szTemp != NULL)
	//		{
	//			//throw std::runtime_error("out of memory");
	//			//}
	//			str = szTemp;
	//			LocalFree(szTemp);
	//			va_end(argList);
	//		}
	//
	//		return str;
	//	}
	//
	std::string FormatMsg(std::string_view szFormat, std::string_view v1, std::string_view v2, std::string_view v3, std::string_view v4, std::string_view v5)
	{
		std::ostringstream ss;

		if (!szFormat.empty() && szFormat.length() > 0)
		{
			try
			{
				int nbParams = (v1.empty() ? 0 : 1) + (v2.empty() ? 0 : 1) + (v3.empty() ? 0 : 1) + (v4.empty() ? 0 : 1) + (v5.empty() ? 0 : 1);
				switch (nbParams)
				{
				case 0: ss << szFormat; break;
				case 1: ss << boost::format(std::string(szFormat)) % v1; break;
				case 2: ss << boost::format(std::string(szFormat)) % v1 % v2; break;
				case 3: ss << boost::format(std::string(szFormat)) % v1 % v2 % v3; break;
				case 4: ss << boost::format(std::string(szFormat)) % v1 % v2 % v3 % v4; break;
				case 5: ss << boost::format(std::string(szFormat)) % v1 % v2 % v3 % v4 % v5; break;
				default: assert(false);  break;
				}
			}
			catch (const boost::io::format_error& e)
			{
				std::cerr << "Boost Format Error: " << e.what() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cerr << "Standard Exception: " << e.what() << std::endl;
			}
		}

		return ss.str();
	}

	void PrintMessage(ERMsg msg)
	{
		for (size_t i = 0; i < msg.dimension(); i++)
		{
			cout << msg[(int)i] << endl;
		}
	}

	//	//*************************************************************************
	//
	//	template<typename T, size_t N>
	//	T * my_end(T(&ra)[N]) {
	//		return ra + N;
	//	}
	//
	//	std::vector<std::string>::std::vector<std::string>(const char* str_array[])
	//	{
	//
	//		size_t size = sizeof(str_array) / sizeof(const char*);
	//		resize(size);
	//		for (size_t i = 0; i < size; i++)
	//			at(i) = str_array[i];
	//		//std::vector<std::string> tmp( str_array );
	//		//std::vector<std::string> tmp(str_array, my_end(str_array));
	//		//operator=();
	//	}
	//
	//	void std::vector<std::string>::LoadString(UINT ID, const std::string& delimiters)
	//	{
	//		std::string str = GetString(ID);
	//		Tokenize(str, delimiters, false);
	//	}
	//
	//

	size_t Find(const std::vector<std::string>& v, const std::string& str, bool bCaseSensitive, bool bExact)
	{
		std::set<size_t> cols = WBSF::FindAll(v, str, bCaseSensitive, bExact);

		return cols.empty() ? NOT_INIT : *cols.begin();
	}

	std::set<size_t> FindAll(const std::vector<std::string>& v, const std::string& str, bool bCaseSensitive, bool bExact)
	{
		std::set<size_t> posVector;

		for (size_t i = 0; i < v.size(); i++)
		{
			if (bExact)
			{
				if (IsEqual(str, v.at(i), bCaseSensitive))
					posVector.insert(i);
			}
			else
			{
				if (WBSF::Find(str, v.at(i), bCaseSensitive))
					posVector.insert(i);
			}
		}


		return posVector;
	}

	//	std::ostream& std::vector<std::string>::operator >> (std::ostream& stream)const
	//	{
	//		stream << "{";
	//		for (std::vector<std::string>::const_iterator it = begin(); it != end(); it++)
	//		{
	//			stream << *it;
	//			stream << "|";
	//		}
	//
	//		stream << "}";
	//
	//		return stream;
	//	}
	//
	//	std::istream& std::vector<std::string>::operator << (std::istream& stream)
	//	{
	//		std::string s;
	//		getline(stream, s, '}');
	//
	//		Tokenize(s, "{|}");
	//
	//		return stream;
	//	}
	//
	//
	//
	//	std::vector<std::string>& std::vector<std::string>::Tokenize(const std::string& str, const std::string& delimiters, bool bRemoveDuplicate, std::string::size_type pos, std::string::size_type posEnd)
	//	{
	//
	//		clear();
	//
	//		if (!str.empty())
	//		{
	//			while (pos != std::string::npos)
	//			{
	//				std::string str2 = WBSF::Tokenize(str, delimiters, pos, bRemoveDuplicate, posEnd);
	//				if (!bRemoveDuplicate || !str2.empty())
	//					push_back(str2);
	//			}
	//		}
	//
	//		return *this;
	//
	//	}
	//

	//	std::string std::vector<std::string>::to_string(const char* sep)const
	//	{
	//		std::string str;
	//		for (const_iterator it = begin(); it != end(); it++)
	//		{
	//			if (it != begin())
	//				str += sep;
	//
	//			str += to_string(*it);
	//		}
	//
	//		return str;
	//	}
	//

}//namespace WBSF

