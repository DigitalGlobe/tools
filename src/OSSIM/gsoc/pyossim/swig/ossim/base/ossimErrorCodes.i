/*-----------------------------------------------------------------------------
 * Filename        : ossimErrorCodes.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>
%}

/* Wrapping the class */
%include "ossim/base/ossimConstants.h"

/* Handling operators */
%rename(__set__) ossimErrorCodes::operator=;

/*!
 *  class ossimErrorCodes
 *  Contains class declaration with common error codes and methods to go
 *  from code to string and string to code.
 */
%include "ossim/base/ossimErrorCodes.h"
