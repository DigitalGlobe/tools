/*-----------------------------------------------------------------------------
Filename        : ossimObject.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimObject
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimHistogramRemapper assignment operator */
%rename(__lshift__) operator<<;

/* Handling the reserved function print */
%rename(ossimObject_print) ossimObject::print;

/* Wrapping the class ossimObject */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimObject.h"

#endif
