//----------------------------------------------------------------------------
// File: Constants.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Constants for ossimjni c++ library.
//
//----------------------------------------------------------------------------
// $Id: Constants.h 19748 2011-06-12 15:34:56Z dburken $

#ifndef ossimjniConstants_HEADER
#define ossimjniConstants_HEADER 1

/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIMJNIEXPORT __declspec(dllexport)
#  define OSSIMJNIIMPORT __declspec(dllimport)
#  ifdef OSSIMJNI_LIBRARY
#    define OSSIMJNIDLL       OSSIMJNIEXPORT
#  else
#    define OSSIMJNIDLL      OSSIMJNIIMPORT
#  endif
#else /* not #if defined(_MSC_VER) */
#  define OSSIMJNIEXPORT
#  define OSSIMJNIIMPORT
#  define OSSIMJNIDLL
#endif /* #if defined(_MSC_VER) */

// Primative data type typedefs
typedef char               ossimjni_int8;
typedef signed char        ossimjni_sint8;
typedef unsigned char      ossimjni_uint8;

typedef short              ossimjni_int16;
typedef unsigned short     ossimjni_uint16;
typedef signed short       ossimjni_sint16;

typedef int                ossimjni_int32;
typedef unsigned int       ossimjni_uint32;
typedef signed int         ossimjni_sint32;

typedef long long          ossimjni_int64;
typedef unsigned long long ossimjni_uint64;
typedef signed long long   ossimjni_sint64;

typedef float              ossimjni_float32;
typedef double             ossimjni_float64;

#endif /* #ifndef ossimjniConstants_HEADER */
