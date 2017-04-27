%module pyossim

%{
#include <iosfwd>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimListenerManager.h>
%}

%rename(ossimObject_print) ossimProcessInterface::print;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimProcessInterface.h"
