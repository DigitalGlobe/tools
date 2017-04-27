/*-----------------------------------------------------------------------------
 * Filename        : ossimContainerEvent.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>

%}

%rename(const_ossimContainerEvent_getAssociatedObject) ossimContainerEvent::getAssociatedObject() const;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimContainerEvent.h"

