/*-----------------------------------------------------------------------------
 * Filename        : ossimIgenGenerator.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimIgenGenerator
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/imaging/ossimIgenGenerator.h>
%}

%rename(ossimIgenGenerator_setOutput) ossimIgenGenerator::setOutput(ossimKeywordlist &);

/* Wrapping class ossimIgenGenerator */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimIgenGenerator.h"

