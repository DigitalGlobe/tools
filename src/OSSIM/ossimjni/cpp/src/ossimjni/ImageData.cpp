//-----------------------------------------------------------------------------
// File:  ImageData.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper for ossimImageData.
//
//-----------------------------------------------------------------------------
// $Id$

#include <ossimjni/ImageData.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageData.h>

ossimjni::ImageData::ImageData() :
   m_imageData( 0 )
{
}

ossimjni::ImageData::ImageData(  const ImageData& obj ) :
   m_imageData( 0 )
{
   if ( obj.getConstImageData() )
   {
      m_imageData = new ossimImageData( *(obj.getConstImageData()) );
   }
}

const ossimjni::ImageData& ossimjni::ImageData::operator=( const ossimjni::ImageData& rhs )
{
   if ( this != &rhs )
   {
      if (m_imageData)
      {
         delete m_imageData;
         m_imageData = 0;
      }
      if ( rhs.getConstImageData() )
      {
         m_imageData = new ossimImageData( *(rhs.getConstImageData()) );
      }
   }
   return *this;
}

ossimjni::ImageData::~ImageData()
{
   if (m_imageData)
   {
      delete m_imageData;
      m_imageData = 0;
   }
}

bool ossimjni::ImageData::valid() const
{
   return m_imageData ? true : false;
}

const ossimImageData* ossimjni::ImageData::getConstImageData() const
{
   return m_imageData;
}

ossimImageData* ossimjni::ImageData::getImageData()
{
   return m_imageData;
}

void ossimjni::ImageData::setImageData( ossimImageData* imageData )
{
   if ( m_imageData )
   {
      delete m_imageData;
   }
   m_imageData = imageData;
}

bool ossimjni::ImageData::write(const std::string& file) const
{
   bool result = false;
   if ( m_imageData )
   {
      ossimFilename f = file;
      result = m_imageData->write( f );
   }
   return result;
}
