//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Common code / functions.
//
//----------------------------------------------------------------------------
// $Id: Ipt.h 11216 2007-06-14 21:20:55Z dburken $
#ifndef pqeCommon_HEADER
#define pqeCommon_HEADER 1

#include <cfloat>
#include <cmath>
#include <pqe/Constants.h>

namespace pqe
{
    /**
     * isnan Test for floating point Not A Number (NAN) value.
     * This should be used test for nan.
     * DO NOT USE operator==.  Like "if (height == ossim::nan())"
     *
     * @return true if nan, false if not.
     *
     * @see nan()
     */
#if defined(WIN32) || defined(_MSC_VER) && !defined(__CYGWIN__) && !defined(__MWERKS__)
  inline bool isnan(const float& v) { return _isnan(v); }
  inline bool isnan(const pqe_float64& v) { return _isnan(v); }
#elif defined(sun) || defined(__sun)
#    if defined(__SVR4) || defined(__svr4__)
/* Solaris */
    inline bool isnan(const float& v)  { return ( ::isnan(v) ); }
    inline bool isnan(const pqe_float64& v) { return ( ::isnan(v) ); }
#    else
/* SunOS */
    inline bool isnan(const float& v)  { return ( ::isnan(v) ); }
    inline bool isnan(const pqe_float64& v) { return ( ::isnan(v) ); }
#   endif
#else
/* linux */    
    inline bool isnan(const float& v)  { return ( std::isnan(v) ); }
    inline bool isnan(const pqe_float64& v) { return ( std::isnan(v) ); }
#endif

    /** @brief Class lets us see bit patterns of floats. */
    class PQEDLL IntFloatBitCoercion
    {
    public:
       union
       { 
          pqe_int64 intname;
          pqe_float64 floatname;
       } bits;
       IntFloatBitCoercion(pqe_int64 x)   { bits.intname = x;   }
       IntFloatBitCoercion(pqe_float64 x) { bits.floatname = x; }
    };
    
    /**
     * @brief Declaration of nan part of nan() declared here for inline
     * pqe::nan().
     */
    extern PQEDLL_DATA(const IntFloatBitCoercion) nanValue;
    
    /**
     * @brief Method to return ieee floating point double precision NAN.
     *
     * @return ieee floating point double precision NAN.
     *
     * @see isnan()
     *
     * @note casts seamlessly to float and long double.
     *
     * @note Use pqe::isnan(v) to test for nan.
     * Like "if (isnan(myDoubleValue)) { doSomething; }"
     * DO NOT USE operator==.  Like "if (myDoubleValue == pqe::nan())"
     */
    inline pqe_float64 nan() { return nanValue.bits.floatname; }

    inline double radiansToDegrees(pqe_float64 x) { return x*DEG_PER_RAD;}
    inline double degreesToRadians(pqe_float64 x) { return x*RAD_PER_DEG;}
    
    //---
    // trig functions:
    //---

    /**
     * @param x degrees
     * @return sin of x.
     */
    inline pqe_float64 sind(pqe_float64 x) { return std::sin(x*RAD_PER_DEG); }

    /**
     * @param x degrees
     * @return cosine of x.
     */
    inline double cosd(double x) { return std::cos(x*RAD_PER_DEG); }
    
    /**
     * @param x radians
     * @return arc cosine of x in degrees.
     */
    inline pqe_float64 acosd(pqe_float64 x)
    { return DEG_PER_RAD*std::acos(x); }

    
    // identical to copysign() but usable in templates
    // signum function, returns 0, 1, -1, or nan
    template <class T> inline T sgn(T x) 
    {
       const T table[] = {T(0), T(1), T(-1)};
       return table[((x < T(0)) << 1) | (x > T(0))];
    }

    // signum function, returns 0, 1, -1, or nan
    template <> inline pqe_float32 sgn<pqe_float32>(pqe_float32 x) 
    {
       const pqe_float32 table[] = { pqe_float32(0), pqe_float32(1),
                                       pqe_float32(-1)};
       return pqe::isnan(x) ? x :
          table[((x < pqe_float32(0)) << 1) | (x > pqe_float32(0))];
    }

    // signum function, returns 0, 1, -1, or nan
    template <> inline pqe_float64 sgn(pqe_float64 x) 
    {
       const pqe_float64 table[] = { pqe_float64(0), pqe_float64(1),
                                       pqe_float64(-1)};
       return pqe::isnan(x) ? x :
          table[((x < pqe_float64(0)) << 1) | (x > pqe_float64(0))];
    }
    
} // End of namespace pqe.

#endif /* End of #ifndef pqeCommon_HEADER */
