%module pyossim

%{
#include <cstring>
#include <ossim/base/ossimConstants.h>
%}

%rename(__eq__) RTTItypeid::operator==;
%rename(__ne__) RTTItypeid::operator!=;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimRtti.h"
