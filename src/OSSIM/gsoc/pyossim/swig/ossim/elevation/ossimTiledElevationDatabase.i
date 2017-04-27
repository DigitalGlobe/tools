/*-----------------------------------------------------------------------------
 * Filename        : ossimTiledElevationDatabase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/elevation/ossimElevationDatabase.h>
#include <ossim/elevation/ossimTiledElevationDatabase.h>

#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimRtti.h>

#include <vector>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling operators */
%rename(__set__) ossimTiledElevationDatabase::operator=;

%include "ossim/base/ossimConstants.h"


/**
 * @class ossimTiledElevationDatabase
 *
 * Elevation source used for working with generic images opened by an
 * ossimImageHandler.  This class supplies a mapRegion method used to map a
 * region of elevation to a grid.  The grid in turn is used for the
 * getHeightAboveMSL.  This class is for applications that know their region
 * of interest up front and want to bypass the ossimElevManager and grid the
 * elevation prior to processing for speed.  Can work on a file or a
 * directory of files.
 */
%include "ossim/elevation/ossimTiledElevationDatabase.h"

#endif
