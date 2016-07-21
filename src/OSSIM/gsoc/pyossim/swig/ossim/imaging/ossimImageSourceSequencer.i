/*-----------------------------------------------------------------------------
Filename        : ossimImageSourceSequencer.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageData
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimConnectableObjectListener.h>

%}
/*
#ifndef TYPE_DATA
#define TYPE_DATA

%include "typemaps.i"
*/
/* Typemaps to convert values from Python to C++ */
/*
%typemap(in) ossimjni_int8
{
                $1 = PyString_AsString($input);
}

%typemap(in) ossimjni_int32
{
                $1 = PyInt_AsLong($input);
}

%typemap(in) ossimjni_int64
{
                $1 = PyLong_AsLongLong($input);
}

%typemap(in) ossimjni_uint64
{
                $1 = PyLong_AsUnsignedLongLong
}

%typemap(in) ossimjni_float64
{
                $1 = PyFloat_AsDouble($input);
}
*/

/* Typemaps to convert from C++ to Python */
/*
%typemap(out) ossimjni_int32
{
                $result = PyInt_FromLong((long) $1);
}

%typemap(out) ossimjni_int64
{
                $result = PyLong_FromLongLong($1);
}

%typemap(out) ossimjni_uint64
{
                $result = PyLong_FromUnsignedLongLong($1);
}

%typemap(out) ossimjni_float64
{
                $result = PyFloat_FromDouble($1);
}

*/
/* Applying the typemaps to other basic primitives */

/*
%apply ossimjni_int8 { ossimjni_sint8, ossimjni_uint8 };
%apply ossimjni_int32 { ossimjni_int16, ossimjni_uint16, ossimjni_uint16, ossimjni_uint32, ossimjni_sint32 };
%apply ossimjni_int64 { ossimjni_sint64 };
%apply ossimjni_float64 { ossimjni_float32 };
*/

/* Include the header constants file */
%import "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageSourceSequencer */
%include "ossim/imaging/ossimImageSourceSequencer.h"

/*#endif        */
