//----------------------------------------------------------------------------
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterFactoryBase.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>

RTTI_DEF1(ossimImageMetaDataWriterFactoryBase,
          "ossimImageMetaDataWriterFactoryBase",
          ossimObjectFactory);

ossimImageMetaDataWriterFactoryBase::ossimImageMetaDataWriterFactoryBase()
{
}

ossimImageMetaDataWriterFactoryBase::ossimImageMetaDataWriterFactoryBase(
   const ossimImageMetaDataWriterFactoryBase&)
{
}

const ossimImageMetaDataWriterFactoryBase&
ossimImageMetaDataWriterFactoryBase::operator=(
   const ossimImageMetaDataWriterFactoryBase&)
{
   return *this;
}

