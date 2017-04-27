/*

*/

%module pyossim

%{
#include <vector>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimPropertyEvent.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA
#endif
/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageCombiner.h"
                                          

