%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>
#include <ossim/imaging/ossimOverviewBuilderFactory.h>
%}

%rename(__set__) ossimOverviewBuilderFactory::operator=;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimOverviewBuilderFactory.h"
