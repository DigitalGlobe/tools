%module pyossim

%{
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimQuickbirdTiffTileSource.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimQuickbirdTiffTileSource.h"

#endif
