/*-----------------------------------------------------------------------------
Filename        : ossimInfoBase.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimInfoBase
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/support_data/ossimInfoBase.h>

%}

/* Handling unclean parsing of Macros */
#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the header file containing the declarations to be wrapped */
%import "ossim/base/ossimConstants.h"
%import "ossim/base/ossimReferenced.h"

%include "ossim/support_data/ossimInfoBase.h"

#endif
