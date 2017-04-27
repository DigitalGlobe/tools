/*-----------------------------------------------------------------------------
 * Filename        : ossimElevCellHandlerFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimElevCellHandlerFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <list>
#include <ossim/base/ossimFactoryBaseTemplate.h>
#include <ossim/elevation/ossimElevCellHandler.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimElevCellHandlerFactory */
%include "ossim/base/ossimFactoryBaseTemplate.h"

%template(ossimFactoryBase_ossimElevCellHandler) ossimFactoryBase< ossimElevCellHandler >;

%include "ossim/elevation/ossimElevCellHandlerFactory.h"

#endif
