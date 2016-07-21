/*-----------------------------------------------------------------------------
 * Filename        : ossimStatePlaneProjectionInfo.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ostream>

#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimString.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
%}

%rename(__lshift__) operator<<;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimStatePlaneProjectionInfo.h"
