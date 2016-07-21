/*-----------------------------------------------------------------------------
 * Filename        : ossimStringProperty.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimStringProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimStringProperty.h>
%}

/* Wrapping the class ossimStringProperty */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimStringProperty.h"

