%module pyossim

%{
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimBitMaskWriter.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimOverviewBuilderBase.h"

#endif
