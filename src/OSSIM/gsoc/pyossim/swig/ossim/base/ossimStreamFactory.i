/*-----------------------------------------------------------------------------
 * Filename        : ossimStreamFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimStreamFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimStreamFactoryBase.h>
#include <ossim/base/ossimStreamFactory.h>
#include <ossim/base/ossimIoStream.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimStreamFactory.h" 
