/*-----------------------------------------------------------------------------
Filename        : ossimJpegWriter.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimJpegWriter 
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/imaging/ossimJpegWriter.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA
#endif

/* Wrapping the class ossimJpegWriter */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimJpegWriter.h"

