/*-----------------------------------------------------------------------------
 * Filename        : ossimDatumFactoryRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDatumFactoryRegistry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDatumFactoryRegistry.h> 
%}

%include "ossim/base/ossimConstants.h"                  /* for OSSIM_DLL macro */

/* Wrapping the class ossimDatumFactoryRegistry */
%include "ossim/base/ossimDatumFactoryRegistry.h" 
