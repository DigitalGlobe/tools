/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoTiffDatumLut.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrPoint
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimGeoTiffDatumLut.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoTiffDatumLut.h"
