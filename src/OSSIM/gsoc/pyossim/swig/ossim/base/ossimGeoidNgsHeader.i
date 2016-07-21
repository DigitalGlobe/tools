/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoidNgsHeader.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimGeoidNgsHeader
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGeoidNgsHeader.h>
#include <iostream>
%}

%rename(__lshift__) operator<<;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoidNgsHeader.h"
