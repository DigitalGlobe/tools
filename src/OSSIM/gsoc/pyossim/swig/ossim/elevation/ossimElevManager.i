/*-----------------------------------------------------------------------------
 * Filename        : ossimElevManager.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimElevManager
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/elevation/ossimElevSource.h>
#include <ossim/elevation/ossimElevationDatabase.h>
#include <OpenThreads/ReadWriteMutex>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(const_ossimElevManager_getElevationDatabase) ossimElevManager::getElevationDatabase(ossim_uint32) const;
%rename(const_ossimElevManager_getElevationDatabaseList) ossimElevManager::getElevationDatabaseList() const;

%include "ossim/base/ossimConstants.h"

%include "ossim/elevation/ossimElevSource.h"

/* Wrapping class ossimElevManager */
%include "ossim/elevation/ossimElevManager.h"

#endif
