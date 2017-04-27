//----------------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// $Id: ossimProcessListener.h 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimProcessListener_HEADER
#define ossimProcessListener_HEADER
#include <ossim/base/ossimListener.h>

class ossimProcessProgressEvent;

class OSSIMDLLEXPORT ossimProcessListener : public ossimListener
{
public:
   ossimProcessListener();
   virtual ~ossimProcessListener();
   virtual void processEvent(ossimEvent& event);
   virtual void processProgressEvent(ossimProcessProgressEvent& event);

TYPE_DATA
};

#endif

