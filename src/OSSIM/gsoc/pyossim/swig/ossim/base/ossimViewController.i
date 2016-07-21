/*-----------------------------------------------------------------------------
 * Filename        : ossimViewController.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimViewController
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimViewController.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/*!
 * All view sources will derive from this class.  For example
 * we can have a mapped view or Perspective view of the scene
 * being rendered.  All sources within the containers are expected
 * to derive from the view interface.
 */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimViewController.h"

#endif
