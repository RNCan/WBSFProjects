//******************************************************************************
//  project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
#pragma once


#include <string>
#include <filesystem>
#include "Basic/ERMsg.h"


namespace WBSF
{

class CCallcURL
{
public:

    CCallcURL(const std::string& exe_filepath="", size_t bufsize = 4096);


    ERMsg get_text(const std::string& arg, std::string& str_out);
    ERMsg copy_file(const std::string& URL, const std::string& outputFilePath, bool bShowCurl=false);
    static ERMsg CallApp(const std::string& cmdline, std::string& str, size_t BUFSIZE = 4096);



    size_t m_timeout;
    size_t m_bufsize;

protected:


    //static ERMsg CreateChildProcess(const std::string& cmdLine, HANDLE& g_hChildStd_ERR_Wr, HANDLE& g_hChildStd_OUT_Wr);
    //static ERMsg ReadFromPipe(HANDLE& g_hChildStd_OUT_Rd, HANDLE& g_hChildStd_ERR_Rd, std::string& str, size_t BUFSIZE);


    std::string m_exe_filepath;
};

}
