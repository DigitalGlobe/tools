/*-----------------------------------------------------------------------------
 * Filename        : ossimEllipsoid.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <cmath> /* std::sqrt */

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/matrix/newmat.h>
%}

/* Handling operators */
%rename(__set__) ossimEllipsoid::operator=;
%rename(__cmp__) ossimEllipsoid::operator==;
%rename(__ne__) ossimEllipsoid::operator!=;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimEllipsoid.h"
