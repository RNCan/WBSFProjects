//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
// 19-03-2021	Rémi Saint-Amant	Bug correction in reading buffer
// 01-10-2020	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************



#include "WBSFconfig.h"
//#include <boost/process/v1/child.hpp>
//#include <boost/process/v1/io.hpp>

#include <boost/asio.hpp> // Or boost::process.hpp
#include <boost/process.hpp>

#include "Basic/UtilStd.h"
#include "CallcURL.h"


using namespace std;

namespace WBSF
{

CCallcURL::CCallcURL(const std::string& exe_filepath, size_t bufsize)
{

    m_exe_filepath = exe_filepath;
    m_bufsize = bufsize;
    m_timeout = 0;

    if (m_exe_filepath.empty())
        m_exe_filepath = GetApplicationPath() += "curl.exe";
}




ERMsg CCallcURL::get_text(const std::string& arg, std::string& str_out)
{
    ERMsg msg;

    str_out.clear();

    string cmdline = "\"" + m_exe_filepath + "\" " + arg;
    msg = CallApp(cmdline, str_out, m_bufsize);

    return msg;
}


ERMsg CCallcURL::copy_file(const std::string& URL, const std::string& output_filepath, bool bShowCurl)
{
    ERMsg msg;

    string argument = string(bShowCurl ? "" : "-s ") + "-k --ssl-no-revoke ";
    if (m_timeout > 0)
        argument += "--connect-timeout " + to_string(m_timeout) + " ";

    //argument += "\"" + URL + "\" --output \"" + output_filepath + "\"";
    argument += URL + " --output " + output_filepath;

    


    int exit_code;
    msg = WinExecWait(m_exe_filepath, argument, "", bShowCurl, &exit_code);
    if (exit_code != 0 && !FileExists(output_filepath))
    {

        msg.ajoute("Unable to download file:");
        msg.ajoute(output_filepath);
    }

    return msg;
}

ERMsg CCallcURL::CallApp(const std::string& cmdline, std::string& str, size_t BUFSIZE)
{
    ERMsg msg;
    //
    //
    ////see https://www.boost.org/doc/libs/1_86_0/doc/html/process.html
    //assert(false);//todo
    //
    //
    //namespace bp = boost::process; //we will assume this for all further examples
    //
    ////filesystem::path p = "/usr/bin/g++"; //or get it from somewhere else.
    ////int result = bp::system(p, "main.cpp");
    //
    //boost::filesystem::path exe_path = bp::search_path("curl"); //or get it from somewhere else.
    ////int result = bp::system(p, cmdline);
    //
    //bp::child child_proc(exe_path, cmdline);
    //
    //while (child_proc.running())
    //{
    //    //do_some_stuff();
    //}
    //
    //child_proc.wait(); //wait for the process to exit
    //int result = child_proc.exit_code();
    //if (result != 0 )
    //{
    //
    //    msg.ajoute("Unable to execute curl command:");
    //    msg.ajoute(cmdline);
    //}
    //
    //return msg;


    //try
    //{
    //    // while (IsPathEndOk(working_dir))
    //      //   working_dir = working_dir.substr(0, working_dir.length() - 1);
//
    //    boost::process::v1::ipstream pipe_stream;
    //    boost::process::v1::child c(cmdline, boost::process::v1::std_out > pipe_stream);
//
    //    std::string data;
    //    data.resize(BUFSIZE);
//
    //    while (pipe_stream.read(&(data[0]), BUFSIZE))
    //        str.append(data);
//
    //    c.wait();
//
    //    if (c.exit_code() != 0)
    //    {
    //        //*pExitCode = c.exit_code();
    //    }
    //}
    //catch (const boost::process::v1::process_error& e)
    //{
    //    //cout << e..what();
    //    msg.ajoute(std::string("Unable to execute command: ") + cmdline);
    //    msg.ajoute(string("Boost.Process Error: ") + e.what());
    //    //std::cerr << "Error Code: " << e.code().value() << std::endl;
    //    //std::cerr << "Error Category: " << e.code().category().name() << std::endl;
    //}
    //catch (const std::exception& e)
    //{
    //    msg.ajoute(std::string("Unable to execute command: ") + cmdline);
    //    msg.ajoute(e.what());
    //    //std::cerr << "General C++ Exception: " << e.what() << std::endl;
    //}

    return msg;
}



//	ERMsg CCallcURL::CallApp(const std::string& cmdline, std::string& str, DWORD BUFSIZE)
//	{
//		ERMsg msg;
//
//		HANDLE g_hChildStd_ERR_Rd = NULL;
//		HANDLE g_hChildStd_ERR_Wr = NULL;
//		HANDLE g_hChildStd_OUT_Rd = NULL;
//		HANDLE g_hChildStd_OUT_Wr = NULL;
//
//		SECURITY_ATTRIBUTES saAttr = { 0 };
//
//		// Set the bInheritHandle flag so pipe handles are inherited.
//		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
//		saAttr.bInheritHandle = TRUE;
//		saAttr.lpSecurityDescriptor = NULL;
//
//		// Create a pipe for the child process's STDOUT.
//		if (msg && !CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
//			msg.ajoute("CreatePipe fail");
//
//		// Ensure the read handle to the pipe for STDOUT is not inherited.
//		if (msg && !SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
//			msg.ajoute("SetHandleInformation fail");
//
//		// Create a pipe for the child process's STDERR.
//		if (msg && !CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &saAttr, 0))
//			msg.ajoute("CreatePipe fail");
//
//		// Ensure the write handle to the pipe for STDERR is not inherited.
//		if (msg && !SetHandleInformation(g_hChildStd_ERR_Wr, HANDLE_FLAG_INHERIT, 0))
//			msg.ajoute("SetHandleInformation fail");
//
//		if (msg)
//		{
//			// Create the child process.
//			CreateChildProcess(cmdline, g_hChildStd_ERR_Wr, g_hChildStd_OUT_Wr);
//
//			// Read from pipe that is the standard output for child process.
//			//string error;
//			msg = ReadFromPipe(g_hChildStd_OUT_Rd, g_hChildStd_ERR_Rd, str, BUFSIZE);
//
//			//string error;
//			//ReadFromPipe(g_hChildStd_ERR_Rd, error);
//			//if (!error.empty())
//			//{
//				//msg.ajoute(error);//humm if warning!!!
//			//}
//
//			//close handles
//			CloseHandle(g_hChildStd_OUT_Rd);
//			CloseHandle(g_hChildStd_ERR_Rd);
//
//		}
//
//		return msg;
//	}
//
//
//    // Create a child process that uses the previously created pipes for STDIN and STDOUT.
//	ERMsg CCallcURL::CreateChildProcess(const std::string& cmdLine, HANDLE& g_hChildStd_ERR_Wr, HANDLE& g_hChildStd_OUT_Wr)
//	{
//		ERMsg msg;
//
//		PROCESS_INFORMATION piProcInfo;
//		STARTUPINFO siStartInfo;
//		BOOL bSuccess = FALSE;
//
//		// Set up members of the PROCESS_INFORMATION structure.
//		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
//
//		// Set up members of the STARTUPINFO structure.
//		// This structure specifies the STDIN and STDOUT handles for redirection.
//
//		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
//		siStartInfo.cb = sizeof(STARTUPINFO);
//		siStartInfo.hStdError = g_hChildStd_ERR_Wr;
//		siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
//		siStartInfo.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
//		siStartInfo.wShowWindow = SW_HIDE;
//
//		// Create the child process.
//		typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> CONVERT_STRING_DEF;
//		CONVERT_STRING_DEF CONVERT_STRING;
//		std::wstring wcommand = CONVERT_STRING.from_bytes(cmdLine);
//		wcommand.resize(wcslen(wcommand.c_str()));
//
//		bSuccess = CreateProcess(NULL,
//			&(wcommand[0]),     // command line
//			NULL,          // process security attributes
//			NULL,          // primary thread security attributes
//			TRUE,          // handles are inherited
//			0,             // creation flags
//			NULL,          // use parent's environment
//			NULL,          // use parent's current directory
//			&siStartInfo,  // STARTUPINFO pointer
//			&piProcInfo);  // receives PROCESS_INFORMATION
//
//		 // If an error occurs, exit the application.
//		if (bSuccess)
//		{
//			// Close handles to the child process and its primary thread.
//			// Some applications might keep these handles to monitor the status
//			// of the child process, for example.
//
//			CloseHandle(piProcInfo.hProcess);
//			CloseHandle(piProcInfo.hThread);
//
//			// Close handles to the stdin and stdout pipes no longer needed by the child process.
//			// If they are not explicitly closed, there is no way to recognize that the child process has ended.
//
//			CloseHandle(g_hChildStd_OUT_Wr);
//			CloseHandle(g_hChildStd_ERR_Wr);
//		}
//		else
//		{
//			msg.ajoute("CreateProcess fail with command:");
//			msg.ajoute(cmdLine);
//		}
//
//		return msg;
//	}
//
//
//	ERMsg CCallcURL::ReadFromPipe(HANDLE& g_hChildStd_OUT_Rd, HANDLE& g_hChildStd_ERR_Rd, std::string& str, DWORD BUFSIZE)
//		// Read output from the child process's pipe for STDOUT
//		// Stop when clhild process close pipe
//	{
//		//static const int BUFSIZE = 4096;
//
//		ERMsg msg;
//
//		DWORD dwRead;
//		//CHAR chBuf[BUFSIZE] = { 0 };
//		std::string chBuf;
//		chBuf.resize(BUFSIZE, 0);
//
//		BOOL bSuccess = FALSE;
//
//		for (;;)
//		{
//			//read error
//			ReadFile(g_hChildStd_ERR_Rd, &(chBuf[0]), BUFSIZE, &dwRead, NULL);
//			if (dwRead > 0)
//			{
//				msg.ajoute(chBuf);
//				break;
//			}
//
//			//read data
//			bSuccess = ReadFile(g_hChildStd_OUT_Rd, &(chBuf[0]), BUFSIZE, &dwRead, NULL);
//
//			if (!bSuccess)
//				break;
//
//			str.append(chBuf.c_str(), dwRead);
//		}
//
//		return msg;
//	}
//

}//namespace WBSF
