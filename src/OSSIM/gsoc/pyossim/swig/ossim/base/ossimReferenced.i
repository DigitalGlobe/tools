/*-----------------------------------------------------------------------------
 * Filename        : ossimReferenced.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimReferenced
 * -----------------------------------------------------------------------------*/
 

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
%}

%rename(__set__) ossimReferenced::operator=;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimReferenced.h"
