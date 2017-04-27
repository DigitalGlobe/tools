//*******************************************************************
// Copyright (C) 2008 Garrett Potts.
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for the class "ossimNitroHandler for NITF handling".
//
//*******************************************************************
//  $Id$

#ifndef ossimNitroHandler_HEADER
#define ossimNitroHandler_HEADER

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimIrect.h>

#include <import/nitf.h>
#include <import/cgm.h>

class ossimImageData;

class ossimNitroHandler : public ossimImageHandler
{
public:
   ossimNitroHandler();
   virtual ~ossimNitroHandler();


   virtual void close();

   /**
    *  @return Returns true on success, false on error.
    *
    *  @note This method relies on the data member ossimImageData::theImageFile
    *  being set.  Callers should do a "setFilename" prior to calling this
    *  method or use the ossimImageHandler::open that takes a file name and an
    *  entry index.
    */   
   virtual bool open();

   /**
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                   ossim_uint32 resLevel=0);

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
   virtual ossim_uint32 getNumberOfOutputBands() const;
  
   /**
    *  Returns the number of bands in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;

   /**
    *  Returns the number of bands available from an image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

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
   
   
   virtual ossimIrect getImageRectangle(ossim_uint32 resLevel = 0) const;
   
   /**
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *  Returns true if geometry info is present, false if not.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
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

   /**
    * @return The current entry number.
    */
   virtual ossim_uint32 getCurrentEntry()const;
   
   /**
    * @param entryIdx Entry number to select.  This will set the data member
    * "theEntryNumberToRender", then call open().
    */
   virtual bool setCurrentEntry(ossim_uint32 entryIdx);

   /**
    * @param entryList This is the list to initialize with entry indexes.
    */
   virtual void getEntryList(std::vector<ossim_uint32>& entryList)const;

   /**
    * @param entryStringList List to initialize with strings associated with
    * entries.
    */
   virtual void getEntryNames(
      std::vector<ossimString>& entryStringList) const;
   
   virtual bool isOpen()const;
   
   virtual ossim_uint32 getNumberOfDecimationLevels()const;
   
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
private:
   void initializeScalarType();
   void initializeOutputTile();
   bool setCompressionFlags();
   void populateLut();
   void lutUncompress(ossimRefPtr<ossimImageData> destination, nitf_Uint8* source);
   void addElement(cgm_Element* element);
   void loadAnnotations();
   void addAnnotationBounds(const ossimDrect& rect);
   void drawAnnotations(ossimRefPtr<ossimImageData>, ossim_uint32 resLevel = 0);
   std::vector<ossimRefPtr<ossimAnnotationObject> >* getAnnotationListForRlevel(ossim_uint32 rlevel);
   
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageData> theUncompressedClipTile;
  /*  This is the reader  */
   nitf_Reader *theReader;
   /*  This is the record of the file we are reading  */
   nitf_Record *theRecord;
      /*  This is the io handle we will give the reader to parse  */
   nitf_IOHandle theIoHandle;
   
   ossim_uint32 theNumberOfImages;
   nitf_ImageReader* theCurrentImageReader;
   nitf_ListIterator theCurrentImageIterator;
   nitf_ImageSegment* theCurrentImageSegment;
   nitf_Uint32 theNumberOfBits;
   nitf_Uint32 theNumberOfInputBands;
   nitf_Uint32 theNumberOfOutputBands;
   nitf_Uint32 theNumberOfRows; 
   nitf_Uint32 theNumberOfColumns;
   size_t theSubImageSize;
   ossim_uint32 theCurrentImageEntry;
   nitf_Uint32 theNumberOfBlocksPerRow;
   nitf_Uint32 theNumberOfBlocksPerColumn;
   nitf_Uint32 theNumberOfPixelsPerHorizBlock;
   nitf_Uint32 theNumberOfPixelsPerVertBlock;
   ossimScalarType theScalarType;
   nitf_Uint8 **theReadBuffer;
   std::vector<nitf_Uint32> theBandList;
   nitf_SubWindow* theSubImage;
   ossimString theImageRepresentation;
   bool theIsVqCompressedFlag;
   
   ossimIrect theCurrentImageRect;
   ossimIrect theAnnotationBounds;
   /************************** Graphics and text support *********************/
   bool                       theEnableAnnotationsFlag;
   ossimRefPtr<ossimRgbImage> theAnnotationImage;
   ossimRefPtr<ossimImageData> theRgbAnnotationImageData;
   nitf_Uint32 theNumberOfGraphicSegments;
   nitf_Uint32 theNumberOfTextSegments;
   nitf_Uint32 theNumberOfLabelSegments;
   ossim_uint32 theMaxAnnotationRlevel;
   
   std::vector<ossimRefPtr<ossimAnnotationObject> > theAnnotations;
   std::vector<std::vector<ossimRefPtr<ossimAnnotationObject> > > theRlevelAnnotations;
   std::vector<ossimIrect> theRlevelAnnotationBounds;
   TYPE_DATA
};

#endif

