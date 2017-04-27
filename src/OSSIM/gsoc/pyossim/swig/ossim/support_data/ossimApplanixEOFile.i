/*

*/

%module pyossim

%{
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <iostream>
#include <map>
#include <vector>
#include <ossim/support_data/ossimApplanixEOFile.h>
%}
%template(ossimApplanixEORecordRefPtr)ossimRefPtr<ossimApplanixEORecord>;

%include "ossim/base/ossimConstants.h"
%include "ossim/support_data/ossimApplanixEOFile.h"
