%module pyossim

%{
#include <vector>
#include <iostream>
#include <ossim/matrix/newmat.h>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimConvolutionSource.h>
%}       

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimConvolutionSource.h"

#endif
