#include <oms/Mosaic.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimCommon.h>

namespace oms
{
   class MosaicImageChain : public ossimReferenced
   {
   public:
      MosaicImageChain(){}
      ossimRefPtr<ossimImageHandler>      theHandler;
      ossimRefPtr<ossimBandSelector>      theBandSelector;
      ossimRefPtr<ossimHistogramRemapper> theHistogramRemapper;
      ossimRefPtr<ossimImageRenderer>     theReprojection;
      
   protected:
      virtual ~MosaicImageChain()
      {
         if(theReprojection.valid())
         {
            theReprojection->disconnect();
            theReprojection = 0;
         }
         if(theHistogramRemapper.valid())
         {
            theHistogramRemapper->disconnect();
            theHistogramRemapper = 0;
         }
         if(theBandSelector.valid())
         {
            theBandSelector->disconnect();
            theBandSelector = 0;
         }
         if(theHandler.valid())
         {
            theHandler->disconnect();
            theHandler = 0;
         }
      }

   };
   class MosaicPrivateData
      {
      public:
         MosaicPrivateData()
         :theMosaic(new ossimImageMosaic)
         {
         }
         virtual ~MosaicPrivateData()
         {
            theMosaic->disconnect();
            theMosaic = 0;
            clear();
         }
         void clear()
         {
            if(theMosaic.valid())
            {
               theMosaic->disconnect();
               theMosaic = 0;
            }
            ossim_uint32 idx = 0;
            for(idx = 0; idx < theImageChainList.size();++idx)
            {
               theImageChainList[idx] = 0;
            }
            theImageChainList.clear();
            theIndexMapping.clear();
         }
         ossimRefPtr<ossimImageGeometry> theViewGeometry;
         ossimRefPtr<ossimImageMosaic> theMosaic;
         std::vector<ossimRefPtr<MosaicImageChain> > theImageChainList;
         std::map<std::string, ossimRefPtr<MosaicImageChain> > theIndexMapping;
      };
   
   oms::Mosaic::Mosaic()
   :thePrivateData(new MosaicPrivateData())
   {
      
   }
   oms::Mosaic::~Mosaic()
   {
      if(thePrivateData)
      {
         delete thePrivateData;
         thePrivateData = 0;
      }
   }
   
   std::string oms::Mosaic::addInputFile(const std::string& filename,
                                       const std::string& entryId)
   {
      std::string id = "";
      ossimRefPtr<MosaicImageChain> chain = new MosaicImageChain();
      chain->theHandler = ossimImageHandlerRegistry::instance()->open(ossimFilename(filename));
      if(chain->theHandler.valid())
      {
         if(!entryId.empty())
         {
            chain->theHandler->setCurrentEntry(ossimString(entryId).toUInt32());
         }
         thePrivateData->theImageChainList.push_back(chain.get());
         id = filename+":"+entryId;
         thePrivateData->theIndexMapping.insert(std::make_pair(id, chain));
      }
      return id;
   }
   void oms::Mosaic::writeImageToFile(const std::string& filename,
                                    const std::string& imageType)
   {
      ossimFilename file(filename);
      ossimString type;
      
      if(type.empty())
      {
         type = "image/"+file.ext();
      }
      ossimRefPtr<ossimImageFileWriter> writer = ossimImageWriterFactoryRegistry::instance()->createWriter(type);
      if(writer.valid())
      {
         writer->connectMyInputTo(0, thePrivateData->theMosaic.get());
         writer->setOutputName(filename);
         writer->setWriteExternalGeometryFlag(false);
         writer->setWriteOverviewFlag(false);
         writer->setWriteHistogramFlag(false);
         writer->execute();
         writer->disconnect();
         writer = 0;
      }
   }

   void oms::Mosaic::setupOrthoThumbnail(const std::string& filename,
                                       const std::string& entryId,
                                       const std::string& srsCode,
                                       ossim_uint32 resolution,
                                       const std::string& histogramAutoStretchModeType)
   {
      thePrivateData->clear();
      std::string id = addInputFile(filename, entryId);
      if(id.empty())
      {
         return;
      }
      if(srsCode.empty())
      {
         ossimRefPtr<ossimImageGeometry> geom = thePrivateData->theImageChainList[0]->theHandler->getImageGeometry();
         if(geom.valid())
         {
            if(!dynamic_cast<ossimMapProjection*>(geom->getProjection()))
            {
               ossimIrect rect = thePrivateData->theImageChainList[0]->theHandler->getBoundingRect();
               if(!rect.hasNans())
               {
                  ossimGpt midPoint;
                  geom->localToWorld(rect.midPoint(), midPoint);
                  ossimMapProjection* mapProj = new ossimEquDistCylProjection();
                  mapProj->setMetersPerPixel(geom->getMetersPerPixel());
                  mapProj->setOrigin(midPoint);
                  thePrivateData->theViewGeometry = new ossimImageGeometry(0, mapProj);                  
               }
            }
            else
            {
               thePrivateData->theViewGeometry = (ossimImageGeometry*)geom->dup();
            }
         }
      }
      else
      {
         ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(srsCode);
         thePrivateData->theViewGeometry = new ossimImageGeometry(0, proj.get());
      }
      if(!thePrivateData->theViewGeometry.valid())
      {
         return;
      }
      thePrivateData->theImageChainList[0]->theBandSelector = new ossimBandSelector;
      thePrivateData->theImageChainList[0]->theBandSelector->connectMyInputTo(0, thePrivateData->theImageChainList[0]->theHandler.get());
      
      thePrivateData->theImageChainList[0]->theHistogramRemapper = new ossimHistogramRemapper;
      thePrivateData->theImageChainList[0]->theHistogramRemapper->connectMyInputTo(0, thePrivateData->theImageChainList[0]->theBandSelector.get());
      
      if(!histogramAutoStretchModeType.empty())
      {
         if(thePrivateData->theImageChainList[0]->theHistogramRemapper->openHistogram(thePrivateData->theImageChainList[0]->theHandler->createDefaultHistogramFilename()))
         {
            thePrivateData->theImageChainList[0]->theHistogramRemapper->setStretchModeAsString(histogramAutoStretchModeType);
         }
      }
      thePrivateData->theImageChainList[0]->theReprojection = new ossimImageRenderer;
      thePrivateData->theImageChainList[0]->theReprojection->setView(thePrivateData->theViewGeometry.get());
      thePrivateData->theImageChainList[0]->theReprojection->connectMyInputTo(0, thePrivateData->theImageChainList[0]->theHistogramRemapper.get());
      
      ossimIrect bounds = thePrivateData->theImageChainList[0]->theReprojection->getBoundingRect();
      
      ossim_uint32 maxRes = ossim::max(bounds.width(), bounds.height());
      if(maxRes > resolution)
      {
         double scale = 0.0;
         
         if(resolution > 1)
         {
            scale = (double)maxRes/(double)(resolution-1);
         }
         else 
         {
            scale = (double)maxRes/(double)(resolution);
         }
         ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(thePrivateData->theViewGeometry->getProjection());
         if(mapProj)
         {
            mapProj->applyScale(ossimDpt(scale,scale), true);
            thePrivateData->theImageChainList[0]->theReprojection->setView(thePrivateData->theViewGeometry.get());
         }
         else
         {
            return;
         }
      }
      thePrivateData->theMosaic->connectMyInputTo(thePrivateData->theImageChainList[0]->theReprojection.get());
      bounds = thePrivateData->theMosaic->getBoundingRect();
   }
}
