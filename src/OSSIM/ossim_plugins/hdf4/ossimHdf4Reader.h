//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Class declaration HDF4 reader.
//
// Specification: ISO/IEC 15444
//
//----------------------------------------------------------------------------
// $Id: ossimHdf4Reader.h 2645 2011-05-26 15:21:34Z oscar.kramer $

#ifndef ossimHdf4Reader_HEADER
#define ossimHdf4Reader_HEADER 1

#include "../ossimPluginConstants.h"
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimAppFixedTileCache.h>

//HDF4 Includes
// #include <hdf/hdf.h>
// #include <hdf/mfhdf.h>
#include <hdf.h>
#include <mfhdf.h>

#include <iosfwd>
#include <fstream>
#include <vector>
#include <set>

// Forward class declarations.
class ossimImageData;
class ossimDpt;
class ossimHdf4SubDataset;
class ossimBilinearProjection;

class OSSIM_PLUGINS_DLL ossimHdf4Reader : public ossimImageHandler
{
public:

   /** default construtor */
   ossimHdf4Reader();
   
   /** virtural destructor */
   virtual ~ossimHdf4Reader();

   /**
    * @brief Returns short name.
    * @return "ossim_hdf4_reader"
    */
   virtual ossimString getShortName() const;
   
   /**
    * @brief Returns long  name.
    * @return "ossim hdf4 reader"
    */
   virtual ossimString getLongName()  const;

   /**
    * @brief Returns class name.
    * @return "ossimHdf4Reader"
    */
   virtual ossimString getClassName() const;

   /**
    *  @brief Method to grab a tile(rectangle) from image.
    *
    *  @param rect The zero based rectangle to grab.
    *
    *  @param resLevel The reduced resolution level to grab from.
    *
    *  @return The ref pointer with the image data pointer.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);   

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
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileHeight which
    * returns the output tile height which can be different than the internal
    * image tile height on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   /**
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;
   

   /**
    * @brief Gets number of lines for res level.
    *
    *  @param resLevel Reduced resolution level to return lines of.
    *  Default = 0
    *
    *  @return The number of lines for specified reduced resolution level.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const;

   /**
    *  @brief Gets the number of samples for res level.
    *
    *  @param resLevel Reduced resolution level to return samples of.
    *  Default = 0
    *
    *  @return The number of samples for specified reduced resolution level.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const;

   /**
    * @brief Open method.
    * @return true on success, false on error.
    */
   virtual bool open();

   /**
    *  @brief Method to test for open file stream.
    *
    *  @return true if open, false if not.
    */
   virtual bool isOpen()const;

   /**
    * @brief Method to close current entry.
    *
    * @note There is a bool kdu_compressed_source::close() and a
    * void ossimImageHandler::close(); hence, a new close to avoid conflicting
    * return types.
    */
   virtual void closeEntry();

   /**
    * Returns the image geometry object associated with this tile source or
    * NULL if non defined.  The geometry contains full-to-local image
    * transform as well as projection (image-to-world).
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   /**
    * @param Method to get geometry.
    */
   virtual ossimRefPtr<ossimImageGeometry> getInternalImageGeometry() const;

   /**
    * @return The number of entries (images) in the image file.
    */
   virtual ossim_uint32 getNumberOfEntries()const;

   /**
    * @param entryList This is the list to initialize with entry indexes.
    *
    * @note This implementation returns puts one entry "0" in the list.
    */
   virtual void getEntryList(std::vector<ossim_uint32>& entryList) const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);   

   virtual bool setCurrentEntry(ossim_uint32 entryIdx);

   /**
   * @return The current entry number.
   */
   virtual ossim_uint32 getCurrentEntry()const;

   std::vector<ossimString> getGlobalMeta() { return m_globalMeta; }

   std::vector<ossimString> getSubDatasetMeta();

   ossimString getHdfDataType() const { return m_hdfDataType; }

   ossimString getMetaValue(const ossimString& key) const;

   static const ossimString HDF4_EOS_GRID;              // HDF-EOS Grid
   static const ossimString HDF4_EOS_SWATH;             // HDF-EOS Swath
   static const ossimString HDF4_SEAWIFS_L1A;		     // SeaWiFS Level-1A Data
   static const ossimString HDF4_SEAWIFS_L2;	           // SeaWiFS Level-2 Data
   static const ossimString HDF4_SEAWIFS_L3;		        // SeaWiFS Level-3 Standard Mapped Image
   static const ossimString HDF4_HYPERION_L1;           // Hyperion L1 Data Product
   static const ossimString HDF4_SDS;                   // SDS

private:

   struct ossimHdfSubdataset
   {
      ossimHdfSubdataset(int32        sdsIndex,
                         ossimString  name,
                         int32        dataType,
                         int32        rank,
                         int32        bands,
                         int32        rows,
                         int32        cols,
                         int32        attributes)
         :m_sdsIndex(sdsIndex),
         m_name(name),
         m_dataType(dataType),
         m_rank(rank),
         m_bands(bands),
         m_rows(rows),
         m_cols(cols),
         m_attributes(attributes)
      {}
     
      int32        m_sdsIndex;
      ossimString  m_name;
      int32        m_dataType;
      int32        m_rank;
      int32        m_bands;
      int32        m_rows;
      int32        m_cols;
      int32        m_attributes;
   };

   struct ossimHdfGroupData
   {
      ossimHdfGroupData(ossimString groupName,
         std::vector<ossimHdfSubdataset> geoSubdatasets,
         std::vector<ossimHdfSubdataset> dataSubdatasets)

         :m_groupName(groupName),
         m_geoSubdatasets(geoSubdatasets),
         m_dataSubdatasets(dataSubdatasets)
      {}

      ossimString m_groupName;
      std::vector<ossimHdfSubdataset> m_geoSubdatasets;
      std::vector<ossimHdfSubdataset> m_dataSubdatasets;
   };

  bool initGlobalMeta();

  bool initSubDatasets(int32 numDatasets);

  void initSubdatasetNames();

  void parseGroupName(std::vector<ossimString> groupInfo, ossimString groupName);

  void initGroupInfos();

  ossimString getAttribuitValues(int32 dataType, int32 numValues, void* attrValue) const;

  void parseMetaString(ossimString attrStr);

  void initHdfDataType();

  ossimRefPtr<ossimProjection> processSwathProjection() const;

  ossimRefPtr<ossimProjection> processGridProjection() const;

  ossim_int32 getSizeByType(int32 dataType) const;

  ossim_int32                                        m_fid;
  ossim_int32                                        m_sd_id;

  ossimIrect                                         m_imageRect; /** Has sub image offset. */
  
  ossim_uint32                                       m_numberOfBands;
  ossimScalarType                                    m_scalarType;
  ossimRefPtr<ossimImageData>                        m_tile;

  std::map<int32, ossimString>                       m_groupInfos;
  std::set<ossimString>                              m_subDatasetNames;
  std::vector<ossimHdfSubdataset>                    m_subDatasets;
  std::vector<ossimHdfGroupData>                     m_groupDatasets;
  std::vector<ossimString>                           m_globalMeta;
  ossimString                                        m_hdfDataType;
  bool                                               m_isHdfEos;
  ossim_uint32                                       m_currentEntryRender;
  ossimHdf4SubDataset*                               m_currentSubDataset;
  ossimString                                        m_structMetadata;

TYPE_DATA
};

#endif /* #ifndef ossimHdf4Reader_HEADER */

