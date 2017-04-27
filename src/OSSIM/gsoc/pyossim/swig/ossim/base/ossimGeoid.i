/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoid.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
%}

%rename(ossimGeoid_open) ossimGeoid::open(ossimFilename const &);

%include "ossim/base/ossimConstants.h"

%include "ossim/base/ossimGeoid.h"
