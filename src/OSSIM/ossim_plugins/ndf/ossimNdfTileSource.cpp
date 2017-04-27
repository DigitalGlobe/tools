//***
// Written by: Kenneth Melero (kmelero@sanz.com)
//
// $Id$
// ***

#include <ossimNdfHeader.h>
#include <ossimNdfTileSource.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

RTTI_DEF1_INST(ossimNdfTileSource,
               "ossimNdfTileSource",
               ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimNdfTileSource:debug");

ossimNdfTileSource::ossimNdfTileSource()
   :
   ossimGeneralRasterTileSource(),
   theHeaderFile()
{ }

ossimNdfTileSource::ossimNdfTileSource(const ossimKeywordlist& kwl,
                                       const char* prefix)
   :
   ossimGeneralRasterTileSource(),
   theHeaderFile()
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

ossimNdfTileSource::~ossimNdfTileSource()
{ }

bool ossimNdfTileSource::open()
{
   // Ensure header file exists
   if(!theHeaderFile.exists())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ERROR: Missing Header File ("
            << theHeaderFile << ")" << std::endl;
      }
      return false;
   }

   try
   {
      // Validate Header to ensure we support this data type
      ossimNdfHeader lnh(theHeaderFile);
      if(lnh.getErrorStatus())
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return false;
      }

      // Use General Raster classes to build NLAPS imagery
      ossimGeneralRasterInfo generalRasterInfo;
      if(lnh.getNumOfBands() == 1)
      {
         generalRasterInfo = ossimGeneralRasterInfo(lnh.getImageFileList(),
                                                    OSSIM_UINT8,
                                                    OSSIM_BSQ,
                                                    lnh.getNumOfBands(),
                                                    lnh.getLines(),
                                                    lnh.getSamples(),
                                                    0,
                                                    ossimGeneralRasterInfo::NONE,
                                                    0);
      }
      else
      {
         generalRasterInfo = ossimGeneralRasterInfo(lnh.getImageFileList(),
                                                    OSSIM_UINT8,
                                                    OSSIM_BSQ_MULTI_FILE,
                                                    lnh.getNumOfBands(),
                                                    lnh.getLines(),
                                                    lnh.getSamples(),
                                                    0,
                                                    ossimGeneralRasterInfo::NONE,
                                                    0);
      }
      
      theMetaData.clear();
      theMetaData.setScalarType(OSSIM_UINT8);
      theMetaData.setNumberOfBands(lnh.getNumOfBands());
      m_rasterInfo = generalRasterInfo;
      
      if(initializeHandler())
      {
         completeOpen();
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return false;
      }  
   }
   catch (const ossimException& e)
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimNdfTileSource::open caught exception:\n"
            << e.what() << std::endl;
      }
      return false;
   }
   
   return true;
}
  
ossimRefPtr<ossimImageGeometry> ossimNdfTileSource::getImageGeometry()
{
   if ( !theGeometry )
   {
      // Check for external geom:
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         theGeometry = new ossimImageGeometry();
         
         // Validate Header to ensure we support this data type
         ossimNdfHeader lnh(theHeaderFile);
         if(lnh.getErrorStatus() == ossimErrorCodes::OSSIM_OK)
         {
            // Get the projection info.
            ossimKeywordlist kwl;
            kwl.add(lnh.getGeom());
            
            // Create the projection.
            ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->
               createProjection(kwl);
            
            if ( proj.valid() )
            {
               // Assign projection to our ossimImageGeometry object.
               theGeometry->setProjection( proj.get() );
            }
         }
         
         // At this point it is assured theGeometry is set.

         //---
         // WARNING:
         // Must create/set theGeometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---         
         
         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }
      
      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }
   
   return theGeometry;
}

bool ossimNdfTileSource::saveState(ossimKeywordlist& kwl,
                                   const char* prefix)const
{
   kwl.add(prefix, "header_filename", theHeaderFile.c_str(), true);

   return ossimGeneralRasterTileSource::saveState(kwl, prefix);
}

bool ossimNdfTileSource::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   const char* lookup = kwl.find(prefix, "header_filename");
   if (lookup)
   {
      theHeaderFile = lookup;
      return ossimGeneralRasterTileSource::loadState(kwl, prefix);
   }

   theErrorStatus = ossimErrorCodes::OSSIM_ERROR;

   return false;

#if 0
   if(lookup)
   {
      // Validate Header to ensure we support this data type
      ossimNdfHeader lnh(lookup);
      if(lnh.getErrorStatus())
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return false;
      }
      
      // Use General Raster classes to build NLAPS imagery
      ossimGeneralRasterInfo generalRasterInfo;
      if(lnh.getNumOfBands() == 1)
      {
         generalRasterInfo = ossimGeneralRasterInfo(lnh.getImageFileList(),
                                                    OSSIM_UINT8,
                                                    OSSIM_BSQ,
                                                    lnh.getNumOfBands(),
                                                    lnh.getLines(),
                                                    lnh.getSamples(),
                                                    0,
                                                    ossimGeneralRasterInfo::NONE,
                                                    0);
      }
      else
      {
         generalRasterInfo = ossimGeneralRasterInfo(lnh.getImageFileList(),
                                                    OSSIM_UINT8,
                                                    OSSIM_BSQ_MULTI_FILE,
                                                    lnh.getNumOfBands(),
                                                    lnh.getLines(),
                                                    lnh.getSamples(),
                                                    0,
                                                    ossimGeneralRasterInfo::NONE,
                                                    0);
      }
      
      return open(generalRasterInfo);   
   }
   else
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return false;
   }
#endif
}

bool ossimNdfTileSource::open(const ossimGeneralRasterInfo& info)
{
   return ossimGeneralRasterTileSource::open(info);
}

ossimString ossimNdfTileSource::getShortName() const
{
   return ossimString("ossim_ndf");
}

ossimString ossimNdfTileSource::getLongName() const
{
   return ossimString("NLAPS Data Format Reader");
}

ossimString  ossimNdfTileSource::className() const
{
   return ossimString("ossimNdfTileSource");
}

double ossimNdfTileSource::getNullPixelValue(ossim_uint32) const
{
   return 0.0;
}

double ossimNdfTileSource::getMinPixelValue(ossim_uint32) const
{
   return 1.0;
}

double ossimNdfTileSource::getMaxPixelValue(ossim_uint32) const
{
   return 255.0;
}

ossimScalarType ossimNdfTileSource::getOutputScalarType() const
{
   return OSSIM_UINT8;
}

void ossimNdfTileSource::setFilename(const ossimFilename& filename)
{
   theHeaderFile = filename;
   
   //---
   // Because all raster band files for multispectral, thermal and pan have
   // the same filename base we must make a unique name for file like the
   // overview, histogram, omd and such.  So is header file is
   // "LE7116034000502150.H1" this will return
   // "LE7116034000502150_H1" so that things like the overviews will come
   // out LE7116034000502150_H1.ovr.
   //---
   theImageFile =
      theHeaderFile.substitute(ossimString("."), ossimString("_"), false);
}
