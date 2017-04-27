/*-----------------------------------------------------------------------------
Filename        : ossimIrect.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimIrect
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimIrect.h>
%}

/* Handling operators */
%rename(__set__) ossimIrect::operator=;
%rename(__ne__) ossimIrect::operator!=;
%rename(__mul__) operator*;
%rename(__cmp__) ossimIrect::operator==;
%rename(__add__) ossimIrect::operator+;
%rename(__sub__) ossimIrect::operator-;
%rename(__iadd__) ossimIrect::operator+=;
%rename(__isub__) ossimIrect::operator-=;
%rename(__imul__) ossimIrect::operator*=;
%rename(__lshift__) operator<<;

%rename(ossimIrect_print) print;

/* Wrapping class ossimIrect */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimIrect.h"
