/*-----------------------------------------------------------------------------
 * Filename        : ossimFileWalker.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/


%module pyossim

%{

#include <string>
#include <vector>
#include <ossim/base/ossimCallback2wRet.h>
#include <ossim/base/ossimConstants.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

%rename(const_ossimFileWalker_getFilteredExtensions) ossimFileWalker::getFilteredExtensions() const;

/* Wrapping class ossimFileWalker */
%include "ossim/base/ossimConstants.h"
%include "ossim/util/ossimFileWalker.h"

#endif
