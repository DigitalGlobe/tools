/*-----------------------------------------------------------------------------
 * Filename        : ossimEpsgDatumFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDatumFactoryInterface.h>
#include <ossim/base/ossimEpsgDatumFactory.h>
#include <ossim/base/ossimString.h>
#include <map>
#include <string>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEpsgDatumFactory.h"
