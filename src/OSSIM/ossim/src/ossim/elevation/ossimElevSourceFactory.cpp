//----------------------------------------------------------------------------
// FILE: ossimElevSourceFactory.cc
//
// Copyright (C) 2002 ImageLinks, Inc.
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Class definition for ossimElevSourceFactory.
//
// This is the base class interface for elevation source factories.  Contains
// pure virtual methods that all elevation source factories must implement.
//
//----------------------------------------------------------------------------
// $Id: ossimElevSourceFactory.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossim/elevation/ossimElevSourceFactory.h>

RTTI_DEF1(ossimElevSourceFactory, "ossimElevSourceFactory" , ossimObject)

ossimElevSourceFactory::ossimElevSourceFactory()
   : theDirectory(ossimFilename::NIL)
{
}

ossimElevSourceFactory::~ossimElevSourceFactory()
{
}

ossimFilename ossimElevSourceFactory::getDirectory() const
{
   return theDirectory;
}

void ossimElevSourceFactory::setDirectory(const ossimFilename& directory)
{
   theDirectory = directory;
}

