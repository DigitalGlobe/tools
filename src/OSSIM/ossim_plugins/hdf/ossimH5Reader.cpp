//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM HDF5 Reader.
//
//----------------------------------------------------------------------------
// $Id: ossimH5Reader.cpp 19878 2011-07-28 16:27:26Z dburken $

#include "ossimH5Reader.h"
#include "ossimH5Util.h"
#include "ossimHdfGridModel.h"

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimU8ImageData.h>

#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimProjection.h>

//---
// This includes everything!  Note the H5 includes are order dependent; hence,
// the mongo include.
//---
#include <hdf5.h>
#include <H5Cpp.h>

RTTI_DEF1(ossimH5Reader, "ossimH5Reader", ossimImageHandler)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id$";
#endif
   
static ossimTrace traceDebug("ossimH5Reader:debug");

static const std::string LAYER_KW = "layer";

ossimH5Reader::ossimH5Reader()
   :
      ossimImageHandler(),
      m_h5File(0),
      m_entries(),
      m_currentEntry(0),
      m_tile(0),
      m_projection(0),
      m_mutex()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimH5Reader::ossimH5Reader entered..." << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << endl;
#endif
   }
}

ossimH5Reader::~ossimH5Reader()
{
   if (isOpen())
   {
      close();
   }
}

void ossimH5Reader::allocate()
{
   m_tile = ossimImageDataFactory::instance()->create(this, this);
   m_tile->initialize();
}

ossimRefPtr<ossimImageData> ossimH5Reader::getTile(
   const ossimIrect& rect, ossim_uint32 resLevel)
{
   if ( m_tile.valid() == false ) // First time through.
   {
      allocate();
   }
   
   if (m_tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_tile->setImageRectangle(rect);
      
      if ( getTile( m_tile.get(), resLevel ) == false )
      {
         if (m_tile->getDataObjectStatus() != OSSIM_NULL)
         {
            m_tile->makeBlank();
         }
      }
   }
   
   return m_tile;
}

bool ossimH5Reader::getTile(ossimImageData* result, ossim_uint32 resLevel)
{
   bool status = false;
   
   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      result->ref(); // Increment ref count.
      
      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);

      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         ossimIrect tile_rect = result->getImageRectangle();

         if ( ! tile_rect.completely_within(getImageRectangle(0)) )
         {
            // We won't fill totally so make blank first.
            result->makeBlank();
         }
         
         if (getImageRectangle(0).intersects(tile_rect))
         {
            // Make a clip rect.
            ossimIrect clipRect = tile_rect.clipToRect(getImageRectangle(0));
            
            if (tile_rect.completely_within( clipRect) == false)
            {
               // Not filling whole tile so blank it out first.
               result->makeBlank();
            }

            // Create buffer to hold the clip rect for a single band.
            ossim_uint32 clipRectSizeInBytes = clipRect.area() *
               ossim::scalarSizeInBytes( m_entries[m_currentEntry].getScalarType() );
            vector<char> dataBuffer(clipRectSizeInBytes);

            // Get the data.
            for (ossim_uint32 band = 0; band < getNumberOfInputBands(); ++band)
            {
               // Hdf5 file to buffer:
               m_entries[m_currentEntry].getTileBuf(&dataBuffer.front(), clipRect, band);

               if ( m_entries[m_currentEntry].getScalarType() == OSSIM_FLOAT32 )
               {
                  //---
                  // NPP VIIRS data has null of "-999.3".
                  // Scan and fix non-standard null value.
                  //---
                  const ossim_float32 NP = getNullPixelValue(band);
                  const ossim_uint32 COUNT = clipRect.area();
                  ossim_float32* float_buffer = (ossim_float32*)&dataBuffer.front();
                  for ( ossim_uint32 i = 0; i < COUNT; ++i )
                  {
                     if ( float_buffer[i] <= -999.0 ) float_buffer[i] = NP;
                  }
               }

               // Buffer to tile:
               result->loadBand((void*)&dataBuffer.front(), clipRect, band);
            }

            // Validate the tile, i.e. full, partial, empty.
            result->validate();
         }
         else // No intersection...
         {
            result->makeBlank();
         }
      }

      result->unref();  // Decrement ref count.
   }

   return status;
}

ossimIrect
ossimH5Reader::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,
                     0,
                     getNumberOfSamples(reduced_res_level) - 1,
                     getNumberOfLines(reduced_res_level)   - 1);
}

bool ossimH5Reader::saveState(ossimKeywordlist& kwl,
                               const char* prefix) const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

bool ossimH5Reader::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      return open();
   }

   return false;
}

bool ossimH5Reader::open()
{
   static const char MODULE[] = "ossimH5Reader::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "File:  " << theImageFile.c_str()
         << std::endl;
   }

   bool status = false;
   
   // Start with a clean slate.
   if (isOpen())
   {
      close();
   }

   // Check for empty filename.
   if (theImageFile.size())
   {
      try
      {
         //--
         // Turn off the auto-printing when failure occurs so that we can
         // handle the errors appropriately
         //---
         // H5::Exception::dontPrint();

         theImageFile = theImageFile.expand();

         if ( H5::H5File::isHdf5( theImageFile.c_str() ) )
         {
            m_h5File = new H5::H5File();

            H5::FileAccPropList access_plist = H5::FileAccPropList::DEFAULT;
            
            m_h5File->openFile( theImageFile.string(), H5F_ACC_RDONLY, access_plist );

            std::vector<std::string> names;
            ossim_hdf5::getDatasetNames( m_h5File, names );

            if ( names.size() )
            {
               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " DEBUG\nDataset names:\n"; 
                  for ( ossim_uint32 i = 0; i < names.size(); ++i )
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "dataset[" << i << "]: " << names[i] << "\n";
                  }
               }

               // Add the image dataset entries.
               addImageDatasetEntries( names );

               if ( m_entries.size() )
               {
                  // Set the return status.
                  status = true;
               }
               else
               {
                  m_h5File->close();
                  delete m_h5File;
                  m_h5File = 0;
               }
            }
         }
         
      } // End: try block
      
      catch( const H5::FileIException& error )
      {
         if ( traceDebug() )
         {
            error.printError();
         }
      }
      
      // catch failure caused by the DataSet operations
      catch( const H5::DataSetIException& error )
      {
         if ( traceDebug() )
         {
            error.printError();
         }
      }
      
      // catch failure caused by the DataSpace operations
      catch( const H5::DataSpaceIException& error )
      {
         if ( traceDebug() )
         {
            error.printError();
         }
      }
      
      // catch failure caused by the DataSpace operations
      catch( const H5::DataTypeIException& error )
      {
         if ( traceDebug() )
         {
            error.printError();
         }
      }
      catch( ... )
      {
         
      }
      
      if ( status )
      {
         completeOpen();
      }
   }

   return status;
}

void ossimH5Reader::addImageDatasetEntries(const std::vector<std::string>& names)
{
   if ( m_h5File && names.size() )
   {
      std::vector<std::string>::const_iterator i = names.begin();
      while ( i != names.end() )
      {
         if ( ossim_hdf5::isExcludedDataset( *i ) == false )
         {
            H5::DataSet dataset = m_h5File->openDataSet( *i );

            // Get the class of the datatype that is used by the dataset.
            H5T_class_t type_class = dataset.getTypeClass();
            
            if ( ( type_class == H5T_INTEGER ) || ( type_class == H5T_FLOAT ) )
            {
               // Get the extents:
               std::vector<ossim_uint32> extents;
               ossim_hdf5::getExtents( &dataset, extents ); 

               if ( extents.size() >= 2 )
               {
                  if ( ( extents[0] > 1 ) && ( extents[1] > 1 ) )
                  {
                     ossimH5ImageDataset hids;
                     hids.initialize( dataset, *i );
                     m_entries.push_back( hids );
                  }     
               }
            }

            dataset.close();
         }
            
         ++i;
      }
   }
   
#if 0 /* Please leave for debug. (drb) */
   std::vector<ossimH5ImageDataset>::const_iterator i = m_entries.begin();
   while ( i != m_entries.end() )
   {
      std::cout << (*i) << endl;
      ++i;
   }
#endif
      
} // End: ossimH5Reader::addImageDatasetEntries

ossim_uint32 ossimH5Reader::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   ossim_uint32 result = 0;
   
   if ( (reduced_res_level == 0) && ( m_currentEntry < m_entries.size() ) )
   {
      result = m_entries[m_currentEntry].getNumberOfLines();
   }
   else if ( theOverview.valid() )
   {
      result = theOverview->getNumberOfLines(reduced_res_level);
   }
   
   return result;
}

ossim_uint32 ossimH5Reader::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   ossim_uint32 result = 0;
   
   if ( (reduced_res_level == 0) && ( m_currentEntry < m_entries.size() ) )
   {
      result = m_entries[m_currentEntry].getNumberOfSamples();
   }
   else if ( theOverview.valid() )
   {
      result = theOverview->getNumberOfSamples(reduced_res_level);
   }
   
   return result; 
}

ossim_uint32 ossimH5Reader::getImageTileWidth() const
{
   return 0; // Not tiled format.
}

ossim_uint32 ossimH5Reader::getImageTileHeight() const
{
   return 0; // Not tiled format.
}

ossimString ossimH5Reader::getShortName()const
{
   return ossimString("ossim_hdf5_reader");
}
   
ossimString ossimH5Reader::getLongName()const
{
   return ossimString("ossim hdf5 reader");
}

ossimString  ossimH5Reader::getClassName()const
{
   return ossimString("ossimH5Reader");
}

ossim_uint32 ossimH5Reader::getNumberOfInputBands() const
{
   ossim_uint32 result = 1;

   if ( m_currentEntry < m_entries.size() )
   {
      result = m_entries[m_currentEntry].getNumberOfBands();
   }

   return result;
}

ossim_uint32 ossimH5Reader::getNumberOfOutputBands()const
{
   // Currently not band selectable.
   return getNumberOfInputBands();
}

ossimScalarType ossimH5Reader::getOutputScalarType() const
{
   ossimScalarType result = OSSIM_SCALAR_UNKNOWN;
   
   if ( m_currentEntry < m_entries.size() )
   {
      result = m_entries[m_currentEntry].getScalarType();
   }
   
   return result;
}

bool ossimH5Reader::isOpen()const
{
   return ( m_entries.size() && ( m_currentEntry < m_entries.size() ) ) ? true : false ;
}

void ossimH5Reader::close()
{
   // Close the datasets.
   m_entries.clear();

   // Then the file.
   if ( m_h5File )
   {
      m_h5File->close();
      delete m_h5File;
      m_h5File = 0;
   }

   // ossimRefPtr so assign to 0(unreferencing) will handle memory.
   m_tile = 0;
   m_projection = 0;

   ossimImageHandler::close();
}

ossim_uint32 ossimH5Reader::getNumberOfEntries() const
{
   return (ossim_uint32)m_entries.size();
}

void ossimH5Reader::getEntryName( ossim_uint32 entryIdx, std::string& name ) const
{
   if ( entryIdx < m_entries.size() )
   {
      name = m_entries[ entryIdx ].getName();
   }
   else
   {
      name.clear();
   }
}

void ossimH5Reader::getEntryList(std::vector<ossim_uint32>& entryList) const
{
   const ossim_uint32 SIZE = m_entries.size();
   entryList.resize( SIZE );
   for ( ossim_uint32 i = 0; i < SIZE; ++i )
   {
      entryList[i] = i; 
   }
}

bool ossimH5Reader::setCurrentEntry( ossim_uint32 entryIdx )
{
   bool result = true;
   if (m_currentEntry != entryIdx)
   {
      if ( isOpen() )
      {
         // Entries always start at zero and increment sequentially..
         if ( entryIdx < m_entries.size() )
         {
            //---
            // Note: Currently sharing ONE geometry so there is no need to clear the geometry
            // file.  This may have to be changed if data has entries that are not relative
            // to same geometry file.
            //---
            // Clear the geometry.
            // theGeometry = 0;
            
            // Must clear or openOverview will use last entries.
            theOverviewFile.clear();
            
            m_currentEntry = entryIdx;
            
            //---
            // Since we were previously open and the the entry has changed we
            // need to reinitialize some things. Setting tile to 0 will force an
            // allocate call on first getTile.
            //---
            
            // ossimRefPtr so assign to 0(unreferencing) will handle memory.
            m_tile = 0;
            
            completeOpen();
         }
         else
         {
            result = false; // Entry index out of range.
         }
      }
      else
      {
         //---
         // Not open.
         // Allow this knowing that open will check for out of range.
         //---
         m_currentEntry = entryIdx;
      }
   }

   return result;
}

ossim_uint32 ossimH5Reader::getCurrentEntry() const
{
   return m_currentEntry;
}

ossimRefPtr<ossimImageGeometry> ossimH5Reader::getImageGeometry()
{
   if ( !theGeometry )
   {
      //---
      // Check for external geom:
      //---
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         //---
         // WARNING:
         // Must create/set the geometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---

         // Check the internal geometry first to avoid a factory call.
         m_mutex.lock();
         theGeometry = getInternalImageGeometry();
         m_mutex.unlock();

         // At this point it is assured theGeometry is set.

         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }

   return theGeometry;
}

ossimRefPtr<ossimImageGeometry> ossimH5Reader::getInternalImageGeometry()
{
   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();

   if ( m_projection.valid() )
   {
      // Stored projection, currently shared by all entries.
      geom->setProjection( m_projection.get() );
   }
   else
   {
      if ( m_h5File )
      {
         // Find the "Latitude" and "Longitude" datasets if present.
         std::string latName;
         std::string lonName;
         if ( getLatLonDatasetNames(  m_h5File, latName, lonName ) )
         {
            H5::DataSet latDataSet = m_h5File->openDataSet( latName );
            H5::DataSet lonDataSet = m_h5File->openDataSet( lonName );

            ossimIrect imageRect = getImageRectangle(0);
            
            ossimIrect validRect;
            if ( ossim_hdf5::getBoundingRect( latDataSet, validRect ) )
            {
               ossimRefPtr<ossimProjection> proj = 0;
               
               if ( imageRect == validRect )
               {
                  // Try for a coarse projection first:
                  proj = processCoarseGridProjection( latDataSet, lonDataSet );
               }
                  
               if ( proj.valid() == false )
               {
                  ossimIrect rect;
                  proj = processBilinearProjection( latDataSet, lonDataSet, validRect );
               }
               
               if ( proj.valid() )
               {
                  // Store it for next time:
                  m_projection = proj;
                  
                  // Set the geometry projection
                  geom->setProjection( proj.get() ); 
               }
            }
               
            latDataSet.close();
            lonDataSet.close();
         }
      }
   }
 
   return geom;
}

ossimRefPtr<ossimProjection> ossimH5Reader::processBilinearProjection(
   H5::DataSet& latDataSet, H5::DataSet& lonDataSet, const ossimIrect& validRect ) const
{
   ossimRefPtr<ossimProjection> proj = 0;

   // Get dataspace of the dataset.
   H5::DataSpace latDataSpace = latDataSet.getSpace();
   H5::DataSpace lonDataSpace = lonDataSet.getSpace();
         
   // Number of dimensions of the input dataspace:
   const ossim_int32 DIM_COUNT = latDataSpace.getSimpleExtentNdims();
         
   if ( DIM_COUNT == 2 )
   {
      // Get the extents. Assuming dimensions are same for lat lon dataset. 
      std::vector<hsize_t> dimsOut(DIM_COUNT);
      latDataSpace.getSimpleExtentDims( &dimsOut.front(), 0 );

      if ( dimsOut[0] && dimsOut[1] )
      {
         std::vector<hsize_t> inputCount(DIM_COUNT);
         std::vector<hsize_t> inputOffset(DIM_COUNT);
               
         inputOffset[0] = 0;
         inputOffset[1] = 0;
               
         inputCount[0] = 1;
         inputCount[1] = 1;
               
         // Output dataspace dimensions.
         const ossim_int32 OUT_DIM_COUNT = 3;
         std::vector<hsize_t> outputCount(OUT_DIM_COUNT);
         outputCount[0] = 1; // single band
         outputCount[1] = 1; // single line
         outputCount[2] = 1; // single sample
               
         // Output dataspace offset.
         std::vector<hsize_t> outputOffset(OUT_DIM_COUNT);
         outputOffset[0] = 0;
         outputOffset[1] = 0;
         outputOffset[2] = 0;
               
         ossimScalarType scalar = ossim_hdf5::getScalarType( &latDataSet );
         if ( scalar == OSSIM_FLOAT32 )
         {
            // See if we need to swap bytes:
            ossimEndian* endian = 0;
            if ( ( ossim::byteOrder() != ossim_hdf5::getByteOrder( &latDataSet ) ) )
            {
               endian = new ossimEndian();
            }

            // Native type:
            H5::DataType latDataType = latDataSet.getDataType();
            H5::DataType lonDataType = lonDataSet.getDataType();
                  
            std::vector<ossimDpt> ipts;
            std::vector<ossimGpt> gpts;
            ossimGpt gpt(0.0, 0.0, 0.0); // Assuming WGS84...
            ossim_float32 latValue = 0.0;
            ossim_float32 lonValue = 0.0;
                  
            // Only grab every 128th value.:
            const ossim_int32 GRID_SIZE = 128;
                  
            // Output dataspace always the same one pixel.
            H5::DataSpace bufferDataSpace( OUT_DIM_COUNT, &outputCount.front());
            bufferDataSpace.selectHyperslab( H5S_SELECT_SET,
                                             &outputCount.front(),
                                             &outputOffset.front() );

            //---
            // Dataset sample has NULL lines at the end so scan for valid rect.
            // Use "<= -999" for test as per NOAA as it seems the NULL value is
            // fuzzy.  e.g. -999.3.
            //---
            const ossim_float32 NULL_VALUE = -999.0;

            //---
            // Get the tie points within the valid rect:
            //---
            ossimDpt ipt = validRect.ul();
            while ( ipt.y <= validRect.lr().y )
            {
               inputOffset[0] = static_cast<hsize_t>(ipt.y);
               
               // Sample loop:
               ipt.x = validRect.ul().x;
               while ( ipt.x <= validRect.lr().x )
               {
                  inputOffset[1] = static_cast<hsize_t>(ipt.x);
                  
                  latDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                &inputCount.front(),
                                                &inputOffset.front() );
                  lonDataSpace.selectHyperslab( H5S_SELECT_SET,
                                                &inputCount.front(),
                                                &inputOffset.front() );
                  
                  // Read data from file into the buffer.
                  latDataSet.read( &latValue, latDataType, bufferDataSpace, latDataSpace );
                  lonDataSet.read( &lonValue, lonDataType, bufferDataSpace, lonDataSpace );
                  
                  if ( endian )
                  {
                     // If the endian pointer is initialized(not zero) swap the bytes.
                     endian->swap( latValue );
                     endian->swap( lonValue );  
                  }
                  
                  if ( ( latValue > NULL_VALUE ) && ( lonValue > NULL_VALUE ) )
                  {
                     gpt.lat = latValue;
                     gpt.lon = lonValue;
                     
                     ipts.push_back( ipt );
                     gpts.push_back( gpt );
                  }
                  
                  if ( ipt.x < validRect.lr().x )
                  {
                     ipt.x += GRID_SIZE;
                     if ( ipt.x > validRect.lr().x )
                     {
                        ipt.x = validRect.lr().x; // Clamp to last sample.
                     }
                  }
                  else
                  {  
                     break; // At the end:
                  }
                  
               } // End sample loop.

               if ( ipt.y < validRect.lr().y )
               {
                  ipt.y += GRID_SIZE;
                  if ( ipt.y > validRect.lr().y )
                  {
                     ipt.y = validRect.lr().y; // Clamp to last line.
                  }
               }
               else
               {  
                  break; // At the end:
               }
               
            } // End line loop.
                  
            if ( ipts.size() )
            {
               // Create the projection:
               ossimRefPtr<ossimBilinearProjection> bp = new ossimBilinearProjection();
                     
               // Add the tie points:
               bp->setTiePoints( ipts, gpts );
                     
               // Assign to output projection:
               proj = bp.get();
            }
         }
         else // Matches: if ( scalar == OSSIM_FLOAT32 ){...}
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimH5Reader::processBilinearProjection WARNING!"
               << "\nUnhandled scalar type: "
               << ossimScalarTypeLut::instance()->getEntryString( scalar )
               << std::endl;
         }
               
      } // Matches: if ( dimsOut...
            
   } // Matches: if ( IN_DIM_COUNT == 2 )
         
   latDataSpace.close();
   lonDataSpace.close();
   
   return proj;
   
} // End: ossimH5Reader::processBilinearProjection()

ossimRefPtr<ossimProjection> ossimH5Reader::processCoarseGridProjection(
   H5::DataSet& latDataSet, H5::DataSet& lonDataSet ) const
{
   ossimRefPtr<ossimProjection> proj = 0;

   ossimRefPtr<ossimHdfGridModel> model = new ossimHdfGridModel();
   
   try
   {
      if ( model->setGridNodes( &latDataSet,
                                &lonDataSet,
                                getNumberOfLines(0), 
                                getNumberOfSamples(0) ) )
      {
         proj = model.get();
         // proj->print(cout);
      }
   }
   catch ( const ossimException& e )
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimH5Reader::processCoarseGridProjection caught exception\n"
            << e.what() << std::endl;
      }
   }

   return proj;
}

bool ossimH5Reader::getLatLonDatasetNames( H5::H5File* h5File,
                                           std::string& latName,
                                           std::string& lonName ) const
{
   bool result = false;

   if ( h5File )
   {
      latName.clear();
      lonName.clear();
      
      // Get the list of datasets.
      std::vector<std::string> datasetNames;
      ossim_hdf5::getDatasetNames( h5File, datasetNames );
      
      if ( datasetNames.size() )
      {
         std::vector<std::string>::const_iterator i = datasetNames.begin();
         while ( i != datasetNames.end() )
         {
            ossimString os = *i;
            if ( os.contains( "Latitude" ) )
            {
               latName = *i;
            }
            else if ( os.contains( "Longitude" ) )
            {
               lonName = *i;
            }
            
            if ( latName.size() && lonName.size() )
            {
               result = true;
               break;
            }
            
            ++i;
         }
      }
   }
   
   return result;

} // End: ossimH5Reader::getLatLonDatasetNames

bool ossimH5Reader::getLatLonDatasets( H5::H5File* h5File,
                                       H5::DataSet& latDataSet,
                                       H5::DataSet& lonDataSet ) const
{
   bool status = false;

   if ( h5File )
   {
      std::string latName;
      std::string lonName;
      if ( getLatLonDatasetNames( h5File, latName, lonName ) )
      {
         // Set the return status:
         status = true;
         latDataSet = h5File->openDataSet( latName );
         lonDataSet = h5File->openDataSet( lonName ); 
      }
      else
      {
         // Look for the key: /N_GEO_Ref
         std::string group = "/";
         std::string key   = "N_GEO_Ref";
         std::string value;
            
         if ( ossim_hdf5::getGroupAttributeValue( h5File, group, key, value ) )
         {
            ossimFilename f = value;
 	    ossimFilename latLonFile = theImageFile.path();
            latLonFile = latLonFile.dirCat( f );

            if ( latLonFile.exists() )
            {
               if ( H5::H5File::isHdf5( latLonFile.string() ) )
               {
                  H5::H5File* h5File = new H5::H5File();
                  
                  H5::FileAccPropList access_plist = H5::FileAccPropList::DEFAULT;
                  h5File->openFile( latLonFile.string(), H5F_ACC_RDONLY, access_plist );

                  if ( getLatLonDatasetNames( h5File, latName, lonName ) )
                  {
                     // Set the return status:
                     status = true;
                     latDataSet = h5File->openDataSet( latName );
                     lonDataSet = h5File->openDataSet( lonName ); 
                  }
                  h5File->close();
                  delete h5File;
                  h5File = 0;
               }
            }
         }
      }
   }

   return status;
   
} // End: ossimH5Reader::getLatLonDatasets( ... )

bool ossimH5Reader::isNppMission( H5::H5File* h5File ) const
{
   bool result = false;
   
   // Look for "Mission_Name"
   const std::string GROUP = "/";
   const std::string KEY = "Mission_Name";
   std::string value;
   
   if ( ossim_hdf5::getGroupAttributeValue( h5File, GROUP, KEY, value ) )
   {
      if ( value == "NPP" )
      {
         result = true;
      }
   }
   
   return result;
}

double ossimH5Reader::getNullPixelValue( ossim_uint32 band ) const
{
   return ossimImageHandler::getNullPixelValue( band );
#if 0
   double result;
   ossimScalarType scalar = getOutputScalarType();
   if ( scalar == OSSIM_FLOAT32 )
   {
      result = -9999.0;
   }
   else
   {
      result = ossimImageHandler::getNullPixelValue( band );
   }
   return result;
#endif
}

void ossimH5Reader::setProperty(ossimRefPtr<ossimProperty> property)
{
   if ( property.valid() )
   {
      if ( property->getName().string() == LAYER_KW )
      {
         ossimString s;
         property->valueToString(s);
         ossim_uint32 SIZE = (ossim_uint32)m_entries.size();
         for ( ossim_uint32 i = 0; i < SIZE; ++i )
         {
            if ( m_entries[i].getName() == s.string() )
            {
               setCurrentEntry( i );
            }
         }
      }
      else
      {
         ossimImageHandler::setProperty(property);
      }
   }
}

ossimRefPtr<ossimProperty> ossimH5Reader::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> prop = 0;
   if ( name.string() == LAYER_KW )
   {
      if ( m_currentEntry < m_entries.size() )
      {
         ossimString value = m_entries[m_currentEntry].getName();
         prop = new ossimStringProperty(name, value);
      }
   }
   else
   {
      prop = ossimImageHandler::getProperty(name);
   }
   return prop;
}

void ossimH5Reader::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back( ossimString("layer") );
   ossimImageHandler::getPropertyNames(propertyNames);
}


void ossimH5Reader::addMetadata( ossimKeywordlist* kwl, const std::string& prefix ) const
{
   // Note: hdf5 library throws exception if groupd is not found:
   
   if ( kwl && m_h5File )
   {
      // Look for country code(s): hdf5.CountryCodes: CA,MX,US
      std::string group = "/";
      std::string key = "CountryCodes";
      std::string value;

      if ( ossim_hdf5::getGroupAttributeValue( m_h5File, group, key, value ) )
      {
         key = "country_code";
         kwl->addPair( prefix, key, value );
      }

      // Look for mission id: hdf5.Mission_Name: NPP
      key = "Mission_Name";
      if ( ossim_hdf5::getGroupAttributeValue( m_h5File, group, key, value ) )
      {
         key = "mission_id";
         kwl->addPair( prefix, key, value );
      }
         

      //---
      // Look for sensor type:
      // hdf5.Data_Products.VIIRS-DNB-SDR.Instrument_Short_Name: VIIRS
      //---
      group = "/Data_Products/VIIRS-DNB-SDR";
      key = "Instrument_Short_Name";
      if ( ossim_hdf5::getGroupAttributeValue( m_h5File, group, key, value ) )
      {
         key = "sensor_id";
         kwl->addPair( prefix, key, value );
      }
      
      //---
      // Look for acquisition date:
      // hdf5.Data_Products.VIIRS-DNB-SDR.VIIRS-DNB-SDR_Aggr.AggregateBeginningDate: 20140113
      // hdf5.Data_Products.VIIRS-DNB-SDR.VIIRS-DNB-SDR_Aggr.AggregateBeginningTime: 082810.354645Z
      //---
      group = "Data_Products/VIIRS-DNB-SDR/VIIRS-DNB-SDR_Aggr";
      key = "AggregateBeginningDate";
      if ( ossim_hdf5::getDatasetAttributeValue( m_h5File, group, key, value ) )
      {
         std::string time;
         key = "AggregateBeginningTime";
         if ( ossim_hdf5::getDatasetAttributeValue( m_h5File, group, key, time ) )
         {
            // Only grab yyyymmddmmss in the form ISO 8601: 2014-01-26T10:32:28Z
            if ( (value.size() >= 8) && ( time.size() >= 6 ) )
            {
               std::string dateTime =
                  ossimString( ossimString(value.begin(), value.begin() + 4) + "-"
                               + ossimString(value.begin() + 4, value.begin() + 6) + "-"
                               + ossimString(value.begin() + 6, value.begin() + 8) + "T"
                               + ossimString(time.begin(), time.begin() + 2) + ":"
                               + ossimString(time.begin() + 2, time.begin() + 4) + ":"
                               + ossimString(time.begin() + 4, time.begin() + 6) + "Z" ).string();
               key = "acquisition_date";
               kwl->addPair( prefix, key, dateTime );
            }
         }
      }
   }
}

