//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:   Simple container for output format type with
// mime type, name, and description.
//
//----------------------------------------------------------------------------
// $Id: ImageOutputFormat.cpp 11646 2007-08-23 21:04:16Z dburken $

#include <oms/ImageOutputFormat.h>


oms::ImageOutputFormat::ImageOutputFormat()
   : theMimeType(),
     theName(),
     theDescription()
{
}

oms::ImageOutputFormat::~ImageOutputFormat()
{
}

std::string oms::ImageOutputFormat::mimeType() const
{
   return theMimeType;
}

std::string oms::ImageOutputFormat::name() const
{
   return theName;
}

std::string oms::ImageOutputFormat::description() const
{
   return theDescription;
}

void oms::ImageOutputFormat::setMimeType(const std::string& type)
{
  theMimeType  = type; }

void oms::ImageOutputFormat::setName(const std::string& name)
{
   theName = name; }

void oms::ImageOutputFormat::setDescription(const std::string& description)
{
   theDescription = description;
}
