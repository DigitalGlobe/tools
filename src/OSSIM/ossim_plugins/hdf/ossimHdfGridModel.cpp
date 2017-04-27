//*****************************************************************************
// FILE: ossimHdfGridModel.cc
//
// License:  See LICENSE.txt file in the top level directory.
//
// AUTHOR: Mingjie Su
//
// DESCRIPTION:
//   Contains implementation of class ossimHdfGridModel. This is an
//   implementation of an interpolation sensor model. 
//
//   IMPORTANT: The lat/lon grid is for ground points on the ellipsoid.
//   The dLat/dHgt and dLon/dHgt partials therefore are used against
//   elevations relative to the ellipsoid.
//
//*****************************************************************************
//  $Id: ossimHdfGridModel.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include "ossimHdfGridModel.h"
#include "ossimH5Util.h"

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>

#include <hdf.h>
// Not in latest 4.2.9 code: #include <hdf4_netcdf.h>
#include <mfhdf.h>
#include <hdf5.h>
#include <H5Cpp.h>

#include <sstream>

static ossimTrace traceDebug("ossimHdfGridModel:debug");

RTTI_DEF1(ossimHdfGridModel, "ossimHdfGridModel", ossimCoarseGridModel);

ossimHdfGridModel::ossimHdfGridModel()
   :
   ossimCoarseGridModel(),
   m_isHdf4(false)
{
   theLatGrid.setDomainType(ossimDblGrid::SAWTOOTH_90);
   theLonGrid.setDomainType(ossimDblGrid::WRAP_180);
}

//*****************************************************************************
//  CONSTRUCTOR: ossimHdfGridModel(filename)
//  
//  Constructs model from geometry file
//  
//*****************************************************************************
ossimHdfGridModel::ossimHdfGridModel(const ossimFilename& file,
                                     const ossimDrect& imageRect,
                                     ossimString latGridIndexOrName,
                                     ossimString lonGridIndexOrName,
                                     const ossimIpt& gridSpacing)
   :  ossimCoarseGridModel(),
   m_isHdf4(true)
{
   theLatGrid.setDomainType(ossimDblGrid::SAWTOOTH_90);
   theLonGrid.setDomainType(ossimDblGrid::WRAP_180);

   if (latGridIndexOrName.contains("/Latitude") == false && 
       lonGridIndexOrName.contains("/Longitude") == false)//hdf4
   {
      ossim_int32 latGridIndex = ossimString::toInt(latGridIndexOrName);
      ossim_int32 lonGridIndex = ossimString::toInt(lonGridIndexOrName);
      ossim_int32 sd_id = SDstart(file.c_str(), DFACC_READ);
      if (sd_id > 0)
      {
         ossim_int32 sds_id = SDselect(sd_id, latGridIndex);
         if (sds_id > 0)
         {
            setGridNodes(theLatGrid, sds_id, gridSpacing);
         }
         SDendaccess (sds_id); 

         sds_id = SDselect(sd_id, lonGridIndex);
         if (sds_id > 0)
         {
            setGridNodes(theLonGrid, sds_id, gridSpacing);
         }
         SDendaccess (sds_id); 
      }
      SDend(sd_id);
   }
   else //hdf5
   {
      m_isHdf4 = false;
      ossim_int32 file_id = H5Fopen(file.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

      // Depreciated, need to fix... (drb)
      // ossim_int32 dataset_id = H5Dopen(file_id, latGridIndexOrName);
      ossim_int32 dataset_id = H5Dopen1(file_id, latGridIndexOrName);
      if (dataset_id >= 0)
      {
         setGridNodes(theLatGrid, dataset_id, gridSpacing);
      }
      H5Dclose(dataset_id);

      // Depreciated, need to fix... (drb)
      // dataset_id = H5Dopen(file_id, lonGridIndexOrName);
      dataset_id = H5Dopen1(file_id, lonGridIndexOrName);      
      if (dataset_id > 0)
      {
         setGridNodes(theLatGrid, dataset_id, gridSpacing);
      }
      H5Dclose(dataset_id);

      H5Fclose(file_id);
   }
   

   // Filter this HDF data as it is often very noisy:
   double filter_kernel[81];
   double weight = 1.0/81.0;
   for (int i=0; i<81; i++)
      filter_kernel[i] = weight;
   theLatGrid.filter(9,9, filter_kernel);
   theLonGrid.filter(9,9, filter_kernel);

   theLatGrid.enableExtrapolation();
   theLonGrid.enableExtrapolation();
   theHeightEnabledFlag = false;
   initializeModelParams(imageRect);
   
   //debugDump(); //###
}

ossimHdfGridModel::~ossimHdfGridModel()
{
}

void ossimHdfGridModel::setGridNodes(ossimDblGrid& grid, ossim_int32 sds_id, const ossimIpt& spacing)
{
   int x=0, y=0;
   ossim_uint32 index = 0;
   if (m_isHdf4)
   {
      int32 dim_sizes[MAX_VAR_DIMS];
      int32 rank, data_type, n_attrs;
      char  name[MAX_NC_NAME];

      ossim_int32 status = SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);
      if (status == -1)
         return;

      int32 origin[2] = {0, 0}; 
      ossim_int32 num_rows = dim_sizes[0]; 
      ossim_int32 num_cols = dim_sizes[1]; 

      ossimDpt grid_origin(0,0); // The grid as used in base class, has UV-space always at 0,0 origin
      ossimIpt grid_size (num_cols, num_rows);
      ossimDpt dspacing (spacing);
      grid.initialize(grid_size, grid_origin, dspacing);

      float32* values = new float32 [num_rows * num_cols]; 
      intn statusN = SDreaddata(sds_id, origin, NULL, dim_sizes, (VOIDP)values);
      if (statusN > -1)
      {
         for (y = 0; y < num_rows; y++)
         {
            for (x = 0; x < num_cols; x++)
            {
               grid.setNode(x, y, values[index++]);
            }
         }
      }
      delete values;
   }
   else
   {
      hsize_t dims_out[2]; //dataset dimensions
      hid_t datatype = H5Dget_type(sds_id);
      hid_t dataspace = H5Dget_space(sds_id);    //dataspace handle
      int rank = H5Sget_simple_extent_ndims(dataspace);
      if (rank == 2)
      {
         H5Sget_simple_extent_dims(dataspace, dims_out, NULL);

         ossim_int32 latGridRows = dims_out[0];
         ossim_int32 lonGridCols = dims_out[1];

         ossim_int32 cols = spacing.x;
         ossim_int32 rows = spacing.y;

         ossim_int32 spacingX = 0;
         ossim_int32 spacingY = 0;

         if (rows % latGridRows == 0 && cols % lonGridCols == 0)
         {
            spacingY = rows/latGridRows; //line increment step
            spacingX = cols/lonGridCols; //pixel increment step
         }
         
         ossimIpt gridSpacing(spacingX, spacingY);

         ossimDpt grid_origin(0,0); // The grid as used in base class, has UV-space always at 0,0 origin
         ossimIpt grid_size (cols, rows);
         ossimDpt dspacing (gridSpacing);
         grid.initialize(grid_size, grid_origin, dspacing);

         if( H5Tequal(H5T_NATIVE_FLOAT,  datatype))
         {
            ossim_float32* data_out = new ossim_float32[dims_out[0] * dims_out[1]];
            H5Dread(sds_id, datatype, dataspace, dataspace, H5P_DEFAULT, data_out);

            index = 0;
            for (y = 0; y < rows; y++)
            {
               for (x = 0; x < cols; x++)
               {
                  index = x + y * cols;

                  //---
                  // NPP VIIRS data has null of "-999.3".
                  // Scan and fix non-standard null value.
                  //---
                  ossim_float32 d = data_out[index];
                  if ( d < -999.0 )
                  {
                     d = ossim::nan();
                  }
                  grid.setNode(x, y, d);
               }
            }
            delete[] data_out;
         }
         else if( H5Tequal(H5T_NATIVE_DOUBLE,  datatype))
         {
            ossim_float64* data_out = new ossim_float64[dims_out[0] * dims_out[1]];
            H5Dread(sds_id, datatype, dataspace, dataspace, H5P_DEFAULT, data_out);

            index = 0;
            for (y = 0; y < rows; y++)
            {
               for (x = 0; x < cols; x++)
               {
                  index = x + y * cols;

                  //---
                  // NPP VIIRS data has null of "-999.3".
                  // Scan and fix non-standard null value.
                  //---
                  ossim_float64 d = data_out[index];
                  if ( d < -999.0 )
                  {
                     d = ossim::nan();
                  }
                  grid.setNode(x, y, d);
               }
            }
            delete[] data_out;
         }
      }
      H5Tclose(datatype);
      H5Sclose(dataspace);
   }
}

bool ossimHdfGridModel::setGridNodes( H5::H5File* h5File,
                                      const std::string& latDataSetName,
                                      const std::string& lonDataSetName,
                                      ossim_uint32 imageRows,
                                      ossim_uint32 imageCols )

{
   bool status = false;

   if ( h5File )
   {
      H5::DataSet latDataSet = h5File->openDataSet( latDataSetName );
      H5::DataSet lonDataSet = h5File->openDataSet( lonDataSetName );  

      try
      {
         status = setGridNodes( &latDataSet, &lonDataSet, imageRows, imageCols );
      }
      catch ( const ossimException& e )
      {
         if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimHdfGridModel::setGridNodes caught exception\n"
               << e.what() << std::endl;
         }
      }

      latDataSet.close();
      lonDataSet.close();
   }
   
   return status;
}

bool ossimHdfGridModel::setGridNodes( H5::DataSet* latDataSet,
                                      H5::DataSet* lonDataSet,
                                      ossim_uint32 imageRows,
                                      ossim_uint32 imageCols )
{
   bool status = false;

   if ( latDataSet && lonDataSet )
   {
      const ossim_uint32 GRID_SIZE = 32; // Only grab every 32nd value.

      // Get dataspace of the dataset.
      H5::DataSpace latDataSpace = latDataSet->getSpace();
      H5::DataSpace lonDataSpace = lonDataSet->getSpace();
      const ossim_int32 LAT_DIM_COUNT = latDataSpace.getSimpleExtentNdims();
      const ossim_int32 LON_DIM_COUNT = latDataSpace.getSimpleExtentNdims();
      
      // Number of dimensions of the input dataspace:
      if ( ( LAT_DIM_COUNT == 2 ) && ( LON_DIM_COUNT == 2 ) )
      {
         //---
         // Get the extents:
         // dimsOut[0] is height, dimsOut[1] is width:         //---
         std::vector<hsize_t> latDimsOut(LAT_DIM_COUNT);
         latDataSpace.getSimpleExtentDims( &latDimsOut.front(), 0 );
         std::vector<hsize_t> lonDimsOut(LON_DIM_COUNT);
         lonDataSpace.getSimpleExtentDims( &lonDimsOut.front(), 0 );
         
         // Verify same as image:
         if ( (latDimsOut[0] == imageRows) &&
              (lonDimsOut[0] == imageRows) &&
              (latDimsOut[1] == imageCols) &&
              (lonDimsOut[1] == imageCols) )
         {
            //---
            // Capture the rectangle:
            // dimsOut[0] is height, dimsOut[1] is width:
            //---
            ossimIrect rect = ossimIrect( 0, 0,
                                          static_cast<ossim_int32>( latDimsOut[1]-1 ),
                                          static_cast<ossim_int32>( latDimsOut[0]-1 ) );
            
            //----
            // Initialize the ossimDblGrids:
            //---
            ossimDpt dspacing (GRID_SIZE, GRID_SIZE);
            
            ossim_uint32 gridRows = imageRows / GRID_SIZE + 1;
            ossim_uint32 gridCols = imageCols / GRID_SIZE + 1;
            
            // Round up if size doesn't fall on end pixel.
            if ( imageRows % GRID_SIZE) ++gridRows;
            if ( imageCols % GRID_SIZE) ++gridCols;
            
            ossimIpt gridSize (gridCols, gridRows);
            
            // The grid as used in base class, has UV-space always at 0,0 origin            
            ossimDpt gridOrigin(0.0,0.0);

            const ossim_float64 NULL_VALUE = -999.0;
            theLatGrid.setNullValue(ossim::nan());
            theLonGrid.setNullValue(ossim::nan());
            theLatGrid.initialize(gridSize, gridOrigin, dspacing);
            theLonGrid.initialize(gridSize, gridOrigin, dspacing);            
            
            std::vector<hsize_t> inputCount(LAT_DIM_COUNT);
            std::vector<hsize_t> inputOffset(LAT_DIM_COUNT);
            
            inputOffset[0] = 0; // row
            inputOffset[1] = 0; // col
            
            inputCount[0] = 1; // row
            inputCount[1] = (hsize_t)imageCols; // col
            
            // Output dataspace dimensions. Reading a line at a time.
            const ossim_int32 OUT_DIM_COUNT = 3;
            std::vector<hsize_t> outputCount(OUT_DIM_COUNT);
            outputCount[0] = 1; // band
            outputCount[1] = 1; // row
            outputCount[2] = imageCols; // col
            
            // Output dataspace offset.
            std::vector<hsize_t> outputOffset(OUT_DIM_COUNT);
            outputOffset[0] = 0;
            outputOffset[1] = 0;
            outputOffset[2] = 0;
            
            ossimScalarType scalar = ossim_hdf5::getScalarType( latDataSet );
            if ( scalar == OSSIM_FLOAT32 )
            {
               // Set the return status to true if we get here...
               status = true;
               
               // See if we need to swap bytes:
               ossimEndian* endian = 0;
               if ( ( ossim::byteOrder() != ossim_hdf5::getByteOrder( latDataSet ) ) )
               {
                  endian = new ossimEndian();
               }
               
               // Native type:
               H5::DataType latDataType = latDataSet->getDataType();
               H5::DataType lonDataType = lonDataSet->getDataType();

               // Output dataspace always the same, width of one line.
               H5::DataSpace bufferDataSpace( OUT_DIM_COUNT, &outputCount.front());
               bufferDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                &outputCount.front(),
                                                &outputOffset.front() );

               //  Arrays to hold a single line of latitude longitude values.
               vector<ossim_float32> latValue(imageCols);
               vector<ossim_float32> lonValue(imageCols);
               hsize_t row = 0;

               // Line loop:
               for ( ossim_uint32 y = 0; y < gridRows; ++y )
               {
                  // row = line in image space
                  row = y*GRID_SIZE;

                  bool hitNans = false;
                  
                  if ( row < imageRows )
                  {
                     inputOffset[0] = row;

                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                  
                     // Read data from file into the buffer.
                     latDataSet->read( &(latValue.front()), latDataType,
                                       bufferDataSpace, latDataSpace );
                     lonDataSet->read( &(lonValue.front()), lonDataType,
                                       bufferDataSpace, lonDataSpace );
                  
                     if ( endian )
                     {
                        // If the endian pointer is initialized(not zero) swap the bytes.
                        endian->swap( &(latValue.front()), imageCols );
                        endian->swap( &(lonValue.front()), imageCols );  
                     }
                  
                     // Sample loop:
                     hsize_t col = 0;
                     
                     for ( ossim_uint32 x = 0; x < gridCols; ++x )
                     {
                        ossim_float32 lat = ossim::nan();
                        ossim_float32 lon = ossim::nan();
                        
                        // col = sample in image space
                        col = x*GRID_SIZE;

                        if ( col < imageCols )
                        {
                           if ( (latValue[col] > NULL_VALUE)&&(lonValue[col] > NULL_VALUE) )
                           {
                              lat = latValue[col];
                              lon = lonValue[col];
                           }
                           else
                           {
                              hitNans = true;
                           }
                        }
                        else // Last column is outside of image bounds.
                        {
                           ossim_float32 latSpacing = ossim::nan();
                           ossim_float32 lonSpacing = ossim::nan();

                           // Get the last two latitude values:
                           ossim_float32 lat1 = latValue[imageCols-2];
                           ossim_float32 lat2 = latValue[imageCols-1];
                           
                           // Get the last two longitude values
                           ossim_float32 lon1 = lonValue[imageCols-2];
                           ossim_float32 lon2 = lonValue[imageCols-1];

                           if ( ( lat1 > NULL_VALUE ) && ( lat2 > NULL_VALUE ) )
                           {
                              // Delta between last two latitude values.
                              latSpacing = lat2 - lat1;
                           
                              // Compute:
                              lat = lat2 + ( col - (imageCols-1) ) * latSpacing;
                           }
                           else
                           {
                              hitNans = true;
                           }

                           if ( ( lon1 > NULL_VALUE ) && ( lon2 > NULL_VALUE ) )
                           {
                              // Consider dateline crossing.
                              if ( (lon1 > 0.0) && ( lon2 < 0.0 ) )
                              {
                                 lon2 += 360.0;
                              }
                           
                              // Delta between last two longitude values.
                              lonSpacing = lon2 - lon1;
                           
                              // Compute:
                              lon = lon2 + ( col - (imageCols-1) ) * lonSpacing;

                              // Check for wrap:
                              if ( lon > 180 )
                              {
                                 lon -= 360.0;
                              }
                           }
                           else
                           {
                              hitNans = true;
                           }

#if 0 /* Please leave for debug. (drb) */                        
                           cout << "lat1: " << lat1 << " lat2 " << lat2
                                << " lon1 " << lon1  << " lon2 " << lon2
                                << "\n";
#endif
                        }

                        if ( hitNans )
                        {
                           std::string errMsg =
                              "ossimHdfGridModel::setGridNodes encountered nans!";
                           throw ossimException(errMsg);
                        }
                        
                        // Assign the latitude and longitude.
                        theLatGrid.setNode( x, y, lat );
                        theLonGrid.setNode( x, y, lon );
                        
#if 0 /* Please leave for debug. (drb) */ 
                        cout << "x,y,col,row,lat,lon:" << x << "," << y << ","
                             << col << "," << row << ","
                             << theLatGrid.getNode(x, y)
                             << "," << theLonGrid.getNode( x, y) << "\n";
#endif
                     
                     } // End sample loop.
                     
                  }
                  else // Row is outside of image bounds:
                  {
                     // Read the last two rows in.
                     vector<ossim_float32> latValue2(imageCols);
                     vector<ossim_float32> lonValue2(imageCols);

                     inputOffset[0] = imageRows-2; // 2nd to last line

                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                  
                     // Read data from file into the buffer.
                     latDataSet->read( &(latValue.front()), latDataType,
                                       bufferDataSpace, latDataSpace );
                     lonDataSet->read( &(lonValue.front()), lonDataType,
                                       bufferDataSpace, lonDataSpace );
                  
                     inputOffset[0] = imageRows-1; // last line

                     latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                     lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                   &inputCount.front(),
                                                   &inputOffset.front() );
                  
                     // Read data from file into the buffer.
                     latDataSet->read( &(latValue2.front()), latDataType,
                                       bufferDataSpace, latDataSpace );
                     lonDataSet->read( &(lonValue2.front()), lonDataType,
                                       bufferDataSpace, lonDataSpace );
                  
                     if ( endian )
                     {
                        // If the endian pointer is initialized(not zero) swap the bytes.
                        endian->swap( &(latValue.front()), imageCols );
                        endian->swap( &(lonValue.front()), imageCols );
                        endian->swap( &(latValue2.front()), imageCols );
                        endian->swap( &(lonValue2.front()), imageCols );  
                     }

                     // Sample loop:
                     hsize_t col = 0;
                     for ( ossim_uint32 x = 0; x < gridCols; ++x )
                     {
                        col = x*GRID_SIZE; // Sample in image space.
                     
                        ossim_float32 lat        = ossim::nan();
                        ossim_float32 lat1       = ossim::nan();
                        ossim_float32 lat2       = ossim::nan();
                        ossim_float32 latSpacing = ossim::nan();
                        ossim_float32 lon        = ossim::nan();
                        ossim_float32 lon1       = ossim::nan();
                        ossim_float32 lon2       = ossim::nan();
                        ossim_float32 lonSpacing = ossim::nan();
                     
                        if ( col < imageCols )
                        {
                           lat1 = latValue[col];
                           lat2 = latValue2[col];
                           lon1 = lonValue[col];
                           lon2 = lonValue2[col];
                        }
                        else // Very last grid point.
                        {
                           // Compute the missing column for the last two image lines:
                           lat1 = latValue[imageCols-1] + ( col - (imageCols-1)) *
                              ( latValue[imageCols-1] - latValue[imageCols-2] );

                           lat2 = latValue2[imageCols-1] + ( col - (imageCols-1)) *
                              ( latValue2[imageCols-1] - latValue2[imageCols-2] );

                           lon1 = lonValue[imageCols-1] + ( col - (imageCols-1)) *
                              ( lonValue[imageCols-1] - lonValue[imageCols-2] );

                           lon2 = lonValue2[imageCols-1] + ( col - (imageCols-1)) *
                              ( lonValue2[imageCols-1] - lonValue2[imageCols-2] );
                        }

#if 0 /* Please leave for debug. (drb) */
                        cout << "lat1: " << lat1 << " lat2 " << lat2
                             << " lon1 " << lon1  << " lon2 " << lon2
                             << "\n";
#endif

                        if ( ( lon1 > NULL_VALUE ) && ( lon2 > NULL_VALUE ) )
                        {
                           // Consider dateline crossing.
                           if ( (lon1 > 0.0) && ( lon2 < 0.0 ) )
                           {
                              lon2 += 360.0;
                           }
                        
                           // Delta between last two longitude values.
                           lonSpacing = lon2 - lon1;
                        
                           // Compute:
                           lon = lon2 + ( row - (imageRows-1) ) * lonSpacing;

                           // Check for wrap:
                           if ( lon > 180 )
                           {
                              lon -= 360.0;
                           }
                        }
                        else
                        {
                           hitNans = true;
                        }
                     
                        if ( ( lat1 > NULL_VALUE ) && ( lat2 > NULL_VALUE ) )
                        {
                           // Delta between last two latitude values.
                           latSpacing = lat2 - lat1;
                        
                           // Compute:
                           lat = lat2 + ( row - (imageRows-1) ) * latSpacing;
                        }
                        else
                        {
                           hitNans = true;
                        }

                        if ( hitNans )
                        {
                           std::string errMsg =
                              "ossimHdfGridModel::setGridNodes encountered nans!";
                           throw ossimException(errMsg);
                        }

                        // Assign the latitude::
                        theLatGrid.setNode( x, y, lat );

                        // Assign the longitude.
                        theLonGrid.setNode( x, y, lon );

#if 0 /* Please leave for debug. (drb) */
                        cout << "x,y,col,row,lat,lon:" << x << "," << y << ","
                             << col << "," << row << "," << lat << "," << lon << "\n";
#endif
                     
                     } // End sample loop.
                  
                  } // Matches if ( row < imageRows ){...}else{
                  
               } // End line loop.

               latDataSpace.close();
               lonDataSpace.close();

               if ( status )
               {
                  theLatGrid.enableExtrapolation();
                  theLonGrid.enableExtrapolation();
                  theHeightEnabledFlag = false;
                  ossimDrect imageRect(rect);
                  initializeModelParams(imageRect);
                  // debugDump();
               }
               
            } // Matches: if ( scalar == OSSIM_FLOAT32 )
            
         } // Matches: if ( (latDimsOut[0] == imageRows) ...
         
      } // Matches: if ( ( LAT_DIM_COUNT == 2 ) ...

   } // Matches: if ( latDataSet && lonDataSet
   
   return status;
   
} // End: bool ossimHdfGridModel::setGridNodes( H5::DataSet* latDataSet, ... )

bool ossimHdfGridModel::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   bool status = ossimCoarseGridModel::saveState( kwl, prefix );
   if ( status )
   {
      std::string myPrefix = ( prefix ? prefix: "" );

      // Add the is hdf4 flag:
      std::string key = "is_hdf4";
      std::string value = ossimString::toString( m_isHdf4 ).string();
      kwl.addPair( myPrefix, key, value, true );
      
      if ( getWktFootprint( value ) )
      {   
         std::string key = "wkt_footprint";
         kwl.addPair( myPrefix, key, value, true );
      }
   }
   return status;
}     

bool ossimHdfGridModel::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = false;
   
   std::string myPrefix = ( prefix ? prefix: "" );

   // Look for type key:
   std::string typeKey = "type";
   std::string value = kwl.findKey( myPrefix, typeKey );
   if ( value == "ossimHdfGridModel" )
   {
      // Look for "is_hdf4" key:
      std::string key   = "is_hdf4";
      value = kwl.findKey( myPrefix, key );
      if ( value.size() )
      {
         m_isHdf4 = ossimString(value).toBool();
      }
      
      // Make a copy of kwl so we can change type.
      ossimKeywordlist myKwl = kwl;
      value = "ossimCoarseGridModel";
      myKwl.addPair( myPrefix, typeKey, value, true );

      // Load the state of base class.
      result = ossimCoarseGridModel::loadState( myKwl, prefix );
   }
   
   return result;                    
}

bool ossimHdfGridModel::getWktFootprint( std::string& s ) const
{
   bool status = true;

   // Loop until complete or a "nan" is hit.
   while ( 1 )
   {
      ossim_float32 lat = 0.0;
      ossim_float32 lon = 0.0;

      std::ostringstream os;

      os <<  setprecision(10) << "POLYGON((";
      const ossim_int32 STEP = 128;
      
      ossimIrect rect( 0, 0, theImageSize.x-1, theImageSize.y-1 );  

      ossim_int32 x = 0;
      ossim_int32 y = 0;
      
      // Walk the top line:
      while ( x < theImageSize.x )
      {
         lat = theLatGrid( x, y );
         lon = theLonGrid( x, y );

         if ( ossim::isnan(lat) || ossim::isnan(lon) )
         {
            status = false;
            break;
         }
         
         os << lon << " " << lat << ",";
         
         if ( x != rect.ur().x )
         {
            x = ossim::min<ossim_int32>( x+STEP, rect.ur().x );
         }
         else
         {
            break; // End of top line walk.
         }
      }

      if ( !status ) break; // Found "nan" so get out.
      
      // Walk the right edge:
      y = ossim::min<ossim_int32>( y+STEP, rect.lr().y );
      while ( y < theImageSize.y )
      {
         lat = theLatGrid( x, y );
         lon = theLonGrid( x, y );

         if ( ossim::isnan(lat) || ossim::isnan(lon) )
         {
            status = false;
            break;
         }

         os << lon << " " << lat << ",";
         
         if ( y != rect.lr().y )
         {
            y = ossim::min<ossim_int32>( y+STEP, rect.lr().y );
         }
         else
         {
            break;
         }
      }

      if ( !status ) break; // Found "nan" so get out.
      
      // Walk the bottom line from right to left:
      x = ossim::max<ossim_int32>( rect.lr().x-STEP, 0 );
      while ( x >= 0 )
      {
         lat = theLatGrid( x, y );
         lon = theLonGrid( x, y );
         
         if ( ossim::isnan(lat) || ossim::isnan(lon) )
         {
            status = false;
            break;
         }

         os << lon << " " << lat << ",";
         
         if ( x != 0 )
         {
            x = ossim::max<ossim_int32>( x-STEP, 0 );
         }
         else
         {
            break;
         }
      }

      if ( !status ) break; // Found "nan" so get out.
      
      // Walk the left edge from bottom to top:
      y = ossim::max<ossim_int32>( y-STEP, 0 );
      while ( y >= 0 )
      {
         lat = theLatGrid( x, y );
         lon = theLonGrid( x, y );

         if ( ossim::isnan(lat) || ossim::isnan(lon) )
         {
            status = false;
            break;
         }

         if ( ( x == 0 ) && ( y == 0 ) )
         {
            os << lon << " " << lat; // Last point:
         }
         else
         {
            os << lon << " " << lat << ",";
         }
         
         if ( y != 0 )
         {
            y = ossim::max<ossim_int32>( y-STEP, 0 );
         }
         else
         {
            break;
         }
      }

      if ( !status ) break; // Found "nan" so get out.
      
      os << "))";
      s = os.str();

      // Trailing break from while ( FOREVER ) loop:
      break;
      
   } // Matches: while ( 1 )
   
   return status;
}

void ossimHdfGridModel::debugDump()
{
   ossimIpt step (theImageSize/200);
   int margin = 0;
   double lat, lon;
   ossimDpt pt (0,0);
   ofstream fslat ("lat_grid.dat");
   ofstream fslon ("lon_grid.dat");
   fslat<< setprecision(10) <<endl;
   fslon<< setprecision(10) <<endl;
   for (pt.v = -margin*step.v; pt.v < theImageSize.v+margin*step.v; pt.v += step.y)
   {
      for (pt.u = -margin*step.u; pt.u < theImageSize.u+margin*step.u ; pt.u += step.u)
      {
         lat = theLatGrid(pt.u, pt.v);
         lon = theLonGrid(pt.u, pt.v);
         fslat << pt.u << " " << pt.v << " " << lat << endl;
         fslon << pt.u << " " << pt.v << " " << lon << endl;
      }
   }
   fslat.close();
   fslon.close();
}
