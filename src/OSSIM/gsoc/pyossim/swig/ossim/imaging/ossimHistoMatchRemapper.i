/*-----------------------------------------------------------------------------
 * Filename        : ossimHistoMatchRemapper.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimHistoMatchRemapper
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimHistoMatchRemapper.h>

%}

/* Include the header file containing the declarations to be wrapped */
%import "ossim/base/ossimConstants.h"


/* Wrapping the class ossimHistoMatchRemapper */
%include "ossim/imaging/ossimImageSourceFilter.h"
%include "ossim/imaging/ossimHistoMatchRemapper.h"
