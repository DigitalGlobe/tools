/*-----------------------------------------------------------------------------
Filename        : ossimDatum.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimDatum
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDatum.h>
%}        

/* Handling ossimHistogramRemapper assignment operator */
%rename(__cmp__) ossimHistogramRemapper::operator==;
%rename(__ne__) ossimHistogramRemapper::operator!=;


%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimDatum */
%include "ossim/base/ossimDatum.h"

