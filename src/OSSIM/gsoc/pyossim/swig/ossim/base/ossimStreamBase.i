/*-----------------------------------------------------------------------------
 * Filename        : ossimStreamBase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimStreamBase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>  /* for OSSIM_DLL */
#include <ossim/base/ossimReferenced.h> /* for base class */
#include <ossim/base/ossimStreamBase.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimStreamBase.h" 
