/*

*/

%module pyossim

%{
#include <vector>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/imaging/ossimFilterTable.h>
#include <ossim/imaging/ossimFilterResampler.h>
%}

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimFilterResampler.h"
