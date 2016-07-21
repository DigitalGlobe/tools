/*-----------------------------------------------------------------------------
* Filename        : ossimFactoryListInterface.i
* Author          : Vipul Raheja
* License         : See top level LICENSE.txt file.
* Description     : Contains SWIG-Python of class ossimFactoryListInterface
* -----------------------------------------------------------------------------*/

%module pyossim


%{
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <vector>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
%}

%include "ossim/base/ossimFactoryListInterface.h"
