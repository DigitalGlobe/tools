/*-----------------------------------------------------------------------------
 * Filename        : ossimEpsgProjectionFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimProjectionFactoryBase.h"
%include "ossim/projection/ossimEpsgProjectionFactory.h"

#endif
