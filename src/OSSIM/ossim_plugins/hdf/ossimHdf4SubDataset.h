//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Class declaration HDF reader.
//
// Specification: ISO/IEC 15444
//
//----------------------------------------------------------------------------
// $Id: ossimHdf4SubDataset.h 2645 2011-05-26 15:21:34Z oscar.kramer $

#ifndef ossimHdf4SubDataset_HEADER
#define ossimHdf4SubDataset_HEADER 1

//ossim includes
#include "../ossimPluginConstants.h"
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/support_data/ossimJ2kSizRecord.h>
#include <ossim/imaging/ossimAppFixedTileCache.h>

//HDF4 Includes
// #include <hdf/hdf.h>
// #include <hdf/mfhdf.h>
#include <hdf.h>
#include <mfhdf.h>



// Forward class declarations.
class ossimImageData;

class OSSIM_PLUGINS_DLL ossimHdf4SubDataset 
{
public:

   /** default construtor */
   ossimHdf4SubDataset();
   
   /** virtural destructor */
   virtual ~ossimHdf4SubDataset();

   /**
    *  @brief Method to grab a tile(rectangle) from image.
    *
    *  @param rect The zero based rectangle to grab.
    *
    *  @param resLevel The reduced resolution level to grab from.
    *
    *  @return The ref pointer with the image data pointer.
    */
   char* getTileBuf(const  ossimIrect& rect, ossim_uint32 band);   

   /**
    *  Returns the number of bands in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfInputBands() const;

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

   std::vector<ossimString> getMetadata() const { return m_meta; }

   /**
    * @brief sets number of lines.
    *
    */
   void setNumberOfLines(int32 lines);

   /**
    *  @brief sets the number of samples.
    *
    */
   void setNumberOfSamples(int32 samples);

   /**
    *  @brief sets the number of samples.
    *
    */
   void setNumberOfBands(int32 bands);

   /**
    * @brief sets the output pixel type of the tile source.
    *
    */
   void setOutputScalarType(int32 dataType);

   /**
    *  @brief Method to set number of attributes for dataset.
    *
    */
   void setAttributeCount(int32 attributes); 

   /**
    *  @brief Method to set sds name for dataset.
    *
    */
   void setSdsName(ossimString name); 

   /**
    *  @brief Method to set sds rank for dataset.
    *
    */
   void setSdsRank(int32 rank); 

   /**
    * @brief Open method.
    * @return true on success, false on error.
    */
   virtual bool open();

   /**
    *  @brief Method to set hdf file name for open file.
    *
    */
   void setHdfFilename(ossimString fileName);

   /**
    *  @brief Method to set SDS id of hdf file for open file.
    *
    */
   void setHdfDataType(ossimString hdfDatyType) { m_hdfDataType = hdfDatyType; }

   /**
    *  @brief Method to set Scientific Data Set Id for get data.
    *
    */
   void setHdfSdsIndex(int32 sds_index);   

private:

  ossim_int32 getDataTypeSize(int32 type);

  ossimString getAttribuitValues(int32 dataType, int32 numValues, void* attrValue);

  void initMeta();

  ossimString                       m_fileName;
  ossimString                       m_sd_name;//SDS name
  int32                             m_sds_index; //SDS index 
  int32                             m_sds_id; //SDS selected id
  int32                             m_sd_id; //SDS id of HDF file
  int32                             m_rank;
  ossim_uint32                      m_numberOfBands;
  ossim_uint32                      m_numberOfSamples;
  ossim_uint32                      m_numberOfLines;
  int32                             m_attributes;
  bool                              m_isGeoField;
  ossimScalarType                   m_scalarType;
  int32                             m_dataType;
  ossimString                       m_hdfDataType;
  std::vector<ossimString>          m_meta;
TYPE_DATA
};

#endif /* #ifndef ossimHdf4SubDataset_HEADER */

