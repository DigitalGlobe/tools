/*-----------------------------------------------------------------------------
Filename        : ossimLookUpTable.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimLookUpTable
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeyword.h>
%}        

%rename(__getitem__) ossimLookUpTable::operator[];

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimDatum */
%include "ossim/base/ossimLookUpTable.h"

