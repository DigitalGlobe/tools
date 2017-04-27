/*-----------------------------------------------------------------------------
 * Filename        : ossimCustomEditorWindow.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimCustomEditorWindow.h>
%}

/* Handling operators */
%rename(__set__) ossimCustomEditorWindow::operator=;

%include "ossim/base/ossimConstants.h"

/* Handling class ossimDtedHandler */
%include "ossim/base/ossimCustomEditorWindow.h"

