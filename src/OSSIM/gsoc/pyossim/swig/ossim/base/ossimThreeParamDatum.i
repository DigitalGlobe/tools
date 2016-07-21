/*-----------------------------------------------------------------------------
 * Filename        : ossimThreeParamDatum.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimThreeParamDatum
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDatum.h>   
%}        

/* Wrapping class ossimNadconGridDatum */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDatum.h"
%include "ossim/base/ossimThreeParamDatum.h"
