/*-----------------------------------------------------------------------------
Filename        : ossimDatumFactoryInterface.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimDatumFactoryInterface
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <vector>
%}        

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimDatumFactoryInterface */
%include "ossim/base/ossimDatumFactoryInterface.h"

