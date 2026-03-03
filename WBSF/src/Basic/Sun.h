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

#define _USE_MATH_DEFINES
#include <cmath>

#include "UtilTime.h"

namespace WBSF
{


class CSunBase
{
public:

    CSunBase(double lat, double lon, double zone = -999);

    double GetSunrise(int year, size_t m, size_t d)const
    {
        Compute(year, m, d);
        return m_sunrise;
    }
    double GetSunset(int year, size_t m, size_t d)const
    {
        Compute(year, m, d);
        return m_sunset;
    }
    double GetSolarNoon(int year, size_t m, size_t d)const
    {
        Compute(year, m, d);
        return m_solarNoon;
    }
    double GetDayLength(int year, size_t m, size_t d)const
    {
        Compute(year, m, d);
        return m_daylength;
    }


    double GetLat()const
    {
        return m_lat;
    }
    double GetLon()const
    {
        return m_lon;
    }
    double GetZone()const
    {
        return m_zone;
    }

protected:


    //input
    double m_lat;
    double m_lon;
    double m_zone;

    //output
    int m_year;
    size_t m_month;
    size_t m_day;
    double m_sunrise;
    double m_sunset;
    double m_daylength;
    double m_solarNoon;


    void Compute(int year, size_t m, size_t d)const;
    static void Compute(double lat, double lon, double zone, int year, size_t m, size_t d, double& sunrise, double& sunset, double& solarNoon, double& daylength);

    // Get the days to J2000
    // h is UT in decimal hours
    // FNday only works between 1901 to 2099 - see Meeus chapter 7
    static double FNday(int year, int month, int day, float h);

    // the function below returns an angle in the range ( 0 to 2*pi )
    static double FNrange(double x);

    // Calculating the hourangle
    static double f0(double lat, double declin);

    // Calculating the hourangle for twilight times
    static double f1(double lat, double declin);

    // Find the ecliptic int32_titude of the Sun
    static double FNsun(double d);

    static const double pi;
    static const double tpi;
    static const double degs;
    static const double rads;

};




class CSun : public CSunBase
{
public:

    CSun(double lat, double lon, double zone = -999):
        CSunBase(lat, lon, zone )
        {

        }

    double GetSunrise(CTRef Tref)const
    {
        Compute(Tref.GetYear(), Tref.GetMonth(), Tref.GetDay());
        return m_sunrise;
    }
    double GetSunset(CTRef Tref)const
    {
        Compute(Tref.GetYear(), Tref.GetMonth(), Tref.GetDay());
        return m_sunset;
    }
    double GetSolarNoon(CTRef Tref)const
    {
        Compute(Tref.GetYear(), Tref.GetMonth(), Tref.GetDay());
        return m_solarNoon;
    }
    double GetDayLength(CTRef Tref)const
    {
        Compute(Tref.GetYear(), Tref.GetMonth(), Tref.GetDay());
        return m_daylength;
    }

};

//double GetDayLength(double latDeg, size_t DOY);

}//namespace WBSF


