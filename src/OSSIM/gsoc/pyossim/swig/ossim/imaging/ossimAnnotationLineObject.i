%module pyossim

%{
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationLineObject.h>
#include <ossim/base/ossimIpt.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimAnnotationLineObject */
%import "ossim/base/ossimConstants.h"
%import "ossim/imaging/ossimAnnotationLineObject.h"

#endif
