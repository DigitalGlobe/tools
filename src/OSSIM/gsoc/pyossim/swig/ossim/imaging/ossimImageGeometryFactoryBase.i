/*-----------------------------------------------------------------------------
 * Filename        : ossimImageGeometryFactoryBase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageGeometryFactoryBase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
%}

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Wrapping the class ossimImageGeometryFactory */
%include "ossim/imaging/ossimImageGeometryFactoryBase.h"

