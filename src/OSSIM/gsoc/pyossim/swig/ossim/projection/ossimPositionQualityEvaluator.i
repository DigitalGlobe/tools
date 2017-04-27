/*-----------------------------------------------------------------------------
 * Filename        : ossimPositionQualityEvaluator.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/projection/ossimPositionQualityEvaluator.h>
%}

%rename(ossimPositionQualityEvaluator_print) ossimPositionQualityEvaluator::print;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class */
%include "ossim/projection/ossimPositionQualityEvaluator.h"
