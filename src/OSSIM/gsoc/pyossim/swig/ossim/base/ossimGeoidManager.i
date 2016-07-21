/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoidManager.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <vector>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimGeoid.h>
#include <ossim/base/ossimGeoidManager.h>
%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoidManager.h" 

