/*-----------------------------------------------------------------------------
 * Filename        : ossim2dTo2dTransformFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransformFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossim2dTo2dTransformFactoryBase.h>
#include <ossim/base/ossim2dTo2dTransformFactory.h>

%}

/* Wrapping class ossim2dTo2dTransformFactory */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossim2dTo2dTransformFactory.h"
