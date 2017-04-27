#ifndef ossimNuiHandler_HEADER
#define ossimNuiHandler_HEADER
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/imaging/ossimAppFixedTileCache.h>
#include <nuiSDK.h>

// ossimNuiData is a normal struct holding all the information
// about an open NUI file but benefits from C++ ctors to initialize
// and the dtor to assert if not cleaned up.

struct ossimNuiData 
{
   ossimNuiData();
   ~ossimNuiData();
   
   NUIF             *nuif;
   NUIF_GEO         *geo;
   NUI_UINT32        tile_width;		//tile width
   NUI_UINT32        tile_height;		//tile height
   NUI_UINT32        tile_size;			//tile size in bytes
   NUI_UINT32        image_width;		//image width of RRD 0
   NUI_UINT32        image_height;		//image height of RRD 0
   NUI_DOUBLE        x_pixel_size;		//size of pixels - X
   NUI_DOUBLE        y_pixel_size;		//size of pixels - Y
   NUI_UINT32        channels;			//# channels
   NUI_UINT32		 bytes_per_channel;	//# bytes per channel
   NUIF_BITS_PER_CHANNEL  bits_per_channel;	//bits per channel
   NUIF_DATA_TYPE    type;			    //data type
   NUIF_DATA_LAYOUT  layout;			//data layout
   NUIF_CODEC		 codec;			    //compression codec
   NUI_DOUBLE		 compression_ratio; //compression ratio
   NUI_UINT32		 total_levels;		//#levels (RRDs)
   NUI_DOUBLE		 top_left_x;        //bounding box TL-x
   NUI_DOUBLE	     top_left_y;        //bounding box TL-y
   NUI_DOUBLE		 bot_right_x;       //bounding box BR-x
   NUI_DOUBLE        bot_right_y;	    //bounding box BR-y
   ossimUnitType     coord_unit;
   ossimString       coord_unit_string;
   ossimString       wkt_string;
   NUI_UCHAR*         tile_buffer;
   ossimIrect         tile_buffer_rect;
   std::vector<double> channelMins;
   std::vector<double> channelMaxs;

   /* COMPRESSION PARAMETERS */
   NUI_J2K2_DECODE *decode;
   NUI_UINT32 codestream_size;
   NUI_UCHAR  *codestream_buffer;
};
     

class ossimNuiHandler : public ossimImageHandler
{
public:
   ossimNuiHandler();
   virtual ~ossimNuiHandler();
   virtual bool open();
   virtual void close();
   virtual bool isOpen()const;
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel)const;
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel)const;
   virtual ossim_uint32 getImageTileWidth()const;
   virtual ossim_uint32 getImageTileHeight()const;
   virtual ossim_uint32 getNumberOfInputBands()const;
   virtual ossim_uint32 getNumberOfOutputBands()const;
   
   virtual ossim_uint32 getNumberOfDecimationLevels()const;
   virtual ossimScalarType getOutputScalarType() const;
 
   virtual ossimRefPtr<ossimImageGeometry> getInternalImageGeometry() const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   ossimIrect tileBounds(const ossimGpt& gpt,
                         ossim_uint32 resLevel);

   virtual void initialize();
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
    
protected:
   void allocateTile();
   void clearCache();
   ossimRefPtr<ossimImageData> theTile;

   std::vector<ossimAppFixedTileCache::ossimAppFixedCacheId> theTileCache;
/*    std::vector<ossimRefPtr<ossimFixedTileCache> > theTileCache; */
   ossimRefPtr<ossimImageData> theNuiTileBuffer;
   ossimNuiData                theNui;
   
   
TYPE_DATA
};

#endif
