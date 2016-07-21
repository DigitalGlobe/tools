/*-----------------------------------------------------------------------------
Filename        : ossimImageMetaData.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageMetaData 
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/imaging/ossimImageMetaData.h>
%}

/* Handling ossimImageMetaData Assignment operator */
%rename(__set__) ossimImageMetaData::operator=;

/* Include the header file containing the declarations to be wrapped */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageMetaData.h"
