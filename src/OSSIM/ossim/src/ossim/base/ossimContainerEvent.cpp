//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerEvent.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossim/base/ossimContainerEvent.h>

RTTI_DEF1(ossimContainerEvent, "ossimContainerEvent", ossimEvent);

ossimContainerEvent::ossimContainerEvent(ossimObject* obj,
                                         long id)
   :ossimEvent(obj, id)
{
}

void ossimContainerEvent::setObjectList(ossimObject* obj)
{
   m_objectList.clear();
   m_objectList.push_back(obj);
}

void ossimContainerEvent::setObjectList(ObjectList& objects)
{
   m_objectList = objects;
}

