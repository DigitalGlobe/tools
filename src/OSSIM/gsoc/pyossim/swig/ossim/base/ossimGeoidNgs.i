/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoidNgs.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossim2dTo2dTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimGeoid.h>
#include <ossim/base/ossimGeoidNgs.h>
#include <ossim/base/ossimGeoidNgsHeader.h>
%}

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoidNgs.h" 

