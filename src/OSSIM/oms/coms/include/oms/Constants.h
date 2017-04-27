//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Constants for oms.
//
// $Id: Constants.h 17135 2010-04-16 19:38:29Z gpotts $
//----------------------------------------------------------------------------
#ifndef omsConstants_HEADER
#define omsConstants_HEADER 1
#include <ossim/base/ossimConstants.h>
/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OMSEXPORT __declspec(dllexport)
#  define OMSIMPORT __declspec(dllimport)
#  ifdef OMS_LIBRARY
#    define OMSDLL       OMSEXPORT
#  else
#    define OMSDLL      OMSIMPORT
#  endif
#else /* not #if defined(_MSC_VER) */
#  define OMSEXPORT
#  define OMSIMPORT
#  define OMSDLL
#endif /* #if defined(_MSC_VER) */

#endif /* #ifndef omsConstants_HEADER */
