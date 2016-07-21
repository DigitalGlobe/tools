%module pyossim

%{
#include <ossim/imaging/ossimNormalizedRemapTable.h>
#include <ossim/imaging/ossimNormalizedU16RemapTable.h>
#include <ossim/base/ossimCommon.h> /* for round */
%}

%rename(__getitem__) ossimNormalizedU16RemapTable::operator[];

%include "ossim/base/ossimConstants.h"

/**
 * @class ossimNormalizedU16RemapTable
 *
 * @brief Unsigned 16 bit normalized remap table to go to/from normalized value
 * to pixel value.
 */
%include "ossim/imaging/ossimNormalizedU16RemapTable.h"
