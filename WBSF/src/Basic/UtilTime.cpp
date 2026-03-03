//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************

//#include "Basic/UtilStd.h"

#include <cmath>
#include <sstream>
#include <iostream>
#include <ctime>
#include <boost/date_time.hpp>





//#include "Basic/UtilStd.h"
#include "Basic/Statistic.h"
#include "Basic/UtilMath.h"


#include "UtilTime.h"
//#include "WeatherBasedSimulationString.h"


using namespace std;

//const locale TIME_FORMATS[] =
//{
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S")),
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y/%m/%d %H:%M:%S")),
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%d %H")),
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y/%m/%d %H")),
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%d")),
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y-%m")),
//    locale(locale::classic(), new boost::posix_time::time_input_facet("%Y"))
//};


namespace WBSF
{


//don't use these variable directly : use interface
static const size_t DAYS_IN_MONTH[NB_MONTHS] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const size_t FIRST_DOY[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
static const size_t FIRST_DOY_LEAP[13] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

static const char* MONTH_NAME[2][12] =
{
    {"January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"},
    {"Jan", "Feb", "Mar", "Apr", "May", "Jun","Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}
};

const char* GetMonthName(size_t m, bool bFull)
{
    assert(m < NB_MONTHS);
    return MONTH_NAME[bFull ? 0 : 1][m];
}






//double MidDOYForMonth(size_t m)
//{
//    assert(m >= 0 && m < 12);
//    return MID_DOY_FOR_MONTH[m];
//}

double MidDOYForMonth(size_t m)
{
    static const double MID_DOY_FOR_MONTH[12] = { 15.5, 45.0, 74.5, 105.0, 135.5, 166.0, 196.5, 227.5, 258.0, 288.5, 319.0, 349.5 };

    assert(m<12);
    return MID_DOY_FOR_MONTH[m];
}


bool IsLeap(int year)
{
    //special case for negative years. Thery are used for NOT_INIT_YEAR and Overall years mode
    return (year > 0) && !(year % 4 || (!(year % 100) && year % 400));
}

size_t GetNbDaysPerYear(int year)
{
    return 365 + IsLeap(year);
}

size_t GetNbDaysPerMonth(int year, size_t m)
{
    assert(year == YEAR_NOT_INIT || (year >= -10'000 && year <= 2500));
    assert(m < NB_MONTHS);

    size_t nbDay = DAYS_IN_MONTH[m];
    if (m == FEBRUARY && IsLeap(year))
        nbDay++;

    return nbDay;
}

size_t GetFirstDOY(int year, size_t m)
{
    assert(m < NB_MONTHS);
    return IsLeap(year) ? FIRST_DOY_LEAP[m] : FIRST_DOY[m];
}

size_t GetLastDOY(int year, size_t m)
{
    assert(m < NB_MONTHS);
    return (IsLeap(year) ? FIRST_DOY_LEAP[m+1] : FIRST_DOY[m+1])-1;
}


//static vector<string> MONTH_TITLE_LONG = { "January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December" };
//static vector<string> MONTH_TITLE_SHORT= { "Jan", "Feb", "Mar", "Apr", "May", "Jun","Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
//	void ReloadString()
//	{
//		MONTH_TITLE_LONG.LoadString(IDS_STR_MONTH_LONG, "|;");
//		MONTH_TITLE_SHORT.LoadString(IDS_STR_MONTH_SHORT, "|;");
//	}

//static constexpr string MONTH_TITLE_LONG = "January|February|March|April|May|June|July|August|September|October|November|December";
//static vector<string> MONTH_TITLE_SHORT= { "Jan", "Feb", "Mar", "Apr", "May", "Jun","Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

//const char* GetMonthTitle(size_t m, bool bFull)
//{
//    assert(m < 12);
//    //	if (MONTH_TITLE_LONG.empty())
//    //	ReloadString();
//
//    assert(MONTH_TITLE_LONG.size() == 12);
//    assert(MONTH_TITLE_SHORT.size() == 12);
//
//    return bFull ? MONTH_TITLE_LONG[m].c_str() : MONTH_TITLE_SHORT[m].c_str();
//}


size_t GetMonth(const char* month)
{
    size_t m = NOT_INIT;
    //string myString(month);
//    const auto it = std::find(begin(MONTH_NAME[0]), end(MONTH_NAME[0]), month);


    for(size_t i =0; i<2&&m == NOT_INIT; i++)
    {
        auto b = begin(MONTH_NAME[i]);
        auto e = end(MONTH_NAME[i]);
        auto it = std::find_if(b, e, [month](const auto& str)
        {
            return strcasecmp(month, str)==0;
        });

        if(it!=e)
        {
            m = std::distance(b, it);
        }
    }

    return m;
}


//    string str(month);
//    transform(str.begin(), str.end(), str.begin(), ::toupper);
//
//    for (size_t i = 0; i < 2 && index == UNKNOWN_POS; i++)
//    {
//        for (size_t j = 0; j < 12 && index == UNKNOWN_POS; j++)
//        {
//            if (str == MONTH_NAME[i][j])
//                index = j;
//        }
//    }

//    return index;
//}

size_t GetDOY(int year, size_t m, size_t d)
{
    assert(year == YEAR_NOT_INIT || (year >= -10'000 && year <= 2500));
    assert(m < NB_MONTHS);
    assert(d < GetNbDaysPerMonth(year, m));

//    size_t DOY = 0;
//
//    for (size_t mm = 0; mm < m; mm++)
//    {
//        DOY += GetNbDaysPerMonth(year, mm);
//    }

    size_t DOY = GetFirstDOY(year, m) + d;
    assert(DOY < GetNbDaysPerYear(year));

    return DOY;
}

//short GetjDay(int year, size_t m, size_t d)
//{
//	return GetJDay(year, month, day);
//}


size_t GetDayOfTheMonth(int year, size_t DOY)
{
    assert(DOY < GetNbDaysPerYear(year));

    size_t d = DOY;

    for (size_t m = 0; m<12 && d >= GetNbDaysPerMonth(year, m); m++)
    {
        d -= GetNbDaysPerMonth(year, m);
    }

    assert(d < MAX_DAY_PER_MONTH);

    return d;
}

size_t GetMonth(int year, size_t DOY)
{
    assert(DOY < GetNbDaysPerYear(year));

    auto b = begin(IsLeap(year) ? FIRST_DOY_LEAP: FIRST_DOY) + 1;
    auto e = end(IsLeap(year) ? FIRST_DOY_LEAP: FIRST_DOY);
    auto it = upper_bound (b, e, DOY);
    assert(it!=e);


    size_t m = distance(b, it);

//    const size_t* pFirst = IsLeap(year) ? FIRST_DOY_LEAP: FIRST_DOY;
//
//
//
//    size_t m = 0;
//    for (int i = 11; i >= 0; i--)
//    {
//        if (DOY >= pFirst[i])
//        {
//            m = i;
//            break;
//        }
//    }

    assert(m < 12);
    return m;
}


//
//2.15. What is the Julian Period?
//The Julian period (and the Julian day number) must not be confused
//with the Julian calendar.
//
//The French scholar Joseph Justus Scaliger (1540-1609) was interested
//in assigning a positive number to every year without having to worry
//about BC/AD. He invented what is today known as the "Julian Period".
//
//The Julian Period probably takes its name from the Julian calendar,
//although it has been claimed that it is named after Scaliger's father,
//the Italian scholar Julius Caesar Scaliger (1484-1558).
//
//Scaliger's Julian period starts on 1 January 4713 BC (Julian calendar)
//and lasts for 7980 years. AD 2003 is thus year 6716 in the Julian
//period. After 7980 years the number starts from 1 again.
//
//Why 4713 BC and why 7980 years? Well, in 4713 BC the Indiction (see
//section 2.14), the Golden Number (see section 2.12.3) and the Solar
//Number (see section 2.4) were all 1. The next times this happens is
//15*19*28=7980 years later, in AD 3268.
//
//Astronomers have used the Julian period to assign a unique number to
//every day since 1 January 4713 BC. This is the so-called Julian Day
//(JD). JD 0 designates the 24 hours from noon UTC on 1 January 4713 BC
//to noon UTC on 2 January 4713 BC.
//
//This means that at noon UTC on 1 January AD 2000, JD 2,451,545
//started.
//
//This can be calculated thus:
//From 4713 BC to AD 2000 there are 6712 years.
//In the Julian calendar, years have 365.25 days, so 6712 years
//correspond to 6712*365.25=2,451,558 days. Subtract from this
//the 13 days that the Gregorian calendar is ahead of the Julian
//calendar, and you get 2,451,545.
//
//Often fractions of Julian day numbers are used, so that 1 January AD
//2000 at 15:00 UTC is referred to as JD 2,451,545.125.
//
//Note that some people use the term "Julian day number" to refer to any
//numbering of days. NASA, for example, uses the term to denote the
//number of days since 1 January of the current year.


//GetRef return the number of day from the 1 december 1
//minus year 0, year aren't leap and return negative value
int32_t GetRef(int year, size_t m, size_t d)
{
    assert(year == YEAR_NOT_INIT || (year >= -10'000 && year <= 2500));
    assert(m < 12);
    assert(d < 31);

    //For a date in the Gregorian calendar:
    int ref = 0;


    int _a = int((13 - m) / 12);
    int _y = year - _a;
    int _m = int(m + 12 * _a - 2);



    //For a date in the Julian calendar:
    if (year > 0)//no leap year under zero
        ref = int32_t(d + (153 * _m + 2) / 5 + _y * 365 + _y / 4 - _y / 100 + _y / 400 - 306);
    else
        ref = int32_t(d + (153 * _m + 2) / 5 + _y * 365 - 306);
    //else JD = int(day + (153*m+2)/5 + y*365 + y/4 - 32083);

    return ref;


}

//JD is the Julian day number that starts at noon UTC on the specified
//date.
//
//The algorithm works fine for AD dates. If you want to use it for BC
//dates, you must first convert the BC year to a negative year (e.g.,
//10 BC = -9). The algorithm works correctly for all dates after 4800 BC,
//i.e. at least for all positive Julian day numbers.
//
//To convert the other way (i.e., to convert a Julian day number, JD,
//to a day, month, and year) these formulas can be used (again, the
//divisions are integer divisions):
void SetRef(int32_t ref, int& year, size_t& m, size_t& d)
{
    if (ref >= 0)
    {
        //  For the Gregorian calendar:
        int _a = ref + 306;//32044;
        int _b = (4 * _a + 3) / 146097;
        int _c = _a - (_b * 146097) / 4;

        int _d = (4 * _c + 3) / 1461;
        int _e = _c - (1461 * _d) / 4;
        int _m = (5 * _e + 2) / 153;

        d = _e - (153 * _m + 2) / 5; //+ 1
        m = _m + 2 - 12 * (_m / 10);
        year = _b * 100 + _d + _m / 10;
    }
    else
    {
        //JD=-JD;
        int _y = -(ref + 1) / 365;
        int _f = ref + _y * 365;
        int _e = _f + 306 - (_f / 307) * 365;
        int _m = (5 * _e + 2) / 153;

        d = _e - (153 * _m + 2) / 5;
        m = _m + 2 - 12 * (_m / 10);
        year = -_y;
    }
}



string GetTimeSpanStr(double sec)
{
    string str;

    int day = int(sec / (24 * 60 * 60));
    int hour = int((sec - day * 24 * 60 * 60) / (60 * 60));
    int minute = int((sec - day * 24 * 60 * 60 - hour * 60 * 60) / (60));
    double second = (sec - day * 24 * 60 * 60 - hour * 60 * 60 - minute * 60);

    if (day > 0)
    {
        str += to_string(day) + " day";
        if (day > 1)
            str += "s";
    }

    if (hour > 0 || day > 0)
    {
        str += to_string(hour) + " hour";
        if (hour > 1)
            str += "s";
    }

    if (minute > 0 || hour > 0 || day > 0)
    {
        str += to_string(minute) + " minute";
        if (minute > 0)
            str += "s";
    }

    str += to_string(second, 2) + " second";
    if (second > 1)
        str += "s";


    return str;
}



string CTRefFormat::GetMissingString(CTM tm)const
{
    string str = GetFormat(tm);
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    str = ReplaceString(str, "#", "");
    str = ReplaceString(str, "%%", "");

    static const char* ALL_SYMBOL[15] = { "%A","%B","%C","%D","%H","%I","%J","%M","%P","%S","%U","%W","%X","%Y","%Z" };
    for (int i = 0; i < 15; i++)
        str=ReplaceString(str, ALL_SYMBOL[i], "??");

    return str;
}


const char* CTRefFormat::DEFAULT_FORMAT[CTM::NB_REFERENCE][CTM::NB_MODE] =
{
    { "%Y", "9999" },
    { "%Y-%m", "9999-%m" },
    { "%Y-%m-%d", "9999-%m-%d" },
    { "%Y-%m-%d %H:00:00", "9999-%m-%d %H:00:00" },
    { "%d", "-32767" }
};
const char* CTRefFormat::DEFAULT_HEADER[CTM::NB_REFERENCE][CTM::NB_MODE] =
{
    { "Year", "Year" },
    { "Year-Month", "Year-Month" },
    { "Year-Month-Day", "Year-Month-Day" },
    { "Year-Month-Day Hour:Minutes:Seconds", "Year-Month-Day Hour:Minutes:Seconds" },
    { "No", "No" }
};

//const char* CTRefFormat::DATE_YMD = "%Y-%m-%d";
//const char* CTRefFormat::DATE_YMDH = "%Y-%m-%d %H";
//const char* CTRefFormat::DATE_YMD_HMS = "%Y-%m-%d %H:%M:%S";
//const char* CTRefFormat::DATE_DMY = "%d-%m-%Y";
//const char* CTRefFormat::DATE_MD = "%m-%d";
//const char* CTRefFormat::DATE_DM = "%d-%m";

//"DateYMD", "DateDMY", "DateMD", "DateDM"
const char* CTRefFormat::FORMAT_NAME[NB_FORMAT] = { "Year", "Month", "Day", "DOY", "Hour", "No", "Date" };

//std::vector<std::string> CTRefFormat::GetTimeReference(const std::string& header)
std::vector<std::string> CTRefFormat::GetTimeReference(const std::vector<std::string>& header)
{
    std::vector<std::string> time_ref;
    for (size_t i = 0; i < NB_FORMAT; i++)
    {
        if (WBSF::FindStringExact(header, FORMAT_NAME[i], false) != header.end())
            time_ref.push_back(FORMAT_NAME[i]);
    }

    return time_ref;
}




//********************************************************************
//CTM


const char CTM::NB_REFERENCES[8][NB_MODE] =
{
    { 1, 0 },
    { 2, 1 },
    { 3, 2 },
    { 4, 3 },
    { 1, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
};

const char* CTM::TYPE_NAME[8] = { "Annual", "Monthly", "Daily", "Hourly", "Atemporal", "", "", "Unknown" };
const char* CTM::MODE_NAME[NB_MODE] = { "For each year", "Overall years" };
const char* CTM::TYPE_MODE_NAME[8][NB_MODE] =
{
    { "Annual (for each year)", "Annual (overall years)" },
    { "Monthly (for each year)", "Monthly (overall years)" },
    { "Daily (for each year)", "Daily (overall years)" },
    { "Hourly (for each year)", "Hourly (overall years)" },
    { "Atemporal (for each ref)", "Atemporal (overall refs)" },
    { "", "" },
    { "", "" },
    { "Unknown", "Unknown" }
};

const char* CTM::GetTypeName(size_t TType)
{
    assert(TType >= 0 && TType < 8);
    return TYPE_NAME[TType];
}


const char* CTM::GetModeName(size_t TMode)
{
    assert(TMode >= 0 && TMode < NB_MODE);
    return MODE_NAME[TMode];
}

//static const char* TYPE_TITLE = {"Annual","Monthly","Daily","Hourly","Atemporal"," "," ","Unknown"};
//static const char* MODE_TITLE = {"For Each Year","Over all Years"};
//	void CTM::ReloadString()
//	{
//		TYPE_TITLE.clear();
//		MODE_TITLE.clear();
//	}

//const char* CTM::GetTypeTitle(size_t TType)
//{
//    assert(TType >= 0 && TType < 8);
//    //if (TYPE_TITLE.empty())
//    //TYPE_TITLE.LoadString(IDS_STR_TM_TYPE, "|;");
//
//    //return TYPE_TITLE.empty() ? TYPE_NAME[TType] : TYPE_TITLE[TType].c_str();
//    return TYPE_NAME[TType];
//}
//
//const char* CTM::GetModeTitle(size_t TMode)
//{
//    assert(TMode >= 0 && TMode < NB_MODE);
//    //if (MODE_TITLE.empty())
//    //MODE_TITLE.LoadString(IDS_STR_TM_MODE, "|;");
//
//    return MODE_NAME[TMode];
//}

const bool CTM::AVAILABILITY[NB_REFERENCE*NB_MODE][NB_REFERENCE*NB_MODE] =
{
    //  AF MF DF HF UF AO MO DO HO UO
    { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },//AF = ANNUAL FOR_EACH_YEAR
    { 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 },//MF = MONTHLY FOR_EACH_YEAR
    { 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 },//DF = DAILY FOR_EACH_YEAR
    { 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 },//HF = HOURLY FOR_EACH_YEAR
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },//UF = ATEMPORAL FOR_EACH_YEAR
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//AO = ANNUAL OVERALL_YEARS
    { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },//MO = MONTHLY OVERALL_YEARS
    { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 },//DO = DAILY OVERALL_YEARS
    { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0 },//HO = HOURLY OVERALL_YEARS
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//UO = ATEMPORAL OVERALL_YEARS
};

//********************************************************************
//CTRef

CTRefFormat CTRef::TREF_FORMAT;

CTRef::CTRef()
{
    clear();
}

//mettre la valeur our year = YEAR_NOT_INIT: a voir
//CTRef::CTRef(float in)
//{
//    operator =(in);
//}
//CTRef::CTRef(int32_t in)
//{
//	clear();
//	operator =(in);
//}
//
//CTRef::CTRef(int32_t in)
//{
//	clear();
//	operator =(in);
//}
//
//CTRef::CTRef(double in)
//{
//    operator =(in);
//}
//
//CTRef::CTRef(unsigned int64_t)
//{
//	assert(false); //que faire de cela???
//}

CTRef::CTRef(int year)
{
    Set(year, 0, 0, 0, CTM::ANNUAL);
}

CTRef::CTRef(int year, size_t m)
{
    Set(year, m, 0, 0, CTM::MONTHLY);
}

CTRef::CTRef(int year, size_t m, size_t d)
{
    Set(year, m, d, 0, CTM::DAILY);
}

CTRef::CTRef(int year, size_t m, size_t d, size_t h)
{
    Set(year, m, d, h, CTM::HOURLY);
}


CTRef::CTRef(int year, size_t m, size_t d, size_t h, const CTM& TM)
{
    Set(year, m, d, h, TM);
}

//CTRef::CTRef(short t, short y_or_r, short m, short d, short h, short mode)
//{
//	Set(t, y_or_r, m, d, h, mode);
//}

CTRef::CTRef(string str)
{
    operator=(FromFormatedString(str));
}

//CTRef::CTRef(string str, CTM TM)
//{
//    assert(TM.Type() != CTM::UNKNOWN);
//
//    FromFormatedString(str);
//    m_TM = TM;
//    ResetUnused();
////
////    Trim(str);
////    if (!str.empty() && str[0] == '-')
////        str[0] = '*';
////
////    vector<string> tmp = Tokenize(str, "/-");
////    if (!tmp.empty() && !tmp[0].empty() && tmp[0][0] == '*')
////        tmp[0][0] = '-';
////
////    int p[4] = { 0,0,0,0 };
////    //int pos=0;
////    for (size_t i = 0; i < min((size_t)4, tmp.size()); i++)
////        p[i] = ToValue<int>(Trim(tmp[i]));
////
////    if (TM.Type() == CTM::ATEMPORAL || TM.Mode() == CTM::FOR_EACH_YEAR)
////    {
////        Set(p[0], p[1] - 1, p[2] - 1, p[3], TM);
////    }
////    else
////    {
////
////        Set(0, p[0] - 1, p[1] - 1, p[2], TM);
////    }
//}


CTRef& CTRef::Set(int y_or_r, size_t m, size_t d, size_t h, const CTM& TM)
{

//		assert(m < 12 || m == DECEMBER || TM.Type() == ATEMPORAL || TM.Type() == ANNUAL);
    //assert(m < 12 || TM.Type() == CTM::ATEMPORAL);
    //assert(d < 31 || d == LAST_DAY || d == DAY_NOT_INIT || TM.Type() == ATEMPORAL || TM.Type() == ANNUAL);
    //assert(d < 31 || TM.Type() == CTM::ATEMPORAL);
    //assert(h <= 24 || TM.Type() == CTM::ATEMPORAL);


    size_t type = TM.Type();
    size_t mode = TM.Mode();

    //add ref when hour is 24
    bool bAddRef = false;
    if (h == 24)
    {
        h = 23;
        bAddRef = true;
    }

    if (y_or_r == YEAR_NOT_INIT)
    {
        mode = CTM::OVERALL_YEARS;
        assert(type != CTM::UNKNOWN);
    }

    if (y_or_r == REF_NOT_INIT)
    {
        mode = CTM::OVERALL_YEARS;
        if(type == CTM::UNKNOWN)
            type = CTM::ATEMPORAL;
    }


    assert(is_valid(y_or_r, m, d, h, type, mode));

    clear();




    m_TM = CTM(type, mode);

    if (m_TM.Type() != CTM::ATEMPORAL)
    {
//			if (d == LAST_DAY)
//				d = GetNbDaysPerMonth(y_or_r, m) - 1;

        m_year = y_or_r;
        m_month = (uint8_t)m;
        m_day = (uint8_t)d;
        m_hour = (uint8_t)h;
    }
    else
    {
        m_ref = y_or_r;
    }



    ResetUnused();

    if (bAddRef)
        Shift(1);

    assert(is_valid());

    return *this;
}



void CTRef::SetDOY(int year, size_t DOY)
{
//		if (DOY == LAST_DAY)
    //		DOY = WBSF::GetNbDaysPerYear(year) - 1;

    size_t m = WBSF::GetMonth(year, DOY);
    size_t d = WBSF::GetDayOfTheMonth(year, DOY);

    Set(year, m, d, 0, CTM::DAILY);
}


void CTRef::ResetUnused()
{
    switch (m_TM.Type())
    {
    case CTM::UNKNOWN:
        m_ref = m_year = m_month = m_day = m_hour = 0;
        break;
    case CTM::ANNUAL:
        m_ref = m_month = m_day = m_hour = 0;
        break;
    case CTM::MONTHLY:
        m_ref = m_day = m_hour = 0;
        break;
    case CTM::DAILY:
        m_ref = m_hour = 0;
        break;
    case CTM::HOURLY:
        m_ref = 0;
        break;
    case CTM::ATEMPORAL:
        m_year = m_month = m_day = m_hour = 0;
        break;
    default:
        assert(false);
    }

    if (m_TM.Mode() == CTM::OVERALL_YEARS)
    {
        if( m_TM.IsTemporal() )
            m_year = YEAR_NOT_INIT;
        else
            m_ref = REF_NOT_INIT;
    }


}

bool CTRef::is_valid(int y_or_r, size_t m, size_t d, size_t h, size_t type, size_t mode)
{
    bool bValid = CTM::is_valid(type, mode);
    if (bValid )
    {
        if(type != CTM::ATEMPORAL)
        {

            if(type>=CTM::ANNUAL)
            {
                if(mode==CTM::FOR_EACH_YEAR)
                {
                    bValid &= (y_or_r>=-10'000 && y_or_r<=2500);
                }
                else
                {
                    bValid &= y_or_r == YEAR_NOT_INIT;
                }
            }

            if(type>=CTM::MONTHLY)
            {
                bValid &= m < 12;
            }

            if(type>=CTM::DAILY)
            {
                bValid &= d < WBSF::GetNbDaysPerMonth(y_or_r, m);
            }

            if (type>=CTM::HOURLY)
            {
                bValid &= h <= 24;//let 24 as a valid input hour because often used
            }

        }
        else
        {
            if(mode==CTM::FOR_EACH_REF)
            {
                //reference always valid
                bValid &= y_or_r != REF_NOT_INIT;
            }
            else
            {
                bValid &= y_or_r == REF_NOT_INIT;
            }
        }
    }

    return bValid;
}


//bool CTRef::is_valid()const
//{
//  return is_valid(m_year, m_month, m_day. m_hour, m_TM);

//    bool bValid = true;
//    if (m_TM.is_valid())
//    {
//        if (!(m_TM.Type() >= 0 && m_TM.Type() < CTM::NB_REFERENCE))
//            bValid = false;
//
//        if (m_year < YEAR_NOT_INIT)
//            bValid = false;
//
//        if (m_month >= 12)
//            bValid = false;
//
//        if (m_day >= WBSF::GetNbDaysPerMonth(m_year, m_month))
//            bValid = false;
//
//        if (m_hour >= 24)
//            bValid = false;
//    }
//
//    return bValid;
//}


void CTRef::clear()
{
    //m_type = CTM::UNKNOWN;
    //m_mode = CTM::FOR_EACH_YEAR;
    m_TM.clear();
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_ref = 0;
}

//CTRef& CTRef::operator =(float in)
//{
//    const void* pBits = this;
//    *((float*)pBits) = in;
//
//    return *this;
//}

//CTRef& CTRef::operator =(int32_t in)
//{
//	const void* pBits = this;
//	*((int32_t*)pBits) = in;
//
//	return *this;
//}

//CTRef& CTRef::operator =(double in)
//{
//    set_int32(int32_t(in));
//    return *this;
//}

CTRef& CTRef::operator =(const CTRef& in)
{
    if (&in != this)
    {
        m_TM = in.m_TM;
        m_year = in.m_year;
        m_month = in.m_month;
        m_day = in.m_day;
        m_hour = in.m_hour;
        m_ref = in.m_ref;
    }

    return *this;
}
bool CTRef::operator == (const CTRef& in)const
{
    bool bEqual = true;

    if (m_TM != in.m_TM)bEqual = false;
    if (m_year != in.m_year)bEqual = false;
    if (m_month != in.m_month)bEqual = false;
    if (m_day != in.m_day)bEqual = false;
    if (m_hour != in.m_hour)bEqual = false;
    if (m_ref != in.m_ref)bEqual = false;

    return bEqual;
}

int32_t CTRef::operator-(const CTRef& in)const
{
    return GetRef() - in.GetRef();
}

CTRef CTRef::operator-(int32_t nbRef)const
{
    CTRef TRef;
    TRef.SetRef(GetRef() - nbRef, TM());
    return TRef;
}

CTRef CTRef::operator+(int32_t nbRef)const
{
    CTRef Tref(*this);
    Tref.Shift(nbRef);
    return Tref;
}

CTRef& CTRef::Shift(int32_t nbRef)
{
    if (is_init())
    {
        int32_t i32 = GetRef();
        i32 += nbRef;
        SetRef(i32, TM());
        assert(is_valid());
    }

    return *this;
}


int32_t CTRef::GetRef()const
{
    int32_t i32 = 0;
    switch (m_TM.Type())
    {
    case CTM::ANNUAL:
        i32 = m_TM.Mode()==CTM::FOR_EACH_YEAR ? int64_t(m_year) : YEAR_NOT_INIT;
        break;
    case CTM::MONTHLY:
        i32 = m_TM.Mode()==CTM::FOR_EACH_YEAR ? int64_t((m_year) * 12 + m_month): m_month;
        break;
    case CTM::DAILY:
        i32 = WBSF::GetRef(m_TM.Mode()==CTM::FOR_EACH_YEAR ? m_year : 1, m_month, m_day);
        break;
    case CTM::HOURLY:
        i32 = WBSF::GetRef(m_TM.Mode()==CTM::FOR_EACH_YEAR ? m_year : 1, m_month, m_day) * 24 + int64_t(m_hour);
        break;
    case CTM::ATEMPORAL:
        i32 = m_TM.Mode()==CTM::FOR_EACH_REF ? m_ref : REF_NOT_INIT;
        break;
    case CTM::UNKNOWN:
        i32 = 0;
        break;
    default:
        assert(false);
    }

    return i32;
}

void CTRef::SetRef(int32_t i32, const CTM& TM)
{
    assert(TM.Type() != CTM::UNKNOWN);
    m_TM = TM;

    short neg = i32 < 0 ? 1 : 0;
    int year = 0;
    size_t m = 0;
    size_t d = 0;

    if (m_TM.IsDaily())
        WBSF::SetRef(i32, year, m, d);
    else if (m_TM.IsHourly())
        WBSF::SetRef((i32 + neg) / 24 - neg, year, m, d);

    switch (m_TM.Type())
    {
    case CTM::ANNUAL:
        m_year = i32;
        break;
    case CTM::MONTHLY:
        m_year = short((i32 + neg) / 12 - neg);
        m_month = (12 + (i32 % 12)) % 12;
        break;
    case CTM::DAILY:
        m_year = year;
        m_month = (uint8_t)m;
        m_day = (uint8_t)d;
        break;
    case CTM::HOURLY:
        m_year = year;
        m_month = (uint8_t)m;
        m_day = (uint8_t)d;
        m_hour = (24 + (i32 % 24)) % 24;
        break;
    case CTM::ATEMPORAL:
        m_ref = i32;
        break;
    default:
        assert(false);
    }

    //if( m_year != 0)
    //	m_mode = ANNUAL;

    ResetUnused();
    assert(is_valid());
}


CTRef CTRef::as(CTRef Tref, const CTM& TM, size_t initType)
{


    if (Tref.is_init())
    {
        if (TM.Type() > Tref.TM().Type())
        {
            switch (initType)
            {
            case CTM::FIRST_TREF:
                break; //*do nothing
            case CTM::MID_TREF:
                Tref.SetRef(int64_t((as(Tref, TM, CTM::FIRST_TREF).GetRef() + as(Tref, TM, CTM::LAST_TREF).GetRef()) / 2), TM);
                break;
            case CTM::LAST_TREF:
                Tref.Shift(1);
                break;//add one ref her
            default:
                assert(false);
            }

            Tref.TM(TM);

            switch (initType)
            {
            case CTM::FIRST_TREF: /*do nothing*/
                break;
            case CTM::MID_TREF:
                break;
            case CTM::LAST_TREF:
                Tref.Shift(-1);
                break;//remove one ref her
            default:
                assert(false);
            }
        }
        else
        {
            Tref.TM(TM);
        }
    }

    return Tref;
}




string CTRef::GetMissingString(CTM tm)
{
    return TREF_FORMAT.GetMissingString(tm);
}

string CTRef::to_string()const
{
    return GetFormatedString();
}

void CTRef::from_string(const string& str)
{
    FromFormatedString(str);
}



//%a	Abbreviated weekday name *	Thu
//%A	Full weekday name *	Thursday
//%b	Abbreviated month name *	Aug
//%B	Full month name *	August
//%c	Date and time representation *	Thu Aug 23 14:55:02 2001
//%d	Day of the month (01-31)	23
//%H	Hour in 24h format (00-23)	14
//%I	Hour in 12h format (01-12)	02
//%j	Day of the year (001-366)	235
//%m	Month as a decimal number (01-12)	08
//%M	Minute (00-59)	55
//%p	AM or PM designation	PM
//%S	Second (00-61)	02
//%U	Week number with the first Sunday as the first day of week one (00-53)	33
//%w	Weekday as a decimal number with Sunday as 0 (0-6)	4
//%W	Week number with the first Monday as the first day of week one (00-53)	34
//%x	Date representation *	08/23/01
//%X	Time representation *	14:55:02
//%y	Year, last two digits (00-99)	01
//%Y	Year	2001
//%Z	Timezone name or abbreviation	CDT
//%%	A % sign	%
//#		remove zero before number

string CTRef::GetFormatedString(std::string format)const
{
    string str;

    if (is_init())
    {

        if (IsTemporal())
        {
            if (format.empty())
                format = TREF_FORMAT.GetFormat(m_TM);

            // get current timeinfo and modify it to the user's choice
            //time ( &rawtime );
            //timeinfo = gmtime ( &rawtime );
            struct tm timeinfo = { 0 };
            timeinfo.tm_sec = 0;     // seconds after the minute - [0,59]
            timeinfo.tm_min = 0;     // minutes after the hour - [0,59]
            timeinfo.tm_hour = m_hour;    // hours since midnight - [0,23]
            timeinfo.tm_mday = m_day + 1;    // day of the month - [1,31]
            timeinfo.tm_wday = (GetRef() + 1) % 7;//day of the week, range 0 to 6, o=sunday
            timeinfo.tm_yday = (int)GetDOY(m_year, m_month, m_day);			  //Day of year (0 - 365)
            timeinfo.tm_mon = m_month;     // months since January - [0,11]
            timeinfo.tm_isdst = 0;

            if (m_year >= 1970 && m_year < 2038)
            {
                timeinfo.tm_year = m_year - 1900;
            }
            else
            {
                timeinfo.tm_year = WBSF::IsLeap(m_year) ? 96 : 95;//leap/non leap year

                string year = WBSF::to_string(GetYear());

                format=ReplaceString(format, "%y", "%Y");
                format=ReplaceString(format, "%#Y", "%Y");
                format=ReplaceString(format, "%Y", year);
            }

            try
            {
                char buffer[256] = { 0 };
                strftime(buffer, 256, format.c_str(), &timeinfo);
                str = buffer;
            }
            catch (...)
            {
            }
        }
        else
        {
            str = WBSF::to_string(int32_t(m_ref));
        }

        //if(m_TM.Mode() == CTM::OVERALL_YEARS)
        //  str += " overall";
    }

    return str;
}


//const size_t NB_TIME_FORMATS = sizeof(TIME_FORMATS) / sizeof(TIME_FORMATS[0]);
//
//time_t pt_to_time_t(const boost::posix_time::ptime& pt)
//{
//    boost::posix_time::ptime timet_start(boost::gregorian::date(1970, 1, 1));
//    boost::posix_time::time_duration diff = pt - timet_start;
//    return diff.ticks() / boost::posix_time::time_duration::rep_type::ticks_per_second;
//
//}
//void seconds_from_epoch(const string& s)
//{
//    boost::posix_time::ptime pt;
//    for (size_t i = 0; i < NB_TIME_FORMATS; ++i)
//    {
//        istringstream is(s);
//        is.imbue(TIME_FORMATS[i]);
//        is >> pt;
//        if (pt != boost::posix_time::ptime())
//            break;
//    }
//}


CTRef FromFormatedString(std::string str, const std::string& format, const char* sep, int base)
{
    CTRef TRef;

    if (!str.empty())
    {
        if (format.empty())
        {
            if (str[0] == '-')
                str[0] = '*';

            vector<string> elems = Tokenize(str.c_str(), sep);
            if (elems[0][0] == '*')
                elems[0][0] = '-';

            if(!elems.empty())
            {


                switch (elems.size())
                {
                case 1:
                    TRef = CTRef(to_int(elems[0]));
                    break;
                case 2:
                    TRef = CTRef(to_int(elems[0]),to_size_t(elems[1])-base);
                    break;
                case 3:
                    TRef = CTRef(to_int(elems[0]),to_size_t(elems[1])-base,to_size_t(elems[2])-base);
                    break;
                default:
                    TRef = CTRef(to_int(elems[0]),to_size_t(elems[1])-base,to_size_t(elems[2])-base,to_size_t(elems[3]));
                }
            }

        }
        else
        {
            // if (!str.empty())
            // {

//            if (format == CTRefFormat::DATE_YMD && elems.size() == 3)
//            {
//
//                type = CTM::DAILY;
//                y_or_r = ToInt(elems[0]);
//                m = ToInt(elems[1]);
//                d = ToInt(elems[2]);
//            }
//            else if (format == CTRefFormat::DATE_YMDH  && elems.size() == 4)
//            {
//                type = CTM::HOURLY;
//                y_or_r = ToInt(elems[0]);
//                m = ToInt(elems[1]);
//                d = ToInt(elems[2]);
//                h = ToInt(elems[3]);
//            }
//            else if (format == CTRefFormat::DATE_YMD_HMS && elems.size() == 4)
//            {
//                type = CTM::HOURLY;
//                y_or_r = ToInt(elems[0]);
//                m = ToInt(elems[1]);
//                d = ToInt(elems[2]);
//                vector<string>  tmp = Tokenize(elems[3], ":");
//                if (tmp.size() > 0)
//                    h = ToInt(tmp[0]);
//            }
//            else
//            {
            auto localImbu = locale(std::locale::classic(), new boost::posix_time::time_input_facet(format));
            boost::posix_time::ptime ptime;

            istringstream is(str);
            is.imbue(localImbu);
            is >> ptime;
            if (ptime != boost::posix_time::ptime())
            {
                size_t n = std::count(format.begin(), format.end(), '%');
                boost::gregorian::date date = ptime.date();
                switch(n)
                {
                case 1:
                    TRef = CTRef(date.year());
                    break;
                case 2:
                    TRef = CTRef(date.year(),date.month()-base);
                    break;
                case 3:
                    TRef = CTRef(date.year(),date.month()-base,date.day()-base);
                    break;
                default:
                    assert(false);
                }
            }
        }

//        if (y_or_r == YEAR_NOT_INIT )//&& type != CTM::UNKNOWN
//        {
//            TRef.Mode(CTM::OVERALL_YEARS);
//        }

    }//str.empty()



    return TRef ;

}


CTRef CTRef::GetCurrentTRef(CTM TM, bool bUTC)
{
    CTRef TRef;

    // For all C++ versions: Using std::chrono::system_clock
    // and converting to C-style time for formatting
    auto now = std::chrono::system_clock::now();
    time_t sys_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* timeinfo = bUTC? std::localtime(&sys_time_t):std::gmtime(&sys_time_t);
    TRef = CTRef(1900 + timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday - 1, timeinfo->tm_hour, TM);


    //{
    //    //time_t ltime;
    //    //tm today = { 0 };
//
    //    //_tzset();
    //    //time(&ltime);
    //    //_gmtime64_s(&today, &ltime);
//
    //    //month is un zero base and day is in 1 base
    //    //TRef = CTRef(1900 + today.tm_year, today.tm_mon, today.tm_mday - 1, today.tm_hour, TM);
    //
    //
    //    std::tm* timeinfo = std::localtime(&ltime);
    //    TRef = CTRef(1900 + timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday - 1, timeinfo->tm_hour, TM);
//
    //}
    //else
    //{
    //    time_t ltime;
    //    tm today = { 0 };
//
    //    _tzset();
    //    time(&ltime);
    //    _localtime64_s(&today, &ltime);
//
    //    // Convert to UTC tm structure
    //    std::tm* gmt_tm = std::gmtime(&sys_time_t);
    //
    //    //month is zero base and day is in 1 base
    //    TRef = CTRef(1900 + gmt_tm->tm_year, gmt_tm->tm_mon, gmt_tm->tm_mday - 1, gmt_tm->tm_hour, TM);
    //}

    return TRef;
}


std::time_t CTRef::get_time64()const
{
    std::time_t time64 = 0;
    if(is_init() && m_TM.IsTemporal() )
        time64 = WBSF::get_time64(m_year, m_month, m_day, m_hour);


    return time64;
}

//CTRef CTRef::Disaggregate(const CTM& TM)const
//{
//    assert(m_TM.Mode() < TM.Mode());
//    assert(m_TM.Type() < TM.Type());
//
//    CTRef copy(*this);
//    //if( copy.GetType() < TM.Type() )
//    //{
//    //	switch(TM.Type())
//    //	{
//    //	case HOURLY:	copy.m_hour = 12;
//    //	case DAILY:		copy.m_day = 15;
//    //	case MONTHLY:	copy.m_month = 5;break;
//    //	case ANNUAL:	//trange transformation
//    //	case ATEMPORAL:
//    //	case UNKNOWN:
//    //	default: assert(false);
//    //	}
//    //}
//    //else
//    //{
//    copy.TM(TM);
//    copy.ResetUnused();
//
//    return copy;
//}

//*************************************************************
CTimeRef::CTimeRef(std::time_t t, CTM TM, bool bUTC)
{
    m_time = t;
    if (t > 0)
    {
        //CTRef CTRef::GetCurrentTRef(CTM TM, bool bUTC);
        //tm theTime = { 0 };
        //_localtime64_s(&theTime, &t);
        std::tm* timeinfo = std::localtime(&m_time);
        Set(1900 + timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday - 1, timeinfo->tm_hour, TM);
    }
}

//*************************************************************
//CTPeriod

CTPeriod::CTPeriod(const CTRef& begin, const CTRef& end)
{
    assert(is_valid( begin, end) );

    m_begin = begin;
    m_end = end;
}
//
//CTPeriod::CTPeriod(int y1, int y2)
//{
//    m_begin = CTRef(y1);
//    m_end = CTRef(y2);
//
//    assert(m_begin <= m_end);
//}
//
//CTPeriod::CTPeriod(int y1, size_t m1, int y2, size_t m2)
//{
//    m_begin = CTRef(y1, m1);
//    m_end = CTRef(y2, m2);
//
//    assert(m_begin <= m_end);
//}
//
//CTPeriod::CTPeriod(int y1, size_t m1, size_t d1, int y2, size_t m2, size_t d2)
//{
//    m_begin = CTRef(y1, m1, d1);
//    m_end = CTRef(y2, m2, d2);
//
//    assert(m_begin <= m_end);
//}

CTPeriod::CTPeriod(const std::string& str1, const std::string& str2)
{
    m_begin.FromFormatedString(str1);
    m_end.FromFormatedString(str2);

    assert(m_begin.TM() == m_end.TM());
    assert(m_begin <= m_end);
}

CTPeriod::CTPeriod(const std::string& str)
{
    operator=(FromFormatedString(str));
}

void CTPeriod::clear()
{
    m_begin.clear();
    m_end.clear();
    //m_segments.clear();

    assert(!is_init());
}

CTPeriod& CTPeriod::operator = (const CTPeriod& in)
{
    if (&in != this)
    {
        m_begin = in.m_begin;
        m_end = in.m_end;
        //m_segments = in.m_segments;
    }

    assert(operator==(in));
    return *this;
}

bool CTPeriod::operator == (const CTPeriod& in)const
{
    assert(m_begin.TM() == m_end.TM());

    bool bEqual = true;

    if (m_begin != in.m_begin) bEqual = false;
    if (m_end != in.m_end) bEqual = false;
    //if (m_segments != in.m_segments) bEqual = false;

    return bEqual;

}

CTPeriod& CTPeriod::inflate(const CTRef& in)
{
    //assert(m_segments.empty());//todo

    if (is_init() && in.is_init())
    {
        assert(in.TM() == TM() );

        if (in < m_begin)
            m_begin = in;
        if (in > m_end)
            m_end = in;
    }
    else if(in.is_init())
    {
        m_begin = m_end = in;
    }

    return *this;
}

CTPeriod& CTPeriod::inflate(const CTPeriod& in)
{
    //assert(m_segments.empty());//todo

    if (is_init() && in.is_init())
    {
        assert(in.TM() == TM() );

        if (in.m_begin < m_begin)
            m_begin = in.m_begin;
        if (in.m_end > m_end)
            m_end = in.m_end;

        
    }
    else if(in.is_init())
    {
        *this = in;
    }

    return *this;
}

bool CTPeriod::is_inside(const CTRef& in)const
{
    //if (!is_init())
      //  return true;
    bool bRep = false;

    if (is_init() && in.is_init())
    {
        assert(m_begin.TM() == m_end.TM());
        assert(in.TM() == m_end.TM());

        
        
        bRep = in >= m_begin && in <= m_end;
        
        //if (bRep && !m_segments.empty())
        //{
        //    bRep = false;
        //    for (size_t i = 0; i < m_segments.size() && !bRep; i++)
        //        bRep = m_segments[i].is_inside(in);
        //}
    }
   
    return bRep;
}


bool CTPeriod::is_inside(const CTPeriod& in)const
{
    //assert(m_segments.empty());
    //assert(in.m_segments.empty());//todo

    bool is_inside = false;
    if(is_init() && in.is_init() )
    {
        assert(in.TM() == TM());
        is_inside = in.begin() >= m_begin && in.end() <= m_end;
    }
//    else if( is_init())
//    {
//        is_inside = false;
//    }
//    else if( p.is_init())
//    {
//        is_inside = true;
//    }
//

    return is_inside;
}


bool CTPeriod::is_intersect(const CTPeriod& in)const
{
    //assert(m_segments.empty());//todo
    //assert(in.m_segments.empty());//todo


    bool is_intersect=false;
    if(is_init() && in.is_init() )
    {
        assert(in.TM() == TM());
        is_intersect=is_inside(in.begin()) || is_inside(in.end()) || in.is_inside(begin()) || in.is_inside(end());
    }
   

    return is_intersect;
}


CTPeriod CTPeriod::unions(const CTPeriod& p1, const CTPeriod& p2)
{
    //assert(p1.m_segments.empty());//todo
    //assert(p2.m_segments.empty());//todo


    CTPeriod p;

    if(p1.is_init() && p2.is_init() )
    {
        assert(p1.TM() == p2.TM());
        CTM TM = p1.TM();
        int32_t b = min(p1.begin().GetRef(), p2.begin().GetRef());
        int32_t e = max(p1.end().GetRef(), p2.end().GetRef());

        p = CTPeriod(CTRef(b, TM), CTRef(e, TM));
    }
    else if( p1.is_init())
    {
        p = p1;
    }
    else if( p2.is_init())
    {
        p=p2;
    }
    else
    {
        //nothing to do
        //return not init period
    }



    return p;
}

CTPeriod CTPeriod::intersect(const CTPeriod& p1, const CTPeriod& p2)
{
    //assert(p1.m_segments.empty());//todo
   //assert(p2.m_segments.empty());//todo


    CTPeriod p;
    if( p1.is_init() && p2.is_init())
    {
        assert(p1.TM() == p2.TM());

        CTM TM = p1.TM();
        int32_t b = max(p1.begin().GetRef(), p2.begin().GetRef());
        int32_t e = min(p1.end().GetRef(), p2.end().GetRef());

        if (b <= e)
            p = CTPeriod(CTRef(b, TM), CTRef(e, TM));
    }
    else if( p1.is_init())
    {
        p = p1;
    }
    else if( p2.is_init())
    {
        p = p2;
    }
    else
    {
        //nothing to do
        //return not init period
    }


    return p;
}


//size_t CTPeriod::GetNbSegments()const
//{
//    if (!is_init())
//        return 0;
//
//    if (GetType() == CONTINUOUS || GetTMode() == OVERALL_YEARS)
//        return 1;
//
//    //return IsInversed()?GetNbYears()-1:GetNbYears();
//    return IsInversed() ? GetNbYears() + 1 : GetNbYears();
//}
//
//int CTPeriod::GetSegmentIndex(CTRef ref)const
//{
//    if (!is_init() || !is_inside(ref))
//        return -1;
//
//    if (GetType() == CONTINUOUS || GetTMode() == OVERALL_YEARS)
//        return 0;
//
//    int index = ref.GetYear() - m_begin.GetYear();
//
//    if (IsInversed())
//    {
//        if (ref.GetDOY() > m_begin.GetDOY())
//            index++;
//    }
//
//    return index;
//    //return ref.GetYear()-m_begin.GetYear();
//    //return ref.GetYear()-m_begin.GetYear();
//}
//
//CTPeriod CTPeriod::GetSegment(size_t i)const
//{
//    assert(i >= 0 && i < GetNbSegments());
//
//    CTPeriod p(*this);
//
//    if (GetType() == CONTINUOUS || GetTMode() == OVERALL_YEARS)
//        return p;
//
//    if (IsInversed())
//    {
//        if (i == 0)
//        {
//            p.SetType(CONTINUOUS);
//            p.begin() = CTRef(begin().m_year, JANUARY, DAY_01);
//            p.end().m_year = begin().m_year;
//        }
//        else if (i == GetNbSegments() - 1)
//        {
//            p.SetType(CONTINUOUS);
//            p.begin().m_year = end().m_year;
//            p.end() = CTRef(end().m_year, DECEMBER, DAY_31);
//        }
//        else
//        {
//            p.SetType(CONTINUOUS);
//            p.begin().m_year = begin().m_year + i;
//            p.end().m_year = begin().m_year + i + 1;
//        }
//    }
//    else
//    {
//        p.SetType(CONTINUOUS);
//        p.begin().m_year = begin().m_year + i;
//        p.end().m_year = begin().m_year + i;
//    }
//
//
//    return p;
//}


string CTPeriod::GetFormatedString(string periodformat, string TRefFormat)const
{
    periodformat=ReplaceString(periodformat, "%1%", "%s");
    periodformat=ReplaceString(periodformat, "%2%", "%s");

    return FormatA(periodformat.c_str(), m_begin.GetFormatedString(TRefFormat).c_str(), m_end.GetFormatedString(TRefFormat).c_str());
}

CTPeriod FromFormatedString(string str, string periodformat, string TRefFormat, const char* sep, int base)
{
    CTPeriod p;
    //clear();

    if (!str.empty())
    {
        periodformat=ReplaceString(periodformat, "%1%", "");
        periodformat=ReplaceString(periodformat, "%2%", "");
        vector<string> elems = Tokenize(str, periodformat.c_str());

        if (elems.size() == 2)
        {
            CTRef TRef1 = FromFormatedString(elems[0], TRefFormat, sep, base);
            CTRef TRef2 = FromFormatedString(elems[1], TRefFormat, sep, base);
            if ( CTPeriod::is_valid(TRef1, TRef2) )
            {
                p = CTPeriod(TRef1,TRef2);
            }
        }
    }

    return p;
}

string CTPeriod::to_string()const
{
    //assert(m_segments.empty());//todo
    
    return m_begin.to_string() + "|" + m_end.to_string();
}

void CTPeriod::from_string(const string& strIn)
{
    vector<string> str = Tokenize(strIn.c_str(), "|");
    if (str.size() == 2)
    {
//        int pos = 0;
        m_begin.from_string(str[0]);
        m_end.from_string(str[1]);
    }
    //int pos=0;
    //m_type = atoi(str.Tokenize("( );", pos));
    //m_begin.FromString(str.Tokenize("( );", pos));
    //if (pos >= 0)
    //	m_end.FromString(str.Tokenize("( );", pos));
}


bool CTPeriod::is_valid(const CTRef& Tref1, const CTRef& Tref2)
{
    return (!Tref1.is_init() && !Tref2.is_init()) || ((Tref1.TM() == Tref2.TM()) && (Tref1 <= Tref2));
}
//
//void CTPeriod::ReadStream(istream& stream)
//{
//    assert(false);//todo
//    int32_t tmp = 0;
//    stream.read((char*)(&tmp), sizeof(tmp));
//    //m_begin.set_int32(tmp);
//    stream.read((char*)(&tmp), sizeof(tmp));
//    //m_end.set_int32(tmp);
//}
//
//void CTPeriod::WriteStream(ostream& stream)const
//{
//    assert(false);//todo
//    int32_t tmp = 0;
//    //tmp = m_begin.get_int32();
//    stream.write((char*)(&tmp), sizeof(tmp));
//    //tmp = m_end.get_int32();
//    stream.write((char*)(&tmp), sizeof(tmp));
//}

//CTPeriod CTPeriod::GetAnnualPeriod(int year)const
//{
//    assert(year != YEAR_NOT_INIT);
//    assert(m_type != CONTINUOUS);
//
//    CTPeriod p(*this);
//    p.SetType(CONTINUOUS);
//
//    if (IsInversed())
//    {
//        //exclusion period: must decrement the reference
//
//        p.m_begin = m_end;
//        p.m_end = m_begin;
//    }
//
//    p.m_begin.m_year = year;
//    p.m_end.m_year = year;
//
//    return p;
//}
//
//CTRef CTPeriod::GetFirstAnnualTRef(size_t y)const
//{
//    assert(y >= 0 && y < GetNbYears());
//
//    CTRef ref = m_begin;
//    if (y != 0)
//    {
//        ref.m_year = m_begin.m_year + y;
//        if (m_type == CONTINUOUS)
//        {
//            ref.m_month = 0;
//            ref.m_day = 0;
//            ref.m_hour = 0;
//        }
//    }
//
//    ref.ResetUnused();
//
//    return ref;
//}
//
//CTRef CTPeriod::GetLastAnnualTRef(size_t y)const
//{
//    assert(y >= 0 && y < GetNbYears());
//
//    CTRef ref = m_end;
//    if (y != GetNbYears() - 1)
//    {
//        ref.m_year = m_begin.m_year + y;
//        ref.m_month = DECEMBER;
//        ref.m_day = DAY_31;
//        ref.m_hour = 23;
//    }
//
//    ref.ResetUnused();
//
//    return ref;
//}

//int32_t CTPeriod::GetNbDays2()const
//{
//    CTPeriod p(*this);
//    p.Transform(CTM(CTM::DAILY, p.GetTMode()));
//    return p.GetNbRef();
//}

//CTPeriod CTPeriod::Get(const CTM& TM)
//{
//    CTPeriod tmp = *this;
//    tmp.Transform(TM);
//    return tmp;
//}


CTPeriod CTPeriod::as(CTPeriod p, const CTM& TMi)
{
    CTM oldTM = p.TM();
    p.begin() = p.begin().as(TMi, CTM::FIRST_TREF);
    p.end() = p.end().as(TMi, CTM::LAST_TREF);

    if (oldTM.is_init() && TMi.is_init() && TMi.Type() > oldTM.Type())
    {
        bool bResetMonth = oldTM.Type() < CTM::MONTHLY&&TMi.Type() >= CTM::MONTHLY;
        bool bResetDay = oldTM.Type() < CTM::DAILY&&TMi.Type() >= CTM::DAILY;
        bool bResetHour = oldTM.Type() < CTM::HOURLY&&TMi.Type() >= CTM::HOURLY;

        if (bResetMonth)
            p.end().m_month = DECEMBER;
        if (bResetDay)
            p.end().m_day = uint8_t(WBSF::GetNbDaysPerMonth(p.end().GetYear(), p.end().GetMonth()) - 1);
        if (bResetHour)
            p.end().m_hour = 23;
    }
    else
    {
        //When we pass from FOR_EACH_YEAR to OVERALL_YEARS,
        //begin and end must be swap to be consistent.
        if(p.begin() > p.end())
        {
            std::swap(p.begin(), p.end());
        }
    }

    assert(p.begin() <= p.end());
    return p;
}


void CTPeriod::SetSegemntType(TSegment type)
{
    
    if (type == YEAR_BY_YEAR)
    {
        assert(false);//todo
        //assert(m_segments.empty());//todo
        

    }
    else if (type == CONTINUOUS)
    {
        //m_segments.clear();
    }

}


//**************************************************************
//CJDayPeriod

//CDOYPeriod::CDOYPeriod(int year, size_t Jday1, size_t Jday2)
//{
//    m_begin.SetJDay(year, Jday1);
//    m_end.SetJDay(year, Jday2);
//    m_type = CONTINUOUS;
//}
//
//CDOYPeriod::CDOYPeriod(int firtYear, size_t  Jday1, int  lastYear, size_t  Jday2)
//{
//    m_begin.SetJDay(firtYear, Jday1);
//    m_end.SetJDay(lastYear, Jday2);
//    m_type = type;
//}

//**************************************************************
//CTTransformation

CTTransformation::CTTransformation(const CTPeriod& p, CTM TM)
{
    ComputeCluster(p, TM);
}

CTTransformation::CTTransformation(const CTPeriod& pIn, const CTPeriod& pOut)
{
    ComputeCluster(pIn, pOut);
}


void CTTransformation::ComputeCluster(const CTPeriod& pIn, const CTPeriod& pOut)
{
    m_pIn = pIn;
    m_pOut = pOut;

    size_t nbCluster = m_pOut.size();
    m_clusters.resize(nbCluster);

    CTRef ref = m_pOut.begin();
    for (size_t c = 0; c < nbCluster; c++, ref++)
    {
        m_clusters[c] = ref;
    }
}


void CTTransformation::ComputeCluster(const CTPeriod& p, CTM TM)
{
    CTPeriod pOut = p.as(TM);
    //pOut.Transform(TM);

    ComputeCluster(p, pOut);
}

bool CTTransformation::is_inside(CTRef TRef)const
{
    bool bInside = m_pIn.is_inside(TRef);

    TRef = TRef.as(m_pOut.TM());
    return bInside && m_pOut.is_inside(TRef);
}

size_t CTTransformation::GetCluster(CTRef TRef)const
{
    bool bInside = m_pIn.is_inside(TRef);
    TRef = TRef.as(m_pOut.TM());

    return (bInside&&m_pOut.is_inside(TRef)) ? m_pOut[TRef] : UNKNOWN_POS;
}

//%a	Abbreviated weekday name					Thu
//%A	Full weekday name							Thursday
//%b	Abbreviated month name						Aug
//%B	Full month name								August
//%c	Date and time representation				Thu Aug 23 14:55:02 2001
//%d	Day of the month				(01-31)		23
//%H	Hour in 24h format				(00-23)		14
//%I	Hour in 12h format				(01-12)		02
//%j	Day of the year					(001-366)	235
//%m	Month as a decimal number		(01-12)		08
//%M	Minute							(00-59)		55
//%p	AM or PM designation						PM
//%S	Second							(00-61)		02
//%U	Week number with the first Sunday as the first day of week one	(00-53)		33
//%w	Weekday as a decimal number with Sunday as 0					(0-6)		4
//%W	Week number with the first Monday as the first day of week one	(00-53)		34
//%x	Date representation							08/23/01
//%X	Time representation							14:55:02
//%y	Year, last two digits			(00-99)		01
//%Y	Year										2001
//%Z	Timezone name or abbreviation				CDT
//%%	A % sign	%



std::string GetCurrentTimeString(const std::string format)
{
    //CTRef TRef = CTRef::GetCurrentTRef(TM, bUTC);

    //TRef.Format(format);

    //time_t rawtime;
    //time(&rawtime);
    //struct tm timeinfo = { 0 };
    //localtime_s(&timeinfo , &rawtime);

  // string str;
    //str.resize(150);
    //strftime(&(str[0]), 150, format.c_str(), &timeinfo);
    //str.resize(strlen(str.c_str()));
    //str.shrink_to_fit();
    assert(false);//todo

    std::time_t t=0;
    std::string str = FormatTime(format, t);

    return str;
}



std::string FormatTime(const std::string& format, std::time_t t)
{
    string str;
    assert(false);

    //struct tm newtime = { 0 };
    //_localtime64_s(&newtime , &t);
    //newtime.tm_isdst = 0;//don't use
//
    //
    //str.resize(150);
    //strftime(&(str[0]), 150, format.c_str(), &newtime);

    //strftime(str.GetBufferSetLength(150), 150, format.c_str(), newtime);
    //str.ReleaseBuffer();
    return str;
}

std::string FormatTime(const std::string& format, int year, size_t m, size_t d, size_t h, size_t minute, size_t second)
{
    assert(m < 12);
    assert(d < GetNbDaysPerMonth(year, m));

    struct tm timeinfo = { 0 };
    timeinfo.tm_isdst = 0;
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = int(m);//month in base zero
    timeinfo.tm_mday = int(d + 1);//day in base one
    timeinfo.tm_hour = int(h);
    timeinfo.tm_min = int(minute);
    timeinfo.tm_sec = int(second);
    timeinfo.tm_yday = (int)WBSF::GetDOY(year, m, d);

    string str;
    str.resize(150);
    strftime(&(str[0]), 150, format.c_str(), &timeinfo);

    //strftime(str.GetBufferSetLength(150), 150, format.c_str(), &timeinfo);
    //str.ReleaseBuffer();

    return str;
}

std::time_t get_time64(int year, size_t m, size_t d, size_t h, size_t minute, size_t second)
{
    assert(year>=1900);

    struct tm timeinfo = { 0 };
    timeinfo.tm_isdst = 0;//never daylight saving
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = int(m);//month in base zero
    timeinfo.tm_mday = int(d + 1);//day in base one
    timeinfo.tm_hour = int(h);
    timeinfo.tm_min = int(minute);
    timeinfo.tm_sec = int(second);
    timeinfo.tm_yday = (int)WBSF::GetDOY(year, m, d);

    return mktime(&timeinfo);
}



//lat: latitude (in degree)
//DOY: day of the year
// return day length (seconds)
double GetDayLength(double latDeg, size_t DOY)
{
    //size_t jd = date.GetDOY();
    assert(latDeg >= -90 && latDeg <= 90);
    assert(DOY < 366);
    static const double SECPERRAD = 13750.9871;     // seconds per radian of hour angle
    static const double RADPERDAY = 0.017214;       // radians of Earth orbit per julian day
    static const double RADPERDEG = 0.01745329;     // radians per degree
    static const double MINDECL = -0.4092797;       // minimum declination (radians)
    static const double DAYSOFF = 11.25;            // julian day offset of winter solstice


    // precalculate the transcendentals
    // check for (+/-) 90 degrees latitude, throws off daylength calc
    double lat = latDeg * RADPERDEG;
    if (lat > 1.5707) lat = 1.5707;
    if (lat < -1.5707) lat = -1.5707;
    double coslat = cos(lat);
    double sinlat = sin(lat);

    // calculate cos and sin of declination
    double decl = MINDECL * cos(((double)DOY + DAYSOFF) * RADPERDAY);
    double cosdecl = cos(decl);
    double sindecl = sin(decl);

    // calculate day length as a function of lat and decl
    double cosegeom = coslat * cosdecl;
    double sinegeom = sinlat * sindecl;
    double coshss = -(sinegeom) / cosegeom;
    if (coshss < -1.0) coshss = -1.0;  // 24-hr daylight
    if (coshss > 1.0) coshss = 1.0;    // 0-hr daylight
    double hss = acos(coshss);                // hour angle at sunset (radians)
    // day length (seconds)
    double daylength = 2.0 * hss * SECPERRAD;

    return daylength;
}




}//namespace WBSF




