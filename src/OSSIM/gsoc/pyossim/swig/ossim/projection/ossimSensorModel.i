/*-----------------------------------------------------------------------------
 * Filename        : ossimSensorModel.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimSensorModel
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimCommon.h> /* for ossim::nan() */
#include <ossim/elevation/ossimElevSource.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/base/ossimException.h>
%}        

#ifndef TYPE_DATA
#define TYPE_DATA
#endif
/* Handling operators */
%rename(__cmp__) ossimSensorModel::operator==;
%rename(ossimSensorModel_print) print;

%include "ossim/base/ossimConstants.h"

/* Wrapping class ossimSensorModel */    
%include "ossim/projection/ossimSensorModel.h"


