//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Class definition for HDF4 reader.
//
//----------------------------------------------------------------------------
// $Id: ossimHdf5Reader.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include "ossimHdf5Reader.h"
#include "ossimHdf5SubDataset.h"
#include "ossimHdfGridModel.h"

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimUnitTypeLut.h>

#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>

#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimPolarStereoProjection.h>
#include <ossim/projection/ossimPolyconicProjection.h>
#include <ossim/projection/ossimAzimEquDistProjection.h>
#include <ossim/projection/ossimObliqueMercatorProjection.h>
#include <ossim/projection/ossimSpaceObliqueMercatorProjection.h>
#include <ossim/projection/ossimSinusoidalProjection.h>

#include <ossim/support_data/ossimAuxFileHandler.h>

#include <fstream>
#include <iostream>
#include <string>

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id";
#endif

const ossimString ossimHdf5Reader::HDF5_EOS_GRID = "GRIDS";              
const ossimString ossimHdf5Reader::HDF5_EOS_SWATH = "SWATHS";                   
const ossimString ossimHdf5Reader::FILE_META_PATH = "/HDFEOS/ADDITIONAL/FILE_ATTRIBUTES";		
const ossimString ossimHdf5Reader::ROOT_META_PATH = "HDFEOS";
const ossimString ossimHdf5Reader::DATA_META_NAME = "ColumnAmountAerosol";	          
const ossimString ossimHdf5Reader::STRUCT_META_PATH = "/HDFEOS INFORMATION/StructMetadata.0";		       

static ossimTrace traceDebug("ossimHdf5Reader:debug");
static ossimTrace traceDump("ossimHdf5Reader:dump");

herr_t Hdf5AttrIterate(hid_t hdfObjId, const char* attrName, void* reader)
{
   hid_t           attrId;
   hid_t           attrTypeID;
   hid_t           attrNativeType;
   hid_t           attrSpace;

   char*           dataValue = NULL;
   hsize_t         size[64];
   unsigned int    attrElmts;
   hsize_t         attrSize;
   hsize_t         i;
   void*           buf = NULL;
   ossim_uint32    attrDims;
   ossimString     attrValue;

   ossimHdf5Reader* hdf5Reader = (ossimHdf5Reader*)reader;

   attrId          = H5Aopen_name(hdfObjId, attrName);
   attrTypeID      = H5Aget_type(attrId);
   attrNativeType  = H5Tget_native_type(attrTypeID, H5T_DIR_DEFAULT);
   attrSpace       = H5Aget_space(attrId);
   attrDims        = H5Sget_simple_extent_dims(attrSpace, size, NULL);

   if( H5Tget_class(attrNativeType) == H5T_STRING ) 
   {
      attrSize = H5Tget_size(attrTypeID);
      dataValue = (char*)malloc(attrSize+1);
      H5Aread(attrId, attrNativeType, dataValue);
      dataValue[attrSize]='\0';
      attrValue = ossimString(dataValue);
      hdf5Reader->setGlobalMeta(ossimString(attrName), attrValue);
   }
   else 
   {
      attrElmts = 1;
      for( i=0; i < attrDims; i++ ) 
      {
         attrElmts *= size[i];
      }
      if( attrElmts > 0 ){
         buf = (void *)malloc(attrElmts*H5Tget_size(attrNativeType));
         dataValue = (char*)malloc(8192);
         dataValue[0]='\0';
         H5Aread(attrId, attrNativeType, buf);
      }

      if(H5Tequal(H5T_NATIVE_CHAR, attrNativeType) || H5Tequal( H5T_NATIVE_UCHAR, attrNativeType))
      {
         for(i = 0; i < attrElmts; i++ ) 
         {
            attrValue += ossimString(((ossim_uint8*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_SHORT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++ ) 
         {
            attrValue += ossimString::toString(((ossim_int16*) buf)[i]) + " ";
         }
      }
      else if(H5Tequal(H5T_NATIVE_USHORT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++) 
         {
            attrValue += ossimString::toString(((ossim_uint16*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_INT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++) 
         {
            attrValue += ossimString::toString(((ossim_int32*) buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_UINT, attrNativeType ) ) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_uint32*) buf)[i]) + " ";   
         }
      }
      else if(H5Tequal( H5T_NATIVE_LONG, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_int64*)buf)[i]) + " ";         
         }
      }
      else if(H5Tequal( H5T_NATIVE_ULONG, attrNativeType))
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_uint64*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_FLOAT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_float32*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_DOUBLE, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_float64*)buf)[i]);
         }
      }
      hdf5Reader->setGlobalMeta(ossimString(attrName), attrValue);

      free(buf);
   }
   H5Aclose(attrId);
   free(dataValue);

   return 0;
}

RTTI_DEF1_INST(ossimHdf5Reader,
               "ossimHdf5Reader",
               ossimImageHandler)

ossimHdf5Reader::ossimHdf5Reader()
   :
   ossimImageHandler(),
   m_fid(-1),
   m_imageRect(ossimIrect(0, 0, 127,127)),
   m_numberOfBands(0),
   m_scalarType(OSSIM_SCALAR_UNKNOWN),
   m_tile(0),
   m_subDatasetNames(0),
   m_geoDatasetNames(0),
   m_globalMeta(0),
   m_hdfDataType("UNKNOWN"),
   m_currentEntryRender(0),
   m_currentSubDataset(0),
   m_structMetadata("")
{
}

ossimHdf5Reader::~ossimHdf5Reader()
{
   closeEntry();
}

ossimString ossimHdf5Reader::getShortName()const
{
   return ossimString("ossim_hdf5_reader");
}

ossimString ossimHdf5Reader::getLongName()const
{
   return ossimString("ossim hdf5 reader");
}

ossimString ossimHdf5Reader::getClassName()const
{
   return ossimString("ossimHdf5Reader");
}

ossim_uint32 ossimHdf5Reader::getNumberOfLines(ossim_uint32 resLevel) const
{
   ossim_uint32 result = 0;
   if ( isValidRLevel(resLevel) )
   {
      if (resLevel == 0)
      {
         if (m_currentSubDataset != NULL)
         {
            return m_currentSubDataset->getNumberOfLines();
         }
      }
      else if (theOverview.valid())
      {
         result = theOverview->getNumberOfSamples(resLevel);
      }
   }
   return result;
}

ossim_uint32 ossimHdf5Reader::getNumberOfSamples(ossim_uint32 resLevel) const
{
   ossim_uint32 result = 0;
   if ( isValidRLevel(resLevel) )
   {
      if (resLevel == 0)
      {
         if (m_currentSubDataset != NULL)
         {
            return m_currentSubDataset->getNumberOfSamples();
         }
      }
      else if (theOverview.valid())
      {
         result = theOverview->getNumberOfSamples(resLevel);
      }
   }
   return result;
}

bool ossimHdf5Reader::open()
{
   static const char MODULE[] = "ossimHdf5Reader::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << "image: " << theImageFile << "\n";
   }

   bool result = false;

   if (m_subDatasetNames.size() == 0)
   {
      if (isOpen())
      {
         closeEntry();
      }

      m_fid = H5Fopen(theImageFile.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

      if (m_fid < 0)
      {
         closeEntry();
         return false;
      }

      initSubdatasetNames();

      result = initGlobalMeta(); 
   }

   if (m_currentEntryRender < m_subDatasetNames.size())
   {
      //close file before open file in sub-dataset
      if (m_currentSubDataset != NULL)
      {
         delete m_currentSubDataset;
         m_currentSubDataset = 0;
      }
      ossimString subDataName = m_subDatasetNames[m_currentEntryRender];
      m_currentSubDataset = new ossimHdf5SubDataset();
      m_currentSubDataset->setSubdatasetName(subDataName);
      m_currentSubDataset->setHdfFilename(theImageFile);

      m_tile = ossimImageDataFactory::instance()->create(this, this);
      m_tile->initialize();

      // Call the base complete open to pick up overviews.
      completeOpen();
      result = true;
   }
   else
   {
      closeEntry();
      result = false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimHdf5Reader::initGlobalMeta()
{
   bool result = false;
   m_globalMeta.clear();
   //---
   // H5Gopen1 is depreciated.  Need to fix. (drb)
   // Should call H5Gopen2 which is typedef H5Gopen.
   // hid_t groupId = H5Gopen(m_fid, FILE_META_PATH);
   //---
   hid_t groupId = H5Gopen1(m_fid, FILE_META_PATH);    
   if (groupId > 0)
   {
      ossimHdf5Reader* reader = this;
      //---
      // H5Aiterate1 is depreciated.  Need to fix. (drb)
      // Should call H5Aiterate2 which typedef H5Aiterate.
      // herr_t ret = H5Aiterate(groupId, 0, Hdf5AttrIterate, (void*)reader);
      //---
      herr_t ret = H5Aiterate1(groupId, 0, Hdf5AttrIterate, (void*)reader);
      if (ret == 0)
      {
         result = true;
      }
   }
   H5Gclose(groupId);

   ossimString columnMeta = ossimString("/" + ROOT_META_PATH + "/" + m_hdfDataType + "/" + DATA_META_NAME);
   
   // depreciated...
   // groupId = H5Gopen(m_fid, columnMeta);
   groupId = H5Gopen1(m_fid, columnMeta);
   if (groupId > 0)
   {
      ossimHdf5Reader* reader = this;
      
      //---
      // H5Aiterate1 id depreciated.  Need to fix. (drb)
      // Should call H5Aiterate2 which is typedef H5Aiterate.
      // herr_t ret = H5Aiterate(groupId, NULL, Hdf5AttrIterate, (void*)reader);
      //---
      herr_t ret = H5Aiterate1(groupId, NULL, Hdf5AttrIterate, (void*)reader);
      if (ret == 0)
      {
         result = true;
      }
   }
   H5Gclose(groupId);
   return result;
}

void ossimHdf5Reader::initSubdatasetNames()
{
   m_subDatasetNames.clear();
   m_geoDatasetNames.clear();
   std::vector<ossimString> attrInfos;
   ossimString groupIdentifier;
   ossimString endGroupIdentifier;
   ossimString groupName;

   // Depreciated, need to fix... (drb)
   // hid_t datasetId = H5Dopen(m_fid, STRUCT_META_PATH);
   hid_t datasetId = H5Dopen1(m_fid, STRUCT_META_PATH);
   if (datasetId >= 0)
   {
      hid_t datatype = H5Dget_type(datasetId);
      size_t t_size = H5Tget_size(datatype);
      hid_t dataspace = H5Dget_space(datasetId);    /* dataspace handle */

      char* attrValues = new char[t_size];
      // herr_t status = H5Dread (datasetId,
      H5Dread (datasetId,                               
               datatype, 
               dataspace,
               dataspace,
               H5P_DEFAULT, 
               attrValues);

      m_structMetadata = ossimString(attrValues);
      delete[] attrValues;

      std::vector<ossimString> attrList = m_structMetadata.split("\n");

      for (ossim_uint32 i  = 0; i < attrList.size(); i++)
      {
         ossimString attrInfo = attrList[i].trim();
         if (attrInfo.contains("GROUP=SwathStructure") && m_subDatasetNames.size() == 0)//initialize the type and group name
         {
            m_hdfDataType = HDF5_EOS_SWATH;
            groupIdentifier = "GROUP=SWATH_";
            groupName = "SwathName";
         }
         else if (attrInfo.contains("GROUP=GridStructure") && m_subDatasetNames.size() == 0)//initialize the type and group name
         {
            m_hdfDataType = HDF5_EOS_GRID;
            groupIdentifier = "GROUP=GRID_";
            groupName = "GridName";
         }
         else //get sub dataset infos
         {
            if (attrInfo.contains(groupIdentifier) && attrInfo.contains("END_") == false)
            {
               endGroupIdentifier = "END_" + attrInfo;
            }
            if (attrInfo != endGroupIdentifier)
            {
               attrInfos.push_back(attrInfo);
            } 
            else
            {
               parseGroupName(attrInfos, groupName);
               attrInfos.clear();
            }
         }
      }
      H5Sclose (dataspace);
   }
   H5Dclose(datasetId);
}

void ossimHdf5Reader::parseGroupName(std::vector<ossimString> groupInfo,
                                     ossimString groupName)
{
   for (ossim_uint32 i = 0; i < groupInfo.size(); i++)
   {
      ossimString info = groupInfo[i];
      if (info.contains(groupName))
      {
         groupName = info.split("=")[1].trim();
         groupName = groupName.substitute("\"", "", true);
      }
      if (info.contains("DataFieldName"))
      {
         ossimString fieldName = info.split("=")[1].trim();

         fieldName = fieldName.substitute("\"", "", true);
         m_subDatasetNames.push_back(ossimString("/" + ROOT_META_PATH + "/" + 
            m_hdfDataType + "/" + groupName + "/Data Fields/" + fieldName));

         if (fieldName.contains("Latitude") || fieldName.contains("Longitude"))
         {
            fieldName = fieldName.substitute("\"", "", true);
            m_geoDatasetNames.push_back(ossimString("/" + ROOT_META_PATH + "/" + 
               m_hdfDataType + "/" + groupName + "/Data Fields/" + fieldName));
         } 
      }
      else if (info.contains("GeoFieldName"))
      {
         ossimString fieldName = info.split("=")[1].trim();
         fieldName = fieldName.substitute("\"", "", true);
         m_geoDatasetNames.push_back(ossimString("/" + ROOT_META_PATH + "/" + 
            m_hdfDataType + "/" + groupName + "/Geolocation Fields/" + fieldName));
      }
   }
}

bool ossimHdf5Reader::isOpen()const
{
   return m_currentSubDataset != NULL;
}

void ossimHdf5Reader::closeEntry()
{
   m_tile = 0;
   H5Fclose(m_fid);

   if (m_currentSubDataset != NULL)
   {
      delete m_currentSubDataset;
      m_currentSubDataset = 0;
   }
   ossimImageHandler::close();
}

ossimRefPtr<ossimImageData> ossimHdf5Reader::getTile(const ossimIrect& tile_rect, 
                                                     ossim_uint32 resLevel)
{
   if (m_tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_tile->setImageRectangle(tile_rect);

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

bool ossimHdf5Reader::getTile(ossimImageData* result,
                              ossim_uint32 resLevel)
{
   bool status = false;

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
      result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      result->ref();  // Increment ref count.

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

         if (getImageRectangle().intersects(tile_rect))
         {
            // Make a clip rect.
            ossimIrect clipRect = tile_rect.clipToRect(getImageRectangle());

            if (tile_rect.completely_within(clipRect) == false)
            {
               // Not filling whole tile so blank it out first.
               result->makeBlank();
            }

            if (m_currentSubDataset != NULL)
            {
               for (ossim_uint32 band = 0; band < getNumberOfInputBands(); band++)
               {
                  void* dataBuffer = m_currentSubDataset->getTileBuf(clipRect, band);
                  result->loadBand(dataBuffer, clipRect, band);
                   delete[] (char*)dataBuffer;
               }
            }
         }
         else // No intersection...
         {
            result->makeBlank();
         }
      }
      result->validate();

      result->unref();  // Decrement ref count.
   }

   return status;
}

ossim_uint32 ossimHdf5Reader::getNumberOfInputBands() const
{
   if (m_currentSubDataset != NULL)
   {
      return m_currentSubDataset->getNumberOfInputBands();
   }
   return m_numberOfBands;
}

ossim_uint32 ossimHdf5Reader::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

ossim_uint32 ossimHdf5Reader::getImageTileWidth() const
{
   return m_imageRect.width();
}

ossim_uint32 ossimHdf5Reader::getImageTileHeight() const
{
   return m_imageRect.height();
}

ossimScalarType ossimHdf5Reader::getOutputScalarType() const
{
   if (m_currentSubDataset != NULL)
   {
      return m_currentSubDataset->getOutputScalarType();
   }
   return m_scalarType;
}

ossimRefPtr<ossimImageGeometry> ossimHdf5Reader::getImageGeometry()
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
         // Check the internal geometry first to avoid a factory call.
         //---
         theGeometry = getInternalImageGeometry();

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

ossimRefPtr<ossimImageGeometry> ossimHdf5Reader::getInternalImageGeometry() const
{
   static const char MODULE[] = "ossimHdf5Reader::getInternalImageGeometry";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();

   if (m_hdfDataType == HDF5_EOS_GRID)
   {
      // Set the projection and return this handler's image geometry:
      geom->setProjection(processGridProjection().get());
   }
   else if (m_hdfDataType == HDF5_EOS_SWATH)
   {
      geom->setProjection(processSwathProjection().get());
   }

   return geom;
}

ossimRefPtr<ossimProjection> ossimHdf5Reader::processGridProjection() const
{
   ossimString geoLatName = "";
   ossimString geoLonName = "";
   for (ossim_uint32 i = 0; i < m_geoDatasetNames.size(); i++)
   {
      if (m_geoDatasetNames[i].contains("/Latitude"))
      {
         geoLatName = m_geoDatasetNames[i];
      }
      else if (m_geoDatasetNames[i].contains("/Longitude"))
      {
         geoLonName = m_geoDatasetNames[i];
      }
   }

   ossim_float32 ll_lat = 0.0;
   ossim_float32 ll_lon = 0.0;

   ossim_float32 lr_lat = 0.0;
   ossim_float32 lr_lon = 0.0;

   ossim_float32 ul_lat = 0.0;
   ossim_float32 ul_lon = 0.0;

   ossim_float32 ur_lat = 0.0;
   ossim_float32 ur_lon = 0.0;

   hsize_t dims_out[2]; //dataset dimensions

   // Depreciated, need to fix... (drb)
   // hid_t datasetId = H5Dopen(m_fid, geoLatName);
   hid_t datasetId = H5Dopen1(m_fid, geoLatName);
   
   ossim_int32 index = 0;
   if (datasetId >= 0)
   {
      hid_t datatype = H5Dget_type(datasetId);
      hid_t dataspace = H5Dget_space(datasetId);    //dataspace handle
      int rank = H5Sget_simple_extent_ndims(dataspace);
      if (rank == 2)
      {
         H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
         ossim_int32 rows = dims_out[0];
         ossim_int32 cols = dims_out[1];
         if( H5Tequal(H5T_NATIVE_FLOAT,  datatype))
         {
            ossim_float32* data_out = new ossim_float32[rows * cols];
            H5Dread(datasetId, datatype, dataspace, dataspace,
               H5P_DEFAULT, data_out);

            for (ossim_int32 y = 0; y < rows; y++)
            {
               for (ossim_int32 x = 0; x < cols; x++)
               {
                  index = x + y * cols;
                  if (x == 0 && y == 0)
                  {
                     ul_lat = data_out[index];
                  }
                  else if (x == (cols-1) && y == 0)
                  {
                     ur_lat = data_out[index];
                  }
                  else if (x == 0 && y == (rows-1))
                  {
                     ll_lat = data_out[index];
                  }
                  else if (x == (cols-1) && y == (rows-1))
                  {
                     lr_lat = data_out[index];
                  }
               }
            }
            delete[] data_out;
         }
         else if( H5Tequal(H5T_NATIVE_DOUBLE,  datatype))
         {
            ossim_float64* data_out = new ossim_float64[rows * cols];
            H5Dread(datasetId, datatype, dataspace, dataspace,
               H5P_DEFAULT, data_out);

            for (ossim_int32 y = 0; y < rows; y++)
            {
               for (ossim_int32 x = 0; x < cols; x++)
               {
                  index = x + y * cols;
                  if (x == 0 && y == 0)
                  {
                     ul_lat = data_out[index];
                  }
                  else if (x == (cols-1) && y == 0)
                  {
                     ur_lat = data_out[index];
                  }
                  else if (x == 0 && y == (rows-1))
                  {
                     ll_lat = data_out[index];
                  }
                  else if (x == (cols-1) && y == (rows-1))
                  {
                     lr_lat = data_out[index];
                  }
               }
            }
            delete[] data_out;
         }
      }
      H5Tclose(datatype);
      H5Sclose(dataspace);
   }
   H5Dclose(datasetId);

   // Depreciated, need to fix... (drb)
   // datasetId = H5Dopen(m_fid, geoLonName);
   datasetId = H5Dopen1(m_fid, geoLonName);
   if (datasetId >= 0)
   {
      hid_t datatype = H5Dget_type(datasetId);
      hid_t dataspace = H5Dget_space(datasetId);    /* dataspace handle */
      int rank = H5Sget_simple_extent_ndims(dataspace);

      if (rank == 2)
      {
         H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
         ossim_int32 rows = dims_out[0];
         ossim_int32 cols = dims_out[1];
         if( H5Tequal(H5T_NATIVE_FLOAT,  datatype))
         {
            ossim_float32* data_out = new ossim_float32[rows * cols];
            H5Dread(datasetId, datatype, dataspace, dataspace,
               H5P_DEFAULT, data_out);

            for (ossim_int32 y = 0; y < rows; y++)
            {
               for (ossim_int32 x = 0; x < cols; x++)
               {
                  index = x + y * cols;
                  if (x == 0 && y == 0)
                  {
                     ul_lon = data_out[index];
                  }
                  else if (x == (cols-1) && y == 0)
                  {
                     ur_lon = data_out[index];
                  }
                  else if (x == 0 && y == (rows-1))
                  {
                     ll_lon = data_out[index];
                  }
                  else if (x == (cols-1) && y == (rows-1))
                  {
                     lr_lon = data_out[index];
                  }
               }
            }
            delete[] data_out;
         }
         else if( H5Tequal(H5T_NATIVE_DOUBLE,  datatype))
         {
            ossim_float64* data_out = new ossim_float64[rows * cols];
            H5Dread(datasetId, datatype, dataspace, dataspace,
               H5P_DEFAULT, data_out);

            for (ossim_int32 y = 0; y < rows; y++)
            {
               for (ossim_int32 x = 0; x < cols; x++)
               {
                  index = x + y * cols;
                  if (x == 0 && y == 0)
                  {
                     ul_lon = data_out[index];
                  }
                  else if (x == (cols-1) && y == 0)
                  {
                     ur_lon = data_out[index];
                  }
                  else if (x == 0 && y == (rows-1))
                  {
                     ll_lon = data_out[index];
                  }
                  else if (x == (cols-1) && y == (rows-1))
                  {
                     lr_lon = data_out[index];
                  }
               }
            }
            delete[] data_out;
         }
      }
      H5Tclose(datatype);
      H5Sclose(dataspace);
   }
   H5Dclose(datasetId);

   if (m_currentSubDataset != NULL)
   {
      ossimString epsg("EPSG:4326");
      ossimRefPtr<ossimMapProjection> mapProj = PTR_CAST(ossimMapProjection, 
         ossimEpsgProjectionFactory::instance()->createProjection(epsg));

      ossim_uint32 rows = m_currentSubDataset->getNumberOfLines();
      ossim_uint32 cols = m_currentSubDataset->getNumberOfSamples();
     
      ossim_float64 xRes = (lr_lon - ul_lon)/cols;
      ossim_float64 yRes = (lr_lat - ul_lat)/rows;
      ossimDpt gsd(fabs(xRes), fabs(yRes));

      mapProj->setUlTiePoints(ossimGpt(ul_lat, ul_lon));
      mapProj->setDecimalDegreesPerPixel(gsd);
      return mapProj.get();
   }

   return ossimRefPtr<ossimProjection>();
}

ossimRefPtr<ossimProjection> ossimHdf5Reader::processSwathProjection() const
{
   ossimString geoLatName = "";
   ossimString geoLonName = "";
   for (ossim_uint32 i = 0; i < m_geoDatasetNames.size(); i++)
   {
      if (m_geoDatasetNames[i].contains("/Latitude"))
      {
         geoLatName = m_geoDatasetNames[i];
      }
      else if (m_geoDatasetNames[i].contains("/Longitude"))
      {
         geoLonName = m_geoDatasetNames[i];
      }
   }

   if (m_currentSubDataset != NULL)
   {
      ossim_uint32 rows = m_currentSubDataset->getNumberOfLines();
      ossim_uint32 cols = m_currentSubDataset->getNumberOfSamples();
      ossimIpt spacing(cols, rows);
      ossimDrect rect(0, 0, cols-1, rows-1);
      return new ossimHdfGridModel(theImageFile, rect, geoLatName, geoLonName, spacing);
   }

   return ossimRefPtr<ossimProjection>();
}

bool ossimHdf5Reader::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   bool result = false;

   if ( ossimImageHandler::loadState(kwl, prefix) )
   {
      result = open();
   }

   return result;
}

ossim_uint32 ossimHdf5Reader::getNumberOfEntries()const
{
   return (ossim_uint32) m_subDatasetNames.size();
}

bool ossimHdf5Reader::setCurrentEntry(ossim_uint32 entryIdx)
{
   if (m_currentEntryRender == entryIdx)
   {
      return true; // Nothing to do...
   }
   m_currentEntryRender = entryIdx;
   theOverview = 0;
   theOverviewFile.clear();
   m_tile = 0;
   return open();
}

std::vector<ossimString> ossimHdf5Reader::getSubDatasetMeta()
{
   if (m_currentSubDataset != NULL)
   {
      return m_currentSubDataset->getMetadata();
   }
   return std::vector<ossimString>();
}

void ossimHdf5Reader::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   for (ossim_uint32 i = 0; i < m_subDatasetNames.size(); i++)
   {
      entryList.push_back(i);
   }
}

ossimString ossimHdf5Reader::getMetaValue(const ossimString& key) const
{
   for (ossim_uint32 i = 0; i < m_globalMeta.size(); i++)
   {
      ossimString info = m_globalMeta[i];
      ossimString keyTmp = info.split(":")[0].trim();
      ossimString value = info.split(":")[1].trim();
      if (keyTmp == key)
      {
         return value;
      }
   }
   return "";
}

void ossimHdf5Reader::setGlobalMeta(const ossimString& key, const ossimString& value)
{
   ossimString meta = ossimString(key + ": " + value);
   m_globalMeta.push_back(meta);
}

ossim_uint32 ossimHdf5Reader::getCurrentEntry()const
{
   return m_currentEntryRender;
}



