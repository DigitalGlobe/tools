%module pyossim

%{
#include <ossim/imaging/ossimNormalizedRemapTable.h>
#include <ossim/imaging/ossimNormalizedU8RemapTable.h>
#include <ossim/base/ossimCommon.h> /* for round */
%}

%rename(__getitem__) ossimNormalizedU8RemapTable::operator[];

%include "ossim/base/ossimConstants.h"

/**
 * @class ossimNormalizedU8RemapTable
 *
 * @brief Eight bit normalized remap table to go to/from normalized value
 * to pixel value.
 */
%include "ossim/imaging/ossimNormalizedU8RemapTable.h"
