/*-----------------------------------------------------------------------------
 * Filename        : ossimEnvironmentUtility.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimEnvironmentUtility.h>
%}

%rename(const_getDataSearchPath) getDataSearchPath()const;
%rename(const_getPluginSearchPath) getPluginSearchPath()const;

/* Wrapping class ossimLsrRay */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEnvironmentUtility.h"
