/*-----------------------------------------------------------------------------
 * Filename        : ossimEvent.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimEvent
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimObject.h>
%}

%rename(const_ossimEvent_getObject) ossimEvent::getObject() const;
%rename(const_ossimEvent_getCurrentObject) ossimEvent::getCurrentObject() const;

/* Wrapping the class ossimEvent */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEvent.h"

