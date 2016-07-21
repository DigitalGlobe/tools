/*-----------------------------------------------------------------------------
 * Filename        : ossimRpfUtil.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/


%module pyossim

%{

#include <ossim/base/ossimReferenced.h>
#include <ossim/imaging/ossimImageGeometry.h> 
#include <ossim/util/ossimRpfUtil.h>

%}

/* Wrapping class ossimRpfUtil */
%include "ossim/base/ossimConstants.h"
%include "ossim/util/ossimRpfUtil.h"
