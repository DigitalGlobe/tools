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
// $Id: ossimWorldWindExports.h 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimWorldWindExports_HEADER
#define ossimWorldWindExports_HEADER

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIM_WORLDWIND_EXPORT __declspec(dllexport)
#  define OSSIM_WORLDWIND_IMPORT __declspec(dllexport)

#  ifdef OSSIM_WORLDWIND_MAKINGDLL
#     define OSSIM_WORLDWIND_DLL OSSIM_WORLDWIND_EXPORT
#  else
#     define  OSSIM_WORLDWIND_DLL OSSIM_WORLDWIND_IMPORT
#  endif
#else
# define OSSIM_WORLDWIND_DLL
#endif

#endif /* #ifndef ossimWorldWindExports_HEADER */

