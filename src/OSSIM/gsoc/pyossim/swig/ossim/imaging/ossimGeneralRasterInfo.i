/*-----------------------------------------------------------------------------
Filename        : ossimGeneralRasterInfo.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimGeneralRasterInfo
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimGeneralRasterInfo.h>

#include <vector>

%}

/* Handling ossimGeneralRasterInfo assignment operator */
%rename(__set__) ossimGeneralRasterInfo::operator=;

/* Handling the reserved print function */
%rename(ossimGeneralRasterInfo_print) ossimGeneralRasterInfo::print;

/* Handling the std::vector for Python */
%include "std_vector.i"
namespace std
{
        %template(vectorOssimFileName) std::vector<ossimFilename>;
}

/* Wrapping the class ossimGeneralRasterInfo */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimGeneralRasterInfo.h"

