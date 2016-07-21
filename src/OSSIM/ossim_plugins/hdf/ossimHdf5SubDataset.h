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
// $Id: ossimHdf5SubDataset.h 2645 2011-05-26 15:21:34Z oscar.kramer $

#ifndef ossimHdf5SubDataset_HEADER
#define ossimHdf5SubDataset_HEADER 1

//ossim includes
#include "../ossimPluginConstants.h"
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageHandler.h>

//HDF4 Includes
#include <hdf5.h>

// Forward class declarations.
class ossimImageData;

class OSSIM_PLUGINS_DLL ossimHdf5SubDataset 
{
public:

   /** default construtor */
   ossimHdf5SubDataset();
   
   /** virtural destructor */
   virtual ~ossimHdf5SubDataset();

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

   void setMeta(const ossimString& key, const ossimString& value);

   /**
    *  @brief Method to set sds name for dataset.
    *
    */
   void setSubdatasetName(const ossimString& name); 

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

   void setOutputScalarType();

   void close();

private:

  bool initMeta();

  ossimString                       m_fileName;
  ossimString                       m_dataset_name;// name
  hid_t                             m_fid;
  hid_t                             m_dataset_id; //SDS selected id
  ossim_int32                       m_rank;
  hid_t                             m_native;
  hid_t                             m_dataType;
  hid_t                             m_dataspace;
  size_t                            m_size;
  ossim_int32                       m_dataSize;

  ossim_uint32                      m_numberOfBands;
  ossim_uint32                      m_numberOfSamples;
  ossim_uint32                      m_numberOfLines;
  ossimScalarType                   m_scalarType;
  ossimString                       m_hdfDataType;
  std::vector<ossimString>          m_meta;
TYPE_DATA
};

#endif /* #ifndef ossimHdf5SubDataset_HEADER */


