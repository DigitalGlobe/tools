/*-----------------------------------------------------------------------------
 * Filename        : ossimCsvFile.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimCsvFile.h>
#include <map>
#include <vector>
#include <fstream>
%}

/* Handling operators */
%rename(__getitem__) ossimCsvFile::operator[];

%include "ossim/base/ossimConstants.h"

/* Handling class */
%include "ossim/base/ossimCsvFile.h"
