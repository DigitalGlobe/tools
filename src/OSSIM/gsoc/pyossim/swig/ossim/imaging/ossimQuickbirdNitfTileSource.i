%module pyossim

%{
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/imaging/ossimQuickbirdNitfTileSource.h>
#include <ossim/base/ossim2dTo2dTransform.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping class ossimQuickbirdNitfTileSource */
%include "ossim/imaging/ossimQuickbirdNitfTileSource.h"

#endif
