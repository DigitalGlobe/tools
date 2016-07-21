/*-----------------------------------------------------------------------------
 * Filename        : ossimInfo.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/util/ossimInfo.h>

#include <ostream>

%}

/* Wrapping class ossimInfo */
%include "ossim/base/ossimConstants.h"
%include "ossim/util/ossimInfo.h"
