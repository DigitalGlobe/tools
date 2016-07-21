/*-----------------------------------------------------------------------------
Filename        : ossimFontInformation.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimObject
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iosfwd>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFontInformation.h>
%}

%typemap(varin) const char * {
SWIG_Error(SWIG_AttributeError,"Variable $symname is read-only.");
SWIG_fail;
}


/* Handling operators */
%rename(__set__) ossimFontInformation::operator=;
%rename(__cmp__) ossimFontInformation::operator==;
%rename(__ne__) ossimFontInformation::operator!=;
%rename(__lshift__) operator<<;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimFontInformation.h"
