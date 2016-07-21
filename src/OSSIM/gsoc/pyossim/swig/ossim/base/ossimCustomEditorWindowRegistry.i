/*-----------------------------------------------------------------------------
 * Filename        : ossimCustomEditorWindowRegistry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimCustomEditorWindow.h>
#include <ossim/base/ossimCustomEditorWindowRegistry.h>
#include <ossim/base/ossimCustomEditorWindowFactoryBase.h>

#include <vector>
%}

/* Handling operators */
%rename(__set__) ossimCustomEditorWindowRegistry::operator=;

%include "ossim/base/ossimConstants.h"

/* Handling class */
%include "ossim/base/ossimCustomEditorWindowRegistry.h"

