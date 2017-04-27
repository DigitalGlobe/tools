//*****************************************************************************
// FILE: ossimTrace.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimTrace
//
// SOFTWARE HISTORY:
//>
//   24Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id: ossimTrace.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimTraceManager.h>
#include <ossim/base/ossimCommon.h>
//*****************************************************************************
//  CONSTRUCTOR: ossimTrace
//  
//*****************************************************************************
ossimTrace::ossimTrace(const ossimString& trace_name)
   :
      theTraceName   (trace_name),
      theEnabledFlag (false)
{
   ossimTraceManager::instance()->addTrace(this);
}

ossimTrace::~ossimTrace()
{
   ossimTraceManager::instance()->removeTrace(this);
}


