/*-----------------------------------------------------------------------------
 * Filename        : ossimImageGeometryFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageGeometryFactory 
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageGeometryFactoryBase.h>
#include <ossim/imaging/ossimImageGeometryFactory.h>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/projection/ossimProjection.h>

%}

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Wrapping the class ossimImageGeometryFactory */
%include "ossim/imaging/ossimImageGeometryFactory.h"

