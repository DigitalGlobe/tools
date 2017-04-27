/*-----------------------------------------------------------------------------
Filename        : ossimImageSourceFactory.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageSourceFactory
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactory.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimImageSourceFactory Assignment operator */
%rename(__set__) ossimImageSourceFactory::operator=;

/* Wrapping class ossimImageSourceFactory */
%include "ossim/imaging/ossimImageSourceFactory.h"

#endif
