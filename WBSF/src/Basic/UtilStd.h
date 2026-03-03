#pragma once


//#include <wtypes.h>
//#undef min
//#undef max


#include <cassert>
#include <map>
#include <set>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <codecvt>
#include <iomanip>
#include <cstring>
#include <filesystem>
#include <cstdint>
#include <utility>
//#include <share.h>


#include "WBSFconfig.h"
#include "Basic/ERMsg.h"



namespace WBSF
{

	//static constexpr size_t NOT_INIT = size_t(-1);
	//static const size_t UNKNOWN_POS = NOT_INIT;
	//
	//extern const std::string STRVMISS;
	//extern const std::string STRDEFAULT;


	////*********************************************************************************************************************
	////std::vector<std::string>
	//
	//class string_quote_reader
	//{
	//public:
	//
	//
	//	string_quote_reader(std::string & str) : m_str(str){}
	//	operator std::string & () { return m_str; }
	//
	//	std::istream& operator<<(std::istream& s)
	//	{
	//		char x;
	//		s >> x;
	//		if ((s) && (x == '"'))
	//		{
	//			std::getline(s, m_str, '"');
	//		}
	//		else
	//		{
	//			s.putback(x);
	//			s >> m_str;
	//		}
	//		return s;
	//	}
	//
	//	friend std::istream& operator>>(std::istream& s, string_quote_reader& str){ return str << s; }
	//
	//protected:
	//
	//	std::string & m_str;
	//};
	//
	//
	//class string_quote_writer
	//{
	//public:
	//
	//	string_quote_writer(std::string const& str) : m_str(const_cast<std::string &>(str)){}
	//	operator std::string const& () const { return m_str; }
	//
	//
	//	std::ostream& operator>>(std::ostream& s)const{ return s << "\"" << m_str << "\""; }
	//	friend std::ostream& operator<<(std::ostream& s, string_quote_writer const& str){ return (str >> s); }
	//
	//protected:
	//
	//	std::string const& m_str;
	//
	//
	//};
	//
	//
	//
	//class std::vector<std::string> : public std::vector < std::string >
	//{
	//public:
	//
	//	std::vector<std::string>() :std::vector<std::string>(){}
	//	explicit std::vector<std::string>(size_t size) :std::vector<std::string>(size){}
	//	explicit std::vector<std::string>(const std::string& str, const char* delimiters){ Tokenize(str, delimiters); }
	//	explicit std::vector<std::string>(const char* str, const char* delimiters){ Tokenize(str, delimiters); }
	//	//explicit std::vector<std::string>(UINT ID, const std::string& delimiters){ LoadString(ID, delimiters); }
	//	explicit std::vector<std::string>(const char* str_array[]);
	//	//void LoadString(UINT ID, const std::string& delimiters);
	//	std::vector<std::string>& Tokenize(const std::string& str, const std::string& delimiters, bool bRemoveDuplicate = true, std::string::size_type pos = 0, std::string::size_type posEnd = std::string::npos);
	std::vector<std::string> TokenizeQuoted(std::string str, const std::string& delimiters);
	//
	//	//size_t Find(const std::string& lem)const;
	//	std::ostream& operator >> (std::ostream& s)const;
	//	std::istream& operator << (std::istream& s);
	//	friend std::ostream& operator << (std::ostream& s, const std::vector<std::string>& in){ in >> s; return s; }
	//	friend std::istream& operator >> (std::istream& s, std::vector<std::string>& out){ out << s; return s; }
	//
	//	size_t Find(const std::string& str, bool bCaseSensitive = true, bool bExact = true)const;
	size_t Find(const std::vector<std::string>& v, const std::string& str, bool bCaseSensitive = true, bool bExact = true);
	std::set<size_t> FindAll(const std::vector<std::string>& v, const std::string& str, bool bCaseSensitive = true, bool bExact = false);
	//
	//	std::string to_string(const char* sep = "|")const;
	//};
	//
	//
	////*********************************************************************************************************************
	////MapSearchByValue
	//
	//
	////find the second element of a map
	//template <class Map, class Val>
	//typename Map::const_iterator MapSearchByValue(const Map & SearchMap, const Val & SearchVal)
	//{
	//	typename Map::const_iterator iRet = SearchMap.end();
	//    for (typename Map::const_iterator iTer = SearchMap.begin(); iTer != SearchMap.end(); iTer ++)
	//    {
	//        if (iTer->second == SearchVal)
	//        {
	//            iRet = iTer;
	//            break;
	//        }
	//    }
	//    return iRet;
	//}
	//
	template < typename T, typename U >
	std::map< U, T > converse_map(std::map< T, U > const& o)
	{
		std::map< U, T > result;
		for (typename std::map< T, U >::const_iterator begin(o.begin()); begin != o.end(); ++begin)
			result.insert(std::make_pair(begin->second, begin->first));

		return result;
	}

	template <typename Map>
	bool map_compare(Map const& lhs, Map const& rhs)
	{
		// No predicate needed because there is operator== for pairs already.
		return	lhs.size() == rhs.size() &&
			std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	////*********************************************************************************************************************
	////String
	//
	//	class CCallback;
	//	std::string RemoveAccented(std::string str);
	std::string PurgeFileName(std::string name);
	std::string ReplaceString(const std::string& str, const std::string& oldStr, const std::string& newStr);
	//	std::string GetText(ERMsg msg);
	//	//std::string GetOutputString(ERMsg& msg, CCallback& callBack = DEFAULT_CALLBACK, bool bAllMessage = false);
	//	std::string GetOutputString(ERMsg msg, CCallback& callBack, bool bAllMessage = true, const char* sep = "\n");
	//
	//
	//
	//	//std::string GetString(UINT nId);
	//	std::string& UppercaseFirstLetter(std::string& str);
	//
	//	bool Match(char const * Wildcards, char const * str);
	//	bool Scan(char const *& Wildcards, char const *& str);
	//
	std::string Tokenize(const std::string& str, const std::string& delimiter, std::string::size_type& pos, bool bRemoveDuplicate = false, std::string::size_type posEnd = std::string::npos);
	std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiter, bool bRemoveDuplicate = true, std::string::size_type pos = 0, std::string::size_type posEnd = std::string::npos);
	//	bool TokenizeWithQuote(const std::string& str, char sep, std::vector<std::string>& out);
	//	bool TokenizeWithQuote(const std::string& str, char* sep, std::vector<std::string>& out);
	//
	std::string FormatA(const char* szFormat, ...);
	std::string FormatV(const char* szFormat, va_list argList);

	//	inline std::string Format(PCSTR szFormat, const std::string& v1, const std::string& v2 = "", const std::string& v3 = "", const std::string& v4 = "", const std::string& v5 = "", const std::string& v6 = "", const std::string& v7 = "", const std::string& v8 = "", const std::string& v9 = "")
	//	{
	//		assert(szFormat && strlen(szFormat)>0 );
	//		std::string str;
	//
	//		int nbParams = 1 + (v2.empty() ? 0 : 1) + (v3.empty() ? 0 : 1) + (v4.empty() ? 0 : 1) + (v5.empty() ? 0 : 1) + (v6.empty() ? 0 : 1) + (v7.empty() ? 0 : 1) + (v8.empty() ? 0 : 1) + (v9.empty() ? 0 : 1);
	//		switch (nbParams)
	//		{
	//		case 1: str = FormatA(szFormat, v1.c_str()); break;
	//		case 2: str = FormatA(szFormat, v1.c_str(), v2.c_str()); break;
	//		case 3: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str()); break;
	//		case 4: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str()); break;
	//		case 5: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str(), v5.c_str()); break;
	//		case 6: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str(), v5.c_str(), v6.c_str()); break;
	//		case 7: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str(), v5.c_str(), v6.c_str(), v7.c_str()); break;
	//		case 8: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str(), v5.c_str(), v6.c_str(), v7.c_str(), v8.c_str()); break;
	//		case 9: str = FormatA(szFormat, v1.c_str(), v2.c_str(), v3.c_str(), v4.c_str(), v5.c_str(), v6.c_str(), v7.c_str(), v8.c_str(), v9.c_str()); break;
	//		default: assert(false);  break;
	//		}
	//
	//		return str;
	//	}
	//
	//	//inline std::string FormatMessage(UINT nId, const std::string& v1, const std::string& v2 = "", const std::string& v3 = "", const std::string& v4 = "", const std::string& v5 = "", const std::string& v6 = "")
	//	//{
	//	//	std::string str;
	//	//	std::string strFmt = GetString(nId);
	//	//	if (!strFmt.empty() && !v1.empty())
	//	//		str = Format(strFmt.c_str(), v1, v2, v3, v4, v5, v6);
	//	//}
	//
	//	std::string FormatMsgA(PCSTR szFormat, ...);
	//	//std::string FormatMsg(UINT nFormatId, ...);
	//
	std::string FormatMsg(std::string_view szFormat, std::string_view v1, std::string_view v2 = "", std::string_view v3 = "", std::string_view  v4 = "", std::string_view v5 = "");


	void PrintMessage(ERMsg msg);
	//	inline std::string FormatMsg(UINT nFormatId, const std::string& v1, const std::string& v2 = "", const std::string& v3 = "", const std::string& v4 = "", const std::string& v5 = "", const std::string& v6 = "", const std::string& v7 = "", const std::string& v8 = "", const std::string& v9 = "")
	//	{
	//		std::string sFormat = GetString(nFormatId);
	//		return FormatMsg(sFormat.c_str(), v1, v2, v3, v4, v5, v6, v7, v8, v9);
	//	}
	//
	//	inline bool Match(char const * Wildcards, char const * str)
	//	{
	//		bool Yes = 1;
	//
	//		//iterate and delete '?' and '*' one by one
	//		while(*Wildcards != '\0' && Yes && *str != '\0')
	//		{
	//			if (*Wildcards == '?') str ++;
	//			else if (*Wildcards == '*')
	//			{
	//				Yes = Scan(Wildcards, str);
	//				Wildcards --;
	//			}
	//			else
	//			{
	//				Yes = (*Wildcards == *str);
	//				str ++;
	//			}
	//			Wildcards ++;
	//		}
	//		while (*Wildcards == '*' && Yes)  Wildcards ++;
	//
	//		return Yes && *str == '\0' && *Wildcards == '\0';
	//	}
	//
	//	// scan '?' and '*'
	//	inline bool Scan(char const *& Wildcards, char const *& str)
	//	{
	//		// remove the '?' and '*'
	//		for(Wildcards ++; *str != '\0' && (*Wildcards == '?' || *Wildcards == '*'); Wildcards ++)
	//			if (*Wildcards == '?') str ++;
	//		while ( *Wildcards == '*') Wildcards ++;
	//
	//		// if str is empty and Wildcards has more characters or,
	//		// Wildcards is empty, return
	//		if (*str == '\0' && *Wildcards != '\0') return false;
	//		if (*str == '\0' && *Wildcards == '\0')	return true;
	//		// else search substring
	//		else
	//		{
	//			char const * wdsCopy = Wildcards;
	//			char const * strCopy = str;
	//			bool  Yes     = 1;
	//			do
	//			{
	//				if (!Match(Wildcards, str))	strCopy ++;
	//				Wildcards = wdsCopy;
	//				str		  = strCopy;
	//				while ((*Wildcards != *str) && (*str != '\0')) str ++;
	//				wdsCopy = Wildcards;
	//				strCopy = str;
	//			}while ((*str != '\0') ? !Match(Wildcards, str) : (Yes = false) != false);
	//
	//			if (*str == '\0' && *Wildcards == '\0')	return true;
	//
	//			return Yes;
	//		}
	//	}
	//
	//
	static const std::string whiteSpaces(" \f\n\r\t\v");


	inline std::string& TrimRight(std::string& str,
		const std::string& trimChars = whiteSpaces)
	{
		std::string::size_type pos = str.find_last_not_of(trimChars);
		str.erase(pos + 1);
		return str;
	}


	inline std::string& TrimLeft(std::string& str,
		const std::string& trimChars = whiteSpaces)
	{
		std::string::size_type pos = str.find_first_not_of(trimChars);
		str.erase(0, pos);
		return str;
	}


	inline std::string& Trim(std::string& str, const std::string& trimChars = whiteSpaces)
	{
		TrimRight(str, trimChars);
		TrimLeft(str, trimChars);
		return str;
	}

	inline std::string TrimConst(const std::string& s, const std::string& trimChars = whiteSpaces)
	{
		std::string ss(s);
		TrimRight(ss, trimChars);
		TrimLeft(ss, trimChars);
		return ss;
	}

	inline std::string& MakeUpper(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}
	inline std::string& MakeLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}
	//	inline std::string Right(std::string const& source, size_t length)
	//	{
	//		if (length >= source.length())
	//			return source;
	//
	//		return source.substr(source.length() - length);
	//	} // tail
	//

	inline std::string::size_type GetNextLinePos(const std::string& str, std::string::size_type begin)
	{
		std::string::size_type end = std::min(str.size(), str.find_first_of("\r\n", begin));
		while (end < str.size() && (str[end] == '\r' || str[end] == '\n'))
			end++;

		return end;
	}

	inline bool ToBool(const std::string& str)
	{
		return atoi(str.c_str()) != 0;
	}
	inline char ToChar(const std::string& str)
	{
		return (char)atoi(str.c_str());
	}
	inline short ToShort(const std::string& str)
	{
		return (short)atoi(str.c_str());
	}
	inline int ToInt(const std::string& str)
	{
		return atoi(str.c_str());
	}
	inline int64_t ToInt64(const std::string& str)
	{
		return atoi(str.c_str());
	}
	inline long ToLong(const std::string& str)
	{
		return atoi(str.c_str());
	}
	inline float ToFloat(const std::string& str)
	{
		return (float)atof(str.c_str());
	}
	inline double ToDouble(const std::string& str)
	{
		return atof(str.c_str());
	}
	inline size_t ToSizeT(const std::string& str)
	{
		return (size_t)atoi(str.c_str());
	}

	inline int to_int(const std::string& str)
	{
		return (int)atoi(str.c_str());
	}

	inline size_t to_size_t(const std::string& str)
	{
		return (size_t)atoi(str.c_str());
	}
	//
	//	COLORREF ToCOLORREF(const std::string& str);

	template <class T> inline
		T to_object(const std::string& str)
	{
		T obj;
		std::istringstream ss(str);
		obj << ss;
		return obj;
	}



	template <typename T> inline
		T to_value(const std::string& str)
	{
		T t = 0;
		if (!str.empty())
		{
			std::istringstream ss(str);
			ss >> t;
		}
		return t;
	}

	template <> inline
		const std::string to_value(const std::string& str)
	{
		return str;
	}

	//template <typename T> inline
	//	T from_string(const std::string& str)
	//{
	//	return to_value<T>(str);
	//}

	//	template <typename T> inline
	//		T as(const std::string& str)
	//	{
	//		T t = 0;
	//		if (!str.empty())
	//		{
	//			std::istringstream ss(str);
	//			ss >> t;
	//		}
	//		return t;
	//	}
	//
	//	template <typename T> inline
	//		T from_string(const std::string& str)
	//	{
	//		return ToValue<T>(str);
	//	}
	//
	//
	//	template <typename T> inline
	//		std::string to_string(const T& v)
	//	{
	//		std::ostringstream ss;
	//		ss << v;
	//		return ss.str();
	//	}
	//
	//
	//	template <typename U, class T=std::vector<U>> inline
	//		T to_object(const std::string& str, const std::string& sep, const std::string& be = "", const std::string& en = "")
	//	{
	//		std::vector<std::string> tmp = Tokenize(str, be + sep + en);
	//
	//		T v;
	//		for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++)
	//			if (!it->empty())
	//				v.insert(v.end(), from_string<U>(*it));
	//
	//		return v;
	//	}
	//
	//	template <typename T> inline
	//		const std::vector<T> ToVector(const std::string& str, const std::string& be = "[", const std::string& sep = "|", const std::string& en = "]")
	//	{
	//		std::vector<std::string> tmp = Tokenize(str, be + sep + en);
	//		std::vector<T> v;
	//		v.reserve(tmp.size());
	//		for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++)
	//			if (!it->empty())
	//				v.push_back(ToValue<T>(*it));
	//
	//		return v;
	//	}
	//
	//	std::string ToDMS(double);
	//
	//
	template <class T>
	std::string to_stringStd(T val, int pres = -1)
	{
		std::ostringstream st;
		auto myloc = std::locale::classic();//to avoid memory leak
		st.imbue(myloc);

		std::string str;
		bool bReal = std::is_same<T, float>::value || std::is_same<T, double>::value;
		if (bReal)
		{
			if (pres < 0)
			{
				st << val;
			}
			else
			{
				st << std::fixed << std::setprecision(pres) << val;
			}

			str = st.str();
			size_t pos = str.find('.');
			if (pos != std::string::npos)//if it's a real;
			{
				int i = (int)str.length() - 1;
				while (i >= 0 && str[i] == '0')
					i--;

				if (i >= 0 && str[i] == '.') i--;
				str = str.substr(0, i + 1);
				if (str.empty())
					str = "0";
			}
		}
		else
		{
			if (pres < 0)
			{
				st << val;
			}
			else
			{
				st << std::setfill(' ') << std::setw(pres) << val;
			}

			str = st.str();
		}



		return str;
	}

	//	template<typename T>
	//	std::ostream& serialize(std::ostream& os, std::vector<T> const& v)
	//	{
	//		// this only works on built in data types (T)
	//		static_assert(std::is_trivial<T>::value && std::is_standard_layout<T>::value,
	//			"Can only serialize build-in types with this function");
	//
	//		auto size = v.size();
	//		os.write(reinterpret_cast<char const*>(&size), sizeof(size));
	//		os.write(reinterpret_cast<char const*>(v.data()), v.size() * sizeof(T));
	//		return os;
	//	}
	//
	//	template<typename T>
	//	std::istream& deserialize(std::istream& is, std::vector<T>& v)
	//	{
	//		static_assert(std::is_trivial<T>::value && std::is_standard_layout<T>::value,
	//			"Can only deserialize build-in types with this function");
	//
	//		decltype(v.size()) size;
	//		is.read(reinterpret_cast<char*>(&size), sizeof(size));
	//		v.resize(size);
	//		is.read(reinterpret_cast<char*>(v.data()), v.size() * sizeof(T));
	//		return is;
	//	}
	//
	//
	//

	bool IsEqualNoCase(const std::string& str1, const std::string& str2);


	template <typename T, typename V>
	bool IsEqual(const T& a, const V& b, bool bCase = false)
	{
		return (bCase ? (strcmp(a, b) == 0) : IsEqualNoCase(a, b));
	}

	inline bool IsEqual(const std::string& str1, const char* str2, bool bCase = false)
	{
		return IsEqual<const char*>(str1.c_str(), str2);
	}

	inline bool IsEqual(const std::string& str1, const std::string& str2, bool bCase = false)
	{
		return IsEqual<const char*>(str1.c_str(), str2.c_str());
	}
	//

	//
	//	inline std::string to_string(const std::string& str) { return str; }
	//	inline std::string to_string(COLORREF c){ return FormatA("%d %d %d", (int)GetRValue(c), (int)GetGValue(c), (int)GetBValue(c)); }
	//inline std::string to_string(int val, int size = -1)
	//{
	//    return to_stringStd(val, size);
	//}
	//inline std::string to_string(int64_t val, int size = -1)
	//{
	//    return to_stringStd(val, size);
	//}
	//inline std::string to_string(float val, int pres=4)
	//{
	//    return to_stringStd((double)val, pres);
	//}
	//inline std::string to_string(double val, int pres=4)
	//{
	//    return to_stringStd(val, pres);
	//}

	//#include <sstream>

	//using std::to_string;
	//template <typename T>
	//std::string to_string(const T a_value, const int n)
	//{
	//    std::ostringstream out;
	//    out.precision(n);
	//    out << std::fixed << a_value;
	//    return std::move(out).str();
	//}
	//	std::string to_stringDMS(double coord, bool bWithFraction);



	template <typename T>
	std::string to_string(const T a_value, const int n)
	{
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return std::move(out).str();
	}




	//	//http://stackoverflow.com/a/13636164/195722
	//template <typename T> inline
	//	std::string to_string(T obj)
	//{
	//	std::ostringstream ss;
	//	ss << obj;
	//	return ss.str();
	//}


	using std::to_string;
	template <template<typename...> class Container, typename... Args>
	std::string to_string(const Container<Args...>& v, const std::string& sep = "|") //const std::string& be = "[", const std::string& sep = "|", const std::string& en = "]")
	{
		std::string str;// = be;

		for (typename Container<Args...>::const_iterator it = v.begin(); it != v.end(); it++)
		{
			if (it != v.begin())
				str += sep;
			str += to_string(*it);
		}

		//str += en;

		return str;
	}
	template <template<typename, size_t N> class Container, typename T, size_t N>
	std::string to_string(const Container<T,N>& v, const std::string& sep = "|") //const std::string& be = "[", const std::string& sep = "|", const std::string& en = "]")
	{
		std::string str;// = be;

		for (typename Container<T,N>::const_iterator it = v.begin(); it != v.end(); it++)
		{
			if (it != v.begin())
				str += sep;
			str += to_string(*it);
		}

		//str += en;

		return str;
	}


	/*template <typename U, class T=std::vector<U>> inline
		T to_object(const std::string& str, const std::string& sep, const std::string& be = "", const std::string& en = "")
	{
		std::vector<std::string> tmp = Tokenize(str, be + sep + en);

		T v;
		for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++)
			if (!it->empty())
				v.insert(v.end(), from_string<U>(*it));

		return v;
	}*/

	template <template<typename, typename> typename Container, typename T, typename V>
	Container<T, V> from_string(const std::string& str)
	{
		std::vector<std::string> tmp = Tokenize(str, "|");
		Container<T, V> v;
		v.reserve(tmp.size());
		for (std::vector<std::string>::const_iterator it = tmp.begin(); it != tmp.end(); it++)
			// if (!it->empty())
			v.push_back(to_value<T>(*it));

		return v;
	}

	//template for array
	template <template <typename, std::size_t> class Container, typename T, std::size_t N>
	Container<T, N> from_string(const std::string& str)
	{
		std::vector<std::string> tmp = Tokenize(str, "|");
		Container<T, N> v;
		//v.reserve(tmp.size());
		for (size_t i = 0; i < tmp.size(); i++)
			v[i] = to_value<T>(tmp[i]);

		return v;
	}

	//
	//	typedef std::unordered_map< std::string, std::string> StringStringMap;
	//	typedef std::pair< std::string, std::string> StringStringPair;
	//
	//	inline std::string to_string(StringStringMap const & Map)
	//	{
	//		std::string str;
	//
	//		for(StringStringMap::const_iterator it=Map.begin(); it!=Map.end(); it++)
	//			str+="{" + it->first + "," + it->second + "}";
	//
	//		return str;
	//	}
	//
	//	template <class T> inline
	//		std::string to_string(const T& v, const std::string& sep, const std::string& begin = "", const std::string& end = "")
	//	{
	//		std::string str = begin;
	//		for (typename T::const_iterator it = v.begin(); it != v.end(); it++)
	//		{
	//			if (it != v.begin())
	//				str += sep;
	//			str += WBSF::to_string(*it);
	//		}
	//
	//		str += end;
	//
	//		return str;
	//	}
	//
	//
	//	inline StringStringMap to_stringStringMap(const std::string& str)
	//	{
	//		StringStringMap Map;
	//		std::vector<std::string> tuples = Tokenize(str, "{}");
	//		for(std::vector<std::string>::const_iterator it=tuples.begin(); it!=tuples.end(); it++)
	//		{
	//			size_t pos2=it->find(',',0);
	//			Map[it->substr(0,pos2)] = it->substr(pos2+1);
	//		}
	//
	//		return Map;
	//	}
	//
	size_t Find(const std::string& str1, const std::string& str2, bool bCaseSensitive = false);
	//	std::string FindString(const std::string& source, const std::string& strBegin, const std::string& strEnd, std::string::size_type& posBegin, std::string::size_type& posEnd);
	//	std::string FindString(const std::string& source, const std::string& strBegin, const std::string& strEnd, std::string::size_type& posBegin);
	//	std::string FindString(const std::string& source, const std::string& strBegin, const std::string& strEnd);
	//
	//
	////	std::string Encrypt(const std::string& str, const std::string& key = "Mingo_La_Fleche5");
	//	//std::string Decrypt(const std::string& str, const std::string& key = "Mingo_La_Fleche5");
	//
	//
	//	std::string  ANSI_2_ASCII(std::string str);
	//	std::wstring UTF8_UTF16(const std::string& str);
	//	std::string UTF16_UTF8(const std::wstring& str);
	//	std::string ANSI_UTF8(const std::string& str);
	//	std::string UTF8_ANSI(const std::string& str);
	//
	//#ifdef _UNICODE
	//
	//	inline std::string UTF8(const std::wstring& utf16){ return UTF16_UTF8(utf16); }
	//	inline std::wstring UTF16(const std::string& utf8){ return UTF8_UTF16(utf8.c_str()); }
	//
	//	inline const std::wstring convert(const std::string& utf8){ return UTF8_UTF16(utf8.c_str()); }
	//	inline const std::wstring convert(const char* utf8){ return UTF8_UTF16(utf8); }
	//
	//#else
	//
	//	inline std::string UTF8(const std::wstring& utf16){ return UTF16_UTF8(utf16); }
	//	inline std::string UTF8(const std::string& utf8){ return utf8; }
	//	inline std::wstring UTF16(const std::string& utf8){ return  UTF8_UTF16(utf8.c_str()); }
	//	inline std::wstring UTF16(const std::wstring& utf16){ return  utf16; }
	//
	//	inline const std::string convert(const std::wstring& utf16){ return UTF16_UTF8(utf16.c_str()); }
	//	inline const std::string convert(const wchar_t* utf16){ return UTF16_UTF8(utf16); }
	//	inline const std::string convert(const std::string& utf8){ return utf8; }
	//
	//#endif
	//
	template <typename T>
	T read_value(std::istream& s)
	{
		T v = 0;
		s.read((char*)&v, sizeof(v));

		return v;
	}

	template <typename T>
	std::istream& read_value(std::istream& s, T& v)
	{
		s.read((char*)&v, sizeof(v));
		return s;
	}

	template <typename T>
	void write_value(std::ostream& s, const T& v)
	{
		s.write((char*)&v, sizeof(v));
	}

	inline std::string ReadBuffer(std::istream& s)
	{
		std::string buffer;
		uint64_t size = read_value<uint64_t>(s);
		buffer.resize((size_t)size);
		s.read(const_cast<char*>(buffer.c_str()), size);

		return buffer;
	}

	inline std::string& ReadBuffer(std::istream& s, std::string& buffer)
	{
		uint64_t size = read_value<uint64_t>(s);
		buffer.resize((size_t)size);
		s.read(const_cast<char*>(buffer.c_str()), size);

		return buffer;
	}


	inline void WriteBuffer(std::ostream& s, const std::string& buffer)
	{
		uint64_t size = buffer.size();
		write_value(s, size);
		s.write(buffer.c_str(), size);
	}

	//	class lexical_sort : public std::binary_function<std::string, std::string, bool>
	//	{
	//	protected:
	//		const std::collate<char> &coll;
	//	public:
	//		lexical_sort()
	//			: coll(std::use_facet<std::collate<char> >(std::locale(""))) {}
	//		bool operator()(const std::string &a, const std::string &b) const
	//		{
	//			// std::collate::compare() takes C-style string (begin, end)s and
	//			// returns values like strcmp or strcoll.  Compare to 0 for results
	//			// expected for a less<>-style comparator.
	//			return coll.compare(a.c_str(), (a.c_str() + a.size()), b.c_str(), (b.c_str() + b.size())) < 0;
	//		}
	//	};
	//
	//
	//	class CCompareString : public std::binary_function<std::string, std::string, bool>
	//	{
	//	public:
	//		CCompareString(const std::locale& loc = std::locale("")) :
	//			m_locale(loc),
	//			m_collate(std::use_facet<std::collate<char> >(m_locale))
	//		{}
	//
	//		bool operator()(const std::pair<std::string, size_t>& a, const std::pair<std::string, size_t>& b) const
	//		{
	//			// std::collate::compare() takes C-style string (begin, end)s and
	//			// returns values like strcmp or strcoll.  Compare to 0 for results
	//			// expected for a less<>-style comparator.
	//			return m_collate.compare(a.first.c_str(), (a.first.c_str() + a.first.size()), b.first.c_str(), (b.first.c_str() + b.first.size())) < 0;
	//		}
	//
	//	protected:
	//		const std::locale m_locale;
	//		const std::collate<char> &m_collate;
	//
	//	};
	//
	//
	//
	////*********************************************************************************************************************
	////CFileInfo


	class CFileInfo
	{
	public:

		std::string m_filePath;
		std::time_t m_time;

		uint64_t    m_size;
		uint64_t    m_attribute;



		CFileInfo()
		{
			m_size = 0;
			m_time = -1;
			m_attribute = 0;
		}
		CFileInfo(const CFileInfo& in)
		{
			operator =(in);
		}

		void clear()
		{
			m_filePath.clear();
			m_time = -1;
			m_size = 0;
			m_attribute = 0;
		}

		bool is_init()const
		{
			return !m_filePath.empty();
		}

		CFileInfo& operator =(const CFileInfo& in)
		{
			if (&in != this)
			{
				m_filePath = in.m_filePath;
				m_time = in.m_time;
				m_size = in.m_size;
				m_attribute = in.m_attribute;
			}

			return *this;
		}

		bool operator ==(const CFileInfo& in)const
		{
			bool bEqual = true;

			if (!IsEqualNoCase(m_filePath, in.m_filePath))bEqual = false;
			if (m_time != in.m_time)bEqual = false;
			if (m_size != in.m_size)bEqual = false;
			if (m_attribute != in.m_attribute)bEqual = false;

			return bEqual;

		}

		bool operator !=(const CFileInfo& in)const
		{
			return !(operator==(in));
		}


		std::ostream& operator>>(std::ostream& s)const
		{
			s << m_filePath << " " << m_time << " " << m_size;
			return s;
		}
		std::istream& operator<<(std::istream& s)
		{
			s >> m_filePath >> m_time >> m_size;
			return s;
		}
		friend std::ostream& operator<<(std::ostream& s, const CFileInfo& pt)
		{
			pt >> s;
			return s;
		}
		friend std::istream& operator>>(std::istream& s, CFileInfo& pt)
		{
			pt << s;
			return s;
		}

		template<class Archive>	void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_filePath& m_time& m_size;
		}
	};

	typedef std::vector<CFileInfo> CFileInfoVector;




	//	inline bool CompareNumber(const std::pair<std::string, size_t>& a, const std::pair<std::string, size_t>& b){ return stof(a.first) < stof(b.first); }

	std::string GenerateNewName(std::string name);
	std::string GenerateNewFileName(std::string name);
	std::string GetUserDataPath();
	std::string GetTempPath();

	std::string GetApplicationPath();
	std::string GetRelativePath(const std::string& basePath, const std::string& filePath);
	std::string GetAbsolutePath(const std::string& basePath, const std::string& filePath);
	//	std::string SimplifyFilePath(const std::string& path);
	//
	//	std::string GetVersionString(const std::string& filerPath);
	//	std::string GetCompilationDateString(char *compilation_date);
	//

	bool is_file_path(const std::string& path_str);


	inline bool IsPathEndOk(const std::string& filePath)
	{
		bool bRep = false;
		std::string::size_type pos = filePath.length() - 1;
		if (pos != std::string::npos)
			bRep = filePath.at(pos) == '\\' || filePath.at(pos) == '/';

		return bRep;
	}

	inline std::string GetFileExtension(const std::string& str, bool bWithDot = true)
	{
		std::string::size_type beg = str.find_last_of("\\/");
		if (beg != std::string::npos)
			beg += 1;
		else
			beg = 0;

		std::string ext;
		std::string::size_type posExt = str.rfind('.');//extension is from the last point
		if (posExt != std::string::npos && posExt > beg)
		{
			if (!bWithDot)
				posExt++;

			ext = str.substr(posExt);
		}

		return ext;
	}


	inline std::string& SetFileExtension(std::string& str, std::string newExt)
	{
		Trim(newExt);


		if (!newExt.empty() && newExt.find('.') == std::string::npos)
			newExt.insert(newExt.begin(), '.');

		std::string::size_type beg = str.find_last_of("\\/");
		if (beg != std::string::npos)
			beg += 1;
		else
			beg = 0;

		std::string::size_type posExt = str.rfind('.');//extension is from the last point
		if (posExt == std::string::npos || posExt < beg)
		{
			str += newExt;
		}
		else
		{
			std::string::size_type extLength = str.length() - posExt;
			str.replace(posExt, extLength, newExt);
		}

		return str;
	}


	inline std::string GetFileName(const std::string& str)
	{
		std::string::size_type pos = str.find_last_of("\\/");
		if (pos != std::string::npos)
			pos += 1;
		else
			pos = 0;

		return str.substr(pos);
	}

	inline std::string& SetFileName(std::string& str, const std::string& newFileTitle)
	{
		//std::string newStr(str);

		std::string::size_type pos = str.find_last_of("\\/");
		if (pos != std::string::npos)
			pos += 1;
		else
			pos = 0;

		str.replace(pos, str.length() - pos, newFileTitle);

		return str;
	}

	bool GDALStyleProgressBar(double dfComplete);
	inline std::string GetFileTitle(const std::string& str)
	{
		std::string::size_type pos = str.find_last_of("\\/");
		if (pos != std::string::npos)
			pos += 1;
		else
			pos = 0;

		std::string::size_type posExt = str.rfind('.');//extension is from the last point
		if (posExt == std::string::npos || posExt <= pos)
			posExt = str.length();



		return str.substr(pos, posExt - pos);
	}

	inline std::string& SetFileTitle(std::string& str, const std::string& newFileName)
	{
		std::string::size_type pos = str.find_last_of("\\/");
		if (pos != std::string::npos)
			pos += 1;
		else
			pos = 0;

		std::string::size_type posExt = str.rfind('.');//extension is from the last point
		if (posExt == std::string::npos || posExt <= pos)
			posExt = str.length();

		str.replace(pos, posExt - pos, newFileName);

		return str;
	}


	//	inline std::string GetLastDirName(std::string filePath)
	//	{
	//		Trim(filePath);
	//
	//		while (filePath.back() == '/' || filePath.back() == '\\')
	//			filePath.pop_back();
	//
	//		return GetFileName(filePath);
	//	}
	//
	//
	//
	inline std::string  GetPath(std::string str)
	{
		size_t pos = str.find_last_of("\\/");
		if (pos != std::string::npos)
		{
			str = str.substr(0, pos + 1);
		}
		else
		{
			size_t pos = str.find_last_of(":");
			if (pos != std::string::npos)
				str = str.substr(0, pos + 1);
			else
				str.clear();
		}

		return str;
	}

	inline std::string& SetPath(std::string& str, std::string& newPath)
	{
		if (!IsPathEndOk(str))
			str += '\\';

		size_t pos = str.find_last_of("\\/");
		if (pos != std::string::npos)
			str.replace(0, pos + 1, newPath);
		else
			str.insert(0, newPath);

		return str;
	}


	//	std::string GetClipboardText();
	//	bool SetClipboardText(const std::string& str);
	//
	//
	ERMsg RemoveFile(const std::string& filePath);
	ERMsg RemoveDir(const std::string& path);
	ERMsg CreateMultipleDir(const std::string& filePath);
	ERMsg RenameFile(const std::string& filePath1, const std::string& filePath2);
	ERMsg RenameDir(const std::string& path1, const std::string& path2);
	ERMsg CopyOneFile(const std::string& filePath1, const std::string& filePath2, bool failIfExist = true);
	ERMsg CopyDirectory(const std::string& pathIn1, const std::string& pathIn2);


	// Function: fileExists
	/**
		Check if a file exists
		@param[in] filename - the name of the file to check

		@return    true if the file exists, else false

		*/
	bool FileExists(const std::string& file_path);
	bool DirectoryExists(const std::string& diretory_path);

	//enum TFileNameType { FILE_TITLE, FILE_NAME, FILE_PATH };
	ERMsg GetFileInfo(const std::string& file_path, CFileInfo& info);
	CFileInfo GetFileInfo(const std::string& file_path);
	//ERMsg GetFilesInfo(const std::vector<std::string>& filesList, CFileInfoVector& filesInfo);

	//void GetFilesInfo(const std::string& filter, bool bSubDirSearch, CFileInfoVector& filesInfo);
	CFileInfoVector GetFilesInfo(const std::vector<std::string>& files_list);


	std::time_t GetFileStamp(const std::string& file_path);

	std::vector<std::string> GetDirectoriesList(const std::string& directory_path, const std::string& filter = "*.*");
	std::vector<std::string> GetFilesList(const std::string& directory_path, const std::string& filter = "*.*", bool bSubDirSearch = false);
	//	void GetFilesList(const CFileInfoVector& filesInfo, int type, std::vector<std::string>& filesList);




	int GetCrc32(const std::string& str, uint64_t begin = NOT_INIT, uint64_t end = NOT_INIT);


	ERMsg WinExecWait(const std::string& exe_path, const std::string& arg, std::string working_dir = "", bool bShow = false, int* pExitCode = NULL);
	//	ERMsg CallApplication(std::string appType, std::string argument, HWND pCaller = NULL, int showMode = SW_HIDE, bool bAddCote = true, bool bWait = false);
	size_t GetTotalSystemMemory();
	//
	std::vector<std::string>::const_iterator FindStringExact(const std::vector<std::string>& list, const std::string& value, bool bCaseSensitive);

	std::string SecondToDHMS(double time);

	std::string FilePath2SpecialPath(const std::string& filePath, const std::string& appPath, const std::string& projectPath);
	std::string SpecialPath2FilePath(const std::string& filePath, const std::string& appPath, const std::string& projectPath);

	//	inline std::string GetErrorDescription(DWORD errnum)
	//	{
	//
	//		char cause[255] = { 0 };
	//
	//		FormatMessageA(
	//			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	//			NULL, errnum, 0, cause, 255, NULL);
	//
	//		return cause;
	//	}
	//
	//
	//	std::string GetErrorString(ERMsg msg, const char* sep = "\n");
	//
	//
	//	ERMsg GetErrorMsgFromString(const std::string& str, const char* sep = "\n");
	//	ERMsg GetLastErrorMessage();
	//
	//
	//	class CCriticalSection : public CRITICAL_SECTION
	//	{
	//	public:
	//
	//
	//		CCriticalSection()
	//		{
	//			InitializeCriticalSection(this);
	//		}
	//
	//		//to avoid shadow copy contructor
	//		CCriticalSection(const CCriticalSection& in)
	//		{
	//			InitializeCriticalSection(this);
	//		}
	//
	//		CCriticalSection& operator=(const CCriticalSection& in)
	//		{
	//			return *this;
	//		}
	//
	//		~CCriticalSection()
	//		{
	//			DeleteCriticalSection(this);
	//		}
	//
	//		void Enter()const
	//		{
	//			EnterCriticalSection(const_cast<CCriticalSection*>(this));
	//		}
	//		void Leave()const
	//		{
	//			LeaveCriticalSection(const_cast<CCriticalSection*>(this));
	//		}
	//
	//	};
	//
	//	inline ERMsg load(const std::string& filePath, zen::XmlDoc& doc) //throw XmlFileError, XmlParsingError
	//	{
	//		ERMsg msg;
	//		try
	//		{
	//			std::string filePathU8 = ANSI_UTF8(filePath);
	//			std::string stream = zen::loadStream(filePathU8); //throw XmlFileError
	//			doc = zen::parse(stream); //throw XmlParsingError
	//		}
	//		catch (const zen::XmlFileError& e)
	//		{
	//			// handle error
	//			msg.ajoute(GetErrorDescription(e.lastError));
	//		}
	//		catch (const zen::XmlParsingError& e)
	//		{
	//			// handle error
	//			msg.ajoute("Error in file : " + filePath);
	//			msg.ajoute("Error parsing XML file: col=" + to_string(e.col) + ", row=" + to_string(e.row));
	//		}
	//
	//		return msg;
	//	}
	//
	//	inline ERMsg save(const zen::XmlDoc& doc, const std::string& filepath, const std::string& lineBreak = "\r\n", const std::string& indent = "    ")
	//	{
	//		ERMsg msg;
	//		try
	//		{
	//			std::string filepathU8 = ANSI_UTF8(filepath);
	//			std::string stream = zen::serialize(doc, lineBreak, indent); //throw ()
	//			zen::saveStream(stream, filepathU8); //throw XmlFileError
	//		}
	//		catch (const zen::XmlFileError& e)
	//		{
	//			// handle error
	//			msg.ajoute(GetErrorDescription(e.lastError));
	//		}
	//		catch (const zen::XmlParsingError& e)
	//		{
	//			// handle error
	//			msg.ajoute("Error saving XML file: col=" + to_string(e.col) + ", row=" + to_string(e.row));
	//		}
	//
	//		return msg;
	//	}
	//
	//	//template <class T>
	//	//ERMsg XLoad(const std::string& filePath, T& x)
	//	//{
	//	//	ERMsg msg;
	//
	//	//	ifStream file;
	//
	//	//	msg = file.open(filePath);
	//	//	if (msg)
	//	//	{
	//	//		ios::pos_type length = file.length();
	//	//		std::string str;
	//	//		str.resize(length);
	//	//		file.read(&(str[0]), length);
	//	//
	//
	//	//		XDoc doc;
	//	//		doc.Load(str.c_str());
	//	//		x.SetXML(&doc);
	//	//	}
	//
	//	//	return msg;
	//	//}
	//
	//	//template <class T>
	//	//ERMsg XSave(const std::string& filePath, const T& x/*, std::string version="1.0"*/)
	//	//{
	//	//	ERMsg msg;
	//
	//	//	ofStream file;
	//
	//	//	msg = file.open(filePath);
	//
	//	//	if (msg)
	//	//	{
	//	//		XDoc doc;
	//
	//	//		doc.LoadProcessingInstrunction("<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
	//	//		LPXNode pRoot = &doc;
	//	//		x.GetXML(pRoot);
	//
	//	//		std::string str = doc.GetXML();
	//	//		file.write(str.c_str(), UINT(str.length()));
	//
	//	//		doc.close();
	//	//	}
	//
	//	//	return msg;
	//	//}
	//
	//	//template <class T>
	//	//void XReadStream(std::istream& stream, T& x)
	//	//{
	//	//	ERMsg msg;
	//
	//	//	long size = 0;
	//	//	stream.read((char*)(&size), sizeof(size));
	//
	//	//	std::string str;
	//	//	str.resize(size);
	//	//	stream.read((char*)(&(str[0])), size);
	//
	//	//	XDoc doc;
	//	//	PARSEINFO pi;
	//	//	doc.Load(str.c_str(), &pi);
	//	//	if (!pi.error_occur)
	//	//		x.SetXML(&doc);
	//
	//
	//	//	doc.Close();
	//	//}
	//
	//	//template <class T>
	//	//void XWriteStream(std::ostream& stream, const T& x)
	//	//{
	//	//	ERMsg msg;
	//
	//	//	XDoc doc;
	//	//	doc.LoadProcessingInstrunction("<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
	//	//	LPXNode pRoot = &doc;
	//	//	x.GetXML(pRoot);
	//
	//	//	DISP_OPT op; op.newline = false;
	//	//	std::string str = doc.GetXML(&op);
	//	//	long size = (long)str.length();
	//
	//	//	stream.write((char*)(&size), sizeof(size));
	//	//	stream.write(str.c_str(), size);
	//
	//	//	doc.Close();
	//	//}
	//
	//
	//	//*********************************************************************************************************************
	//	//ifStream
	inline std::string getLastLine(std::ifstream& in)
	{
		std::string line;
		while (in >> std::ws && std::getline(in, line)) // skip empty lines
			;

		return line;
	}


	//by default deny read and write
	class ofStream : public std::ofstream
	{
	public:

		ERMsg open(const std::string& filePath,
			std::ios_base::openmode _Mode = ofStream::out)
		{
			ERMsg msg;

			std::ofstream::open(filePath, _Mode);

			if (fail())
			{
				//msg = GetLastErrorMessage();
				msg.ajoute("Unable to open: " + filePath);
			}


			return msg;
		}

		using std::ofstream::write;
		std::ostream& write(const std::string& str)
		{
			return std::ofstream::write(&(str[0]), str.length());
		}

		template <typename T>
		void write_value(const T& v)
		{
			write((char*)&v, sizeof(v));
		}

		void write_buffer(const std::string& buffer)
		{
			size_t size = buffer.size();
			write_value(size);
			write(buffer.c_str(), size);
		}

	};


	//by default share deny write
	class ifStream : public std::ifstream
	{
	public:


		ERMsg open(const std::string& filePath,
			std::ios_base::openmode _Mode = ifStream::in)
		{
			ERMsg msg;

			std::ifstream::open(filePath, _Mode);

			if (fail())
			{
				//msg = GetLastErrorMessage();
				msg.ajoute("Unable to open: " + filePath);
			}

			return msg;
		}

		std::ios::pos_type length()const
		{
			ifStream& me = const_cast<ifStream&>(*this);
			std::ios::pos_type pos = me.tellg();
			me.seekg(0, std::ios::end);
			std::ios::pos_type length = me.tellg();
			me.seekg(pos);

			return length;
		}

		std::string GetText(bool bBinary = false)
		{
			std::string contents;
			seekg(0, std::ios::end);
			contents.resize((size_t)tellg());
			seekg(0, std::ios::beg);
			read(&contents[0], contents.size());
			if (bBinary == false)
				contents.resize(strlen(contents.c_str()));

			return contents;
		}

		template <typename T>
		T read_value()
		{
			T v = 0;
			read((char*)&v, sizeof(v));

			return v;
		}

		std::string read_buffer()
		{
			std::string buffer;
			size_t size = read_value<size_t>();
			buffer.resize((size_t)size);
			read((char*)(intptr_t)(buffer[0]), size);

			return buffer;
		}




	};




	//by default share deny write
	class fStream : public std::fstream
	{
	public:


		ERMsg open(const std::string& filePath,
			std::ios_base::openmode _Mode = std::fstream::in | std::fstream::out | std::fstream::app)
		{
			ERMsg msg;

			std::fstream::open(filePath.c_str(), _Mode);//, _Prot
			//this->set

			if (fail())
			{
				//msg = GetLastErrorMessage();
				msg.ajoute("Unable to open: " + filePath);
			}

			return msg;
		}

		uint64_t lengthg()const
		{
			fStream& me = const_cast<fStream&>(*this);
			std::ios::pos_type pos = me.tellg();
			me.seekg(0, std::ios::end);
			std::ios::pos_type length = me.tellg();
			me.seekg(pos);

			return uint64_t(length);
		}
		uint64_t lengthp()const
		{
			fStream& me = const_cast<fStream&>(*this);
			std::ios::pos_type pos = me.tellp();
			me.seekp(0, std::ios::end);
			std::ios::pos_type length = me.tellp();
			me.seekp(pos);

			return uint64_t(length);
		}

		//********************************
		template <typename T>
		void write_value(const T& v)
		{
			write((char*)&v, sizeof(v));
		}

		void write_buffer(const std::string& buffer)
		{
			uint64_t size = buffer.size();
			write_value(size);
			write(buffer.c_str(), size);
		}
		//********************************
		template <typename T>
		T read_value()
		{
			T v = 0;
			read((char*)&v, sizeof(v));

			return v;
		}

		template <typename T>
		void read_value(const T& v)
		{
			read((char*)&v, sizeof(v));
		}
		std::string read_buffer()
		{
			std::string buffer;
			uint64_t size = read_value<uint64_t >();
			buffer.resize((size_t)size);
			read((char*)(buffer.data()), size);

			return buffer;
		}
	};

	
	



}//WBSF


//*********************************************************************************************************************
//zen

//namespace zen
//{
//	template <> inline
//		void writeStruc(const WBSF::std::vector<std::string>& in, XmlElement& output)
//	{
//		output.setValue(WBSF::to_string(in));
//	}
//
//	template <> inline
//		bool readStruc(const XmlElement& input, WBSF::std::vector<std::string>& out)
//	{
//		std::string str;
//		input.getValue(str);
//		out = WBSF::Tokenize(str, "{,}");
//
//		return true;
//	}
//}

