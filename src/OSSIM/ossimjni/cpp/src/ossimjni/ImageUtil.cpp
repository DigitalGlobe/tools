//-----------------------------------------------------------------------------
// File:  ImageUtil.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for ossimImageUtil with swig readable
// interfaces.
//
//-----------------------------------------------------------------------------
// $Id: ImageUtil.cpp 21258 2012-07-08 16:15:09Z dburken $

#include <ossimjni/ImageUtil.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/util/ossimImageUtil.h>

ossimjni::ImageUtil::ImageUtil() :
   m_imageUtil(new ossimImageUtil)
{
}
      
ossimjni::ImageUtil::~ImageUtil()
{
   if (m_imageUtil)
   {
      delete m_imageUtil;
      m_imageUtil = 0;
   }
}

bool ossimjni::ImageUtil::initialize(int argc, char* argv[])
{
   ossimArgumentParser ap(&argc, argv);
   return m_imageUtil->initialize(ap);
}

void ossimjni::ImageUtil::execute()
{
   m_imageUtil->execute();
}

void ossimjni::ImageUtil::processFile(const std::string& file)
{
   m_imageUtil->processFile( ossimFilename(file) );
}

void ossimjni::ImageUtil::setCreateOverviewsFlag( bool flag )
{
   m_imageUtil->setCreateOverviewsFlag( flag );
}

void ossimjni::ImageUtil::setRebuildOverviewsFlag( bool flag )
{
   m_imageUtil->setRebuildOverviewsFlag( flag );
}

void ossimjni::ImageUtil::setRebuildHistogramFlag( bool flag )
{
   m_imageUtil->setRebuildHistogramFlag( flag );
}

void ossimjni::ImageUtil::setOverviewType( const std::string& type )
{
   m_imageUtil->setOverviewType( type );
}

void ossimjni::ImageUtil::setOverviewStopDimension( ossimjni_uint32 dimension )
{
   m_imageUtil->setOverviewStopDimension( dimension );
}

void ossimjni::ImageUtil::setOverviewStopDimension( const std::string& dimension )
{
   m_imageUtil->setOverviewStopDimension( dimension );
}

void ossimjni::ImageUtil::setTileSize( ossimjni_uint32 tileSize )
{
   m_imageUtil->setTileSize ( tileSize );
}

void ossimjni::ImageUtil::setCreateHistogramFlag( bool flag )
{
   m_imageUtil->setCreateHistogramFlag( flag );
}

void ossimjni::ImageUtil::setCreateHistogramFastFlag( bool flag )
{
   m_imageUtil->setCreateHistogramFastFlag( flag );
}

void ossimjni::ImageUtil::setCreateHistogramR0Flag( bool flag )
{
   m_imageUtil->setCreateHistogramR0Flag( flag );
}

void ossimjni::ImageUtil::setScanForMinMax( bool flag )
{
   m_imageUtil->setScanForMinMax( flag );
}

void ossimjni::ImageUtil::setScanForMinMaxNull( bool flag )
{
   m_imageUtil->setScanForMinMaxNull( flag );
}

void ossimjni::ImageUtil::setCompressionQuality( const std::string& quality )
{
   m_imageUtil->setCompressionQuality( quality );
}
      
void ossimjni::ImageUtil::setCompressionType( const std::string& type )
{
   m_imageUtil->setCompressionType( type );
}

void ossimjni::ImageUtil::setCopyAllFlag( bool flag )
{
   m_imageUtil->setCopyAllFlag( flag );
}

void ossimjni::ImageUtil::setOutputDirectory( const std::string& directory )
{
   m_imageUtil->setOutputDirectory( directory );
}

void ossimjni::ImageUtil::setNumberOfThreads( ossimjni_uint32 threads )
{
   m_imageUtil->setNumberOfThreads( threads );
}
