//*****************************************************************************
// FILE: ossimHdfGridModel.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Mingjie Su
//
// DESCRIPTION:
//   Contains declaration of class ossimHdfGridModel. This is an
//   implementation of an interpolation sensor model.
//
//*****************************************************************************
//  $Id: ossimHdfGridModel.h 2645 2011-05-26 15:21:34Z oscar.kramer $

#ifndef ossimHdfGridModel_HEADER
#define ossimHdfGridModel_HEADER 1

#include <ossim/projection/ossimCoarseGridModel.h>
#include "../ossimPluginConstants.h"

#include <string>

//HDF4 Includes
// #include <hdf/hdf.h>
// #include <hdf/mfhdf.h>
// #include <hdf.h>
// #include <mfhdf.h>

//HDF5 Includes
// #include <hdf5.h>

// Forward class declarations:
namespace H5
{
   class DataSet;
   class H5File;
}


/******************************************************************************
 *
 * CLASS:  ossimHdfGridModel
 *
 *****************************************************************************/



class OSSIM_PLUGINS_DLL ossimHdfGridModel : public ossimCoarseGridModel
{
public:

   /** @brief default constructor. */
   ossimHdfGridModel();
   
    //! CONSTRUCTOR (filename) Accepts name of hdf file and the index of SDS data
   ossimHdfGridModel(const ossimFilename& file, 
                    const ossimDrect& imageRect,
                    ossimString latGridIndexOrName, 
                    ossimString lonGridIndexOrName, 
                    const ossimIpt& gridSpacing);

   /** @brief virtual destructor */
   virtual ~ossimHdfGridModel();

   /**
    * @brief Initializes the latitude and longitude ossimDblGrids from file.
    * @param h5File Pointer to file.
    * @param latDataSetName e.g. /All_Data/VIIRS-DNB-GEO_All/Latitude
    * @param lonDataSetName e.g. /All_Data/VIIRS-DNB-GEO_All/Longitude
    * @param imageRows Number of lines in the image.
    * @param imageCols Number of samples in the image.
    * @return true on success, false on error.
    */
   bool setGridNodes( H5::H5File* h5File,
                      const std::string& latDataSetName,
                      const std::string& lonDataSetName,
                      ossim_uint32 imageRows,
                      ossim_uint32 imageCols );

   /**
    * @brief Initializes the latitude and longitude ossimDblGrids from file.
    * @param latDataSet H5::DataSet* to layer,
    *    e.g. /All_Data/VIIRS-DNB-GEO_All/Latitude
    * @param lonDataSet H5::DataSet* to layer,
    *    e.g. /All_Data/VIIRS-DNB-GEO_All/Longitude
    * @param imageRows Number of lines in the image.
    * @param imageCols Number of samples in the image.
    * @return true on success, false on error.
    *
    * This can throw an ossimException on nans.
    */
   bool setGridNodes( H5::DataSet* latDataSet,
                      H5::DataSet* lonDataSet,
                      ossim_uint32 imageRows,
                      ossim_uint32 imageCols );

   /**
    * @brief saveState Saves state of object to a keyword list.
    * @param kwl Initialized by this.
    * @param prefix E.g. "image0.geometry.projection."
    * @return true on success, false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief loadState Loads state of object from a keyword list.
    * @param kwl Keyword list to initialize from.
    * @param prefix E.g. "image0.geometry.projection."
    * @return true on success, false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   
private:
   
   void setGridNodes(ossimDblGrid& grid, ossim_int32 sds_id, const ossimIpt& spacing);

   bool getWktFootprint( std::string& s ) const;
   
   void debugDump();
   

   bool m_isHdf4;

   TYPE_DATA
};

#endif

