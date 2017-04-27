//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: Uses the LibRaw library for reading raw sensor images
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimLibRawReader_HEADER
#define ossimLibRawReader_HEADER

#include <png.h>
#include <vector>

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimAppFixedTileCache.h>
#include <libraw/libraw.h>

class ossimImageData;

class ossimLibRawReader : public ossimImageHandler
{
public:


   /** default constructor */
   ossimLibRawReader();

   /** virtual destructor */
   virtual ~ossimLibRawReader();

   /** @return "png" */
   virtual ossimString getShortName() const;

   /** @return "ossim png" */
   virtual ossimString getLongName()  const;

   /** @return "ossimLibRawReader" */
   virtual ossimString getClassName()    const;

   /**
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   
   bool getTile(ossimImageData* result,
                ossim_uint32 resLevel);
    /**
     *  Returns the number of bands in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfInputBands() const;

   /**
    * Returns the number of bands in a tile returned from this TileSource.
    * Note: we are supporting sources that can have multiple data objects.
    * If you want to know the scalar type of an object you can pass in the
    */
   virtual ossim_uint32 getNumberOfOutputBands()const;

   /**
     *  Returns the number of lines in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;

   /**
    *  Returns the number of samples in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

   /**
    * Returns the zero based image rectangle for the reduced resolution data
    * set (rrds) passed in.  Note that rrds 0 is the highest resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 reduced_res_level = 0) const;

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   bool isOpen()const;

   virtual double getMaxPixelValue(ossim_uint32 band = 0)const;

   /** Close method. */
   virtual void close();

protected:
   
   /**
    *  @brief open method.
    *  @return true on success, false on error.
    */
   virtual bool open();

   virtual bool cacheImage()const;
   /**
    */ 
   void allocate();
   void destroy();

   mutable ossimRefPtr<ossimImageData>  theTile;
   mutable ossimRefPtr<ossimImageData> theMemoryTile;
   mutable LibRaw *theRawProcessor;
   mutable OpenThreads::Mutex theMutex;
   
TYPE_DATA
};

#endif

