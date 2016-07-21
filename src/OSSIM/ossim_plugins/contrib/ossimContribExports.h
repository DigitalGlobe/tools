//----------------------------------------------------------------------------
// Copyright (c) 2005, Garrett Potts, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: Constants file for ossim plugins.
//
// $Id: ossimContribExports.h 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimContribExports_HEADER
#define ossimContribExports_HEADER

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIM_CONTRIB_EXPORT __declspec(dllexport)
#  define OSSIM_CONTRIB_IMPORT __declspec(dllexport)

#  ifdef OSSIM_CONTRIB_MAKINGDLL
#     define OSSIM_CONTRIB_DLL OSSIM_CONTRIB_EXPORT
#  else
#     define  OSSIM_CONTRIB_DLL OSSIM_CONTRIB_IMPORT
#  endif
#else
# define OSSIM_CONTRIB_DLL
#endif

#endif /* #ifndef ossimRegistrationExports_HEADER */

