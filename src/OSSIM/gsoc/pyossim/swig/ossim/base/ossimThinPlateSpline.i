/*-----------------------------------------------------------------------------
 * Filename        : ossimThinPlateSpline.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimThinPlateSpline
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <vector>
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimThinPlateSpline.h>
%}

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimThinPlateSpline.h" 
