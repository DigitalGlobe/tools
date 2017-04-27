/*-----------------------------------------------------------------------------
 * Filename        : ossimProperty.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimProperty.h>
%}

/* Handling assignment operator */
%rename(__set__) ossimProperty::operator=;

%rename(const_ossimProperty_asContainer) ossimProperty::asContainer() const;

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimProperty.h"

