/*-----------------------------------------------------------------------------
 * Filename        : ossimFilenameProperty.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <vector>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimString.h>

%}

/* Handling assignment operator */
%rename(__set__) ossimFilenameProperty::operator=;

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimFilenameProperty.h"

