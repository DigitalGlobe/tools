/*-----------------------------------------------------------------------------
Filename        : ossimLine.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimLine
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimLine.h>
%}

/* Handling operators */
%rename(__lshift__) operator<<;

/* Wrapping class ossimLine */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimLine.h"
