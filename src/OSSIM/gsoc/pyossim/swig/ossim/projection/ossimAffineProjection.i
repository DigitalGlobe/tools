%module pyossim

%{
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimAffineProjection.h>
#include <ossim/base/ossimIpt.h>
#include <iosfwd>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <ossim/base/ossimRefPtr.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%rename(ossimAffineProjection_print) print;
%rename(__cmp__) ossimAffineProjection::operator==;
%rename(__lshift__) operator<<;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/projection/ossimAffineProjection.h"

#endif
