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
// $Id: ossimNuiExports.h 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimNuiExports_HEADER
#define ossimNuiExports_HEADER

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIM_NUI_EXPORT __declspec(dllexport)
#  define OSSIM_NUI_IMPORT __declspec(dllexport)

#  ifdef OSSIM_NUI_MAKINGDLL
#     define OSSIM_NUI_DLL OSSIM_NUI_EXPORT
#  else
#     define  OSSIM_NUI_DLL OSSIM_NUI_IMPORT
#  endif
#else
# define OSSIM_NUI_DLL
#endif

#endif /* #ifndef ossimNuiExports_HEADER */

