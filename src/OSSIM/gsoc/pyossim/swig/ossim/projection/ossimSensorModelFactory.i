/*-----------------------------------------------------------------------------
 * Filename        : ossimSensorModelFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimSensorModelFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjectionFactoryBase.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimSensorModelFactory */
%include "ossim/projection/ossimProjectionFactoryBase.h"
%include "ossim/projection/ossimSensorModelFactory.h"

#endif
