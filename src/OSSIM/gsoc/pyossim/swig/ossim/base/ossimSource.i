%module pyossim

%{
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimConstants.h>
%}

%rename(__set__) ossimSource::operator=;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimErrorStatusInterface.h"
%include "ossim/base/ossimSource.h"
