/*-----------------------------------------------------------------------------
Filename        : ossimImageSource.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageSource 
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimIrect.h>
#include <tiffio.h>

%}


/* Handling ossimImageSource Assignment operator */

/* Include the header file containing the constants */
%import "ossim/base/ossimConstants.h"

/* Wrapping the ossimImageSource class */
%include "ossim/imaging/ossimTiffTileSource.h"
