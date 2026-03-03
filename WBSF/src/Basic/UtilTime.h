//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//
//******************************************************************************
#pragma once

#include <cassert>
#include <vector>
#include <string>

#include <boost/dynamic_bitset.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include "Basic/ERMsg.h"
#include "Basic/UtilStd.h"




namespace WBSF
{

	//class CJDayRef;

	//enum {  = 31 };

	enum TMonth { JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER, NB_MONTHS };
	enum TDayOfMonth { DAY_01, DAY_02, DAY_03, DAY_04, DAY_05, DAY_06, DAY_07, DAY_08, DAY_09, DAY_10, DAY_11, DAY_12, DAY_13, DAY_14, DAY_15, DAY_16, DAY_17, DAY_18, DAY_19, DAY_20, DAY_21, DAY_22, DAY_23, DAY_24, DAY_25, DAY_26, DAY_27, DAY_28, DAY_29, DAY_30, DAY_31, MAX_DAY_PER_MONTH };
	enum TShortMonth { JAN, FEB, MAR, APR, /*MAY,:already define*/ JUN = JUNE, JUL, AUG, SEP, OCT, NOV, DEC };
	//enum { DAY_01 = 0, LAST_DAY = -2, DAY_NOT_INIT = -1 };
	//enum { JANUARY = 0, DECEMBER = 11, MONTH_NOT_INIT = -1 };
	//enum { 0 = 0, 23 = 23, HOUR_NOT_INIT = -1 };
	constexpr int YEAR_NOT_INIT = 9999;
	constexpr int REF_NOT_INIT = -32767;

	//don't use these variable directly : use interface
	//static const size_t DAYS_IN_MONTH[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	//static const size_t FIRST_DOY_INDEX[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	//static const size_t FIRST_DOY_LEAP_INDEX[12] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
	//static const double MID_DOY_IN_MONTH[12] = { 15.5, 45.0, 74.5, 105.0, 135.5, 166.0, 196.5, 227.5, 258.0, 288.5, 319.0, 349.5 };




	const char* GetMonthName(size_t m, bool bFull = true);
	inline const char* GetMonthTitle(size_t m, bool bFull = true)
	{
		return GetMonthName(m, bFull);
	}

	double MidDOYForMonth(size_t m);
	bool IsLeap(int year);
	size_t GetNbDaysPerYear(int year);
	size_t GetNbDaysPerMonth(int year, size_t m);
	inline size_t GetLastDayOfMonth(int year, size_t m) {return GetNbDaysPerMonth(year, m) - 1;}
	size_t GetDOY(int year, size_t m, size_t d);
	size_t GetFirstDOY(int year, size_t m);
	size_t GetLastDOY(int year, size_t m);
	size_t GetDayOfTheMonth(int year, size_t DOY);
	size_t GetMonth(int year, size_t DOY);
	size_t GetMonth(const char* month);



	inline size_t GetNbDaysPerMonth(size_t m)
	{
		return GetNbDaysPerMonth(YEAR_NOT_INIT, m);
	}

	inline size_t GetDOY(size_t m, size_t d)
	{
		return GetDOY(YEAR_NOT_INIT, m, d);
	}

	inline size_t GetFirstDOY(size_t m)
	{
		return GetFirstDOY(YEAR_NOT_INIT, m);
	}

	inline size_t GetLastDOY(size_t m)
	{
		return GetLastDOY(YEAR_NOT_INIT, m);
	}

	inline size_t GetDayOfTheMonth(size_t DOY)
	{
		return GetDayOfTheMonth(YEAR_NOT_INIT, DOY);
	}
	inline size_t GetMonth(size_t DOY)
	{
		return GetMonth(YEAR_NOT_INIT, DOY);
	}


	std::string GetCurrentTimeString(const std::string format);
	std::string FormatTime(const std::string& format, std::time_t t);
	std::string FormatTime(const std::string& format, int year, size_t m, size_t d, size_t h, size_t minute = 0, size_t second = 0);
	std::time_t get_time64(int year, size_t m, size_t d, size_t h, size_t minute = 0, size_t second = 0);

	//For backward compatibility
	//[[deprecated("Use GetDOY instead.")]]
	//size_t GetJDay(int year, size_t m, size_t d)
	//{
	//    return GetDOY(year, m, d);
	//}
	//
	//[[deprecated("Use GetDOY instead.")]]
	//size_t GetJDay(size_t m, size_t d)
	//{
	//    return GetDOY(m, d);
	//}



	int32_t GetRef(int year, size_t m, size_t d);
	void SetRef(int32_t int32_ref, int& year, size_t& m, size_t& d);


	std::string GetTimeSpanStr(double sec);



	class CTM
	{
	public:

		//enum TTemporal { TEMPORAL, /*ATEMPORAL,*/ NB_TEMPORAL=2 };
		//static_assert();
		enum TType { ANNUAL, MONTHLY, DAILY, HOURLY, ATEMPORAL, NB_REFERENCE, UNKNOWN = 15 };
		enum TMode { FOR_EACH_YEAR, FOR_EACH_REF = FOR_EACH_YEAR, OVERALL_YEARS, OVERALL_REF = OVERALL_YEARS, NB_MODE };
		enum TTranformType { FIRST_TREF, MID_TREF, LAST_TREF, NB_TRANSFORM_INIT };


		const char* GetTypeModeName()const
		{
			return TYPE_MODE_NAME[m_type][m_mode];
		}

		//static void ReloadString();
		static const char* GetTypeName(size_t TType);
		static const char* GetModeName(size_t TMode);
		//static const char* GetTypeTitle(size_t TType);
		//static const char* GetModeTitle(size_t TMode);
	//    static bool IsTypeAvailable(CTM TM1, size_t t)
	//    {
	//        return AVAILABILITY[TM1.i()][CTM(t, FOR_EACH_YEAR).i()] || AVAILABILITY[TM1.i()][CTM(t, OVERALL_YEARS).i()];
	//    }
		static bool IsAvailable(CTM TM1, CTM TM2)
		{
			return AVAILABILITY[TM1.i()][TM2.i()];
		}


		//    bool IsTypeAvailable(int16_t t)const
		//    {
		//        return IsTypeAvailable(*this, t);
		//    }
		//    bool IsModeAvailable(CTM TM)const
		//    {
		//        return IsModeAvailable(*this, TM);
		//    }
		//    size_t GetNbTimeReferences()const
		//    {
		//        assert(is_valid());
		//        return NB_REFERENCES[m_type][m_mode];
		//    }

		CTM(size_t t = UNKNOWN, size_t m = FOR_EACH_YEAR)
		{
			m_type = (uint32_t)t;
			m_mode = (uint32_t)m;
			assert(is_valid());
		}

		CTM(const CTM& in)
		{
			operator=(in);
		}

		CTM& operator=(const CTM& in)
		{
			assert(in.is_valid());

			m_type = in.m_type;
			m_mode = in.m_mode;
			assert(is_valid());

			return *this;
		}

		CTM& operator=(const std::string& in)
		{
			clear();

			std::vector<std::string> tmp = Tokenize(in, " ");
			if (tmp.size() == 2)
			{
				m_type = stoi(tmp[0]);
				m_mode = stoi(tmp[1]);
				assert(is_valid());
			}

			return *this;
		}

		bool operator==(const CTM& TM)const
		{
			return m_type == TM.m_type && m_mode == TM.m_mode;
		}
		bool operator!=(const CTM& TM)const
		{
			return !operator==(TM);
		}

		std::string GetTypeName()const
		{
			return GetTypeName(Type());
		}
		std::string GetModeName()const
		{
			return GetModeName(Mode());
		}

		void clear()
		{
			m_type = UNKNOWN;
			m_mode = FOR_EACH_YEAR;
		}
		bool is_init()const
		{
			assert(is_valid());
			return m_type != UNKNOWN;
		}

		static bool is_valid(size_t type, size_t mode)
		{
			bool type_valide = type == UNKNOWN || type < NB_REFERENCE;
			bool mode_valide = mode < NB_MODE;

			return type_valide && mode_valide;
		}
		bool is_valid()const
		{
			return is_valid(m_type, m_mode);
		}
		bool IsAtemporal()const
		{
			return m_type == ATEMPORAL;
		}
		bool IsTemporal()const
		{
			return m_type != UNKNOWN && m_type != ATEMPORAL;
		}
		//    bool IsAnnual()const
		//    {
		//        return m_type != UNKNOWN&&m_mode == FOR_EACH_YEAR;
		//    }
		//    bool HaveYear()const
		//    {
		//        return IsAnnual() && m_type != ATEMPORAL;
		//    }
		//    bool HaveMonth()const
		//    {
		//        return m_type >= MONTHLY&&m_type != ATEMPORAL;
		//    }
		//    bool HaveDay()const
		//    {
		//        return m_type >= DAILY&&m_type != ATEMPORAL;
		//    }
		//    bool HaveHour()const
		//    {
		//        return m_type >= HOURLY&&m_type != ATEMPORAL;
		//    }

		bool IsHourly()const
		{
			return m_type == HOURLY;
		}
		bool IsDaily()const
		{
			return m_type == DAILY;
		}
		size_t Type()const
		{
			return (size_t)m_type;
		}
		size_t Mode()const
		{
			return (size_t)m_mode;
		}

		void Type(size_t in)
		{
			assert(in < NB_REFERENCE);
			m_type = (int32_t)in;
		}
		void Mode(size_t in)
		{
			assert(in < NB_MODE);
			m_mode = (int32_t)in;
		}

		std::string to_string()const
		{
			return std::to_string(m_type) + " " + std::to_string(m_mode);
		}
		std::istream& operator << (std::istream& io)
		{
			io.read((char*)(&m_type), sizeof(m_type));
			io.read((char*)(&m_mode), sizeof(m_mode));

			return io;
		}
		std::ostream& operator >> (std::ostream& io)const
		{
			io.write((char*)(&m_type), sizeof(m_type));
			io.write((char*)(&m_mode), sizeof(m_mode));

			return io;
		}
		friend std::ostream& operator<<(std::ostream& io, const CTM& TM)
		{
			TM >> io;
			return io;
		}
		friend std::istream& operator>>(std::istream& io, CTM& TM)
		{
			TM << io;
			return io;
		}

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using boost::serialization::make_nvp;
			ar& make_nvp("type", m_type);
			ar& make_nvp("mode", m_mode);
		}



	protected:

		size_t i()const
		{
			return (m_type != UNKNOWN) ? (m_type + (m_mode * NB_REFERENCE)) : NOT_INIT;
		}
		//operator int16_t()const{ return (m_type != UNKNOWN) ? (m_type + (m_mode*NB_REFERENCE)) : -1; }

		//size_t m_type;
		//size_t m_mode;
		//struct
		//{
		//uint8_t m_type : 4;
		//uint8_t m_mode : 4;
		//};

		uint32_t m_type;
		uint32_t m_mode;

		static const char* TYPE_NAME[8];
		static const char* MODE_NAME[NB_MODE];
		static const bool AVAILABILITY[NB_REFERENCE * NB_MODE][NB_REFERENCE * NB_MODE];
		static const char* TYPE_MODE_NAME[8][NB_MODE];
		static const char NB_REFERENCES[8][NB_MODE];

		//static std::vector<std::string> TYPE_TITLE;
		//static std::vector<std::string> MODE_TITLE;
	};



	class CTRefFormat
	{
	public:

		enum TFormat { YEAR, MONTH, DAY, JDAY, HOUR, REFERENCE, DATE_FORMAT, NB_FORMAT };
		static const char* GetFormatName(size_t i)
		{
			assert(i < NB_FORMAT);
			return FORMAT_NAME[i];
		}
		
		static std::vector<std::string> GetTimeReference(const std::vector<std::string>& header);

		//static const char* DATE_YMD;
		//static const char* DATE_YMDH;
		//static const char* DATE_YMD_HMS;

		static const char* GetDefaultHeader(CTM tm)
		{
			return DEFAULT_HEADER[tm.Type()][tm.Mode()];
		}
		static const char* GetDefaultFormat(CTM tm)
		{
			return DEFAULT_FORMAT[tm.Type()][tm.Mode()];
		}


		CTRefFormat()
		{
			for (size_t t = 0; t < CTM::NB_REFERENCE; t++)
			{
				for (size_t m = 0; m < CTM::NB_MODE; m++)
				{
					m_header[t][m] = DEFAULT_HEADER[t][m];
					m_format[t][m] = DEFAULT_FORMAT[t][m];
				}
			}
		}

		const char* GetHeader(CTM tm)const
		{
			assert(tm.is_init());
			return m_header[tm.Type()][tm.Mode()].c_str();
		}
		void SetHeader(CTM tm, const char* format)
		{
			m_header[tm.Type()][tm.Mode()] = format;
		}

		const char* GetFormat(CTM tm)const
		{
			assert(tm.is_init());
			return m_format[tm.Type()][tm.Mode()].c_str();
		}
		void SetFormat(CTM tm, const char* format)
		{
			m_format[tm.Type()][tm.Mode()] = format;
		}

		std::string GetMissingString(CTM tm)const;

		
		

	protected:

		std::string m_format[CTM::NB_REFERENCE][CTM::NB_MODE];
		std::string m_header[CTM::NB_REFERENCE][CTM::NB_MODE];
		static const char* DEFAULT_FORMAT[CTM::NB_REFERENCE][CTM::NB_MODE];
		static const char* DEFAULT_HEADER[CTM::NB_REFERENCE][CTM::NB_MODE];

		static const char* FORMAT_NAME[NB_FORMAT];
	};


	class CTRef;
	CTRef FromFormatedString(std::string str, const std::string& format = "", const char* sep = "- :", int base = 1);
	//Temporal reference
	class CTRef
	{
	public:




		static const CTRefFormat& GetFormat()
		{
			return TREF_FORMAT;
		}
		static void SetFormat(const CTRefFormat& format)
		{
			TREF_FORMAT = format;
		}

		static CTRef GetCurrentTRef(CTM TM = CTM(CTM::DAILY), bool bUTC = false);



		//    [[deprecated("Don't use this.")]]
		//    operator const float()const
		//    {
		//        assert(false);//todo
		//        float d = float (get_int32());
		//        const void* pBits = &d;
		//        //const void* pBits = this;
		//        return *((float*)pBits);
		//    }
		//
		//    [[deprecated("Don't use this.")]]
		//    operator const double()const
		//    {
		//        double d = double(get_int32());
		//        const void* pBits = &d;
		//        return *((double*)pBits);
		//    }


		CTRef();
		CTRef(const CTRef& in)
		{
			operator=(in);
		}


		//    [[deprecated("Don't use.")]]
		//    explicit CTRef(float in);
		//    [[deprecated("Don't use.")]]
		//    explicit CTRef(double in);

		//    int32_t get_int32()const
		//    {
		//        assert(false);//toto
		//        const void* pBits = this;
		//        return *((int32_t*)pBits);
		//    }
		//
		//    void set_int32(int32_t in)
		//    {
		//        assert(false);//toto
		//        const void* pBits = this;
		//        *((int32_t*)pBits) = in;
		//
		//    }

			//explicit CTRef(int y);

		CTRef(int year);
		CTRef(int year, size_t m);
		CTRef(int year, size_t m, size_t d);
		CTRef(int year, size_t m, size_t d, size_t h);
		CTRef(int year, size_t m, size_t d, size_t h, const CTM& TM);
		CTRef(std::string str);
		CTRef(int32_t Tref, const CTM& TM)
		{
			SetRef(Tref, TM);
		}
		//CTRef(std::string str, CTM TM);

		CTRef& Set(int y_or_r, size_t m, size_t d, size_t h, const CTM& TM);

		int32_t GetRef()const;
		void SetRef(int32_t Tref, const CTM& TM);

		//CTRef Disaggregate(const CTM& TM)const;
		bool is_init()const
		{
			return m_TM.is_init();
		}

		void clear();
		void ResetUnused();



		const CTM& TM()const
		{
			return m_TM;
		}

		void TM(const CTM& TM)
		{
			m_TM = TM;
			ResetUnused();
			//m_type = TM.Type();
			//m_mode = TM.Mode();
		}


		// [[deprecated("Don't use this.")]]
		 //CTRef& operator =(float in);
		 //[[deprecated("Don't use this.")]]
		 //CTRef& operator =(double in);
		CTRef& operator =(const CTRef& in);
		bool operator == (const CTRef& in)const;
		bool operator != (const CTRef& in)const
		{
			return !operator==(in);
		}
		bool operator > (const CTRef& in)const
		{
			assert(in.TM() == TM());
			return ((is_init() && in.is_init()) ? GetRef() > in.GetRef() :false);
		}
		bool operator < (const CTRef& in)const
		{
			assert(in.TM() == TM());
			return ((is_init() && in.is_init()) ? GetRef() < in.GetRef() : false);
		}
		bool operator >= (const CTRef& in)const
		{
			assert(in.TM() == TM());
			return ((is_init() && in.is_init()) ? GetRef() >= in.GetRef() : false);
		}
		bool operator <= (const CTRef& in)const
		{
			assert(in.TM() == TM());
			return ((is_init() && in.is_init()) ? GetRef() <= in.GetRef() : false);
		}
		CTRef& operator+=(int32_t nbRef)
		{
			return Shift(nbRef);
		}
		CTRef& operator-=(int32_t nbRef)
		{
			return Shift(-nbRef);
		}
		int32_t operator-(const CTRef& in)const;
		CTRef operator+(int32_t nbRef)const;
		CTRef operator-(int32_t nbRef)const;
		//    CTRef operator+(long nbRef)const
		//    {
		//        return operator+(int64_t(nbRef));
		//    }
		//    CTRef operator-(long nbRef)const
		//    {
		//        return operator+(int64_t(-nbRef));
		//    }
		//    CTRef operator+(size_t nbRef)const
		//    {
		//        return operator+(int64_t(nbRef));
		//    }
		//    CTRef operator-(size_t nbRef)const
		//    {
		//        return operator-(int64_t(nbRef));
		//    }
		CTRef& operator++(int)
		{
			return Shift(1);
		}
		CTRef& operator--(int)
		{
			return Shift(-1);
		}
		CTRef& Shift(int32_t nbRef);


		static CTRef as(CTRef Tref, const CTM& TM, size_t initType = CTM::FIRST_TREF);
		CTRef as(const CTM& TM, size_t initType = CTM::FIRST_TREF)const
		{
			return as(*this, TM, initType);
		}

		//    bool IsInversed(CTRef in)const
		//    {
		//        bool bRep = false;
		//        if (m_mode == FOR_EACH_YEAR)
		//        {
		//            in.m_year = m_year;//remove year and look if it's greater
		//            bRep = *this > in;
		//        }
		//
		//        return bRep;
		//    }

		//    bool IsAnnual()const
		//    {
		//        return m_TM.IsAnnual();
		//    }

		//    bool HaveYear()const
		//    {
		//        return IsAnnual() && m_type != ATEMPORAL;
		//    }
		//    bool HaveMonth()const
		//    {
		//        return m_type >= MONTHLY&&m_type != ATEMPORAL;
		//    }
		//    bool HaveDay()const
		//    {
		//        return m_type >= DAILY&&m_type != ATEMPORAL;
		//    }
		//    bool HaveHour()const
		//    {
		//        return m_type >= HOURLY&&m_type != ATEMPORAL;
		//    }
		bool IsTemporal()const
		{
			return m_TM.IsTemporal();
		}
		bool IsAtemporal()const
		{
			return m_TM.IsAtemporal();
		}
		//    bool IsReference()const
		//    {
		//        return m_type == ATEMPORAL;
		//    }

		std::string to_string()const;
		void from_string(const std::string& str);


		static std::string GetMissingString(CTM tm);

		std::string GetFormatedString(std::string format = "")const;
		CTRef& FromFormatedString(std::string str, const std::string& format = "", const char* sep = "- :", int base = 1)
		{
			operator=(WBSF::FromFormatedString(str, format, sep, base));
			return *this;
		}


		static bool is_valid(int y_or_r, size_t m, size_t d, size_t h, size_t type, size_t mode = CTM::FOR_EACH_YEAR);
		bool is_valid()const
		{
			return is_valid(m_year, m_month, m_day, m_hour, m_TM.Type(), m_TM.Mode());
		}

		//    size_t GetType()const
		//    {
		//        return m_type;
		//    }
		//    size_t GetMode()const
		//    {
		//        return m_mode;
		//    }
		size_t GetHour()const
		{
			return is_init() ? size_t(m_hour) : 0;
		}
		size_t GetDay()const
		{
			return is_init() ? size_t(m_day) : 0;
		}
		size_t GetMonth()const
		{
			return is_init() ? size_t(m_month) : 0;
		}
		int GetYear()const
		{
			return is_init() ? int(m_year) : 0;
		}

		size_t GetDOY()const
		{
			assert(m_TM.IsDaily() || m_TM.IsHourly());
			return (m_TM.IsDaily() || m_TM.IsHourly()) ? WBSF::GetDOY(m_year, m_month, m_day) : 0;
		}
		[[deprecated("Use GetDOY instead.")]]
		size_t GetJDay()const
		{
			return GetDOY();
		}
		void SetDOY(int year, size_t DOY);
		void SetDOY(size_t DOY)
		{
			SetDOY(YEAR_NOT_INIT, DOY);
		}

		[[deprecated("Use GetDOY instead.")]]
		void SetJDay(int year, size_t DOY)
		{
			SetDOY(year, DOY);
		}
		[[deprecated("Use SetDOY instead.")]]
		void SetJDay(size_t DOY)
		{
			SetDOY(DOY);
		}

		std::time_t get_time64()const;




		//put member protected????
		//    protected:

		CTM m_TM;
		int16_t m_year;   // -32767..32768
		uint8_t m_month;  // 0..256
		uint8_t m_day;    // 0..256
		uint8_t m_hour;   // 0..256
		int16_t m_ref;   // -32767..32768



		//    union
		//    {
		//        struct
		//        {
		//            int32_t m_year : 16;   // -32767..32768  (16 bits)
		//            uint32_t m_month : 4;  // 0..16 (4 bits)
		//            uint32_t m_day : 5;    // 0..32 (5 bits)
		//            uint32_t m_hour : 5;    // 0..32 (5 bits)
		//            //uint32_t m_type : 3;    // 0..7 (3 bits)
		//            uint32_t m_temporal : 1;    // 0..1 (1 bits)
		//            //uint32_t m_mode : 1;    // 0..1 (1 bits)
		//            uint32_t m_unused : 1;    // 0..1 (1 bits)
		//        };
		//
		//        struct
		//        {
		//            uint32_t m_overall: 16;   //-32767 for overall reference;
		//            uint32_t m_ref : 15;    // 0..32768 (16 bits)
		//            uint32_t m_atemporal : 1;    // 0..1 (1 bits)
		//            //uint32_t _temporal : 1;    // 0..1 (1 bits)
		//            //uint32_t _mode : 1;	  // mode
		//            //uint32_t m_unused : 2;    // 0..1 (1 bits)
		//            //uint32_t _temporal : 1;   // 0..1 (1 bits)
		//            //uint32_t _unused : 1;     // 0..1 (1 bits)
		////            uint32_t _type : 3;     // type
		//  //          uint32_t _mode : 1;	  // mode
		//        };
		//    };

		size_t GetNbDaysPerYear()const
		{
			return WBSF::GetNbDaysPerYear(m_year);
		}
		size_t GetNbDaysPerMonth()const
		{
			return GetNbDaysPerMonth(m_year, m_month);
		}

		bool IsLeap()const
		{
			return IsLeap(m_year);
		}

		//static method
		static const char* GetMonthName(size_t m, bool bFull = true)
		{
			return WBSF::GetMonthName(m, bFull);
		}
		static size_t GetNbDaysPerMonth(size_t m)
		{
			return WBSF::GetNbDaysPerMonth(m);
		}
		//    static double MidDOYForMonth(size_t m)
		//    {
		//        return WBSF::MidDOYForMonth(m);
		//    }
		static int16_t IsLeap(int year)
		{
			return WBSF::IsLeap(year);
		}
		static size_t GetNbDaysPerYear(int year)
		{
			return WBSF::GetNbDaysPerYear(year);
		}
		static size_t GetNbDaysPerMonth(int year, size_t m)
		{
			return WBSF::GetNbDaysPerMonth(year, m);
		}

		static size_t GetDOY(int year, size_t m, size_t d)
		{
			return WBSF::GetDOY(year, m, d);
		}
		static size_t GetDOY(size_t m, size_t d)
		{
			return WBSF::GetDOY(m, d);
		}
		[[deprecated("Use GetDOY instead.")]]
		static size_t GetJDay(int year, size_t m, size_t d)
		{
			return WBSF::GetDOY(year, m, d);
		}
		[[deprecated("Use GetDOY instead.")]]
		static size_t GetJDay(size_t m, size_t d)
		{
			return WBSF::GetDOY(m, d);
		}

		static size_t GetFirstDOY(int year, size_t m)
		{
			return WBSF::GetFirstDOY(year, m);
		}
		static size_t GetFirstDOY(size_t m)
		{
			return WBSF::GetFirstDOY(m);
		}
		static size_t GetLastDOY(int year, size_t m)
		{
			return WBSF::GetLastDOY(year, m);
		}
		static size_t GetLastDOY(size_t m)
		{
			return WBSF::GetLastDOY(m);
		}

		static size_t GetDayOfTheMonth(int year, size_t DOY)
		{
			return WBSF::GetDayOfTheMonth(year, DOY);
		}
		static size_t GetDayOfTheMonth(size_t DOY)
		{
			return WBSF::GetDayOfTheMonth(DOY);
		}
		static size_t GetMonth(int year, size_t DOY)
		{
			return WBSF::GetMonth(year, DOY);
		}
		static size_t GetMonth(const char* month)
		{
			return WBSF::GetMonth(month);
		}
		static size_t GetMonth(size_t DOY)
		{
			return WBSF::GetMonth(DOY);
		}

		int32_t GetRef(int year, size_t m, size_t d);
		void SetRef(int32_t ref, int& year, size_t& m, size_t& d);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using boost::serialization::make_nvp;
			//    int32_t i = get_int32();
			  //  ar & i;
				//set_int32(i);
			ar& make_nvp("CTM", m_TM);
			if (m_TM.IsTemporal())
			{
				ar& make_nvp("year", m_year);
				ar& make_nvp("month", m_month);
				ar& make_nvp("day", m_day);
				ar& make_nvp("hour", m_hour);
			}
			else
			{
				ar& make_nvp("ref", m_ref);
			}
		}


		std::istream& operator << (std::istream& io)
		{
			m_TM << io;
			io.read((char*)(&m_year), sizeof(m_year));
			io.read((char*)(&m_month), sizeof(m_month));
			io.read((char*)(&m_day), sizeof(m_day));
			io.read((char*)(&m_hour), sizeof(m_hour));
			io.read((char*)(&m_ref), sizeof(m_ref));

			return io;
		}

		std::ostream& operator >> (std::ostream& io)const
		{
			io << m_TM;
			io.write((char*)(&m_year), sizeof(m_year));
			io.write((char*)(&m_month), sizeof(m_month));
			io.write((char*)(&m_day), sizeof(m_day));
			io.write((char*)(&m_hour), sizeof(m_hour));
			io.write((char*)(&m_ref), sizeof(m_ref));

			return io;
		}


		friend std::istream& operator >>(std::istream& io, CTRef& Tref)
		{
			Tref << io;
			return io;
		}
		friend std::ostream& operator <<(std::ostream& io, const CTRef& Tref)
		{
			Tref >> io;
			return io;
		}


	protected:

		//static int16_t TM(int16_t t, int16_t m){ assert(t>=0&&t<NB_REFERENCE);assert(m>=0&&m<NB_MODE);return t+(m*NB_REFERENCE);}
		//CTM TM()const{ assert(is_init());return CTM(m_type, m_mode);}

		static CTRefFormat TREF_FORMAT;

	};


	inline int GetCurrentYear()
	{
		return CTRef::GetCurrentTRef().GetYear();
	}




	class CARef : public CTRef
	{
	public:

		CARef(int32_t no) :
			CTRef(no, 0, 0, 0, CTM::ATEMPORAL)
		{
			//        m_TM = CTM(CTM::ATEMPORAL, no==REF_NOT_INIT?CTM::FOR_EACH_REF:CTM::OVERALL_REF);
			  //      m_ref = no;
		}
	};

	class CDOYRef : public CTRef
	{
	public:

		CDOYRef(size_t DOY)
		{
			SetDOY(DOY);
		}

		CDOYRef(int year, size_t DOY)
		{
			SetDOY(year, DOY);
		}
	};


	class CTimeRef : public CTRef
	{
	public:

		CTimeRef(std::time_t t, CTM TM = CTM::DAILY, bool bUTC = false);


		std::time_t m_time;
	};


	typedef std::vector<CTRef>CTRefVector;

	//*************************************************************
	//CTPeriod
	class CTPeriod;
	CTPeriod FromFormatedString(std::string str, std::string periodformat = "%1%|%2%", std::string TRefFormat = "", const char* sep = "- :", int base = 1);

	class CTPeriod
	{
	public:

		enum TSegment{ YEAR_BY_YEAR, CONTINUOUS, NB_SEGMENTS_TYPE };

		static bool is_valid(const CTRef& Tref1, const CTRef& Tref2);


		//default constructor with not init reference
		CTPeriod()
		{
		}
		CTPeriod(const CTRef& begin, const CTRef& end);
		CTPeriod(const std::string& str);
		CTPeriod(const std::string& str1, const std::string& str2);

		CTPeriod(const CTPeriod& in)
		{
			operator = (in);
		}

		CTPeriod& operator = (const CTPeriod& in);

		void clear();


		size_t size()const
		{
			return is_init() ? (m_end - m_begin) + 1 : 0;
		}
		const CTRef& begin()const
		{
			return m_begin;
		}
		CTRef& begin()
		{
			return m_begin;
		}
		const CTRef& end()const
		{
			return m_end;
		}
		CTRef& end()
		{
			return m_end;
		}

		size_t length(CTM TM)const {return is_init() ? as(TM).size(): 0;}
		bool operator == (const CTPeriod& in)const;
		bool operator != (const CTPeriod& in)const
		{
			return !operator==(in);
		}
		CTPeriod& operator += (const CTRef& in)
		{
			return inflate(in);
		}
		CTPeriod& operator += (const CTPeriod& in)
		{
			return inflate(in);
		}
		CTRef operator[](size_t i)const
		{
			return is_init() && is_inside(m_begin + (int32_t)i) ? (m_begin + (int32_t)i) : CTRef();
		}
		size_t operator[](const CTRef& t)const
		{
			assert(t.TM() == TM());
			return is_init() && is_inside(t) ? size_t(t.GetRef() - m_begin.GetRef()) : NOT_INIT;
		}

		CTPeriod& operator |= (const CTPeriod& in)
		{
			*this = unions(*this, in);
			return *this;
		}

		CTPeriod& operator &= (const CTPeriod& in)
		{
			*this = intersect(*this, in);
			return *this;
		}


		//    int32_t GetLength()const
		//    {
		//        return is_init() ? (m_end - m_begin) + 1 : 0;
		//    }

		CTPeriod& inflate(const CTRef& in);
		CTPeriod& inflate(const CTPeriod& in);

		//    int16_t GetType()const
		//    {
		//        return m_type;
		//    }
		//    void SetType(int16_t type)
		//    {
		//        m_type = type;
		//    }
		//    int16_t GetTType()const
		//    {
		//        assert(m_begin.TM().Type() == m_end.TM().Type());
		//        return m_begin.TM().Type();
		//    }
		//    int16_t GetTMode()const
		//    {
		//        assert(m_begin.TM().Mode() == m_end.TM().Mode());
		//        return m_begin.TM().Mode();
		//    }

		int GetFirstYear()const
		{
			return m_begin.m_year;
		}
		int GetLastYear()const
		{
			return m_end.m_year;
		}
		size_t GetNbYears()const
		{
			assert(m_begin <= m_end);
			//&& TM().Mode() != CTM::OVERALL_YEARS
			return  (is_init() && TM().Type() != CTM::ATEMPORAL) ? m_end.m_year - m_begin.m_year + 1 : NOT_INIT;
		}
		//    int32_t GetNbMonths()const
		//    {
		//        return int32_t(GetNbYears()) * 12;
		//    }
		//    int32_t GetNbDays()const
		//    {
		//        assert(TM().Type() == CTM::DAILY);
		//        return GetNbRef();
		//    }
			//int32_t GetNbDays2()const;
		//    int32_t GetNbHours()const
		//    {
		//        return GetNbDays2() * 24;
		//    }
		//    int32_t GetNbRef()const
		//    {
		//        return is_init() ? (m_end.GetRef() - m_begin.GetRef() + 1) : 0;
		//    }
		//    int32_t GetRefPos(const CTRef& t)const
		//    {
		//        assert(t.TM() == TM());
		//        return t.GetRef() - m_begin.GetRef();
		//    }
		bool is_init()const
		{
			assert(m_begin.is_init() == m_end.is_init());
			return m_begin.is_init() && m_end.is_init();
		}

		bool is_countinious()const
		{
			return true;//m_exclusion == nullptr;
		}



		CTM TM()const
		{
			assert(m_begin.TM() == m_end.TM());
			return m_begin.TM();
		}
		void TM(const CTM& TM)
		{
			operator=(as(TM));
			//m_begin.TM(TM);
			//m_end.TM(TM);
		}

		//CTPeriod Get(const CTM& TM);
		static CTPeriod as(CTPeriod p, const CTM& TM);
		CTPeriod as(const CTM& TM)const
		{
			return as(*this, TM);
		}
		//    CTPeriod& Transform(const CTPeriod& p)
		//    {
		//        return Transform(p.TM());
		//    }
		//    CTPeriod as(const CTM& TM)const
		//    {
		//        CTPeriod p(*this);
		//        p.Transform(TM);
		//        return p;
		//    }

		CTPeriod& FromFormatedString(std::string str, std::string periodformat = "%1%|%2%", std::string TRefFormat = "", const char* sep = "- :", int base = 1)
		{
			operator=(WBSF::FromFormatedString(str, periodformat, TRefFormat, sep, base));
			return *this;
		}

		std::string GetFormatedString(std::string periodformat = "%1%|%2%", std::string TRefFormat = "")const;
		std::string to_string()const;
		void from_string(const std::string& str);


		bool is_inside(const CTRef& Tref)const;
		bool is_inside(const CTPeriod& p)const;
		bool is_intersect(const CTPeriod& p)const;

		static CTPeriod intersect(const CTPeriod& p1, const CTPeriod& p2);
		static CTPeriod intersect(const CTPeriod& p, const CTRef& TRef)
		{
			return intersect(p, CTPeriod(TRef, TRef));
		}

		CTPeriod intersect(const CTPeriod& p)const
		{
			return intersect(*this, p);
		}
		CTPeriod intersect(const CTRef& TRef)const
		{
			return intersect(*this, TRef);
		}

		static CTPeriod unions(const CTPeriod& p1, const CTPeriod& p2);
		static CTPeriod unions(const CTPeriod& p, const CTRef& TRef)
		{
			return unions(p, CTPeriod(TRef, TRef));
		}

		CTPeriod unions(const CTPeriod& p)const
		{
			return unions(*this, p);
		}
		CTPeriod unions(const CTRef& TRef)const
		{
			return unions(*this, TRef);
		}

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using boost::serialization::make_nvp;
			ar& make_nvp("begin", m_begin);
			ar& make_nvp("end", m_end);
		}

		std::istream& operator << (std::istream& io)
		{
			io >> m_begin >> m_end;
			return io;
		}
		std::ostream& operator >> (std::ostream& io)const
		{
			io << m_begin << m_end;
			return io;
		}
		friend std::istream& operator >>(std::istream& io, CTPeriod& p)
		{
			p << io;
			return io;
		}
		friend std::ostream& operator <<(std::ostream& io, const CTPeriod& p)
		{
			p >> io;
			return io;
		}
		
		
		void SetSegemntType(TSegment type);

		//void WriteStream(std::ostream& stream)const;
		//void ReadStream(std::istream& stream);

	protected:


		//bool IsOutsideInclusive(CTRef ref)const;

		//int16_t m_type;//YEAR_BY_YEAR, CONTINUOUS
		CTRef m_begin;
		CTRef m_end;

		//std::vector<CTPeriod> m_segments;
	};

	
	class CDOYPeriod : public CTPeriod
	{
	public:
		CDOYPeriod(int year, size_t DOY1, size_t DOY2);
		CDOYPeriod(int firtYear, size_t first_DOY, int lastYear, size_t last_DOY);
	};

	

	

	//
	//
	//template <class T>
	//class CTReferencedVectorIterator
	//{
	//public:
	//
	//    CTReferencedVectorIterator(T& vector, CTRef Tref)
	//    {
	//        m_vector = vector;
	//        m_Tref = Tref;//actual lkevel of the reference
	//    }
	//
	//    CTReferencedVectorIterator operator[](size_t i)
	//    {
	//        assert(m_Tref.TM() != m_vector.TM());//to musch level of indirection
	//
	//        CTM TM(m_Tref.TM().Type() + 1, m_Tref.TM().Mode());
	//        return CTReferencedVectorIterator(m_vector, CTRef(m_Tref).Transform(TM));
	//    }
	//
	//    operator T&()
	//    {
	//        return m_vector[m_Tref];
	//    }
	//
	//protected:
	//
	//    T& m_vector;
	//    CTRef m_Tref;
	//};
	//
	//template <class T>
	//class CTReferencedVectorConstIterator
	//{
	//public:
	//
	//    CTReferencedVectorConstIterator(const T& vector, CTRef Tref)
	//    {
	//        m_vector = vector;
	//        m_Tref = Tref;//actual lkevel of the reference
	//    }
	//
	//    CTReferencedVectorConstIterator operator[](size_t i)const
	//    {
	//        assert(m_Tref.TM() != m_vector.TM());//to musch level of indirection
	//
	//        CTM TM(m_Tref.TM().Type() + 1, m_Tref.TM().Mode());
	//        return CTReferencedVectorConstIterator(m_vector, CTRef(m_Tref).Transform(TM));
	//    }
	//
	//    operator const T&()const
	//    {
	//        return m_vector[m_Tref];
	//    }
	//
	//protected:
	//
	//    const T& m_vector;
	//    CTRef m_Tref;
	//};

	template <class T>
	class CTReferencedVector : public std::vector < T >
	{
	public:


		CTReferencedVector(CTPeriod period = CTPeriod())//, size_t timeStep = 1)//timeStep
		{
			Init(period);
		}

		void Init(CTPeriod period = CTPeriod())
		{
			m_period = period;
			std::vector<T>::resize(m_period.size());
		}

		CTReferencedVector(const CTReferencedVector& in)
		{
			operator=(in);
		}

		CTReferencedVector& operator=(const CTReferencedVector& in)
		{
			if (&in != this)
			{
				std::vector<T>::operator =(in);
				m_period = in.m_period;
			}

			return *this;
		}

		//    CTRef GetFirstTRef()const
		//    {
		//        return m_period.begin();
		//    }

		CTM TM()const
		{
			return m_period.TM();
		}
		bool is_init()const
		{
			return m_period.is_init();
		}


		using std::vector<T>::at;
		using std::vector<T>::operator[];
		const T& operator[](const CTRef& TRef)const
		{
			return at(TRef);
		}
		T& operator[](const CTRef& TRef)
		{
			return at(TRef);
		}
		const T& at(const CTRef& TRef)const
		{
			return std::vector<T>::operator[]((TRef - m_period.begin()));
		}
		T& at(const CTRef& TRef)
		{
			return std::vector<T>::operator[]((TRef - m_period.begin()));
		}

		const CTPeriod& period()const
		{
			return m_period;
		}

		//CTPeriod& period()
		//{
		//     return m_period;
		//}

		CTRef TRef(size_t i)const
		{
			return m_period[i];
		}

	protected:

		CTPeriod m_period;


	};

	//*************************************************************
	//CTTransformation

	class CTTransformation
	{
	public:

		CTTransformation(const CTPeriod& p, CTM TM);
		CTTransformation(const CTPeriod& pIn, const CTPeriod& pOut);

		void ComputeCluster(const CTPeriod& p, CTM TM);
		void ComputeCluster(const CTPeriod& p, const CTPeriod& pOut);

		size_t GetNbCluster()const
		{
			return m_clusters.size();
		}
		CTRef GetClusterTRef(size_t c)const
		{
			return (c < m_clusters.size() ? m_clusters[c] : CTRef());
		}
		size_t GetCluster(CTRef t)const;

		CTM GetTMIn()const
		{
			return m_pIn.TM();
		}
		CTM GetTMOut()const
		{
			return m_pOut.TM();
		}

		bool is_init()const
		{
			return m_pIn.is_init() && m_pOut.is_init() && m_pIn != m_pOut;
		}
		bool is_inside(CTRef TRef)const;
		CTPeriod GetPeriodIn()const
		{
			return m_pIn;
		}
		CTPeriod GetPeriodOut()const
		{
			return m_pOut;
		}

	protected:

		CTRefVector m_clusters;
		//full output period
		CTPeriod m_pIn;
		CTPeriod m_pOut;
	};




	double GetDayLength(double latDeg, size_t DOY);
	inline double GetDayLength(double latDeg, size_t m, size_t d)
	{
		return GetDayLength(latDeg, GetDOY(m, d));
	}



}

//namespace std
//{
//	inline string to_string(const WBSF::CTM& in){ return WBSF::to_string(in); }
//	inline WBSF::CTM to_CTM(const string& str){ return WBSF::ToObject<WBSF::CTM>(str); }
//}

/*namespace zen
{

	template <> inline
		void writeStruc(const WBSF::CTRef& in, XmlElement& output)
	{
		output.setValue(in.to_string());
	}

	template <> inline
		bool readStruc(const XmlElement& input, WBSF::CTRef& out)
	{
		std::string str;
		input.getValue(str);
		out.FromString(str);

		return true;
	}

	template <> inline
		void writeStruc(const WBSF::CTM& TM, XmlElement& output)
	{
		output.setValue(std::to_string(TM));
	}

	template <> inline
		bool readStruc(const XmlElement& input, WBSF::CTM& TM)
	{
		std::string str;
		input.getValue(str);
		TM = std::to_CTM(str);

		return true;
	}


	template <> inline
		void writeStruc(const WBSF::CTPeriod& in, XmlElement& output)
	{
		output.setValue(in.to_string());
	}

	template <> inline
		bool readStruc(const XmlElement& input, WBSF::CTPeriod& out)
	{
		std::string str;
		input.getValue(str);
		out.FromString(str);

		return true;
	}

}

*/

BOOST_CLASS_IMPLEMENTATION(WBSF::CTM, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(WBSF::CTM, boost::serialization::track_never)
BOOST_CLASS_IMPLEMENTATION(WBSF::CTRef, boost::serialization::object_serializable);
BOOST_CLASS_TRACKING(WBSF::CTRef, boost::serialization::track_never)
