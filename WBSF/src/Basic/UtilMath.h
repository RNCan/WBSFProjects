//******************************************************************************
//  project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <cassert>
#include <iostream>
#include <string>
//#include "Basic/UtilStd.h"




namespace WBSF
{


//inline const int Max(int x, int y){ return (x > y) ? x : y; }
//template<class T, class U> inline double Max(const T& x, const U& y){ return double((x > y) ? x : y); }
//template<class T> inline const T& Max(const T& x, const T& y){ return (x > y) ? x : y; }


//inline const float Min(float x, float y){ return (x < y)?x:y; }
//inline const double Min(double x, double y){ return (x < y)?x:y; }
// template function returns y if x > y
// To avoid conflicts with min and max in WINDEF.H
//inline const int Min(int x, int y){ return (x < y) ? x : y; }
//template<class T, class U> inline double Min(const T& x, const U& y){ return double((x < y) ? x : y); }
//template<class T> inline const T& Min(const T& x, const T& y){ return (x < y) ? x : y; }


static const double VMISS = -9999999.f;
static const double EPSILON_DATA = 1.0e-5;

static const double PI = 3.1415926535897932384626433832795;
//static const double TWO_PI = PI * 2;
//static const double DEG2RAD = 2 * PI / 360.0;
//static const double RAD2DEG = 360.0 / (2 * PI);
//static const double FEET2METER = 0.3048;
//static const double METER2FEET = 1 / FEET2METER;

inline double Deg2Rad(double d)
{
    return d* 2 * PI / 360.0;
}
inline double Rad2Deg(double r)
{
    return r* 360.0 / (2 * PI);
}
inline double Feet2Meter(double f)
{
    return f* 0.3048;
}
inline double Meter2Feet(double m)
{
    return m/ 0.3048;
}

//template<class T>
//int Signe(const T& a)
//{
//    return a >= 0 ? 1 : -1;
//}

template <typename T> int signe(T val) {
    return (T(0) < val) - (val < T(0));
}

template<class T>
int64_t Trunk(const T& a, double prec = 0.0000000001)
{
    return int64_t(a + signe(a)*prec);
}

inline int TrunkLowest(double a, double prec = 0.0000000001)
{
    return (int)floor(a + prec);
}

template<class T>
void Switch(T& x, T& y)
{
    T tmp = x;
    x = y;
    y = tmp;
}

template<class T>
T square(const T& x)
{
    return x*x;
}

template <class T>
T cube(const T& x)
{
    return x*x*x;
}

template <class T>
T quart(const T&  x)
{
    return x*x*x*x;
}

//template<class T = int64_t, class U>
//T round(const U& a)
//{
//    return T(int64_t(a + 0.5*signe(a)));
//}

using std::round;
inline double round(double a, size_t digit)
{
    a *= pow(10, digit);
    a = double(std::round(a));
    a /= pow(10, digit);
    return a;
}





double e0(double Tair);//hourly vapor pressure
double e0(double Tmin, double Tmax);//daily vapor pressure

double Pv2Hs(double Pv);
double Pv2Hr(double Tair, double Pv); //hourly algo
double Pv2Hr(double Tmin, double Tmax, double Pv); //daily algo
double Hs2Pv(double Pv);
double Hs2Hr(double Tair, double Hs);//hourly algo
double Hs2Hr(double Tmin, double Tmax, double Hs);//daily algo

double Hr2Pv(double Tair, double Pv);
double Hr2Pv(double Tmin, double Tmax, double Hr);
double Hr2Hs(double Tair, double Hr);
double Hr2Hs(double Tmin, double Tmax, double Hr);

double Td2Hs(double Tair, double Td);
double Td2Pv(double Td);
double Td2Hr(double Tair, double Td);
double Hr2Td(double Tair, double RH);
double Pv2Td(double Td);
double Hs2Td(double Tair, double Hs);

double Hr2Twb(double Tair, double Hr);
double Twb2Hr(double Tair, double Twb);
void GetWindUV(double windSpeed, double windDir, double& U, double &V, bool from=true);
double GetWindDirection(double u, double v, bool from = true);

double GetPressure(double alt);
double GetAltitude(double P);

enum THourlyGenration { HG_DOUBLE_SINE, HG_SINE_EXP_BRANDSMA, HG_SINE_EXP_SAVAGE, HG_SINE_POWER, HG_ERBS, HG_ALLEN_WAVE, HG_POLAR, NB_HOURLY_GENERATION };
enum TSineExponential { SE_BRANDSMA, SE_SAVAGE, NB_SINE_EXP };

double GetPolarWinter(double Tmin[3], double Tmax[3], double h);
double GetPolarSummer(double Tmin[3], double Tmax[3], double h);
double GetSineExponential(double Tmin[3], double Tmax[3], double  h, double Tr, double Ts, size_t method = SE_SAVAGE);
double GetSinePower(double Tmin[3], double Tmax[3], double t, double Tsr, double Tss);
double GetErbs(double Tmin[3], double Tmax[3], double t);
double GetDoubleSine(double Tmin[3], double Tmax[3], double h, double hourTmin, double hourTmax);


double msl2atp(double msl, double elev);//mean sea level to atmospheric pressure (at elevation)
double atp2msl(double atp, double elev);//atmospheric pressure (at elevation) to mean sea level

double InvLogistic(double y, double k1, double k2);
double InvWeibull(double y, double alpha, double beta, double gamma);
double Weibull(double t, double tb, double tm, double b1, double b2, double b3, double b4);
double erf(double x);
double erfc(double x);


// Random
void    Randomize(unsigned rand = 0);
int     Rand(int num = RAND_MAX);
int     Rand(int l, int u);
double	Rand(double l, double u);
double  Randu(bool bExcLower = false, bool bExcUpper = false);
double  RanduExclusif(void);
double  RandNormal(double x, double s);
double  RandLogNormal(double x, double s);




double  Polynom(double x, int degree, const double *coeffs); //in MATH.H but not ANSI
double  Linearize(double x1, double y1, double x3, double y3, double x2);
double  zScore(void);




enum TSchoolfieldParameters { SF_PRHO25, SF_PHA, SF_PHL, SF_PTL, SF_PHH, SF_PTH, NB_SCHOOLFIELD_PARAMETERS };
double Schoolfield(const double p[NB_SCHOOLFIELD_PARAMETERS], double T);

//compute exposition from latitude, slope(%) and orienttion(°)
double GetExposition(double lat, double slopePourcent, double orientation);
void ComputeSlopeAndAspect(double lat, double exposition, double& slopePourcent, double& aspect);

inline int GetDegrees(double dec, double mult = 1)
{
    int nbSec = long(dec * 3600 * mult + signe(dec)*0.5);
    return int(nbSec / (3600 * mult));
}

inline int GetMinutes(double dec, double mult = 1)
{
    int nbSec = abs(int(dec * 3600 * mult + signe(dec)*0.5));
    return int((nbSec - abs(GetDegrees(dec, mult)) * 3600 * mult) / (60 * mult));
}
inline int GetSeconds(double dec)
{
    int nbSec = abs(int(dec * 3600 + signe(dec)*0.5));
    return int(nbSec - (abs(GetDegrees(dec)) * 3600 + GetMinutes(dec) * 60));
}

inline double GetSeconds(double dec, double mult)
{
    int nbSec = abs(int(dec * 3600 * mult + signe(dec)*0.5));
    return (nbSec - (abs(GetDegrees(dec, mult)) * 3600 * mult + GetMinutes(dec, mult) * 60 * mult)) / mult;
}

inline double GetDecimalDegree(int degree, int minute, double second = 0)
{
    assert(degree >= -360 && degree <= 360);
    assert(minute >= 0 && minute <= 60);
    assert(second >= 0 && second <= 60);


    int signe = degree < 0 ? -1 : 1;

    double degDec = degree + (minute*signe) / 60.0 + (second*signe) / 3600.0;

    return degDec;
}

double StringToCoord(const std::string& coordStr);
std::string Coordto_string(double coord, int prec = 0);


//Get distance in meter from 2 points
double GetDistance(double lat1, double lon1, double lat2, double lon2);

bool IsValidSlopeWindow(double window[3][3], double noData);
void GetSlopeAndAspect(double window[3][3], double ewres, double nsres, double scale, double& slope, double& aspect);


// Note that the functions Gamma and LogGamma are mutually dependent.
double LogGamma(double);
double Gamma(double);
inline double Factorial(double x)
{
    return Gamma(x + 1);
}
inline double LogFactorial(double x)
{
    return LogGamma(x + 1);
}




class CMathEvaluation
{
public:
    enum TOperation { UNKNOWN = -1, EQUAL, NOT_EQUAL, GREATER_EQUAL, LOWER_EQUAL, GREATER, LOWER, NB_OPERATORS };
    static const char* OPERATOR_STR[NB_OPERATORS];


    CMathEvaluation(const char* str);
    bool Evaluate(double value)const
    {
        return Evaluate(value, m_op, m_value);
    }

    static TOperation GetOp(const std::string& str);
    static bool Evaluate(double value1, TOperation op, double value2);

protected:

    TOperation m_op;
    double m_value;

    static const double EPSILON;
};

//	//**************************************************
//	class CSchoolfield
//	{
//	public:
//
//		double operator[](double T)const;
//
//		enum TParameters{ PRHO25, PHA, PHL, PTL, PHH, PTH, NB_PARAMETERS };
//		const double m_p[NB_PARAMETERS];
//	};
//
//	class CUnknown
//	{
//	public:
//		double operator[](double T)const;
//
//		enum TParametersE1{ PT0, PA, PB, PX, NB_PARAMETERS };
//		const double m_p[NB_PARAMETERS];
//	};

template <typename RealType = double>
class beta_distribution
{
public:
    typedef RealType result_type;

    class param_type
    {
    public:
        typedef beta_distribution distribution_type;

        explicit param_type(RealType a = 2.0, RealType b = 2.0)
            : a_param(a), b_param(b) { }

        RealType a() const
        {
            return a_param;
        }
        RealType b() const
        {
            return b_param;
        }

        bool operator==(const param_type& other) const
        {
            return (a_param == other.a_param &&
                    b_param == other.b_param);
        }

        bool operator!=(const param_type& other) const
        {
            return !(*this == other);
        }

    private:
        RealType a_param, b_param;
    };

    explicit beta_distribution(RealType a = 2.0, RealType b = 2.0)
        : a_gamma(a), b_gamma(b) { }
    explicit beta_distribution(const param_type& param)
        : a_gamma(param.a()), b_gamma(param.b()) { }

    void reset() { }

    param_type param() const
    {
        return param_type(a(), b());
    }

    void param(const param_type& param)
    {
        a_gamma = gamma_dist_type(param.a());
        b_gamma = gamma_dist_type(param.b());
    }

    template <typename URNG>
    result_type operator()(URNG& engine)
    {
        return generate(engine, a_gamma, b_gamma);
    }

    template <typename URNG>
    result_type operator()(URNG& engine, const param_type& param)
    {
        gamma_dist_type a_param_gamma(param.a()),
                        b_param_gamma(param.b());
        return generate(engine, a_param_gamma, b_param_gamma);
    }

    result_type min() const
    {
        return 0.0;
    }
    result_type max() const
    {
        return 1.0;
    }

    result_type a() const
    {
        return a_gamma.alpha();
    }
    result_type b() const
    {
        return b_gamma.alpha();
    }

    bool operator==(const beta_distribution<result_type>& other) const
    {
        return (param() == other.param() &&
                a_gamma == other.a_gamma &&
                b_gamma == other.b_gamma);
    }

    bool operator!=(const beta_distribution<result_type>& other) const
    {
        return !(*this == other);
    }

private:
    typedef std::gamma_distribution<result_type> gamma_dist_type;

    gamma_dist_type a_gamma, b_gamma;

    template <typename URNG>
    result_type generate(URNG& engine,
                         gamma_dist_type& x_gamma,
                         gamma_dist_type& y_gamma)
    {
        result_type x = x_gamma(engine);
        return x / (x + y_gamma(engine));
    }
};

template <typename CharT, typename RealType>
std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os,
                                      const beta_distribution<RealType>& beta)
{
    os << "~Beta(" << beta.a() << "," << beta.b() << ")";
    return os;
}

template <typename CharT, typename RealType>
std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is,
                                      beta_distribution<RealType>& beta)
{
    std::string str;
    RealType a, b;
    if (std::getline(is, str, '(') && str == "~Beta" &&
            is >> a && is.get() == ',' && is >> b && is.get() == ')')
    {
        beta = beta_distribution<RealType>(a, b);
    }
    else
    {
        is.setstate(std::ios::failbit);
    }
    return is;
}


class CRandomGenerator
{
public:


    enum TSeed { RANDOM_SEED, FIXE_SEED };
    //static const int RAND_MAX_INT = 2147483600;//2147483647;


    CRandomGenerator(size_t	seed = 0)
    //	m_uniformInt(0, RAND_MAX_INT)
    {
        Randomize(seed);
    }

    void Randomize(size_t seed = RANDOM_SEED);


    unsigned int Rand()const
    {
        std::uniform_int_distribution<> uniformInt;
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return uniformInt(me.m_gen);
    }

    //returns a double on the interval
    //[0,1] : rand()/RAND_MAX
    //]0,1] : (rand()+1)/(RAND_MAX+1)
    //[0,1[ : rand()/(RAND_MAX+1)
    //]0,1[ : (rand()+1)/(RAND_MAX+2)
    double Randu(bool bExcLower = false, bool bExcUpper = false)const
    {
        std::uniform_int_distribution<> uniformInt;
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);

        long double numerator = (long double)uniformInt(me.m_gen);
        long double denominator = (long double)uniformInt.max();

        if (bExcLower)
        {
            numerator++;
            denominator++;
        }

        if (bExcUpper)
            denominator++;

        double u = double (numerator / denominator);

        assert(bExcLower || (u >= 0));
        assert(!bExcLower || (u > 0));
        assert(bExcUpper || (u <= 1));
        assert(!bExcUpper || (u < 1));

        return u;
    }

    //special case of randu
    double RanduExclusive(void)const
    {
        return Randu(true, true);
    }
    double Randv(void) const
    {
        //std::uniform_real_distribution <double> uniformReal(0, 1);
        //return uniformReal(m_gen);

        return Randu(false, true);
    }

    //returns an integer on the range [0,num]
    int Rand(int num) const
    {
        double x = (num + 1)*Randu(false, true);
        return (int)x;
    }

    //returns an integer on the interval [l,u] or [l,u[ or ]l,u] or ]l,u[
    int Rand(int l, int u) const
    {
        if (l > u)
            WBSF::Switch(l, u);

        assert(l <= u);

        //std::uniform_int<int> uniformInt(l, u);
        //return uniformInt(m_gen);

        //get a number [0, u-l] and add l
        return l + Rand(u - l);
    }

    double Rand(double l, double u) const
    {
        if (l > u)
            WBSF::Switch(l, u);

        assert(l <= u);

        //get a number [0, u-l] and add l
        return l + (u - l)*Randu();
    }

    //return value between [low,high[
    double RandUniform(double low = 0, double high=1)const
    {
        std::uniform_real_distribution<> uniformDist(low, high);
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return uniformDist(me.m_gen);
    }

    double RandBeta(const double alpha, const double beta) const
    {
        beta_distribution<double> betaDist(alpha, beta);
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return betaDist(me.m_gen);
    }

    //rand normal take mean and standard deviation
    double RandNormal(const double x, const double s) const
    {
        std::normal_distribution<double> uniformNormal(x, s);
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return uniformNormal(me.m_gen);
    }

    //rand log normal take mean and standard deviation
    double RandUnbiasedLogNormal(double x, double s)const
    {
        std::lognormal_distribution<double> uniformLogNormal(x - WBSF::square(s) / 2.0, s);
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return uniformLogNormal(me.m_gen);
    }

    double RandGumbel(double a, double b)const
    {
        std::extreme_value_distribution<double> gumbel(a, b);
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return gumbel(me.m_gen);
    }

    double RandWeibull(double a, double b)const
    {
        std::weibull_distribution<double> weibull(a, b);
        CRandomGenerator& me = const_cast<CRandomGenerator&>(*this);
        return weibull(me.m_gen);
    }



protected:


    std::mt19937 m_gen;

};



}//namespace WBSF
