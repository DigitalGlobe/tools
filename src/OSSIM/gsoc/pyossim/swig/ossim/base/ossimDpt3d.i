/*-----------------------------------------------------------------------------
Filename        : ossimDpt3d.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimDpt3d 
-----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimCommon.h>             /* for ossim::isnan */
#include <ossim/base/ossimDpt3d.h>
%}

/* Handling operators */
%rename(__cmp__) ossimDpt3d::operator==;
%rename(__ne__) ossimDpt3d::operator!=;
%rename(__add__) ossimDpt3d::operator+;
%rename(__sub__) ossimDpt3d::operator-;
%rename(__mul__) ossimDpt3d::operator*;
%rename(__div__) ossimDpt3d::operator/;
%rename(__iadd__) ossimDpt3d::operator+=;
%rename(__isub__) ossimDpt3d::operator-=;
%rename(__imul__) ossimDpt3d::operator*=;
%rename(__idiv__) ossimDpt3d::operator/=;
%rename(__lshift__) operator<<;

/* Wrapping the class ossimDpt3d */
%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimDpt3d.h"
