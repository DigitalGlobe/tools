%module pyossim

%{
#include <ossim/base/ossimFactoryBaseTemplate.h>
#include <ossim/imaging/ossimAnnotationObject.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimAnnotationObject.h"
%include "ossim/base/ossimFactoryBaseTemplate.h"

%template(ossimFactoryBase_ossimAnnotationObject) ossimFactoryBase<ossimAnnotationObject>;

%include "ossim/imaging/ossimAnnotationObjectFactory.h" 

#endif
