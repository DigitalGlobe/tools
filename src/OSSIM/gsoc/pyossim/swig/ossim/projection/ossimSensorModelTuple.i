/*-----------------------------------------------------------------------------
 * Filename        : ossimSensorModelTuple.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimSensorModelTuple
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimSensorModelTuple.h>
%}

/* Handling operators */
%rename(ossimSensorModelTuple_print) print;

/*typedef vector<ossimDpt> DptSet_t;*/

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimSensorModelTuple */
%include "ossim/projection/ossimSensorModelTuple.h"
