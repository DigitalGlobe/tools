/*-----------------------------------------------------------------------------
 * Filename        : ossimImageHandlerFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageHandlerFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

#ifndef TYPE_DATA
#define TYPE_DATA

%{
#include <ossim/imaging/ossimImageHandlerFactory.h>
%}

%rename(__set__) ossimImageHandlerFactory::operator=; 

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageHandlerFactory */
%include "ossim/imaging/ossimImageHandlerFactory.h"

#endif
