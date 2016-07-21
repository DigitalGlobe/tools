/*-----------------------------------------------------------------------------
 * Filename        : ossimNitfMapModel.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimNitfMapModel.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <iostream>
%}       

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling the reserved function print */
%rename(ossimNitfMapModel_print) ossimNitfMapModel::print;

%include "ossim/base/ossimConstants.h"

%include "ossim/projection/ossimNitfMapModel.h"

#endif
