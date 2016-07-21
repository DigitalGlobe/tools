/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <map>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConnectableObjectListener.h>
//#include <ossim/base/ossimConnectableContainerInterface.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(const_ossimConnectableContainer_getObject) ossimConnectableContainer::getObject() const;

%include "../base/ossimConnectableContainerInterface.i"
/* Wrapping class ossimConnectableContainer */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimConnectableContainer.h" 

#endif
