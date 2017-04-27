//----------------------------------------------------------------------------
// 
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Constants for pqe.
//
// $Id: Constants.h 11216 2007-06-14 21:20:55Z dburken $
//----------------------------------------------------------------------------
#ifndef pqe_Constants_HEADER
#define pqe_Constants_HEADER 1

#include <cfloat>

/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define PQEEXPORT __declspec(dllexport)
#  define PQEIMPORT __declspec(dllimport)
#  ifdef PQEMAKINGDLL
#    define PQEDLL       PQEEXPORT
#    define PQEDLL_DATA(type) PQEEXPORT type
#  else
#    define PQEDLL      PQEIMPORT
#    define PQEDLL_DATA(type) PQEIMPORT type
#  endif
#else /* not #if defined(_MSC_VER) */
#  define PQEEXPORT
#  define PQEIMPORT
#  define PQEDLL
#  define PQEDLL_DATA(type) type
#endif /* #if defined(_MSC_VER) */

// Primative data type typedefs
typedef char               pqe_int8;
typedef signed char        pqe_sint8;
typedef unsigned char      pqe_uint8;

typedef short              pqe_int16;
typedef unsigned short     pqe_uint16;
typedef signed short       pqe_sint16;

typedef int                pqe_int32;
typedef unsigned int       pqe_uint32;
typedef signed int         pqe_sint32;

typedef long long          pqe_int64;
typedef unsigned long long pqe_uint64;
typedef signed long long   pqe_sint64;

typedef float              pqe_float32;
typedef double             pqe_float64;

//---
// Integer nan kept for pqeIpt.
// This should be the most negative int.
//---
#define PQE_INT_NAN ((pqe_int32)0x80000000)

/*
 Definitions for scalar type identification.
*/
enum ScalarType
{
   PQE_SCALAR_UNKNOWN    =  0, 
   PQE_UINT8             =  1, /**< 8 bit unsigned integer        */
   PQE_SINT8             =  2, /**< 8 bit signed integer          */
   PQE_UINT16            =  3, /**< 16 bit unsigned integer       */
   PQE_SINT16            =  4, /**< 16 bit signed integer         */
   PQE_UINT32            =  5, /**< 32 bit unsigned integer       */
   PQE_SINT32            =  6, /**< 32 bit signed integer         */
   PQE_FLOAT32           =  7, /**< 32 bit floating point         */
   PQE_FLOAT64           =  8, /**< 64 bit floating point         */
   PQE_CINT16            =  9, /**< 16 bit complex integer        */
   PQE_CINT32            = 10, /**< 32 bit complex integer        */
   PQE_CFLOAT32          = 11, /**< 32 bit complex floating point */
   PQE_CFLOAT64          = 12, /**< 64 bit complex floating point */
};

/*
 Definitions for coordinate system type.
*/
enum pqeCoordinateSystemType
{
   PQE_COORDINATE_SYSTEM_UNKNOWN = 0,
   PQE_IMAGE_SPACE               = 1,
   PQE_GEOGRAPHIC_SPACE          = 2,
   PQE_PROJECTED_SPACE           = 3,
   PQE_GEOCENTRIC_SPACE          = 4
};

/*
 Definitions for coordinate system orientation mode .
*/
enum pqeCoordSysOrientMode
{
   PQE_LEFT_HANDED  = 0,
   PQE_RIGHT_HANDED = 1
};


#ifndef M_PI
#  define M_PI             ((pqe_float64)3.141592653589793238462643)
#endif
#ifndef TWO_PI
#  define TWO_PI           ((pqe_float64)(2.0 * M_PI))
#endif
#ifndef DEG_PER_RAD
#  define DEG_PER_RAD      ((pqe_float64)(180.0/M_PI))
#endif
#ifndef RAD_PER_DEG
#  define RAD_PER_DEG      ((pqe_float64)(M_PI/180.0))
#endif

#endif /* #ifndef pqe_Constants_HEADER */
