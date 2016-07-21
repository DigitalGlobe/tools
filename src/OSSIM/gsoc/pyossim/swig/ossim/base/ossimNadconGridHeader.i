/*-----------------------------------------------------------------------------
 * Filename        : ossimNadconGridHeader.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimNadconGridHeader
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimNadconGridHeader.h>
%}

/* Handling operators */
%rename(__lshift__) operator<<;

/* Wrapping class ossimNadconGridHeader */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimNadconGridHeader.h"
