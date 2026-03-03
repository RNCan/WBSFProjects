#pragma once

#include <cstddef>
#include <string>

//#include <cassert>
//#include <map>
//#include <set>
//#include <unordered_map>
//#include <sstream>
//#include <functional>
//#include <algorithm>
//#include <cctype>
//#include <fstream>
//#include <codecvt>
//#include <iomanip>
//#include <cstring>
//#include <filesystem>
//#include <cstdint>
//#include <utility>



#ifdef _MSC_VER


#include <SDKDDKVer.h>


#define strncasecmp _strnicmp
#define strcasecmp _stricmp


//#elseif __linux


#endif


namespace WBSF
{

    static constexpr size_t NOT_INIT = size_t(-1);
    static const size_t UNKNOWN_POS = NOT_INIT;

    static const std::string STRVMISS = "VMiss";
    static const std::string STRDEFAULT = "Default";

}
