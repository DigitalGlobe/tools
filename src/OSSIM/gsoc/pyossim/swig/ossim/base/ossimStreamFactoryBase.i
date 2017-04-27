/*-----------------------------------------------------------------------------
 * Filename        : ossimStreamFactoryBase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimStreamFactoryBase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIoStream.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimStreamFactoryBase.h" 
