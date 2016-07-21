#ifndef ossimWorldWindHandler_HEADER
#define ossimWorldWindHandler_HEADER
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimXmlDocument.h>
#include "ossimDiskCache.h"
#include <ossim/imaging/ossimAppFixedTileCache.h>

class ossimWorldWindHandler : public ossimImageHandler
{
public:
   ossimWorldWindHandler();
   virtual ~ossimWorldWindHandler();
   virtual bool open();
   virtual void close();
   virtual bool isOpen()const;
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel)const;
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel)const;
   virtual ossim_uint32 getImageTileWidth()const;
   virtual ossim_uint32 getImageTileHeight()const;
   virtual ossim_uint32 getNumberOfInputBands()const;
   virtual ossim_uint32 getNumberOfOutputBands()const;
   virtual ossim_uint32 getCurrentEntry()const;
   virtual ossim_uint32 getNumberOfEntries()const;
   virtual bool setCurrentEntry(ossim_uint32 entryIdx);
   virtual void getEntryList(std::vector<ossim_uint32>& entryList) const;
   virtual ossim_uint32 getNumberOfDecimationLevels()const;
   virtual ossimScalarType getOutputScalarType() const;
  
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   ossimIrect tileBounds(const ossimGpt& gpt,
                         ossim_uint32 resLevel);
   
   ossimRefPtr<ossimImageData> requestTile(ossim_uint32 level,
                                           ossim_uint32 X,
                                           ossim_uint32 Y);

   void resetMemoryCache();

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
    
protected:
   class ossimWorldWindHandlerInfo
   {
   public:
      enum AccessorType
      {
         WMS = 0,
         WW_TILE_SERVER = 1
      };
      ossimWorldWindHandlerInfo()
      {}

      ossimWorldWindHandlerInfo::AccessorType theAccessorType;
      ossimString   theUrl;
      ossim_float64 theNorth;
      ossim_float64 theSouth;
      ossim_float64 theEast;
      ossim_float64 theWest;
      ossim_uint32  theLevels;
      ossim_float64 theGsd;
      ossim_float64 theLevel0Gsd;
      ossim_uint32  theTileSize;
      ossim_uint64  theImageWidth;
      ossim_uint64  theImageHeight;
      ossimString   theDatasetName;
      ossimString   theExtension;
      ossimString   theChildLayerName;
      ossimString   theQuadTileSetName;
   };
   void initialize();
   bool populateWorldWindStructure();
   
   ossimFilename buildTileFilename(ossim_uint32 level,
                                   ossim_uint32 row,
                                   ossim_uint32 col)const;
   ossimFilename buildFullRelativeFilename(ossim_uint32 level,
                                           ossim_uint32 row,
                                           ossim_uint32 col)const;
   ossimRefPtr<ossimImageData> requestMemoryCacheTile(ossim_uint32 level,
                                                      ossim_uint32 X,
                                                      ossim_uint32 Y);
   ossimRefPtr<ossimImageData> requestDiskCacheTile(ossim_uint32 level,
                                                    ossim_uint32 X,
                                                    ossim_uint32 Y);
   ossimRefPtr<ossimImageData> convertToProperResult(ossimRefPtr<ossimImageData> data);
   
   ossimRefPtr<ossimXmlDocument> theXmlDocument;

   std::vector<ossimWorldWindHandlerInfo> theLayers;
   ossim_uint32 theCurrentIdx;

   ossimRefPtr<ossimDiskCache> theDiskCache;
   ossimRefPtr<ossimImageData> theLocalTile;
   ossimRefPtr<ossimImageData> theResultTile;
/*    std::vector<ossim_uint8>    theJpegDecodeBuffer; */
   std::vector<ossimAppFixedTileCache::ossimAppFixedCacheId> theRLevelCacheId;

   
TYPE_DATA
};

#endif
