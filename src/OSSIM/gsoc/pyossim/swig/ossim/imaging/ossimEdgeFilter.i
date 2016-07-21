%module pyossim

%{
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimEdgeFilter.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/**
 * class ossimEdgeFilter
 *
 * This has default implementation for the certain filter types.  For the
 * different filter type please see method setFilterType.
 *
 *
 */
%include "ossim/imaging/ossimEdgeFilter.h"

#endif
