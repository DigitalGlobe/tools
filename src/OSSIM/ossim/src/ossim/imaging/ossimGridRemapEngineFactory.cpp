//*****************************************************************************
// FILE: ossimGridRemapEngineFactory.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimGridRemapEngineFactory
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGridRemapEngineFactory.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossim/imaging/ossimGridRemapEngineFactory.h>
#include <ossim/imaging/ossimHsvGridRemapEngine.h>
#include <ossim/imaging/ossimRgbGridRemapEngine.h>
#include <ossim/imaging/ossimMonoGridRemapEngine.h>

//*****************************************************************************
//  STATIC METHOD: ossimGridRemapEngineFactory::create()
//  
//*****************************************************************************
ossimGridRemapEngine* ossimGridRemapEngineFactory::create(const char* s)
{
   if (!s)
      return 0;
   
   if (strcmp(s, "ossimHsvGridRemapEngine") == 0)
      return new ossimHsvGridRemapEngine;

   else  if (strcmp(s, "ossimRgbGridRemapEngine") == 0)
      return new ossimRgbGridRemapEngine;

   else  if (strcmp(s, "ossimMonoGridRemapEngine") == 0)
      return new ossimMonoGridRemapEngine;

   return 0;
}


