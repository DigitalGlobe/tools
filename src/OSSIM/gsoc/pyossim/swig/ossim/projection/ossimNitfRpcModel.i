/*-----------------------------------------------------------------------------
 * Filename        : ossimNitfRpcModel.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/projection/ossimRpcModel.h>
#include <ossim/projection/ossimNitfRpcModel.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimNitfRpcModel.h"

#endif
