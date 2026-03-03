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

#include "Sun.h"




using namespace std;


namespace WBSF
{
//**************************************************************
//CSunBase

// Revised on January 6th, 2000
// A simple C++ program calculating the sunrise and sunset for
// the current date and a set location (latitude,int32_titude)
// Jarmo Lammi © 1999 - 2000
// jjlammi@netti.fi


const double CSunBase::pi = 3.141592653589793238;
const double CSunBase::tpi = 2 * pi;
const double CSunBase::degs = 180.0 / pi;
const double CSunBase::rads = pi / 180.0;

// Get the days to J2000
// h is UT in decimal hours
// FNday only works between 1901 to 2099 - see Meeus chapter 7
double CSunBase::FNday(int year, int month, int day, float h)
{
    int64_t luku = -7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day;

    // Typecasting needed for TClite on PC DOS at least, to avoid product overflow
    luku += (int64_t)year * 367;

    return (double)luku - 730531.5 + h / 24.0;
}

// the function below returns an angle in the range 0 to 2*pi
double CSunBase::FNrange(double x)
{
    double b = x / tpi;
    double a = tpi * (b - (int32_t)(b));
    if (a < 0) a = tpi + a;

    return a;
}

// Calculating the hour angle
double CSunBase::f0(double lat, double declin)
{
    static const double  SunDia = 0.53;  // Sun radius degrees
    static const double  AirRefr = 34.0 / 60.0; // athmospheric refraction degrees

    // Correction: different sign at S HS
    double dfo = rads * (0.5*SunDia + AirRefr);
    if (lat < 0.0) dfo = -dfo;
    double fo = tan(declin + dfo) * tan(lat*rads);

    if (fo > 0.99999) fo = 1.0; // to avoid overflow //
    if (fo < -0.99999) fo = -1.0; // to avoid overflow //
    fo = asin(fo) + pi / 2.0;

    return fo;
}

// Calculating the hour angle for twilight times
double CSunBase::f1(double lat, double declin)
{
    // Correction: different sign at S HS
    double df1 = rads * 6.0;
    if (lat < 0.0) df1 = -df1;
    double fi = tan(declin + df1) * tan(lat*rads);

    if (fi > 0.99999) fi = 1.0; // to avoid overflow //
    if (fi < -0.99999) fi = -1.0; // to avoid overflow //
    fi = asin(fi) + pi / 2.0;

    return fi;
}

// Find the ecliptic altitude of the Sun
double CSunBase::FNsun(double d)
{
    // mean altitude of the Sun
    double L = FNrange(280.461 * rads + .9856474 * rads * d);

    // mean anomaly of the Sun
    double g = FNrange(357.528 * rads + .9856003 * rads * d);

    // Ecliptic altitude of the Sun
    return FNrange(L + 1.915 * rads * sin(g) + .02 * rads * sin(2 * g));
}

CSunBase::CSunBase(double lat, double lon, double zone)
{
    m_lat = lat;
    m_lon = lon;
    m_zone = zone;
}


void CSunBase::Compute(int year, size_t m, size_t d)const
{
    assert(m < 12);
    assert(d < GetNbDaysPerMonth(year, m));

    if (year != m_year || m != m_month || d != m_day)
    {
        CSunBase& me = const_cast<CSunBase&>(*this);
        me.m_year = year;
        me.m_month = (short)m;
        me.m_day = (short)d;
        me.Compute(m_lat, m_lon, m_zone, m_year, m_month, m_day, me.m_sunrise, me.m_sunset, me.m_solarNoon, me.m_daylength);
    }
}

void CSunBase::Compute(double lat, double lon, double tzone, int year, size_t m, size_t d, double& sunrise, double& sunset, double& solarNoon, double& daylength)
{
    assert(lat >= -90 && lat <= 90);
    assert(lon >= -180 && lon <= 180);
    assert(m < 12);
    assert(d < 31);


    if (tzone == -999)
        tzone = round(lon / 15);//If we don't round it, it's the "center" of the default zone. Which one?

    // Timezone hours
    double jd = FNday(year, int(m) + 1, int(d) + 1, 12);

    // Use FNsun to find the ecliptic int32_titude of the Sun
    double lambda = FNsun(jd);

    // Obliquity of the ecliptic
    double obliq = 23.439 * rads - .0000004 * rads * jd;

    // Find the RA and DEC of the Sun
    double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
    double delta = asin(sin(obliq) * sin(lambda));


    // Find the Equation of Time in minutes
    // Correction suggested by David Smith

    // mean int32_titude of the Sun
    double L = FNrange(280.461 * rads + .9856474 * rads * jd);
    double LL = L - alpha;
    if (L < pi) LL += tpi;
    double equation = 1440.0 * (1.0 - LL / tpi);


    double ha = f0(lat, delta);
    double hb = f1(lat, delta);
    double twx = hb - ha;   // length of twilight in radians
    twx = 12.0*twx / pi;      // length of twilight in degrees
    // Conversion of angle to hours and minutes
    double daylen = degs * ha / 7.5;
    if (daylen < 0.0001)
    {
        daylen = 0.0;
    }
    if (daylen > 24.0)
    {
        daylen = 24.0;
    }
    // arctic winter

    double riset = 12.0 - 12.0 * ha / pi + tzone - lon / 15.0 + equation / 60.0;
    double settm = 12.0 + 12.0 * ha / pi + tzone - lon / 15.0 + equation / 60.0;
    double noont = riset + 12.0 * ha / pi;

    //étrange que ça ne soit plus utilisé Rémi????
    //double altmax = 90.0 + delta * degs - lat;
    // Correction suggested by David Smith
    // to express as degrees from the N horizon
    // if (delta * degs > lat)
    //   altmax = 90.0 + lat - delta * degs;


//    double twam = riset - twx;      // morning twilight begin
//   double twpm = settm + twx;      // evening twilight end
    if (riset > 24.0) riset -= 24.0;
    if (riset < 0.0) riset = 0;
    if (settm > 24.0) settm -= 24.0;
    if (noont > 24.0)   noont -= 24.0;
    if (daylen >= 24)
    {
        riset = 0;
        settm = 24.0;
    }
    //set output
    sunrise = riset;
    sunset = settm;
    solarNoon = noont;
    daylength = daylen;

    assert(!std::isnan(sunrise) && std::isfinite(sunrise));
    assert(!std::isnan(sunset) && std::isfinite(sunset));
    assert(sunrise >= 0 && sunrise <= 24);
    assert(sunset >= 0 && sunset <= 24);
    assert(solarNoon >= 0 && solarNoon <= 24);
    assert(daylength >= 0 && daylength <= 24);

}

}//namespace WBSF



