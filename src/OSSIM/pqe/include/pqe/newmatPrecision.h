//$$ precisio.h                          floating point constants

#ifndef pqe_newmatPrecision_HEADER
#define pqe_newmatPrecision_HEADER 1

#include <cmath>
#include <limits>
using namespace std;

#include <pqe/newmatInclude.h>

namespace pqe
{
   class FloatingPointPrecision
   {
   public:
      static int Dig()              // number of decimal digits or precision
      { return numeric_limits<Real>::digits10 ; }
      
      static Real Epsilon()         // smallest number such that 1+Eps!=Eps
      { return numeric_limits<Real>::epsilon(); }
      
      static int Mantissa()         // bits in mantisa
      { return numeric_limits<Real>::digits; }
      
      static Real Maximum()         // maximum value
      { return numeric_limits<Real>::max(); }
      
      static int MaximumDecimalExponent()  // maximum decimal exponent
      { return numeric_limits<Real>::max_exponent10; }
      
      static int MaximumExponent()  // maximum binary exponent
      { return numeric_limits<Real>::max_exponent; }
      
      static Real LnMaximum()       // natural log of maximum
      { return (Real)std::log(Maximum()); }
      
      static Real Minimum()         // minimum positive value
      { return numeric_limits<Real>::min(); } 
      
      static int MinimumDecimalExponent() // minimum decimal exponent
      { return numeric_limits<Real>::min_exponent10; }
      
      static int MinimumExponent()  // minimum binary exponent
      { return numeric_limits<Real>::min_exponent; }
      
      static Real LnMinimum()       // natural log of minimum
      { return (Real)std::log(Minimum()); }
      
      static int Radix()            // exponent radix
      { return numeric_limits<Real>::radix; }
      
      static int Rounds()           // addition rounding (1 = does round)
      {
         return numeric_limits<Real>::round_style ==
            round_to_nearest ? 1 : 0;
      }
      
   }; // class FloatingPointPrecision

} // namespace pqe

#endif /* #ifndef pqe_newmatPrecision_HEADER */
