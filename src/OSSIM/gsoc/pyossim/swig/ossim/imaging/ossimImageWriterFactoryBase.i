/*-----------------------------------------------------------------------------
 * Filename        : ossimImageWriterFactoryBase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageWriterFactoryBase
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageWriterFactoryBase.h>
#include <vector>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the required header files */
%import "ossim/base/ossimConstants.h"

/* Wrapping the class ossimImageWriterFactoryBase */
%include "ossim/imaging/ossimImageWriterFactoryBase.h"

#endif                                    
