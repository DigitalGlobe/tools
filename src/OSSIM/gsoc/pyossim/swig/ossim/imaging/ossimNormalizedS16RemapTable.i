%module pyossim

%{
#include <ossim/imaging/ossimNormalizedRemapTable.h>
#include <ossim/imaging/ossimNormalizedS16RemapTable.h>
#include <ossim/base/ossimCommon.h> /* for round */
%}

%rename(__getitem__) ossimNormalizedS16RemapTable::operator[];

%include "ossim/base/ossimConstants.h"

/**
 * @class ossimNormalizedS16RemapTable
 *
 * @brief Signed 16 bit normalized remap tables to go to/from
 * normalized value to pixel value.
 */
%include "ossim/imaging/ossimNormalizedS16RemapTable.h"
