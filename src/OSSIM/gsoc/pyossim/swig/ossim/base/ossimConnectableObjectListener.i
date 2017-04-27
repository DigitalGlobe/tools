/*-----------------------------------------------------------------------------
 * Filename        : ossimConnectableObjectListener.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimConnectableObjectListener
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConnectableObjectListener.h>
%}

/* Wrapping the class ossimConnectableObjectListener */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimConnectableObjectListener.h"

