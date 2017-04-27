/*-----------------------------------------------------------------------------
Filename        : ossimProjection.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimProjection which is the 
                  Base class for all projections (2D-to-3D transform).
-----------------------------------------------------------------------------*/

%module pyossim



#ifndef TYPE_DATA
#define TYPE_DATA
#endif
%rename(__set__) ossimGpt::operator=;
%rename(ossimProjection_print) print;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimProjection */
%include "ossim/projection/ossimProjection.h"


