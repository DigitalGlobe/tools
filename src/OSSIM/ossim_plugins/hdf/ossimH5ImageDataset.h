//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM HDF5 Image DataSet.
//
//----------------------------------------------------------------------------
// $Id

#ifndef ossimH5ImageDataset_HEADER
#define ossimH5ImageDataset_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <iosfwd>
#include <string>

// Forward class declarations:
class ossimImageData;
class ossimEndian;
class ossimIrect;
namespace H5
{
   class DataSet;
   class H5File;
}

/**
 * @brief Class encapsulates a HDF5 Data set that can be loaded as an image.
 */
class ossimH5ImageDataset
{
public:
   /** default constructor */
   ossimH5ImageDataset();
   
   /** copy constructor */
   ossimH5ImageDataset( const ossimH5ImageDataset& obj );
   
   /** destructor */
   ~ossimH5ImageDataset();

   /** @brief Calls H5::DataSet::close then deletes data set. */
   void close();

   const ossimH5ImageDataset& operator=( const ossimH5ImageDataset& rhs );

   /**
    * @brief Opens datasetName and initializes all data members on success.
    * @return true on success, false on error.
    */
   void initialize( const H5::DataSet& dataset,
                    const std::string& datasetName );

   /**
    * @brief Get const pointer to dataset.
    *
    * This can be null if not open.
    * 
    * @return const pointer to dataset.
    */
   const H5::DataSet* getDataset() const;
   
   /**
    * @brief Get pointer to dataset.
    *
    * This can be null if not open.
    * 
    * @return pointer to dataset.
    */
   H5::DataSet* getDataset();

   /** @return The dataset name.  This is the full path used for open. */
   const std::string& getName() const;

   /** @return The output scalar type. */
   ossimScalarType getScalarType() const;

   /** @return the number of . */
   ossim_uint32 getNumberOfBands() const;

   /** @return The number of lines. */
   ossim_uint32 getNumberOfLines() const;
   
   /** @return The number of samples. */
   ossim_uint32 getNumberOfSamples() const;

   /** @return The swap flag. */
   bool getSwapFlag() const;

   /**
    *  @brief Method to grab a tile(rectangle) from image.
    *
    *  @param buffer Buffer for data for this method to copy data to.
    *  Should be the size of rect * bytes_per_pixel for scalar type.
    *
    *  @param rect The zero based rectangle to grab.
    *
    *  @param band
    */
   void getTileBuf(void* buffer, const ossimIrect& rect, ossim_uint32 band);   

   /**
    * @brief print method.
    * @return std::ostream&
    */
   std::ostream& print(std::ostream& out) const;

   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& out,
                                                  const ossimH5ImageDataset& obj);
private:
   H5::DataSet*    m_dataset;
   std::string     m_datasetName;
   ossimScalarType m_scalar;
   ossim_uint32    m_bands;
   ossim_uint32    m_lines;
   ossim_uint32    m_samples;
   ossimEndian*    m_endian; // For byte swapping if needed.
   
}; // End: class ossimH5ImageDataset

#endif /* #ifndef ossimH5ImageDataset_HEADER */

