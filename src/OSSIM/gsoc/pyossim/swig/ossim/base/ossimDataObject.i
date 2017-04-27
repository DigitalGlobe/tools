/*-----------------------------------------------------------------------------
 * Filename        : ossimDataObject.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDataObject
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDpt3d.h>
%}

/* OPERATORS */
/* Handling operators */
%rename(__set__) ossimDataObject::operator=;
%rename(__cmp__) ossimDataObject::operator==;
%rename(__ne__) ossimDataObject::operator!=;
%rename(ossimDataObject_print) print;

%rename(const_ossimDataObject_getOwner) ossimDataObject::getOwner() const;
%rename(const_ossimDataObject_operator_eq) ossimDataObject::operator=(ossimDataObject const &);

/* Wrapping class ossimDataObject */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDataObject.h"

