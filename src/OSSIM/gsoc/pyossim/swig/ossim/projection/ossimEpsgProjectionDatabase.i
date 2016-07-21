/*-----------------------------------------------------------------------------
 * Filename        : ossimEpsgProjectionDatabase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <ossim/base/ossimFilename.h>
#include <fstream>
#include <ossim/projection/ossimMapProjection.h>
%}

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimEpsgProjectionDatabase.h"
