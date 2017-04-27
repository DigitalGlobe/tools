/*-----------------------------------------------------------------------------
 * Filename        : ossimEquTokenizer.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimEquTokenDefines.h>
#include <ossim/base/ossimFlexLexer.h>
%}

%include "ossim/base/ossimEquTokenizer.h"
