%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimNormalizedRemapTable.h>
%}

%rename(__getitem__) ossimNormalizedRemapTable::operator[];

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"

/**
 * @class ossimNormalizedRemapTable
 *
 * @brief Base class implemetation of normalized remap tables to go to/from
 * normalized value to pixel value.
 */
%include "ossim/imaging/ossimNormalizedRemapTable.h"
