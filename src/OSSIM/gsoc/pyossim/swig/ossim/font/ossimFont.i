/*-----------------------------------------------------------------------------
 * Filename        : ossimFont.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimFont
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/font/ossimFont.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Wrapping class ossimFont */
%include "ossim/font/ossimFont.h"

#endif
