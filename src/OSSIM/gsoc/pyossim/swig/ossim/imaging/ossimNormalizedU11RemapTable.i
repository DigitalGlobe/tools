%module pyossim

%{
#include <ossim/imaging/ossimNormalizedRemapTable.h>
#include <ossim/imaging/ossimNormalizedU11RemapTable.h>
#include <ossim/base/ossimCommon.h> /* for round */
%}

%rename(__set__) ossimNormalizedU11RemapTable::operator=;
%rename(__getitem__) ossimNormalizedU11RemapTable::operator[];

%include "ossim/base/ossimConstants.h"

/* Wrapping the ossimImageSource class */

/**
 * @class ossimNormalizedU11RemapTable
 *
 * @brief Eleven bit normalized remap table to go to/from normalized value
 * to pixel value.
 */
%include "ossim/imaging/ossimNormalizedU11RemapTable.h"
