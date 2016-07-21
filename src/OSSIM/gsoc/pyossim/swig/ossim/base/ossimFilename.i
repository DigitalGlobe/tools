/*-----------------------------------------------------------------------------
 * Filename        : ossimFilename.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimProperty
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

%}

/* Handling assignment operator */

%rename(const_wildCardRemove) ossimFilename::wildcardRemove() const;
%rename(const_remove) ossimFilename::remove() const;

%rename(__cmp__) ossimFilename::operator==;

/* Wrapping class */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimFilename.h"
