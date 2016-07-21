#include <oms/TileCacheSupport.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimReferenced.h>

namespace oms{

   class Entry : public ossimReferenced
   {
   public:
      Entry(ossim_int32 entryId = 0)
      :
      m_ulGpt(ossimGpt()),
      m_urGpt(ossimGpt()),
      m_lrGpt(ossimGpt()),
      m_llGpt(ossimGpt()),
      m_entryId(entryId),
      m_envelope(Envelope()),
      m_width(0),
      m_height(0),
      m_outputScalarType(OSSIM_SCALAR_UNKNOWN),
      m_numberOfResolutions(0),
      m_gsdMeters(0),
      m_gsdDegrees(0),
      m_geometry(0)
      {

      }
      virtual ~Entry(){m_geometry = 0;}


      ossimGpt      m_ulGpt;
      ossimGpt      m_urGpt;
      ossimGpt      m_lrGpt;
      ossimGpt      m_llGpt;

      ossim_int32   m_entryId;
      Envelope      m_envelope;
      ossim_uint64  m_width;
      ossim_uint64  m_height;
      ossimScalarType m_outputScalarType;
      ossim_uint32  m_numberOfResolutions;
      ossim_float64 m_gsdMeters;
      ossim_float64 m_gsdDegrees;

      std::vector<ossimIrect> m_rlevelRects;
      std::vector<ossimDpt> m_decimationFactors;
      ossimRefPtr<ossimImageGeometry> m_geometry;

   };

   class TileCacheSupportPrivateData
   {
   public:
      virtual ~TileCacheSupportPrivateData()
      {
         if(m_imageHandler.valid()) m_imageHandler->close();
      
         m_imageHandler = 0;
         m_entries.clear();
      }
      bool open(const std::string& imageFile);
      void initializeProperties();

      ossim_uint32 m_tileWidth;
      ossim_uint32 m_tileHeight;
      ossimString  m_epsgCode;

      ossimRefPtr<ossimImageHandler> m_imageHandler;
      std::vector<ossimRefPtr<Entry> > m_entries;
   };
   bool TileCacheSupportPrivateData::open(const std::string& imageFile)
   {
      if(m_imageHandler.valid()) m_imageHandler->close();
      m_imageHandler = 0;
      m_imageHandler = ossimImageHandlerRegistry::instance()->open(imageFile);

      initializeProperties();

      return m_imageHandler.valid();
   }
   void TileCacheSupportPrivateData::initializeProperties()
   {
      m_entries.clear();
      if(m_imageHandler.valid())
      {
         ossim_uint32 idx= 0;
         ossim_uint32 nEntries = m_imageHandler->getNumberOfEntries();
         for(idx = 0; idx < nEntries;++idx)
         {
            ossimDpt mpp;
            ossimRefPtr<Entry> entry = new Entry(idx);
            m_imageHandler->setCurrentEntry(idx);
            entry->m_geometry = m_imageHandler->getImageGeometry();
            if(entry->m_geometry.valid())
            {
               entry->m_outputScalarType = m_imageHandler->getOutputScalarType();
               ossimDpt metersPerDegree = ossimGpt().metersPerDegree();
               mpp = entry->m_geometry->getMetersPerPixel();

               entry->m_width = m_imageHandler->getNumberOfSamples();
               entry->m_height = m_imageHandler->getNumberOfLines();
               entry->m_numberOfResolutions = m_imageHandler->getNumberOfDecimationLevels();

               entry->m_gsdMeters = mpp.y; // just need an estimate so we can find levels
               entry->m_gsdDegrees = mpp.y*(1.0/metersPerDegree.y);
               entry->m_geometry->getCornerGpts(entry->m_ulGpt, entry->m_urGpt, 
                                                entry->m_lrGpt, entry->m_llGpt);

               double minx = ossim::min(entry->m_ulGpt.lond(), 
                                        ossim::min(entry->m_urGpt.lond(), 
                                                   ossim::min(entry->m_lrGpt.lond(), entry->m_llGpt.lond())));
               
 
               double maxx = ossim::max(entry->m_ulGpt.lond(), 
                                        ossim::max(entry->m_urGpt.lond(), 
                                                   ossim::max(entry->m_lrGpt.lond(), entry->m_llGpt.lond())));
               double miny = ossim::min(entry->m_ulGpt.latd(), 
                                        ossim::min(entry->m_urGpt.latd(), 
                                                   ossim::min(entry->m_lrGpt.latd(), entry->m_llGpt.latd())));
               
               double maxy = ossim::max(entry->m_ulGpt.latd(), 
                                        ossim::max(entry->m_urGpt.latd(), 
                                                   ossim::max(entry->m_lrGpt.latd(), entry->m_llGpt.latd())));
               
               entry->m_envelope = Envelope(minx, miny, maxx, maxy);

               entry->m_rlevelRects.resize(entry->m_numberOfResolutions);
               ossim_uint32 idx = 0;
               for(idx = 0; idx < entry->m_numberOfResolutions;++idx)
               {
                  entry->m_rlevelRects[idx] = m_imageHandler->getBoundingRect(idx);
               }
               m_imageHandler->getDecimationFactors(entry->m_decimationFactors);

               m_entries.push_back(entry);
            }
            else
            {
               entry = 0;
               m_entries.push_back(entry);
            }
         }
      }
   }


   TileCacheSupport::TileCacheSupport(unsigned int tileWidth,
                       unsigned int tileHeight,
                       const std::string& epsgCode)
   :m_privateData(new TileCacheSupportPrivateData())
   {
      m_privateData->m_tileWidth = tileWidth;
      m_privateData->m_tileHeight = tileHeight;
      m_privateData->m_epsgCode   = epsgCode;
   }
   TileCacheSupport::~TileCacheSupport()
   {
      if(m_privateData) delete m_privateData;

      m_privateData = 0;
   }
   bool TileCacheSupport::openImage(const std::string& file)
   {
      return m_privateData->open(file);
   }
   int TileCacheSupport::getNumberOfEntries()const
   {
      return m_privateData->m_entries.size();
   }
   
   long int TileCacheSupport::getWidth(int entry, int rlevel)const
   {
      long int result = 0;

      if(entry < getNumberOfEntries())
      {
         if(m_privateData->m_entries[entry].valid())
         {
            result = m_privateData->m_entries[entry]->m_rlevelRects[rlevel].width();
         }
      }

      return result;
   }

   long int TileCacheSupport::getHeight(int entry, int rlevel)const
   {
      long int result = 0;

      if(entry < getNumberOfEntries())
      {
         if(m_privateData->m_entries[entry].valid())
         {
            result = m_privateData->m_entries[entry]->m_rlevelRects[rlevel].height();
         }
      }

      return result;
   }

   Envelope TileCacheSupport::getEnvelope(int entry)const
   {
      Envelope result;

      if(entry < getNumberOfEntries())
      {
         if(m_privateData->m_entries[entry].valid())
         {
            result = m_privateData->m_entries[entry]->m_envelope;
         }
      }
      return result;
   }
   double TileCacheSupport::getDegreesPerPixel(int entry, int rlevel)const
   {
      double result = 0.0;
      if(entry < getNumberOfEntries())
      {
         if(m_privateData->m_entries[entry].valid()&&(rlevel<m_privateData->m_entries[entry]->m_numberOfResolutions))
         {
            double average = (m_privateData->m_entries[entry]->m_decimationFactors[rlevel].y+
                              m_privateData->m_entries[entry]->m_decimationFactors[rlevel].x)*0.5;
            result = m_privateData->m_entries[entry]->m_gsdDegrees/average;
         }
      }

      return result;
   }
   double TileCacheSupport::getMetersPerPixel(int entry, int rlevel)const
   {
      double result = 0.0;
      if(entry < getNumberOfEntries())
      {
         if(m_privateData->m_entries[entry].valid()&&(rlevel<m_privateData->m_entries[entry]->m_numberOfResolutions))
         {
            double average = (m_privateData->m_entries[entry]->m_decimationFactors[rlevel].y+
                              m_privateData->m_entries[entry]->m_decimationFactors[rlevel].x)*0.5;
            result = m_privateData->m_entries[entry]->m_gsdMeters/average;
         }
      }

      return result;
   }

   int TileCacheSupport::getNumberOfResolutionLevels(int entry)const
   {
      int result = 0;
      if(entry < getNumberOfEntries())
      {
         if(m_privateData->m_entries[entry].valid())
         {
            result = m_privateData->m_entries[entry]->m_numberOfResolutions;
         }
      }

      return result;
   }

   bool TileCacheSupport::findBestResolutionsGeographic(int entry, 
                                                        int* minLevel, int* maxLevel, 
                                                        const ossim_float64* resLevels, int nResLevels)const
   {
      double dpp = getDegreesPerPixel(entry);
      //double minGsd = 0.0;
      //double maxGsd = 0.0;
      *minLevel = 0;
      *maxLevel = 0;

      if(m_privateData->m_entries[entry].valid()&&nResLevels>0)
      {
         // first we will find the number of decimation for the image to be within a single tile
         //
         double highestRes = dpp;
         double lowestRes  = dpp;
         ossim_uint32 tileSize = ossim::max(m_privateData->m_tileWidth, m_privateData->m_tileHeight);
         ossim_uint32 largestSize = ossim::max(m_privateData->m_entries[entry]->m_width, 
                                               m_privateData->m_entries[entry]->m_height);
         ossim_uint32 maxDecimationLevels = 0;
         if(largestSize > tileSize)
         {
            ossim_uint32 testSize = largestSize;
            while((testSize > tileSize)&&(testSize>0))
            {
               ++maxDecimationLevels;
               testSize = testSize>>1;  
            }
         }

         // once we find the number of decimations then we will find the estimate for the
         // resolution at that decimation
         //
         lowestRes = highestRes*(1<<maxDecimationLevels);

         // now we have the full res of the image gsd and the corsest res of the image within the 
         // decimation range.
         //
         // now find the min and max levels from the passed in pyramid resolution 
         // identified by the resLevels array
         //
         ossim_int32 currentResLevel = 0;
         *maxLevel = nResLevels-1;
         *minLevel = 0;

         for(currentResLevel=0; currentResLevel < nResLevels;++currentResLevel)
         {
            if(highestRes > resLevels[currentResLevel])
            {
               *maxLevel = currentResLevel;
               if(currentResLevel > 0) (*maxLevel)--;
               break;
            }
         }
         for(currentResLevel=nResLevels-1; currentResLevel >= 0;--currentResLevel)
         {
            if(lowestRes < resLevels[currentResLevel])
            {
               *minLevel = currentResLevel;
               break;
            }
         }
       //  std::cout << "minLevel ==== " << *minLevel << "maxLevel = " << *maxLevel << std::endl;
        // std::cout << "LOWEST RES = " << lowestRes << " HIGHEST ==== " << highestRes << std::endl;
      }


      return *minLevel<=*maxLevel;
   }
   int TileCacheSupport::getOutputScalarType(int entry)const
   {
      int result = OSSIM_SCALAR_UNKNOWN;

      if(m_privateData->m_entries[entry].valid())
      {
         m_privateData->m_entries[entry]->m_outputScalarType;
      }

      return result;
   }
   int TileCacheSupport::getBitDepth(int entry)const
   {
      return ossim::getActualBitsPerPixel((ossimScalarType)getOutputScalarType(entry));
   }
}