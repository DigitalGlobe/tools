/*-----------------------------------------------------------------------------
 * Filename        : ossimLandSatModel.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLandSatModel
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimLandSatModel.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(ossimLandSatModel_print) print;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimLandSatModel */
%include "ossim/projection/ossimLandSatModel.h"

#endif
