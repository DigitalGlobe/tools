//----------------------------------------------------------------------------
// File:  GpkgWriter.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for GpkgWriter.
//
//----------------------------------------------------------------------------
// $Id$

#include <oms/GpkgWriter.h>
#include <oms/ImageData.h>
#include <oms/Keywordlist.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimGpkgWriterInterface.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>

static ossimTrace traceDebug("oms::GpkWriter:debug");

oms::GpkgWriter::GpkgWriter()
   : m_writer(0)
{
   ossimImageFileWriter* writer = ossimImageWriterFactoryRegistry::instance()->
      createWriter( ossimString("ossimGpkgWriter") );

   if ( writer )
   {
      m_writer = dynamic_cast<ossimGpkgWriterInterface*>( writer );

      if ( !m_writer )
      {
         delete writer;
         writer = 0;
      }
   }
}

oms::GpkgWriter::~GpkgWriter()
{
   if ( m_writer )
   {
      //---
      // Cast back for deletion. Deleting the ossimGpkgWriterInterface* was
      // causing compiler issues as it has no virtual destructor.
      //---
      ossimImageFileWriter* writer = dynamic_cast<ossimImageFileWriter*>( m_writer );
      if ( writer )
      {
         delete writer;
      }
      m_writer = 0;
   }
}

bool oms::GpkgWriter::getErrorStatus() const
{
   return (m_writer ? true : false);
}

bool oms::GpkgWriter::openFile( const Keywordlist& options )
{
   bool status = false;
   if ( m_writer )
   {
      try
      {
         status = m_writer->openFile( *(options.getKeywordlist()) );
      }
      catch ( const ossimException& e )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "GpkgWriter::openFile caught exception: " << e.what() << std::endl;
         }
      }
      catch ( ... )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "GpkgWriter::openFile caught unknown exception!" << std::endl;
         }
      }
   }
   return status;
   
}// End: oms::GpkgWriter::openFile( ... )

bool  oms::GpkgWriter::openFile( const std::map<std::string, std::string>& options)
{
   bool result = false;
   try
   {
      Keywordlist kwl;
      kwl.getKeywordlist()->getMap() = options;
      result = this->openFile( kwl );
   }
   catch ( const ossimException& e )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception: " << e.what() << std::endl;
      }
      result = false;
   }

   return result;
}

ossim_int32 oms::GpkgWriter::beginTileProcessing()
{
   ossim_int32 status = -1;
   if ( m_writer )
   {
      status = m_writer->beginTileProcessing();
   }
   return status;
}

bool oms::GpkgWriter::writeTile( ImageData* tile,
                                 ossim_int32 zoomLevel,
                                 ossim_int64 row,
                                 ossim_int64 col )
{
   bool status = false;

   if ( m_writer && tile )
   {
      ossimRefPtr<ossimImageData> id = (ossimImageData*)tile->getOssimImageData();
      if ( id.valid() )
      {
         try
         {
            status = m_writer->writeTile( id, zoomLevel, row, col );
         }
         catch ( const ossimException& e )
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "GpkgWriter::writeTile caught exception: " << e.what() << std::endl;
            }
         }
         catch ( ... )
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "GpkgWriter::writeTile caught unknown exception!" << std::endl;
            } 
         }
      }
   }

   return status;
   
} // End: oms::GpkgWriter::writeTile(...)

bool oms::GpkgWriter::writeCodecTile( ossim_int8* codecTile,
                                      ossim_int32 codecTileSize,
                                      ossim_int32 zoomLevel,
                                      ossim_int64 row,
                                      ossim_int64 col)
{
   bool status = false;
   if ( codecTile&&(codecTileSize>0))
   {
      try
      {
         status = m_writer->writeCodecTile( reinterpret_cast<ossim_uint8*>(codecTile), codecTileSize, zoomLevel, row, col );
      }
      catch ( const ossimException& e )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "GpkgWriter::writeTile caught exception: " << e.what() << std::endl;
         }
      }
      catch ( ... )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "GpkgWriter::writeTile caught unknown exception!" << std::endl;
         } 
      }
   }

   return status;
}

void oms::GpkgWriter::finalizeTileProcessing()
{
   if ( m_writer )
   {
      m_writer->finalizeTileProcessing();
   }
}
