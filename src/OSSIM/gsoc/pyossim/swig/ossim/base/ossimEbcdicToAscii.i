/*-----------------------------------------------------------------------------
 * Filename        : ossimEbcdicToAscii.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimEbcdicToAscii.h>
%}

%include "ossim/base/ossimConstants.h"

/* Handling class */
%include "ossim/base/ossimEbcdicToAscii.h"
