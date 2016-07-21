/*

*/

%{
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>
#include <iosfwd>
#include <map>
#include <vector>
#include <algorithm>
%}


%ignore ossimKeywordlist::operator[];
%rename(const_trimAllValues) trimAllValues(const ossimString& valueToTrim= ossimString(" \t\n\r"))const;
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimKeywordlist.h"
