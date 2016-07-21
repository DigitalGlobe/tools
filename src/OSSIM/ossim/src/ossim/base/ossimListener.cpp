//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  Garrett Potts 
//
//*******************************************************************
//  $Id: ossimListener.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <ossim/base/ossimListener.h>

RTTI_DEF(ossimListener, "ossimListener");

ossimListener::ossimListener()
  :theListenerEnableFlag(true)
{}

ossimListener::~ossimListener()
{
}

void ossimListener::processEvent(ossimEvent& /* event */)
{
}

void ossimListener::enableListener()
{
  theListenerEnableFlag = true;
}

void ossimListener::disableListener()
{
  theListenerEnableFlag = false;
}

void ossimListener::setListenerEnableFlag(bool flag)
{
  theListenerEnableFlag = flag;
}

bool ossimListener::isListenerEnabled()const
{
  return theListenerEnableFlag;
}

bool ossimListener::getListenerEnableFlag()const
{
  return theListenerEnableFlag;
}

