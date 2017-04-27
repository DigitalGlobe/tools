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

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>

#include <hdf.h>
// Not in latest 4.2.9 code: #include <hdf4_netcdf.h>
#include <mfhdf.h>

#include <sstream>

static ossimTrace traceDebug("ossimHdfGridModel:debug");

RTTI_DEF1(ossimHdfGridModel, "ossimHdfGridModel", ossimCoarseGridModel);

ossimHdfGridModel::ossimHdfGridModel()
   :
   ossimCoarseGridModel()
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
   :  ossimCoarseGridModel()
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

bool ossimHdfGridModel::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   bool status = ossimCoarseGridModel::saveState( kwl, prefix );
   if ( status )
   {
      std::string myPrefix = ( prefix ? prefix: "" );
      std::string value;
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
