//----------------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// $Id: ossimProcessListener.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimProcessProgressEvent.h>

RTTI_DEF1(ossimProcessListener, "ossimProcessListener", ossimListener);

ossimProcessListener::ossimProcessListener()
   : ossimListener()
{}

ossimProcessListener::~ossimProcessListener()
{}

void ossimProcessListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
   case OSSIM_EVENT_PROCESS_PROGRESS_ID:
   {
      ossimProcessProgressEvent* eventCast = static_cast<ossimProcessProgressEvent*>(&event);
      processProgressEvent(*eventCast);
      break;
   }
   default:
   {
      ossimListener::processEvent(event);
      break;
   }
   }
}

void ossimProcessListener::processProgressEvent(ossimProcessProgressEvent& /* event */ )
{}

