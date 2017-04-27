%module pyossim

%{
#include <iostream>
#include <ossim/imaging/ossimAnnotationSource.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/base/ossimViewInterface.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(__lshift__) operator<<;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeoAnnotationSource.h"

#endif
