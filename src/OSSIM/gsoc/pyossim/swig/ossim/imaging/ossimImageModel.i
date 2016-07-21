/*-----------------------------------------------------------------------------
Filename        : ossimImageModel.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageModel 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/imaging/ossimImageModel.h>

%}

/* Handling unclean parsing of Macros */
#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling the vector<ossimDpt> */
%include "std_vector.i"
namespace std
{
        %template(vectorossimDpt) vector<ossimDpt>;
};
        
/* Include the header file containing the declarations to be wrapped */
%import "ossim/base/ossimDrect.h"
%import "ossim/base/ossimConstants.h"

%include "ossim/imaging/ossimImageModel.h"

#endif
