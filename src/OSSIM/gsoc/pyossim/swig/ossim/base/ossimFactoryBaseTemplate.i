%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <list>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimFactoryBaseTemplate.h"

%template(ossimFactoryBase) ossimFactoryBase<Product>;
