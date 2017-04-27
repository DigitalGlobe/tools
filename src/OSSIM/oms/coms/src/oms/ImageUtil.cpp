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
// $Id: ImageUtil.cpp 21257 2012-07-08 16:13:18Z dburken $

#include <oms/ImageUtil.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/util/ossimImageUtil.h>

oms::ImageUtil::ImageUtil() :
   m_imageUtil(new ossimImageUtil)
{
}
      
oms::ImageUtil::~ImageUtil()
{
   if (m_imageUtil)
   {
      delete m_imageUtil;
      m_imageUtil = 0;
   }
}

bool oms::ImageUtil::initialize(int argc, char* argv[])
{
   ossimArgumentParser ap(&argc, argv);
   return m_imageUtil->initialize(ap);
}

void oms::ImageUtil::execute()
{
   m_imageUtil->execute();
}

void oms::ImageUtil::processFile(const std::string& file)
{
   m_imageUtil->processFile( ossimFilename(file) );
}

void oms::ImageUtil::setCreateOverviewsFlag( bool flag )
{
   m_imageUtil->setCreateOverviewsFlag( flag );
}

void oms::ImageUtil::setRebuildOverviewsFlag( bool flag )
{
   m_imageUtil->setRebuildOverviewsFlag( flag );
}

void oms::ImageUtil::setRebuildHistogramFlag( bool flag )
{
   m_imageUtil->setRebuildHistogramFlag( flag );
}

void oms::ImageUtil::setOverviewType( const std::string& type )
{
   m_imageUtil->setOverviewType( type );
}

void oms::ImageUtil::setOverviewStopDimension( ossim_uint32 dimension )
{
   m_imageUtil->setOverviewStopDimension( dimension );
}

void oms::ImageUtil::setOverviewStopDimension( const std::string& dimension )
{
   m_imageUtil->setOverviewStopDimension( dimension );
}

void oms::ImageUtil::setTileSize( ossim_uint32 tileSize )
{
   m_imageUtil->setTileSize ( tileSize );
}

void oms::ImageUtil::setCreateHistogramFlag( bool flag )
{
   m_imageUtil->setCreateHistogramFlag( flag );
}

void oms::ImageUtil::setCreateHistogramFastFlag( bool flag )
{
   m_imageUtil->setCreateHistogramFastFlag( flag );
}

void oms::ImageUtil::setCreateHistogramR0Flag( bool flag )
{
   m_imageUtil->setCreateHistogramR0Flag( flag );
}

void oms::ImageUtil::setScanForMinMax( bool flag )
{
   m_imageUtil->setScanForMinMax( flag );
}

void oms::ImageUtil::setScanForMinMaxNull( bool flag )
{
   m_imageUtil->setScanForMinMaxNull( flag );
}

void oms::ImageUtil::setCompressionQuality( const std::string& quality )
{
   m_imageUtil->setCompressionQuality( quality );
}
      
void oms::ImageUtil::setCompressionType( const std::string& type )
{
   m_imageUtil->setCompressionType( type );
}

void oms::ImageUtil::setCopyAllFlag( bool flag )
{
   m_imageUtil->setCopyAllFlag( flag );
}

void oms::ImageUtil::setOutputDirectory( const std::string& directory )
{
   m_imageUtil->setOutputDirectory( directory );
}

void oms::ImageUtil::setNumberOfThreads( ossim_uint32 threads )
{
   m_imageUtil->setNumberOfThreads( threads );
}
