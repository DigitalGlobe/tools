//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM HDF5 utility class.
//
//----------------------------------------------------------------------------
// $Id

#ifndef ossimH5Util_HEADER
#define ossimH5Util_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>

#include <iosfwd>
#include <string>
#include <vector>

// Forward class declarations:
class ossimIrect;
namespace H5
{
   class AbstractDs;
   class Attribute;
   class DataSet;
   class H5File;
}

// Anonymous enums:
enum 
{
   OSSIM_H5_MAX_RECURSION_LEVEL = 8
};

namespace ossim_hdf5
{
   // Anonymous enums:
   enum 
   {
      //---
      // Used by iterateGroupForDatasetNames which can recursively call itself
      // to avoid blowing away the stack on an error.
      //---
      MAX_RECURSION_LEVEL = 8
   };

   /**
    * @brief Print method.
    *
    * @param file Pointer to H5File instance.
    * @param out Stream to print to.
    * @return std::ostream&
    */
   std::ostream& print( H5::H5File* file,
                        std::ostream& out );   

   /**
    * @brief Iterative print method.
    * @param file Pointer to H5File instance.
    * @param prefix to start the walk with.
    * @param recursedCount Callers should start at zero. This is used
    * internally to avoid an infinite loop as this method recursively
    * calls itself.
    * @param out Stream to print to.
    */   
   void printIterative( H5::H5File* file,
                        const std::string& groupName,
                        const std::string& prefix,
                        ossim_uint32& recursedCount,
                        std::ostream& out );

   /**
    * @brief Prints an object.
    * @param file Pointer to H5File instance.
    * @param prefix to start the walk with.
    * @param recursedCount Callers should start at zero. This is used
    * internally to avoid an infinite loop as this method recursively
    * calls itself.
    * @param out Stream to print to.
    */ 
   void printObject(  H5::H5File* file,
                      const std::string& objectName,
                      const std::string& prefix,
                      std::ostream& out );

   /**
    * @brief Gets string value for attribute key.
    *
    * This assumes H5::Attribute type class is a H5T_STRING.
    * 
    * @param file Pointer to H5File instance.
    *
    * @param group Group to open. Examples:
    * "/"
    * "/Data_Products/VIIRS-DNB-GEO"
    * 
    * @param key This is key string. Examples:
    * "Mission_Name"
    * "N_Collection_Short_Name"
    *
    * @param value Initialized by this.
    *
    * @return true on success, false on error.
    */    
   bool getGroupAttributeValue( H5::H5File* file,
                                const std::string& group,
                                const std::string& key,
                                std::string& value );
   
   /**
    * @brief Gets string value for attribute key.
    *
    * This assumes H5::Attribute type class is a H5T_STRING.
    * 
    * @param file Pointer to H5File instance.
    *
    * @param dataset Data to open. Examples:
    * "/"
    * "/Data_Products/VIIRS-DNB-GEO"
    * 
    * @param key This is key string. Examples:
    * "Mission_Name"
    * "N_Collection_Short_Name"
    *
    * @param value Initialized by this.
    *
    * @return true on success, false on error.
    */    
   bool getDatasetAttributeValue( H5::H5File* file,
                                const std::string& objectName,
                                const std::string& key,
                                std::string& value );
   
   void printAttribute( const H5::Attribute& attr,
                        const std::string& prefix,
                        std::ostream& out );
   
   void combine( const std::string& left,
                 const std::string& right,
                 char separator,
                 std::string& result );
   
   void getDatasetNames(H5::H5File* file, std::vector<std::string>& names );

   std::string getDatatypeClassType( ossim_int32 type );

   bool isLoadableAsImage( H5::H5File* file,
                           const std::string& datasetName );

   bool isExcludedDataset( const std::string& datasetName );

   void iterateGroupForDatasetNames( H5::H5File* file,
                                     const std::string& group,
                                     std::vector<std::string>& names,
                                     ossim_uint32& recursedCount );

   void getExtents( const H5::DataSet* dataset,
                    std::vector<ossim_uint32>& extents );

   // ossimScalarType getScalarType( ossim_int32 typeClass, ossim_int32 id );
   ossimScalarType getScalarType( const H5::DataSet* dataset );


   ossimScalarType getScalarType( ossim_int32 id );
   
   ossimByteOrder getByteOrder( const H5::AbstractDs* dataset );

   /**
    * @brief Gets the valid bounding rect of the dataset excluding nulls on front and back.
    * @param dataset
    * @param name Name of dataset.  Used for null scanning.
    * @param rect Initialized by this.
    */
   bool getValidBoundingRect( H5::DataSet& dataset,
                              const std::string& name,
                              ossimIrect& rect );

   /**
    * @brief Gets bilinear projection from Latitude, Longitude layer.
    *
    * @param latDataSet H5::DataSet& to layer,
    *    e.g. /All_Data/VIIRS-DNB-GEO_All/Latitude
    * @param lonDataSet H5::DataSet& to layer,
    *    e.g. /All_Data/VIIRS-DNB-GEO_All/Longitude
    */
   ossimRefPtr<ossimProjection> getBilinearProjection(
      H5::DataSet& latDataSet,
      H5::DataSet& lonDataSet,
      const ossimIrect& validRect );

   /**
    * @brief Checks for dateline cross.
    * @param dataset
    * @param validRect Initialized by this.
    * @return true if dateline cross is detected, false if not.
    */
   bool crossesDateline( H5::DataSet& dataset, const ossimIrect& validRect );
   bool crossesDateline( const std::vector<ossim_float32>& lineBuffer);
   
} // End: namespace ossim_hdf5{...}

#endif /* #ifndef ossimH5Util_HEADER */

