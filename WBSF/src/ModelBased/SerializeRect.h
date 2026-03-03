#pragma once

//#include <WTypes.h>

//#include "Basic/zenXml.h"


namespace WBSF
{

class tagRECT
{
public:

    int top;
    int left;
    int bottom;
    int right;
};

inline std::string to_string(const tagRECT& rect)
{
    std::stringstream s;
    s << rect.top << " " << rect.left << " " << rect.bottom << " " << rect.right;

    return s.str();
}

inline tagRECT to_rect(const std::string& str)
{
    tagRECT rect;

    std::stringstream s(str);
    s >> rect.top >> rect.left >> rect.bottom >> rect.right;


    //
    //sscanf_s( str.c_str(), "%d %d %d %d", &rect.top, &rect.left, &rect.bottom, &rect.right);

    return rect;
}


}

//namespace zen
//{
//template <> inline
//void writeStruc(const tagRECT& rect, XmlElement& output)
//{
//    std::string str = WBSF::to_string(rect);
//
//    output.setValue( str );
//}
//
//template <> inline
//bool readStruc(const XmlElement& input, tagRECT& rect)
//{
//    std::string str;
//    input.getValue(str);
//
//    rect = WBSF::ToRect(str);
//
//    return true;
//}
//
//}
